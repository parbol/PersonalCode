#List of libraries used by the program
import string


#==================== Class OutputModule ===================#
###### Writes the output of the program #####################
#===========================================================# 
class OutputModule:

  #Constructor
  def __init__(self, __name, __output):
    self.name = __name
    self.output = __output

  def write(self):
    f = open(self.name, "w")
    label = ""
    for tag in self.output[0]:
      label = label + tag[0] + "_" + tag[1] + " "
      parsedLabel = label[0:-1] + "\n"
    f.write(parsedLabel)
    for step in self.output:
      chain = ""
      for tag in step:
        chain = chain + tag[2] + " "
        parsedChain = chain[0:-1] + "\n" 
      f.write(parsedChain)
    f.close()


#====================== End of Class ======================#    


