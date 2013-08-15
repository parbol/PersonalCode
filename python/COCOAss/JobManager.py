
import os

from COCOAParser import COCOAParser
from COCOAOutputParser import COCOAOutputParser

#==================== Class JobManager ====================#
##################### Manage Jobs ##########################
#==========================================================# 

class JobManager:
  def __init__(self, __nameOfCocoa, __listOfCombinations):
    self.nameOfCocoaFile = __nameOfCocoa
    self.listOfCombinations = __listOfCombinations[0]
    self.listOfOutputVariables = __listOfCombinations[1]  	         
    f = open(self.nameOfCocoaFile)
    self.cocoaFile = f.readlines()
    self.finalOutput = []
    print "[JobManager] JobManager Created!"
    
  def loop(self):
    if(self.checkOK() != 0):
      for tag in self.listOfCombinations:
        print "[JobManager] Working with input tag:"
        print tag
        output = self.evolve(tag)
        if(output == "Error"):
          print "[JobManager]: An error ocurred. Skipping result"
        else:
          print "[JobManager] Output produced: "
          print output
          combinedOutput = []
          for line in tag:
            tri = [line[2], line[3], line[4]]
            combinedOutput.append(tri)
          for line in output:
            tri = line
            combinedOutput.append(tri)
          self.finalOutput.append(combinedOutput)
    return self.finalOutput

  def evolve(self, tag):
    self.createCocoaFile(tag)
    self.runCocoaFile()
    output = self.readOutput()
    self.clean()
    return output
  
  def clean(self):
    os.system("rm output.txt")
    os.system("rm descriptionFile.txt")
    os.system("rm report.out")

  def readOutput(self):
    listOfVars = []
    myoutputParser = COCOAOutputParser("report.out")
    if(myoutputParser.numberOfIterations != ""):
      pair = ["chi2", "", myoutputParser.chi2]
      listOfVars.append(pair)
      pair = ["numberOfIterations", "", myoutputParser.numberOfIterations]
      listOfVars.append(pair)
    for var in self.listOfOutputVariables:
      value = myoutputParser.takeValue(var[2], var[3])
      if(value != "Error"):
        pair = [var[2], var[3], value]
        listOfVars.append(pair)
    return listOfVars
  
  def createCocoaFile(self,tag):
    copyOfCocoaFile = self.cocoaFile[:]
    myParser = COCOAParser(copyOfCocoaFile)
    for line in tag:
      status = myParser.changeLine(line)
      #if(status != "Error"):
    myParser.write("descriptionFile.txt")

  def runCocoaFile(self):
    os.system("cocoa descriptionFile.txt > output.txt")

  def checkOK(self):
    if(self.cocoaFile == [] or self.listOfCombinations == [] or self.listOfOutputVariables == []):
      return 0
    return 1
    
#====================== End of Class ======================#    
