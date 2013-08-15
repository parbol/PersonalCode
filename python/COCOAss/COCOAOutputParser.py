#=============== Class COCOAOutputParser ==================#
################ Parses COCOA output reports ###############
#==========================================================# 

class COCOAOutputParser:

  def __init__(self, __cocoaOutputFile):
    f = open(__cocoaOutputFile)
    self.cocoaOutputFile = f.readlines()
    f.close()
    self.numberOfIterations = ""
    self.chi2 = ""
    self.lastIteration()
    print "[COCOAOutputParser] COCOAOutputParser created"
  
  def takeValue(self, name, coor):
    index = self.find(name)
    if(len(index) == 0):
      return "Error"
    nameCoor = ""
    if(coor == "X"):
      nameCoor = "centre_X"
    if(coor == "Y"):
      nameCoor = "centre_Y"
    if(coor == "Z"):
      nameCoor = "centre_Z"
    if(coor == "AX"):
      nameCoor = "angles_X"
    if(coor == "AY"):
      nameCoor = "angles_Y"
    if(coor == "AZ"):
      nameCoor = "angles_Z"
    for line in index:
      if(self.cocoaOutputFile[line].find(nameCoor) != -1):
        return self.cocoaOutputFile[line].split()[12]
    return "Error"
  
  def lastIteration(self):
    listOfLines = []
    listOfMatches = self.find("Fit iteration")
    self.numberOfIterations = str(len(listOfMatches))
    if(listOfMatches != []):
      self.cocoaOutputFile = self.cocoaOutputFile[listOfMatches[-1]:-1]
      self.chi2 = self.cocoaOutputFile[self.find("Chi2")[0]].split()[5]

  def find(self, name):
    counter = 0
    listOfMatches = []
    for oneLine in self.cocoaOutputFile:
      if(oneLine.find(name) != -1):
        word = oneLine.split()[2]
        subString = word[word.find(name):-1]
        if(subString.find("/") == -1): 
          listOfMatches.append(counter)
      counter = counter + 1
    return listOfMatches 

#====================== End of class =====================#
