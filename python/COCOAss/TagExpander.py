

#==================== Class TagExpander ===================#
###### Expands the tags to fill all the possibilities ######
#==========================================================# 
class TagExpander:
  #Constructor
  def __init__(self, __tags):
    self.tags = __tags[0]
    self.outputTags = __tags[1]
    self.listOfTags = []
    self.finalListOfTags = []
    print "[TagExpander] TagExpander Created!"  

  #Expand method
  def expand(self):
    for singleTag in self.tags:
      self.listOfTags.append(self.expandSingleTag(singleTag))
    self.convolution()
    print "[TagExpander] list of expanded Input Tags"
    for tag in self.finalListOfTags:
      print tag
    return [self.finalListOfTags, self.outputTags]    

  #convolution method: makes the combination
  def convolution(self):
    for block in self.listOfTags:
      self.crossProduct(block)
  
  #crossProduct: performs cartesian product over tags
  def crossProduct(self, block):
    if (self.finalListOfTags == []):
      for item in block:
        newList = []
        newList.append(item)
        self.finalListOfTags.append(newList)
    else:
      oldListOfTags = self.finalListOfTags
      self.finalListOfTags = []
      for currentBlock in block:
        for current in oldListOfTags:
          newCurrent = current[:]
          newCurrent.append(currentBlock)
          self.finalListOfTags.append(newCurrent)

  #expandSingleTag method: expands a single tag 
  def expandSingleTag(self, singleTag):
    tagCollect = []
    if(singleTag[0] == 1):
      rangeInf = float(singleTag[4])
      rangeSup = float(singleTag[5])
      nStep = float(singleTag[6])
      step = (rangeSup-rangeInf)/nStep
      currentValue = rangeInf
      while(currentValue < rangeSup):
        tag = [singleTag[0], singleTag[1], singleTag[2], singleTag[3], str(currentValue)]
        tagCollect.append(tag)
        currentValue = currentValue + step
    if(singleTag[0] != 1):
      tag = [singleTag[0], singleTag[1], singleTag[2], singleTag[3], singleTag[4]]
      tagCollect.append(tag)
      tag = [singleTag[0], singleTag[1], singleTag[2], singleTag[3], singleTag[5]]
      tagCollect.append(tag)
      if(singleTag[0] == 2):
        tag = [singleTag[0], singleTag[1], singleTag[2], singleTag[3], singleTag[6]]
        tagCollect.append(tag)
    return tagCollect  
      
#====================== End of Class ======================#    

