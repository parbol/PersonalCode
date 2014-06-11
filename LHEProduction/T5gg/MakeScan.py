#!/bin/python

############################################################################################################################### 
#                                                      MakeScan.py                                                            #  
############################################################################################################################### 
#  +Usage: ./MakeScan.py configuration.txt                                                                                    #
#                                                                                                                             #
#  +Description: Run an SMS scan starting from already produced LHE files (only decays are run)                               # 
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
  print "[MakeScan] " + message


#-------------------------------------------------------------#
# printUsage method: Prints how to use the program #
#-------------------------------------------------------------#
def printUsage():
  print "./MakeScan.py configuration.txt"
  print "---------------------------------------"
  print "LHEDir dir"
  print "LHEName name"
  print "PYTHIATemplate pythia.txt"
  print "LHATemplate lha.txt"
  print "minP1 0"
  print "maxP1 1000"
  print "stepP1 25"
  print "minP2 0"
  print "maxP2 1000"
  print "stepP2 25"
  print "SLHAINSERTName Inser.py"
  print "bannerName bannerTemplate"
  print "leptonFilterName filterName"
  print "---------------------------------------"


#-------------------------------------------------------------#
# parseInputFile method: Parses the input file #
# More options could be added if needed #
#-------------------------------------------------------------#
def parseInputFile(name):
  ShowMessage("Parsing " + name + " file ...")
  file = open(name)
  stringInfo = [[], [], [], [], [], [], [], [], [], [], [], [], [], []]
  for line in file.readlines():
    splitLine = line.split()
    if(splitLine[0] == "LHEDir"):
      stringInfo[0] = splitLine[1]
    if(splitLine[0] == "LHEName"):
      stringInfo[1] = splitLine[1]
    if(splitLine[0] == "PYTHIATemplate"):
      stringInfo[2] = splitLine[1]
    if(splitLine[0] == "LHATemplate"):
      stringInfo[3] = splitLine[1]
    if(splitLine[0] == "MERGERName"):
      stringInfo[4] = splitLine[1]
    if(splitLine[0] == "minP1"):
      stringInfo[5] = splitLine[1]
    if(splitLine[0] == "maxP1"):
      stringInfo[6] = splitLine[1]
    if(splitLine[0] == "stepP1"):
      stringInfo[7] = splitLine[1]
    if(splitLine[0] == "minP2"):
      stringInfo[8] = splitLine[1]
    if(splitLine[0] == "maxP2"):
      stringInfo[9] = splitLine[1]
    if(splitLine[0] == "stepP2"):
      stringInfo[10] = splitLine[1]
    if(splitLine[0] == "SLHAINSERTName"):
      stringInfo[11] = splitLine[1]
    if(splitLine[0] == "bannerName"):
      stringInfo[12] = splitLine[1]
    if(splitLine[0] == "leptonFilterName"):
      stringInfo[13] = splitLine[1]

  file.close()
  for i in range(0,len(stringInfo)):
    if(stringInfo[i] == []):
      return "Error"
  ShowMessage("Configuration Parameters:")
  ShowMessage("LHEDir = " + stringInfo[0])
  ShowMessage("LHEName = " + stringInfo[1])
  ShowMessage("PYTHIATemplate = " + stringInfo[2])
  ShowMessage("LHATemplate = " + stringInfo[3])
  ShowMessage("MERGERName = " + stringInfo[4])
  ShowMessage("minP1 = " + stringInfo[5])
  ShowMessage("maxP1 = " + stringInfo[6])
  ShowMessage("stepP1 = " + stringInfo[7])
  ShowMessage("minP2 = " + stringInfo[8])
  ShowMessage("maxP2 = " + stringInfo[9])
  ShowMessage("stepP2 = " + stringInfo[10])
  ShowMessage("SLHAINSERTName = " + stringInfo[11])
  ShowMessage("bannerName = " + stringInfo[12])
  ShowMessage("leptonFilterName = " + stringInfo[13])
  return stringInfo

#-------------------------------------------------------------#
# getListOfFiles method: Select the files for given masses    #
#-------------------------------------------------------------#

def getListOfFiles(files, m1, m2):
  listOfFiles = [] 
  stringToCompare = "_" + str(m1) + "_" + str(m2) + "_"
  for l in files:
    if(l.find(stringToCompare) != -1):
      name = l[0:len(l)-1]
      listOfFiles.append(name)
  #listOfFiles2 = []
  #listOfFiles2.append(listOfFiles[0])
  #listOfFiles = listOfFiles2
  return listOfFiles
   

