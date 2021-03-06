#!/usr/bin/python


###############################################################
#                       runManager.py                         #
###############################################################
# + Python script which allows to manage multiple jobs in the #
# context of a lsfbatch system.                               #
#                                                             #
# + type runManager -h to see a description of the usage          #   
#                                                             #
# + Feedback to: pablom@cern.ch                               #
#                                                             #
############################################################### 
   


# System libraries to interact with the shell
import sys
import os



#-------------------------------------------------------------#
# getFiles method: Obtains a list with the name of the files  #
# that will be used as input for a given job "step"           #        
# returns a string with the list                              # 
#-------------------------------------------------------------#
def getFiles(step, FilesPerJob, NumberOfJobs, OverloadedJobs, listOfFiles):
  
  if(step <= NumberOfJobs-OverloadedJobs):
    first = step*FilesPerJob
  else:
    first = FilesPerJob*(NumberOfJobs-OverloadedJobs)+(FilesPerJob+1)*(step-(NumberOfJobs-OverloadedJobs))-1

  counter = 0
  files = []
  for line in listOfFiles:
    if(step <= NumberOfJobs-OverloadedJobs):
      if(counter >= first and first+FilesPerJob > counter):
        files.append(line)
    else:
      if(counter >= first and first+FilesPerJob+1 > counter):
        files.append(line) 
    counter = counter + 1    
  
  return files
  #Check the ending in case it has a ',' 
  #if(files[-1][-2] == ','):
  #  files[-1] = files[-1][0:-2]
  #stringCad = ""
  #for fileLine in files:
  #  stringCad = stringCad + fileLine
  #return stringCad 
      

#-------------------------------------------------------------#
# createDirectory method: Creates a directory                 #
# returns 0 if everything was all right                       #
#-------------------------------------------------------------# 
def createDirectory(nameOfDirectory):
  result = os.system("mkdir " + nameOfDirectory)
  result2 = os.system("cd " + nameOfDirectory)
  return result 

#-------------------------------------------------------------#
# fixPath method: Just makes sure the path is ending          #
# with slash. Otherwise appends it                            #
#-------------------------------------------------------------# 
def fixPath(path):
  if(path[-1] == '/'):
    return path
  else:
    return path + "/"


#-------------------------------------------------------------#
# createCMSConf method: creates CMS configuration file        #
#                                                             #
#-------------------------------------------------------------# 
def createCMSConf(step, nameOfDirectory, releasePath, nameOfConf, inputString, executablePath, nameOfSRM, hpname, task):

  CFGFile = open(nameOfConf)
  cfgText = CFGFile.read()

  cadFile = "\""
  for file in inputString:
    cadFile = cadFile + file + " " 
  cadFile = cadFile + "\"" 

  nameOfFolder = task[0]
  if(nameOfFolder[len(nameOfFolder)-1] == "/"):
    nameOfFolder = nameOfFolder[0:len(nameOfFolder)-1]
  value = nameOfFolder.rfind("/")
  taskName = nameOfFolder[value+1:len(nameOfFolder)] 


  newcfgText1 = cfgText.replace("sourcefile" , cadFile)
  newcfgText2 = newcfgText1.replace("jobdir" , taskName + "/" + nameOfSRM + "_" + str(step))
  newcfgText3 = newcfgText2.replace("exefile", executablePath)
  newcfgText4 = newcfgText3.replace("srmdir", nameOfSRM + "/" + taskName)
  newcfgText5 = newcfgText4.replace("hpname", hpname)
  newcfgText6 = newcfgText5.replace("argumentsname", "\"" + task[1] + "\"")
  newcfgText7 = newcfgText6.replace("therelease", releasePath)

  nameOfConf2 = nameOfConf.replace(".", "_"+str(step)+ ".")
 
  outputCFGFile = open(nameOfDirectory + taskName + "/" + nameOfConf2, "w")
  outputCFGFile.write(newcfgText7)
  CFGFile.close()
  outputCFGFile.close()
  
  outputName = "output_" + str(step) + ".root"
  #Send to the queue
  print "qsub -e /tmp/ -o /tmp/ " + nameOfDirectory + nameOfConf2 + " " + str(step)
  result = os.system("qsub -e /tmp/ -o /tmp/ " + nameOfDirectory + taskName + "/" + nameOfConf2 + " " + str(step))
 # return result

###############################################################
# createJob method: Prepares everything for a single job      #
#                                                             #
############################################################### 
def createJob(step, FilesPerJob, NumberOfJobs, OverloadedJobs, stringInfo, listOfFiles, task):
 
  #Definition of the different substrings needed
  nameOfSourceFile = stringInfo[0]
  releasePath = fixPath(stringInfo[1])
  executablePath = stringInfo[2]
  nameOfConfigurationFile = stringInfo[3]
  nameOfSRMPath = stringInfo[4]
  nameOfCurrentDirectory = stringInfo[5]
  hpname = stringInfo[6]
  
  inputFilesForCMS = getFiles(step, FilesPerJob, NumberOfJobs, OverloadedJobs, listOfFiles)
  if(inputFilesForCMS == ""):
    return "Error: No input files available"
  

  result = createCMSConf(step, nameOfCurrentDirectory, releasePath, 
           nameOfConfigurationFile, inputFilesForCMS, executablePath, nameOfSRMPath, hpname, task)

  if(result != 0):
    return "Error creating Python-Configuration file"
  return "OK"

###############################################################
# parseInputFile method: Parses the input file                #
# More options could be added if needed                       #
############################################################### 

