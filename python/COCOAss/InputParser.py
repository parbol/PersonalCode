#List of libraries used by the program
import string


#==================== Class InputParser ===================#
###### Reads and parses the input file #####################
#==========================================================# 
class InputParser:

  #Constructor
  def __init__(self, __nameOfInputFile, __nameOfCocoaFile):
    self.nameOfInputFile = __nameOfInputFile
    self.nameOfCocoaFile = __nameOfCocoaFile
    self.listOfInputTags = []
    self.listOfOutputTags = []
    self.listOfTags = []
    print "[InputParser] InputParser Created!"
    
  #read method: returns a list of tags 
  def read(self):
    print "[InputParser] Parsing ..."
    f = open(self.nameOfInputFile)
    counter = 0
    for line in f:
      result = self.parseLine(line.split())
      if(result == "Error"):
        print "[InputParser] Line " + str(counter) + ": parse error. Ignoring"
      else:
        if(result == "Empty Line"):
          print "[InputParser] Line " + str(counter) + ": empty line"
        else:
          if(result == "Comment"):
            print "[InputParser] Line " + str(counter) + ": comment"
          else:
            if(result[1] == 'I'):
              self.listOfInputTags.append(result)
              print "[InputParser] Line " + str(counter) + ": parse OK. " + result[2] + " " + result[3]
            if(result[1] == 'O'):
              self.listOfOutputTags.append(result)
              print "[InputParser] Line " + str(counter) + ": parse OK. " + result[2] + " " + result[3]  
      counter = counter + 1
    self.listOfTags.append(self.listOfInputTags)
    self.listOfTags.append(self.listOfOutputTags)
    print "[InputParser] List of InputTags"
    for inputTag in self.listOfInputTags:
      print inputTag
    print "[InputParser] List of OutputTags"
    for outputTag in self.listOfOutputTags:
      print outputTag
    return self.listOfTags

  #parseLine method: parses a line from the input file
  def parseLine(self, words):
    if(words == []):
      return "Empty Line"
    if(words[0][0] != '#' and words[0] != 'I' and words[0] != 'O'):
      return "Error"
    if(words[0][0] == "#"):
      return "Comment"
    if(words[0] == 'I'):
      if(self.nameOK(words[1]) != 0 and self.coordOK(words[2]) != 0):
        if(len(words) == 6):
          if(self.rangeOK(words[3], words[4], words[5])):
            sintax = [1]
            result = sintax + words
            return result
          if(self.typeOK(words[3], words[4], words[5]) != 0):
            sintax = [2]
            result = sintax + words
            return result
        if(len(words) == 5):
          if(self.typeOK(words[3], words[4], "") != 0):
            sintax = [3]
            result = sintax + words  
            return result
      return "Error"
    if(words[0] == 'O'):
      if(self.nameOK(words[1]) != 0):
        result = ['O', 'O', words[1], words[2]]
        return result 
    return "Error"
  
  #nameOK method: checks if the name is valid
  def nameOK(self, word):
    c = open(self.nameOfCocoaFile)
    if(c.read().find(word) == -1):
      return 0
    return 1
    c.close()
  
  #coordOK method: checks if the coordinate is known
  def coordOK(self, word):
    if(word == "X" or word == "Y" or word == "Z" or 
       word == "R" or word == "PHI" or word == "AX" or
       word == "AY" or word == "AZ" or word == "EX" or
       word == "EY" or word == "EZ" or word == "ER" or
       word == "EPHI" or word == "EAX" or word == "EAY" or 
       word == "EAZ"):
      return 1
    return 0
  
  #typeOK method: checks if the type is correct
  def typeOK(self, word1, word2, word3):
    if((word1 == "fix" or word1 == "cal" or word1 == "unk") and
       (word2 == "fix" or word2 == "cal" or word2 == "unk") and
       (word3 == "fix" or word3 == "cal" or word3 == "unk" or word3 == "")): 
      return 1
    return 0

  #rangeOK method: checks if the range is valid
  def rangeOK(self, word1, word2, word3):
    if(word1 == "cal" or word1 == "fix" or word1 == "unk"):
      return 0
    if(float(word3) > 0 and (float(word1) < float(word2))):
      return 1
    return 0

#====================== End of Class ======================#    


