#!/usr/bin/python

import sys
import os

print "-----------------------------------------------------"
print "-------- Muon HLT DQM Offline validation ------------"
print "-----------------------------------------------------"

fullsimTest = raw_input("Name of the fullsim release to be validated: ")
fullsimRef = raw_input("Name of the reference fullsim release: ")
fastsimTest = raw_input("Name of the fastsim release to be validated: ")
fastsimRef = raw_input("Name of the reference fastsim release: ")
dataTest = raw_input("Name of the data release to be validated: ")
dataTestVersion = raw_input("Name of the version for data release. Example: v3. ")
dataRef = raw_input("Name of the reference data release: ")
dataRefVersion = raw_input("Name of the version for the reference data release. Example: v1. ")


###Some definitions
endDQM = "/DQM"
endFS = "/GEN-SIM-DIGI-RECO"

###List of samples to validate
relvaljpsi = "/RelValJpsiMM/"
relvalsinglemupt1000 = "/RelValSingleMuPt1000/"
relvalsinglemupt100 = "/RelValSingleMuPt100/"
relvalsinglemupt10 = "/RelValSingleMuPt10/"
relvalwm = "/RelValWM/"
relvalzmm = "/RelValZMM/"
relvalzMuB11r = "177790"
relvalzMuB11s = "/DoubleMu/"
relvalzMuB11eTest = "_zMu2011B-"+dataTestVersion+"/DQM"
relvalzMuB11eRef = "_zMu2011B-"+dataRefVersion+"/DQM"
relvalzMuA11r = "172802"
relvalzMuA11s = "/DoubleMu/"
relvalzMuA11eTest = "_zMu2011A-"+dataTestVersion+"/DQM"
relvalzMuA11eRef = "_zMu2011A-"+dataRefVersion+"/DQM"
relvalMuA11r = "165121"
relvalMuA11s = "/SingleMu/"
relvalMuA11eTest = "_mu2011A-"+dataTestVersion+"/DQM"
relvalMuA11eRef = "_mu2011A-"+dataRefVersion+"/DQM"

relvalendreference = "_Reference-v2/DQM"
relvalendupdate = "_Update-v2/DQM"


tagMC = "python makeLinks2.py mc HLT_Mu5 1 "
tag = "python makeLinks2.py data HLT_IsoMu24 "

commandforjpsi = tagMC + relvaljpsi + fullsimTest + endDQM + " " + relvaljpsi + fullsimRef + endDQM
commandforpt1000 = tagMC + relvalsinglemupt1000 + fullsimTest + endDQM + " " + relvalsinglemupt1000 + fullsimRef + endDQM
commandforpt100 = tagMC + relvalsinglemupt100 + fullsimTest + endDQM + " " + relvalsinglemupt100 + fullsimRef + endDQM
commandforpt10 = tagMC + relvalsinglemupt10 + fullsimTest + endDQM + " " + relvalsinglemupt10 + fullsimRef + endDQM
commandforwm = tagMC + relvalwm + fullsimTest + endDQM + " " + relvalwm + fullsimRef + endDQM
commandforzmm = tagMC + relvalzmm + fullsimTest + endDQM + " " + relvalzmm + fullsimRef + endDQM
commandforpt100FS = tagMC + relvalsinglemupt100 + fastsimTest + endDQM + " " + relvalsinglemupt100 + fastsimRef + endDQM
commandforpt10FS = tagMC + relvalsinglemupt10 + fastsimTest + endDQM + " " + relvalsinglemupt10 + fastsimRef + endDQM
commandforzMuB11 = tag + relvalzMuB11r + " " + relvalzMuB11s + dataTest + relvalzMuB11eTest + " " + relvalzMuB11s + dataRef + relvalzMuB11eRef
commandforzMuA11 = tag + relvalzMuA11r + " " + relvalzMuA11s + dataTest + relvalzMuA11eTest + " " + relvalzMuA11s + dataRef + relvalzMuA11eRef
commandforMuA11 = tag + relvalMuA11r + " " + relvalMuA11s + dataTest + relvalMuA11eTest +  " " + relvalMuA11s + dataRef + relvalMuA11eRef

outputjpsi = os.popen(commandforjpsi).read()
outputpt1000 = os.popen(commandforpt1000).read()
outputpt100 = os.popen(commandforpt100).read()
outputpt10 = os.popen(commandforpt10).read()
outputwm = os.popen(commandforwm).read()
outputzmm = os.popen(commandforzmm).read()
outputpt100FS = os.popen(commandforpt100FS).read()
outputpt10FS = os.popen(commandforpt10FS).read()
outputzMuB11 = os.popen(commandforzMuB11).read()
outputzMuA11 = os.popen(commandforzMuA11).read()
outputMuA11 = os.popen(commandforMuA11).read()


fullsim = fullsimTest + "_To_" + fullsimRef + "_FULLSIM.txt"
fastsim = fastsimTest + "_To_" + fastsimRef + "_FASTSIM.txt"
data = dataTest + "_To_" + dataRef + "_DATA.txt"

 

fFULLSIM = open(fullsim, "w")
fFULLSIM.write(outputjpsi)
fFULLSIM.write(outputpt1000)
fFULLSIM.write(outputpt100)
fFULLSIM.write(outputwm)
fFULLSIM.write(outputzmm)
fFULLSIM.close()


fFASTSIM = open(fastsim, "w")
fFASTSIM.write(outputpt100FS)
fFASTSIM.write(outputpt10FS)
fFASTSIM.close()


fDATA = open(data, "w")
fDATA.write(outputzMuB11)
fDATA.write(outputzMuA11)
fDATA.write(outputMuA11)
fDATA.close()


print "The output was saved in the files:"
print fullsim
print fastsim
print data




