#!/usr/bin/python

import sys
import os

def getDataSet(input):

  list = []
  for line in input:
    if line.find("/DQM") != -1:
      nameRawList = line.split()
      nameRaw = nameRawList[0]
      name = nameRaw[1:len(nameRaw)]
      if name.find("/DoubleMu/") != -1:
        if name.find("zMu2011B") != -1:
          list.append(name)
        if name.find("zMu2011A") != -1:
          list.append(name)

      if name.find("SingleMu") != -1:
        if name.find("mu2011A") != -1:
          list.append(name)

  return list


def getFullSimSet(input):

  list = []
  for line in input:
    if line.find("/DQM") != -1 and line.find("FastSim") == -1:
      nameRawList = line.split()
      nameRaw = nameRawList[0]
      name = nameRaw[1:len(nameRaw)]
      if name.find("RelValSingleMuPt1000") != -1 or name.find("RelValSingleMuPt100") != -1 or name.find("RelValSingleMuPt10") != -1 or name.find("RelValJpsiMM") != -1 or name.find("RelValWM") != -1 or name.find("RelValZMM") != -1:
         list.append(name)


  return list


def getFastSimSet(input):

  list = []
  for line in input:
    if line.find("/GEN-SIM-DIGI-RECO") != -1 and line.find("FastSim") != -1:
      nameRawList = line.split()
      nameRaw = nameRawList[0]
      name = nameRaw[1:len(nameRaw)]
      if name.find("RelValSingleMuPt100") != -1 or name.find("RelValSingleMuPt10") != -1:
        list.append(name)

    
  return list


def validateData(target, reference):

  if target == [] or reference == []:
    print "No Data samples to validate"
    return
  
  textTarget = target[0]
  b = textTarget.find("/", 1, len(textTarget))
  e = textTarget.find("/", b+1, len(textTarget))
  nameTarget = textTarget[b+1:e]
  textRef = reference[0]
  b = textRef.find("/", 1, len(textRef))
  e = textRef.find("/", b+1, len(textRef))
  nameRef = textRef[b+1:e]

  filename = nameTarget + "_To_" + nameRef + "_DATA.txt"
  theFile = open(filename, "w")

  tag = "python makeLinks2.py data HLT_IsoMu24 "
  command = ""
  for line in target:
    for line2 in reference:
      if line.find("/DoubleMu/") != -1 and line.find("zMu2011B") != -1 and line2.find("/DoubleMu/") != -1 and line2.find("zMu2011B") != -1:
        command = tag + "177790 " + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("/DoubleMu/") != -1 and line.find("zMu2011A") != -1 and line2.find("/DoubleMu/") != -1 and line2.find("zMu2011A") != -1:
        command = tag + "172802 " + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("/SingleMu/") != -1 and line.find("mu2011A") != -1 and line2.find("/SingleMu/") != -1 and line2.find("mu2011A") != -1:
        command = tag + "165121 " + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)

  theFile.close()


def validateFullSim(target, reference):

  if target == [] or reference == []:
    print "No FullSim samples to validate"
    return

  textTarget = target[0]
  b = textTarget.find("/", 1, len(textTarget))
  e = textTarget.find("/", b+1, len(textTarget))
  nameTarget = textTarget[b+1:e]
  textRef = reference[0]
  b = textRef.find("/", 1, len(textRef))
  e = textRef.find("/", b+1, len(textRef))
  nameRef = textRef[b+1:e]

  filename = nameTarget + "_To_" + nameRef + "_FULLSIM.txt"
  theFile = open(filename, "w")

  tag = "python makeLinks2.py mc HLT_Mu5 1 "
  command = ""
  for line in target:
    for line2 in reference:
      if line.find("RelValSingleMuPt1000") != -1 and line2.find("RelValSingleMuPt1000") != -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValSingleMuPt100") != -1 and line.find("RelValSingleMuPt1000") == -1 and line2.find("RelValSingleMuPt100") != -1 and line2.find("RelValSingleMuPt1000") == -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValSingleMuPt10") != -1 and line.find("RelValSingleMuPt100") == -1 and line2.find("RelValSingleMuPt10") != -1 and line2.find("RelValSingleMuPt100") == -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValJpsiMM") != -1 and line2.find("RelValJpsiMM") != -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValWM") != -1 and line2.find("RelValWM") != -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValZMM") != -1 and line2.find("RelValZMM") != -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)

    
  theFile.close()


def validateFastSim(target, reference):

  if target == [] or reference == []:
    print "No Fastsim samples to validate"
    return

  textTarget = target[0]
  b = textTarget.find("/", 1, len(textTarget))
  e = textTarget.find("/", b+1, len(textTarget))
  nameTarget = textTarget[b+1:e]
  textRef = reference[0]
  b = textRef.find("/", 1, len(textRef))
  e = textRef.find("/", b+1, len(textRef))
  nameRef = textRef[b+1:e]

  filename = nameTarget + "_To_" + nameRef + "_FASTSIM.txt"
  theFile = open(filename, "w")

  tag = "python makeLinks2.py mc HLT_Mu5 1 "
  command = ""
  for line in target:
    for line2 in reference:
      if line.find("RelValSingleMuPt100") != -1 and line2.find("RelValSingleMuPt100") != -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
      if line.find("RelValSingleMuPt10") != -1 and line.find("RelValSingleMuPt100") == -1 and line2.find("RelValSingleMuPt10") != -1 and line2.find("RelValSingleMuPt100") == -1:
        command = tag + line + " " + line2
        output = os.popen(command).read()
        theFile.write(output)
    
  theFile.close()







print "-----------------------------------------------------"
print "-------- Muon HLT DQM Offline validation ------------"
print "-----------------------------------------------------"


if len(sys.argv) != 3:
  print "Wrong number of arguments"
  print "Usage: python makeTagAdvance.py linkToTxTfile linkToTxtfileReference"
  sys.exit(0)

output = os.popen("wget " + sys.argv[1] + " -O myListOfFile.txt")
theOutput = output.read()
if theOutput.find("unable") != -1:
  print "The address of the target is not valid. Please check."
  sys.exit(0)

file = open("myListOfFile.txt")
lines = file.readlines()
file.close()
rmtarget = os.popen("rm myListOfFile.txt")


outputReference = os.popen("wget " + sys.argv[2] + " -O myListOfFileRef.txt")
theOutputReference = outputReference.read()
if theOutputReference.find("unable") != -1:
  print "The address of the reference is not valid. Please check."
  sys.exit(0)

fileRef = open("myListOfFileRef.txt")
linesRef = fileRef.readlines()
fileRef.close()
rmref = os.popen("rm myListOfFileRef.txt")


namesData = getDataSet(lines)
namesFullSim = getFullSimSet(lines)
namesFastSim = getFastSimSet(lines)

print namesFastSim

namesDataRef = getDataSet(linesRef)
namesFullSimRef = getFullSimSet(linesRef)
namesFastSimRef = getFastSimSet(linesRef)

validateData(namesData, namesDataRef)
validateFullSim(namesFullSim, namesFullSimRef)
validateFastSim(namesFastSim, namesFastSimRef)



