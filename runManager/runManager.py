#!/usr/bin/python


###############################################################
#                       runMan.py                             #
###############################################################
# + Python script which allows to manage multiple jobs in the #
# context of a lsfbatch system.                               #
#                                                             #
# + type runMan -h to see a description of the usage          #   
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
def getFiles(step, FilesPerJob, NumberOfJobs, OverloadedJobs, name):
  file = open(name)
  counter = 0
  first = 0
  if(step <= NumberOfJobs-OverloadedJobs):
    first = step*FilesPerJob
  else:
    first = FilesPerJob*(NumberOfJobs-OverloadedJobs)+(FilesPerJob+1)*(step-(NumberOfJobs-OverloadedJobs))-1
  files = []
  for line in file.readlines():
    #if(counter >= first and first+FilesPerJob+1 > counter):
    if(step <= NumberOfJobs-OverloadedJobs):
      if(counter >= first and first+FilesPerJob > counter):
        files.append(line)
    else:
      if(counter >= first and first+FilesPerJob+1 > counter):
        files.append(line) 
    counter = counter + 1    
  #Check the ending in case it has a ',' 
  if(files[-1][-2] == ','):
    files[-1] = files[-1][0:-2]
  stringCad = ""
  for fileLine in files:
    stringCad = stringCad + fileLine
  return stringCad 
      
#-------------------------------------------------------------#
# numberFiles method: Returns the number of lines that a file #
# contains.                                                   #
# FIXME: There are smarter ways of doing this                 #
#-------------------------------------------------------------#  
def numberFiles(name):
  file = open(name)
  counter = 0
  for line in file.readlines():
    counter = counter + 1
  return counter

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
# fixPath method: Just makes sure the path is ending          #
# with slash. Otherwise appends it                            #
#-------------------------------------------------------------# 
def createLaunch(nameOfDirectory, releasePath, workingPath, nameOfConf, castorPath, outputFileName):
  outputLaunch = open(nameOfDirectory +  "launch.sh", "w")
  outputLaunch.write("#!/bin/sh\n")
  outputLaunch.write("pushd " + releasePath + "\n")
  outputLaunch.write("eval `scramv1 run -sh` \n")
  outputLaunch.write("pushd" + "\n")
  outputLaunch.write("cp " + workingPath + "/" + nameOfDirectory + nameOfConf + " .\n") 
  outputLaunch.write("cmsRun " + nameOfConf + "\n")
  outputLaunch.write("rfmkdir -p " + castorPath + nameOfDirectory + "\n")
  outputLaunch.write("rfcp " + outputFileName + " " + castorPath + nameOfDirectory + "\n") 
  outputLaunch.close() 
  #Give execution permission 
  result = os.system("chmod +x " + nameOfDirectory + "launch.sh")
  return result

#-------------------------------------------------------------#
# createCMSConf method: creates CMS configuration file        #
#                                                             #
#-------------------------------------------------------------# 
def createCMSConf(step, nameOfDirectory, releasePath, nameOfConf, inputString, executablePath, nameOfSRM, hpname):

  CFGFile = open(nameOfConf)
  cfgText = CFGFile.read()

  cadFile = "\""
  for file in inputString:
    if(file == "\n"):
      file = " "
    cadFile = cadFile + file 
  cadFile = cadFile + "\"" 

  newcfgText1 = cfgText.replace("sourcefile" , cadFile)
  newcfgText2 = newcfgText1.replace("jobdir" , nameOfSRM + "_" + str(step))
  newcfgText3 = newcfgText2.replace("exefile", executablePath)
  newcfgText4 = newcfgText3.replace("srmdir", nameOfSRM)
  newcfgText5 = newcfgText4.replace("hpname", hpname)

  nameOfConf2 = nameOfConf.replace(".", "_"+str(step)+ ".")
 
  outputCFGFile = open(nameOfDirectory + nameOfConf2, "w")
  outputCFGFile.write(newcfgText5)
  CFGFile.close()
  outputCFGFile.close()
  
  outputName = "output_" + str(step) + ".root"
  #Send to the queue
  print "qsub " + nameOfDirectory + nameOfConf2 + " " + str(step)
  result = os.system("qsub " + nameOfDirectory + nameOfConf2 + " " + str(step))
 # return result

