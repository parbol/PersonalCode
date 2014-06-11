#!/bin/python

############################################################################################################################### 
#                                                      InsertSLHA.py                                                          #  
############################################################################################################################### 
#  +Usage: ./InsertSLHA.py lhefile.lhe shlafile.slha                                                                          #
#                                                                                                                             #
#  +Description: Insert SLHA section in lhe file and other tools                                                              # 
#                                                                                                                             #
#  +Feedback to: pablom@cern.ch                                                                                               #
############################################################################################################################### 


#Needed libraries
import sys
import os


#-------------------------------------------------------------#
# ShowMessage method: Show MakeScan banner                    #
#-------------------------------------------------------------#
def ShowMessage(message):
  print "[InserSLHA] " + message


#-------------------------------------------------------------#
# printUsage method: Prints how to use the program #
#-------------------------------------------------------------#
def printUsage():
  print "./InsertSLHA.py lhefile.lhe shlafile.slha output.lhe"



#-------------------------------------------------------------#
# Main Section                                                #
#-------------------------------------------------------------#
if(len(sys.argv) == 1):
  printUsage()
  sys.exit()

if(sys.argv[1] == "-h"):
  printUsage()
  sys.exit()

#Insert SLHA in undecayed LHE file before pythia decays 
if(sys.argv[1] == "--slha"):
  if(len(sys.argv) == 5):
    slha_text = open(sys.argv[3]).readlines()
    lhe_text = open(sys.argv[2]).readlines()
    lhe = open(sys.argv[4], "w")
    #How many events
    nevents = 0
    for l in lhe_text:
      if(l.find("<event>") != -1):
        nevents = nevents + 1
    replace_nevents = str(nevents) + "       = nevents ! Number of unweighted events requested\n"
    #Replace number of events
    counter = 0
    for l in lhe_text:
      if(l.find("nevents") != -1):
        lhe_text[counter] = replace_nevents
        break
      counter = counter + 1 

    positionOfTag = 0
    for l in lhe_text:
      if(l.find("<slha>") != -1): 
        break
      positionOfTag = positionOfTag + 1
    positionOfNoTag = 0
    for l in lhe_text:
      if(l.find("</slha>") != -1): 
        break
      positionOfNoTag = positionOfNoTag + 1

    for counter in range(0, positionOfTag+1):
      lhe.write(lhe_text[counter])
    for l in slha_text:
      lhe.write(l)
    for counter in range(positionOfNoTag, len(lhe_text)):
      lhe.write(lhe_text[counter])
        
    lhe.close()        
  else:
    printUsage()
    sys.exit()

#Tag the processed file and insert the banner
if(sys.argv[1] == "--lhe"):
  if(len(sys.argv) == 7):
    model_name = sys.argv[3]
    banner = open(sys.argv[4]).readlines()
    
    initial_lhe = open(sys.argv[5]).readlines()
    lines_to_add = []
    counter = 0
    for l in initial_lhe:
      if(l.find("</event>") != -1):
        lines_to_add.append(initial_lhe[counter-1])
      counter = counter + 1    
    
    final_lhe = open(sys.argv[6], "w")
    for l in banner:
      final_lhe.write(l)
    lhe_text = open(sys.argv[2]).readlines()
    the_write = 0
    counter = 0
    for l in lhe_text:
      if(l.find("<event>") != -1):
        the_write = 1 
      if(l.find("</event>") != -1):
        final_lhe.write(lines_to_add[counter])
        counter = counter + 1
        final_lhe.write("# model " + model_name + "\n")
      if(the_write == 1):
        final_lhe.write(l)
  else:
    printUsage()
    sys.exit()