def parseInputFile(name):   
  showMessage("Parsing " + name + " file ...")
  file = open(name)
  stringInfo = [[],[],[],[],[], [], []]
  for line in file.readlines():
    splitLine = line.split()
    if(splitLine[0] == "ReleasePath"):
      stringInfo[1] = splitLine[1]
    if(splitLine[0] == "Executable"):
      stringInfo[2] = splitLine[1]
    if(splitLine[0] == "NameOfConf"):
      stringInfo[3] = splitLine[1]
    if(splitLine[0] == "NameOfSource"):
      stringInfo[0] = splitLine[1]
    if(splitLine[0] == "SrmPath"):
      stringInfo[4] = splitLine[1]
    if(splitLine[0] == "WorkPath"):
      stringInfo[5] = splitLine[1]
    if(splitLine[0] == "HPName"):
      stringInfo[6] = splitLine[1]

  file.close()

  for i in range(0,len(stringInfo)):
    if(stringInfo[i] == []):
      return "Error"   
  showMessage("Configuration Parameters:")
  showMessage("ReleasePath = " + stringInfo[1])
  showMessage("Executable = " + stringInfo[2])
  showMessage("NameOfConf = " + stringInfo[3])
  showMessage("NameOfSource = " + stringInfo[0])
  showMessage("SrmPath = " + stringInfo[4])
  showMessage("WorkPath = " + stringInfo[5])
  showMessage("HPName = " + stringInfo[6])
  return stringInfo

  
###############################################################
# showMessage method: Adds label and print message            #
#                                                             #
############################################################### 
def showMessage(message):
  print "[runManager] " + message


###############################################################
# printUsage method: Prints how to use the program            #
#                                                             #
###############################################################
def printUsage():
  print "./runMan configuration.txt"
  print "configuration.txt should look like this:"
  print "---------------------------------------"
  print "ReleasePath path"
  print "Executable name"
  print "NameOfConf path"
  print "NameOfSource path"
  print "SrmPath path"
  print "WorkPath path"
  print "HPName path"
  print "---------------------------------------"
 
###############################################################
# List of tasks: from the source file                         #
#                                                             #
###############################################################
def getListOfTasks(nameOfFile):

  file = open(nameOfFile)
  
  tasks = []

  for line in file.readlines():
    if(line[0] == "#"):
      continue
    splitedLine = line.split()
    argumentBegin = line.find("'")
    argumentEnd = line.rfind("'")

    if(argumentBegin == -1 or argumentEnd == -1 or argumentBegin == argumentEnd):
      showMessage("Problem parsing task " + line)
    else:  
      arguments = line[argumentBegin+1:argumentEnd]
      endLine = line[argumentEnd+1:len(line)]
      splitedEndLine = endLine.split()
      folder = splitedLine[0] 
      numberOfJobs = splitedEndLine[0]
      numberOfFilesPerJob = splitedEndLine[1]
      task = [folder, arguments, numberOfJobs, numberOfFilesPerJob]
      if(numberOfJobs == '-1' or numberOfFilesPerJob == '-1'):
        tasks.append(task)

  return tasks  


###############################################################
# process: process a single task                              #
#                                                             #
###############################################################
def process(task, conf):
 
 
  dcapPath = "dcap://t3se01.psi.ch:22125/"
  srmPath = "srm://t3se01.psi.ch:8443/srm/managerv2?SFN=/pnfs/psi.ch/cms/trivcat/store/user/"
  folderToList = srmPath + task[0]
  theList = os.popen("lcg-ls " + folderToList)
  list = theList.readlines()
  for li in theList:
    if(li.find("not found") != -1):
      showMessage("Folder not found")
      return "Error"
  
  numberOfFiles = len(list)
  if(numberOfFiles == 0):
    showMessage("No files found")
    return "Error"


  correctList = [];
  for fileName in list:
    auxiliar = dcapPath + fileName
    auxiliar = auxiliar[0:len(auxiliar)-1]
    correctList.append(auxiliar)

  if(task[2] != "-1"):
    FilesPerJob = int(numberOfFiles/int(task[2]))
    OverloadedJobs = numberOfFiles + 1 - FilesPerJob * int(task[2])
    NumberOfJobs = int(task[2])
  else:
    FilesPerJob = int(task[3])
    NumberOfJobs = int(numberOfFiles/FilesPerJob) + 1
    OverloadedJobs = numberOfFiles + 1 - FilesPerJob * NumberOfJobs  

  nameOfFolder = task[0]
  if(nameOfFolder[len(nameOfFolder)-1] == "/"):
    nameOfFolder = nameOfFolder[0:len(nameOfFolder)-1]
  value = nameOfFolder.rfind("/")
  taskName = nameOfFolder[value+1:len(nameOfFolder)]  
  os.system("mkdir " + taskName)

  showMessage(str(NumberOfJobs) + " with " + str(FilesPerJob) + " each will be created")

  if(FilesPerJob > 0):
    for step in range(0, NumberOfJobs):
      showMessage("Producing job: " + str(step))
    
 
      createJob(step, FilesPerJob, NumberOfJobs, OverloadedJobs, conf, correctList, task) 





##################################################################################
#                                  MAIN                                          #
##################################################################################

if(len(sys.argv) != 2 or sys.argv[1] == "-h"):
  printUsage()
else:
  result = parseInputFile(sys.argv[1])
  if(result == "Error"):
    showMessage("Error parsing input file")
  else:
    listOfTasks = getListOfTasks(result[0])
    for l in listOfTasks:
      showMessage("Processing " + l[0])
      output = process(l, result)
   