#-------------------------------------------------------------#
# createJob method: Prepares everything for a single job      #
# with slash. Otherwise appends it                            #
#-------------------------------------------------------------# 
def createJob(step, FilesPerJob, NumberOfJobs, OverloadedJobs, stringInfo, srmPath):
 
  #Definition of the different substrings needed
  nameOfSourceFile = stringInfo[0]
  releasePath = fixPath(stringInfo[1])
  executablePath = stringInfo[2]
  nameOfConfigurationFile = stringInfo[3]
  nameOfSRMPath = stringInfo[5]
  nameOfCurrentDirectory = stringInfo[6]
  hpname = stringInfo[7]
  #result = createDirectory(nameOfCurrentDirectory)
  #if(result != 0):
  #  return "Error creating directory: " + nameOfCurrentDirectory
  
  #result = createLaunch(nameOfCurrentDirectory, releasePath,
  #         workingPath, nameOfConfigurationFile, CASTORPath, outputFileName)
  #if(result != 0):
  #  return "Error creating Launch.sh"
  
  inputFilesForCMS = getFiles(step, FilesPerJob, NumberOfJobs, OverloadedJobs, nameOfSourceFile)
  if(inputFilesForCMS == ""):
    return "Error: No input files available"

  result = createCMSConf(step, nameOfCurrentDirectory, releasePath, 
           nameOfConfigurationFile, inputFilesForCMS, executablePath, nameOfSRMPath, hpname)

  if(result != 0):
    return "Error creating Python-Configuration file"
  return "OK"

#-------------------------------------------------------------#
# parseInputFile method: Parses the input file                #
# More options could be added if needed                       #
#-------------------------------------------------------------# 
def parseInputFile(name):   
  showMessage("Parsing " + name + " file ...")
  file = open(name)
  stringInfo = [[], [],[],[],[],[], [], []]
  for line in file.readlines():
    splitLine = line.split()
    if(splitLine[0] == "NumberOfJobs"):
      stringInfo[4] = splitLine[1]
    if(splitLine[0] == "ReleasePath"):
      stringInfo[1] = splitLine[1]
    if(splitLine[0] == "Executable"):
      stringInfo[2] = splitLine[1]
    if(splitLine[0] == "NameOfConf"):
      stringInfo[3] = splitLine[1]
    if(splitLine[0] == "NameOfSource"):
      stringInfo[0] = splitLine[1]
    if(splitLine[0] == "SrmPath"):
      stringInfo[5] = splitLine[1]
    if(splitLine[0] == "WorkPath"):
      stringInfo[6] = splitLine[1]
    if(splitLine[0] == "HPName"):
      stringInfo[7] = splitLine[1]

  file.close()
  for i in range(0,len(stringInfo)):
    if(stringInfo[i] == []):
      return "Error"   
  showMessage("Configuration Parameters:")
  showMessage("ReleasePath = " + stringInfo[1])
  showMessage("Executable = " + stringInfo[2])
  showMessage("NameOfConf = " + stringInfo[3])
  showMessage("NumberOfJobs = " + stringInfo[4])
  showMessage("NameOfSource = " + stringInfo[0])
  showMessage("SrmPath = " + stringInfo[5])
  showMessage("WorkPath = " + stringInfo[6])
  showMessage("HPName = " + stringInfo[7])
  return stringInfo

  
#-------------------------------------------------------------#
# showMessage method: Adds label and print message            #
#                                                             #
#-------------------------------------------------------------# 
def showMessage(message):
  print "[runMan] " + message


#-------------------------------------------------------------#
# printUsage method: Prints how to use the program            #
#                                                             #
#-------------------------------------------------------------# 
def printUsage():
  print "./runMan configuration.txt"
  print "configuration.txt should look like this:"
  print "---------------------------------------"
  print "NumberOfJobs number"
  print "ReleasePath path"
  print "Executable name"
  print "NameOfConf path"
  print "NameOfSource path"
  print "SrmPath path"
  print "WorkPath path"
  print "HPName path"
  print "---------------------------------------"
 

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
    showMessage("Calculating blocks ...")
    numberOfFile = numberFiles(result[0])
    FilesPerJob = int(numberOfFile/int(result[4]))
    OverloadedJobs = numberOfFile - FilesPerJob * int(result[4])
    srmPath = result[5]
    workPath = result[6]
    res = createDirectory(workPath)
    if(res == 0):
      showMessage("Cannot create directory")
      exit 
    
    showMessage(result[4] + " with " + str(FilesPerJob) + " each will be created")
    
    if(FilesPerJob > 0):
      for step in range(0, int(result[4])):
        showMessage("Producing job: " + str(step))
        createJob(step, FilesPerJob, int(result[4]), OverloadedJobs, result, srmPath) 