#-------------------------------------------------------------#
# makePythiaFile method: adapt the pythia template and copy   #    
#-------------------------------------------------------------#

def makePythiaFile(pythia, m1, m2, dirbase):
  workDir = dirbase + "/job_" + str(m1) + "_" + str(m2)
  filein = "./lhe_" + str(m1) + "_" + str(m2) + ".lhe"
  slhafile = "./slha_" + str(m1) + "_" + str(m2) + ".slha"
  pythia_copy1 = pythia.replace("FILEIN", filein)
  pythia_copy2 = pythia_copy1.replace("SLHACARD", slhafile)
  file_pythia = open(workDir + "/pythia_" + str(m1) + "_" + str(m2) + ".py", "w")
  file_pythia.write(pythia_copy2)
  file_pythia.close()

#-------------------------------------------------------------#
# makeSLHAFile method: adapt the SLHA template and copy       #    
#-------------------------------------------------------------#

def makeSLHAFile(slha, m1, m2, dirbase):
  workDir = dirbase + "/job_" + str(m1) + "_" + str(m2)
  slha_copy1 = slha.replace("PARTICLE1", str(m1))
  slha_copy2 = slha_copy1.replace("PARTICLE2", str(m2))
  file_slha = open(workDir + "/slha_" + str(m1) + "_" + str(m2) + ".slha", "w")
  file_slha.write(slha_copy2)
  file_slha.close()
 
#-------------------------------------------------------------#
# makeScript method: makes the script for running             #    
#-------------------------------------------------------------#

def makeScript(name, m1, m2, lhedir, lhe, merger, model, banner, leptonFilter, dirbase):

  short_lhe = []
  for f in lhe:
    short_lhe.append(f[len(lhedir)+1:len(f)])
    
  workDir = dirbase + "/job_" + str(m1) + "_" + str(m2)


  file_script = open(workDir + "/" + name, "w")
  file_script.write("#!/bin/bash\n")
  file_script.write("#Script automatically generated by MakeScan.py\n")
  file_script.write("export CMSSW_SEARCH_PATH=$CMSSW_SEARCH_PATH:$1\n") 
  file_script.write("cp $1/* .\n")
  file_script.write("WORKPATH=$PWD\n")
  file_script.write("cd $1\n")
  file_script.write("eval `scramv1 runtime -sh`\n")
  file_script.write("cd $WORKPATH\n")

  file_script.write("\n\n\n\n\n")
  file_script.write("#Copy lhe files\n")
 
  counter = 0
  for f in lhe:
    file_script.write("xrdcp xroot://eoscms/" + f + " ./" + short_lhe[counter] + "\n")
    counter = counter + 1

  file_script.write("\n\n\n\n\n")
  file_script.write("#uncompressing files\n")
  file_list = open(workDir + "/theList.txt", "w")
  for f in short_lhe:
    file_script.write("gzip -d " + f + "\n")
    #file_script.write("vim -c 1,\$s/1.72500000E+03/1.72500000E+02/g -c x " + f[0:f.find(".gz")] + "\n")
    #file_script.write("vim -c 1,\$s/400.00000000E+00/0.00000000E+00/g -c x " + f[0:f.find(".gz")] + "\n")
    file_list.write(f[0:f.find(".gz")] + "\n")
  file_list.close()


  file_script.write("\n\n\n\n\n")
  if(len(short_lhe) > 1):
    file_script.write("#merging files\n")
    file_script.write("g++ -Wall -o mergeLheFiles merger.cpp\n")
    file_script.write("./mergeLheFiles theList.txt " + " lhe_" + str(m1) + "_" + str(m2) + ".lhe_aux\n")
    for f in short_lhe:
      file_script.write("rm " + f[0:f.find(".gz")] + "\n")
  else:
    for f in short_lhe:
      file_script.write("mv " + f[0:f.find(".gz")] + " lhe_" + str(m1) + "_" + str(m2) + ".lhe_aux\n")

  file_script.write("python InsertSLHA.py --slha " + "lhe_" + str(m1) + "_" + str(m2) + ".lhe_aux " + "slha_" + str(m1) + "_" + str(m2) + ".slha lhe_" + str(m1) + "_" + str(m2) + ".lhe\n")
  file_script.write("rm " + "lhe_" + str(m1) + "_" + str(m2) + ".lhe_aux\n")
  


  file_script.write("\n\n\n\n\n")
  file_script.write("#running\n")
  file_script.write("cmsRun " + "pythia_" + str(m1) + "_" + str(m2) + ".py\n")
  file_script.write("python InsertSLHA.py --lhe fort.69 " + model + "_" + str(m1) + "_" + str(m2) + " " + " banner_template " + "lhe_" + str(m1) + "_" + str(m2) + ".lhe " + "lhe_" + model + "_" + str(m1) + "_" + str(m2) + ".lhe\n") 
  file_script.write("rm lhe_" + str(m1) + "_" + str(m2) + ".lhe\n")
  file_script.write("rm fort.69\n")
  
  if(leptonFilter != ""):
    lepton_filter = open(workDir + "/lepton_filter.py", "w")
    lepton_filter.write(leptonFilter)
    lepton_filter.close() 
    file_script.write("python lepton_filter.py lhe_" + model + "_" + str(m1) + "_" + str(m2) + ".lhe 1000000\n")

  
  file_script.write("rfcp lhe_" + model + "_" + str(m1) + "_" + str(m2) + ".lhe /castor/cern.ch/user/p/pablom/T5ZZ/\n")
  file_script.close()
  
  file_merger = open(workDir + "/" + "merger.cpp", "w")
  file_merger.write(merger)
  file_merger.close() 
  file_banner = open(workDir + "/" + "banner_template", "w")
  file_banner.write(banner)
  file_banner.close() 

