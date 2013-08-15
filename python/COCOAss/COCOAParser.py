
#==================== Class JobManager ====================#
###### Expands the tags to fill all the possibilities ######
#==========================================================# 
class COCOAParser:

  def __init__(self, __cocoaFile):
    self.cocoaFile = __cocoaFile
    print "[COCOAParser] COCOAParser created"

  def changeLine(self, tag):
    name = tag[2]
    coor = tag[3]
    type = tag[0]
    print "[COCOAParser] Changing " + name + " " + coor + " into:"
    print tag
    lineNumber = self.returnLine(name, coor)
    if(lineNumber == []):
      return "error"
    splitedLine = self.cocoaFile[lineNumber].split()
    subValue = ""
    if(type == 1):
      if(coor[0] != "E"):
        subValue = splitedLine[1]
      else:
        subValue = splitedLine[2]
    else:                     
      subValue = splitedLine[3]
      
    self.cocoaFile[lineNumber] = self.cocoaFile[lineNumber].replace(subValue, tag[4])
       

  def returnLine(self, name, coor):
    block = self.returnBlock(name)
    if(block != []):
      if(coor == "X" or coor == "EX" or coor == "R" or coor == "ER"):
        return block[0]
      if(coor == "Y" or coor == "EY" or coor == "PHI" or coor == "EPHI"):
        return block[1]
      if(coor == "Z" or coor == "EZ"):
        return block[2]
      if(coor == "AX" or coor == "EAX"):
        return block[3]
      if(coor == "AY" or coor == "EAY"):
        return block[4]
      if(coor == "AZ" or coor == "EAZ"):
        return block[5]
    return []  

  def returnBlock(self, name):
    listOfLines = []
    listOfMatches = self.find(name)
    for lineNumber in listOfMatches:
      if(self.cocoaFile[lineNumber+1].find("ENTRY") != -1):
        counter = 2
        while(self.cocoaFile[lineNumber+counter].find("}") == -1):
          counter = counter + 1
        if(self.cocoaFile[lineNumber+counter+1].find("centre") == -1):
          break
        else:
          listOfLines.append(lineNumber+counter+2)
          listOfLines.append(lineNumber+counter+3)
          listOfLines.append(lineNumber+counter+4)
          listOfLines.append(lineNumber+counter+6)
          listOfLines.append(lineNumber+counter+7)
          listOfLines.append(lineNumber+counter+8)
          break
      else:
        if(self.cocoaFile[lineNumber+1].find("centre") != -1):
          listOfLines.append(lineNumber+2)
          listOfLines.append(lineNumber+3)
          listOfLines.append(lineNumber+4)
          listOfLines.append(lineNumber+6)
          listOfLines.append(lineNumber+7)
          listOfLines.append(lineNumber+8)
          break
        else:
          break
    if(len(listOfLines) != 6):
      return []
    else:
      return listOfLines
    
  def find(self, name):
    counter = 0
    listOfMatches = []
    for oneLine in self.cocoaFile:
      if(oneLine.find(name) != -1):
        splitedValue = oneLine.split()[1]
        if(name == splitedValue):
          listOfMatches.append(counter)
      counter = counter + 1
    return listOfMatches 

  def write(self, name):
    f = open(name, "w")
    for line in self.cocoaFile:
      f.write(line)
    f.close()
  
    
