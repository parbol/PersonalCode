###!/usr/bin/env python

## /RelVal(ZMM|WM|TTbar|JpsiMM|SingleMu.*)/CMSSW_(4_1_2|3_11_1)

from optparse import OptionParser
import bitly

shortener=bitly.Api('jklukas', 'R_478578d1bd76cea3a334bd56425de23f')

link_template="https://cmsweb.cern.ch/dqm/relval/start?runnr=%(runNum)s;dataset=%(dataset1)s;sampletype=offline_data;filter=all;referencepos=overlay;referenceshow=all;referenceobj1=other::%(dataset2)s:;striptype=object;stripruns=;stripaxis=run;stripomit=none;workspace=Everything;size=M;root=%(root)s;zoom=no;"

def main():
    parser = OptionParser()
    options, args = parser.parse_args()
    
    kind, hltpath, runNum, dataset1, dataset2 = args
    if(kind == "mc"):
      root = 'HLT/Muon/Efficiency_Layouts/' + hltpath
    else:
      root = 'HLT/Muon/DistributionsGlobal/' + hltpath
    link = link_template % locals()
    short = shortener.shorten(link)
    print "#############################################################################################################"
    print "Validation of: " + dataset1
    print "Reference sample: " + dataset2
    print "Type of Sample: " + kind
    print "HLT Path: " + hltpath
    print "Run Number: " + runNum   
    print "Link: " + link
    print "#############################################################################################################"


if __name__ == '__main__':
    main()
