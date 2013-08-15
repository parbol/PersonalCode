######################################################################################
#################### runMuonStandaloneAlgorithm.py ###################################
###################################################################################### 

import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBSetup_cfi import *

#################################### Metadata ########################################
process = cms.Process("runMuonMillepedeAlgorithm")
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('runMuonMillepedeAlgorithm'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/pablom/python/InternalAlignment/internalAlignment.py,v $'),
    annotation = cms.untracked.string('runMuonMillepeAlgorithm')
)

###################################### Services #######################################
#MessageLogging
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("DEBUG"))


#Databases 
process.load("CondCore.DBCommon.CondDBSetup_cfi")
#Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 

#################################### Source block ####################################
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource", 
    #fileNames = cms.untracked.vstring("file:/tmp/pablom/020D1EC6-01CC-DD11-906C-001D0967C9AA.root")
    #fileNames = cms.untracked.vstring("file:/tmp/pablom/B429434C-32CC-DD11-8ADD-001D0968F684.root")
    fileNames = cms.untracked.vstring(nameOfTheFile)
)

#################################### Geometry ##########################################
process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/EventContent/EventContent_cff')


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRUZET4_V2P::All"
#process.GlobalTag.globaltag = "IDEAL_V11::All"
process.GlobalTag.globaltag = "CRAFT_ALL_V4::All"

process.load("RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff")



process.load("RecoLocalMuon.DTSegment.SegAnal_cfi")


process.allPath = cms.Path( process.dtlocalreco * process.SegAnal )
#process.allPath = cms.Path( process.SegAnal )


####Muon Geometry
import CondCore.DBCommon.CondDBSetup_cfi
process.muonAlignment = cms.ESSource("PoolDBESSource",
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT2/CMSSW_2_2_0/src/RecoLocalMuon/DTSegment/test/AlignmentsFromNominal.db"),
connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT2/CMSSW_2_2_0/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsNominalLargeErrors.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT2/CMSSW_2_2_0/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternal.db"),
DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd")),
                  cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd")),
                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignmentRcd")),
                  cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCAlignmentErrorRcd"))))
process.es_prefer_muonAlignment = cms.ESPrefer("PoolDBESSource", "muonAlignment")



