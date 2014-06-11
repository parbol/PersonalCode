import math,sys,re,os,shutil
from xml.dom import minidom

import fileinput


#main part of analysis
if len(sys.argv)!=3:
    print "Usage: filter.py <infile> <target>   "
    print "     : input file is backed up but overwritten "
    print "     : target is the desired number of events "
    print " Last modified: Thu Jun 30 11:06:16 CDT 2011 "

    sys.exit(0)
#else:
#    print "Running  filter.py to reformat Pythia LHE file"

file = sys.argv[1]

target = int(sys.argv[2])

#shutil.copyfile(file,file+".bak")


f=open(file,'r')
o=open(file+".tmp",'w')

checkevent=0

line=1

while line:
	line=f.readline()	
	o.write(line)
	if line.find("</init>")>-1:
		line=0

line=1
check_event = 1
nleptons = 0

nwrite = 0
nread = 0
while line and nwrite<target:
	line=f.readline()
	if line.find("</LesHouches")>-1:
		line=0
		continue
	elif line.find("<event>")>-1:
		check_event=1
		lines=[line]
		line=f.readline()
		lines.append(line)
		nleptons=0
		nread = nread + 1
		continue
	elif line.find("</event>")>-1:
		check_event=0
		lines.append(line)
	elif line.find("#")>-1:
		lines.append(line)
		continue
	elif line == "":
		lines.append(line)
		continue

	if check_event:
		
		pdgId=int(line.split()[0])
		if pdgId==11 or pdgId==13 or pdgId==15 or pdgId==-11 or pdgId==-13 or pdgId==-15:
			nleptons=nleptons+1
		lines.append(line)

	if check_event==0 and nleptons>0:
		o.writelines(lines)
		nwrite = nwrite + 1

FILE=os.path.basename('%s' %file)
	
o.write("</LesHouchesEvents>\n")
print FILE,nwrite #; desired ",target," ; read ",nread)



f.close()
o.close()
os.remove(file)
os.rename(file+".tmp", file)


