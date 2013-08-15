#!/usr/bin/python

############################################################
################## COCOA simulation suite ##################
############################################################
##### @ Usage: COCOAss.py --help     #######################
##### @ Author: Pablo Martinez       #######################
##### @ Feedback to: pablom@cern.ch  #######################
##### @ Date: 29 July 2008           #######################
############################################################ 


#List of modules used by the program
#COCOAss modules
from InputParser import InputParser
from TagExpander import TagExpander
from JobManager import JobManager
from OutputModule import OutputModule
#Python modules
import string
import sys



########################## Begin ############################


if(len(sys.argv) == 2 and sys.argv[1] == "--help"):
  print "Usage: ./COCOAss cocoaInputFile.txt baseCOCOADescriptionFile output.txt"
  sys.exit()
if(len(sys.argv) != 4):
  print "Bad usage. Type ./COCOAss --help"
  sys.exit()

#Names 
nameOfCocoa = sys.argv[2]
nameOfInput = sys.argv[1]
nameOfCVS = sys.argv[3]

#InputParser
myParser = InputParser(nameOfInput, nameOfCocoa)
#TagExpander
myExpander = TagExpander(myParser.read())
expanded = myExpander.expand()
#JobManager
manager = JobManager(nameOfCocoa, expanded) 
output = manager.loop()
#WriteOutput
outputObject = OutputModule(nameOfCVS, output)
outputObject.write()























