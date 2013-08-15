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
def getFiles(step, FilesPerJob, rootFiles):
  counter = 0
  first = step*FilesPerJob
  files = []
  for line in rootFiles:
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
# getNameOfRoots method: Returns the root files               #
# FIXME: There are smarter ways of doing this                 #
#-------------------------------------------------------------#  
def getNameOfRoots(dataset, runs):
  command = "./DDSearchCLI.py --input=\"find file where dataset=" + dataset 
  if(runs != "all"):
    command = command + runs 
  command = command + "\" --limit=-1"
  output = os.popen(command).readlines()
  rootfiles = []
  for line in output:
    if(line.find(".root") != -1):
      rootfiles.append("'" + line[0:-2] + "',\012")
  if(rootfiles == []):
    return "Error"
  else:
    rootfiles[-1] = rootfiles[-1][0:-2]
  return rootfiles     

#-------------------------------------------------------------#
# createDirectory method: Creates a directory                 #
# returns 0 if everything was all right                       #
#-------------------------------------------------------------# 
def createDirectory(nameOfDirectory):
  result = os.system("mkdir " + nameOfDirectory)
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
def createLaunch(nameOfDirectory, releasePath, workingPath, nameOfConf):
  outputLaunch = open(nameOfDirectory +  "launch.sh", "w")
  outputLaunch.write("#!/bin/sh\n")
  outputLaunch.write("cd " + releasePath + "\n")
  outputLaunch.write("eval `scramv1 run -sh` \n")
  outputLaunch.write("cd " + workingPath + nameOfDirectory + "\n")
  outputLaunch.write("cmsRun " + nameOfConf + "\n")
  outputLaunch.close() 
  #Give execution permission 
  result = os.system("chmod +x " + nameOfDirectory + "launch.sh")
  return result

#-------------------------------------------------------------#
# createCMSConf method: creates CMS configuration file        #
#                                                             #
#-------------------------------------------------------------# 
def createCMSConf(nameOfDirectory, releasePath, workingPath, nameOfConf, inputString):
  CFGFile = open(nameOfConf)
  cfgText = CFGFile.read()
  newcfgText = cfgText.replace("nameOfTheFile" , inputString)
  outputCFGFile = open(nameOfDirectory + nameOfConf, "w")
  outputCFGFile.write(newcfgText)
  CFGFile.close()
  outputCFGFile.close()
  #Send to the queue
  result = os.system("bsub -q cmscaf " + workingPath + nameOfDirectory + "launch.sh")
  return result

#-------------------------------------------------------------#
# createJob method: Prepares everything for a single job      #
# with slash. Otherwise appends it                            #
#-------------------------------------------------------------# 
def createJob(step, FilesPerJob, stringInfo, inputFilesForCMS):
  #Definition of the different substrings needed
  releasePath = fixPath(stringInfo[1])
  workingPath = fixPath(stringInfo[2])
  nameOfConfigurationFile = stringInfo[3]
  nameOfCurrentDirectory = fixPath("./job_" + str(step)) 

  result = createDirectory(nameOfCurrentDirectory)
  if(result != 0):
    return "Error creating directory: " + nameOfCurrentDirectory
  
  result = createLaunch(nameOfCurrentDirectory, releasePath,
           workingPath, nameOfConfigurationFile)
  if(result != 0):
    return "Error creating Launch.sh"


  nameOfFiles = getFiles(step, FilesPerJob, inputFilesForCMS)
  
  if(nameOfFiles == ""):
    return "Error: No input files available"
 
  result = createCMSConf(nameOfCurrentDirectory, releasePath, 
           workingPath, nameOfConfigurationFile, nameOfFiles)
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
  stringInfo = [[],[],[],[],[],[]]
  for line in file.readlines():
    if(line[0] == "#"):
      continue
    splitLine = line.split()
    if(splitLine[0] == "NumberOfJobs"):
      stringInfo[4] = splitLine[1]
    if(splitLine[0] == "ReleasePath"):
      stringInfo[1] = splitLine[1]
    if(splitLine[0] == "WorkingPath"):
      stringInfo[2] = splitLine[1]
    if(splitLine[0] == "NameOfConf"):
      stringInfo[3] = splitLine[1]
    if(splitLine[0] == "Dataset"):
      stringInfo[0] = splitLine[1]
    if(splitLine[0] == "Runs"):
      stringInfo[5] = splitLine[1] 
  file.close()
  for i in range(0,len(stringInfo)):
    if(stringInfo[i] == []):
      return "Error"   
  showMessage("Configuration Parameters:")
  showMessage("ReleasePath = " + stringInfo[1])
  showMessage("WorkingPath = " + stringInfo[2])
  showMessage("NameOfConf = " + stringInfo[3])
  showMessage("NumberOfJobs = " + stringInfo[4])
  showMessage("Dataset = " + stringInfo[0])
  showMessage("Runs = " + stringInfo[5])
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
  print "./runManDataset configuration.txt"
  print "configuration.txt should look like this:"
  print "---------------------------------------"
  print "NumberOfJobs number"
  print "ReleasePath path"
  print "WorkingPath path"
  print "NameOfConf path"
  print "Dataset path"
  print "Runs runs(separated with comma, all for all the runs"
  print "---------------------------------------"
  print "A valid cmssw configuration py file should be present"
  print "with the tag nameOfTheFile instead a valid list of input files"
  print "This program uses DDSearchCLI.py available at: "
  print "https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookCommissioningLocateData"
  print "Make sure it is in your working directory" 

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
    rootFiles = getNameOfRoots(result[0], result[5])
    if(rootFiles == "Error"):
      showMessage("Error getting the root files")
      sys.exit(-1)
    numberOfFile = len(rootFiles)
    FilesPerJob = int(numberOfFile/int(result[4]))
    showMessage(result[4] + " with " + str(FilesPerJob) + " each will be created")   
    if(FilesPerJob > 0):
      for step in range(0, int(result[4])):
        showMessage("Producing job: " + str(step))
        createJob(step, FilesPerJob, result, rootFiles) 


 
