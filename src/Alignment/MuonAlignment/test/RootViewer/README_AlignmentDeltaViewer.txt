
AlignmentDeltaViewer is a graphical utility to compare two alignment geometries.

It uses the following inputs:

1) 
Two text files with the nominal geometry descriptions called 

NominalGeometryDT.txt
NominalGeometryCSC.txt

used only to define the displayed DTs and CSCs.

2)
A root file which is obtained from Pablo's CMSSW tool and which contains
the differences between two given geometries.

AlignmentDeltaViewer is ONLY a graphical interface to display this information in a useful
way. To run it simply start root and type:

root [0] .x AlignmentDeltaViewer.C+("InputFileName",Choice)

where InputFileName is the root file used as input, usually with extension .root
and choice is

0 to run interactively
1 to make all barrel plots
2 to make all barrel summary plots
3 to make all endcap plots
4 to make all endcap summary plots

or, to save time, run it in batch mode as follows

> root -l -q -b 'AlignmentDeltaViewer.C+("InputFileName",Choice)'

It will dump all plots to a pdf file. Here choice must be 1,2,3 or 4, not 0.
Notice the usage of single and double quotes, it is important.

Note1: 
The "+" is important. It tells root to compile the code. Some of the
classes inherit from ROOT classes and they override some of the
ROOT class methods. Unless the code is compiled this override does not take
place and the code will not work properly.

Note2: 
If all YB and YE plots are done in one single run it crashes probably due to memory leaks,
so it must be run for YB or for YE separately 