#-------------------------------------------------------------#
# Main Section                                                #
#-------------------------------------------------------------#

if(len(sys.argv) != 2):
  printUsage()
else:
  if(sys.argv[1] == "-h"):
    printUsage()
  else:
    result = parseInputFile(sys.argv[1])
    if(result == "Error"):
      ShowMessage("Error parsing input file")
      exit
    else:
      dirbase = "."
      LHEDir = result[0]
      LHEName = result[1]
      PYTHIATemplate = result[2]
      LHATemplate = result[3]
      MergerName = result[4]
      minP1 = int(result[5])
      maxP1 = int(result[6])
      stepP1 = int(result[7])
      minP2 = int(result[8])
      maxP2 = int(result[9])
      stepP2 = int(result[10])
      slhaInsert = result[11] 
      bannerName = result[12]
      leptonFilterName = result[13] 
      #Check if there are LHE files and put them in LHEFiles 
      LHEFilesOutput = os.popen("xrd eoscms dirlist " + LHEDir + " | awk '{print $5}'")
      LHEFiles = LHEFilesOutput.readlines()
      #Open PythiaTemplate
      pythia = open(PYTHIATemplate).read()
      slha = open(LHATemplate).read()
      merger = open(MergerName).read()  
      banner = open(bannerName).read()  
      leptonFilter = open(leptonFilterName).read() 
      if(len(LHEFiles) == 0):
        ShowMessage("No LHE files found")
        exit()

      run_man = open("run_man.sh", "w")
      run_man.write("#!/bin/bash\n")
 
      #Start scan
      for mp1 in range(minP1, maxP1, stepP1):
        for mp2 in range(minP2, maxP2, stepP2):
          if(mp2 + 10 > mp1):
            continue
          listOfFiles = getListOfFiles(LHEFiles, mp1, mp2)       
          os.system("mkdir " + dirbase + "/job_" + str(mp1) + "_" + str(mp2))
          os.system("cp " + slhaInsert + " " + dirbase + "/job_" + str(mp1) + "_" + str(mp2))
          makePythiaFile(pythia, mp1, mp2, dirbase)
          makeSLHAFile(slha, mp1, mp2, dirbase)
          makeScript("run.sh", mp1, mp2, LHEDir, listOfFiles, merger, LHEName, banner, leptonFilter, dirbase) 
          
          run_man.write("cd " + dirbase + "/job_" + str(mp1) + "_" + str(mp2) + "\n")
          run_man.write("chmod +x run.sh\n")
          run_man.write("bsub -q cmsexpress run.sh $PWD\n")
          run_man.write("cd ..\n")

      run_man.close()
           





