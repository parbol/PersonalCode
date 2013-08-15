#!/usr/bin/python

import sys
import os
import time


######## Find a line in par matching text and return value word#######
def findLineWithTextAndGet(par, text, value):
  for line in par:
    if(line.find(text) != -1):
      return line.split()[value]
  return "0"

##################### Give date in proper format #####################
def giveDate():
  mydatem = "[" + str(time.localtime()[3]) + ":" + str(time.localtime()[4]) + ":" + str(time.localtime()[5]) + " " + str(time.localtime()[2]) +  " " + str(time.localtime()[1]) + " " + str(time.localtime()[0]) + "]" 
  return mydatem
######################################################################

##################### Log information ################################
def logInfo(msg):
  print giveDate() + " " + msg
######################################################################

##################### Start Crab Submission ##########################
def startSubmission(numberOfJobs, eventsPerJob):
  logInfo("Generating crab.cfg file")
  crabModelFile = open("crabmodel.cfg").read()
  crabModelFile_1 = crabModelFile.replace("numberOfJobs" , str(numberOfJobs))
  crabModelFile_2 = crabModelFile_1.replace("eventsPerJob" , str(eventsPerJob))
  f = open("crab.cfg", "w")
  f.write(crabModelFile_2) 
  f.close() 
  logInfo("Starting new crab submission")
  submissionOutput = os.popen("crab -create -submit").readlines()
  retval = findLineWithTextAndGet(submissionOutput, "jobs submitted", 3)
  print retval
  if(retval != "0"):
    logInfo(retval + " jobs were submitted")
    return int(retval)
  else:
    logInfo("Submission failed")
    return 0
######################################################################


############################## Check Status ##########################
def checkStatus():
  logInfo("Checking Status")
  statusOutput = os.popen("crab -status").readlines()
  retval = findLineWithTextAndGet(statusOutput, "Jobs Done", 1)
  if(retval != "0"):
    return int(retval)
  else:
    return 0
######################################################################


############################## Clean the Area ########################
def cleanArea(numberOfJobs):
  #More verbosity is needed here
  if(numberOfJobs != 0):
    logInfo("Retrieving Output")
    retrieveOutput = os.popen("crab -get -USER.dontCheckSpaceLeft=1").readlines()
    killOutput = os.popen("crab -kill 1-" + str(numberOfJobs)).readlines()
  logInfo("Cleaning area")
  cleanOutput = os.popen("crab  -clean").readlines()
  os.system("rm -r crab_*")
######################################################################
 


numberOfJobs = 100
eventsPerJob = 500
numberOfIterations = 24
sleepTime = "1h" 

goOn = 1
retval = 0
for crabLaunch in range(0, numberOfIterations):
  if(goOn == 1):
    retval = startSubmission(numberOfJobs, eventsPerJob)
    if(retval == 0):
      cleanArea(retval)
      continue
  logInfo("Sleeping")
  os.system("sleep " + sleepTime)
  numberOfDoneJobs = checkStatus()  
  if(numberOfDoneJobs > 8.0*retval/10.0):
    logInfo("Sucess with " + str(numberOfDoneJobs) + " jobs")
    goOn = 1
    cleanArea(retval)
  else:
    logInfo("Not enough jobs have terminated")
    goOn = 0



