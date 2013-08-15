import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBSetup_cfi import *

#################################### Metadata ########################################
process = cms.Process("runMuonMillepedeAlgorithm")
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('runMuonMillepedeAlgorithm'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/pablom/MuonAlignmentAlgorithms/test/runMuonMillepedeLocal.py,v $'),
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
#################################### Source block #####################################
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(200))

process.source = cms.Source("PoolSource", 
    #fileNames = cms.untracked.vstring(nameOfTheFile)
    fileNames = cms.untracked.vstring("file:/tmp/pablom/FE1AE5EE-84B2-DE11-B6B2-003048678D78.root")
)

#################################### Geometry ##########################################
process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
#process.load("Configuration.StandardSequences.MagneticField_38T_UpdatedMap_cff")
process.VolumeBasedMagneticFieldESProducer.version = 'grid_1103l_090322_3_8t'
#process.load("MagneticField.Engine.volumeBasedMagneticField_1103l_090216_cfi")
process.load('Configuration/EventContent/EventContent_cff')

################################ Tags and databases ####################################
#process.load("CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi")
#process.SiStripQualityESProducer.ListOfRecordToMerge = cms.VPSet(
#     cms.PSet( record = cms.string("SiStripFedCablingRcd"), tag    = cms.string("") ),
#     cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
#     cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") )
#)
#process.prefer("SiStripQualityESProducer")


#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRUZET4_V2P::All"
#process.GlobalTag.globaltag = "IDEAL_V11::All"
process.GlobalTag.globaltag = "GR09_E_V2::All"
#process.GlobalTag.globaltag = "IDEAL_V12::All"

#del process.cscBadChambers
#del process.DTFakeVDriftESProducer

process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff") ## Needed at least on version 2_1_10

################################## Propagation ############################################
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagator_cfi")

# patch needed for CRUZET2 but not used in CRUZET3 cfg
#process.SteppingHelixPropagatorAny.useInTeslaFromMagField = True
#process.SteppingHelixPropagatorAlong.useInTeslaFromMagField = True
#process.SteppingHelixPropagatorOpposite.useInTeslaFromMagField = True
#process.SteppingHelixPropagatorAny.SetVBFPointer = True
#process.SteppingHelixPropagatorAlong.SetVBFPointer = True
#process.SteppingHelixPropagatorOpposite.SetVBFPointer = True
#process.SteppingHelixPropagator.SetVBFPointer = True


################################# MuonStandaloneAlgorithm ####################################

process.load("RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff")

process.load("Alignment.CommonAlignmentProducer.AlignmentProducer_cff")

process.looper.doMuon = cms.untracked.bool(True)
process.looper.algoConfig = cms.PSet(process.MuonMillepedeAlgorithm)
process.looper.ParameterBuilder = cms.PSet(
  parameterTypes = cms.vstring('Selector,RigidBody4D'),
  Selector = cms.PSet(
    # selection of alignables and their parameters:
    # comma separated pairs of detector parts/levels as defined in AlinmentParameterSelector
    # (note special meaning if the string contains "SS" or "DS" or ends with "Layers"
    # followed by two digits)
    # and of d.o.f. to be aligned (x,y,z,alpha,beta,gamma) in local frame:
    # '0' means: deselect, '1' select. Others as 1, but might be interpreted in a special
    # way in the used algorithm (e.g. 'f' means fixed for millepede)
   alignParams = cms.vstring('MuonDTChambers,110000')
  )
)   

process.looper.tjTkAssociationMapTag = cms.InputTag("globalCosmicMuons:Refitted:runMuonMillepedeAlgorithm")



process.load("TrackingTools.TrackRefitter.globalCosmicMuonTrajectories_cff")
process.globalCosmicMuons.Tracks = cms.InputTag("ALCARECOMuAlGlobalCosmics:GlobalMuon")

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.allPath = cms.Path(process.offlineBeamSpot * process.MuonAlignmentFromReferenceGlobalCosmicRefit)



#For the new geometry 
#process.SiStripLorentzAngle = cms.ESSource("PoolDBESSource",
#    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
#    DBParameters = cms.PSet(
#        messageLevel = cms.untracked.int32(2),
#        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('SiStripLorentzAngleRcd'),
#       tag = cms.string('SiStripLA_CRAFT/GlobalAlignment_layers')
#    )),
#    connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment2/CMSSW_2_2_0/src/Alignment/MuonAlignmentAlgorithms/test/LA_CRAFT/GlobalAlignment_layers.db')#LA_CRAFT/GlobalAlignment_UNIFORM.db')
#)
#process.es_prefer_SiStripLorentzAngle = cms.ESPrefer("PoolDBESSource","SiStripLorentzAngle")


####Muon Geometry
import CondCore.DBCommon.CondDBSetup_cfi
process.muonAlignment = cms.ESSource("PoolDBESSource",

#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternalLatestTracker4th.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternalLatestTracker3th.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternalLatestTracker2nd.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternalLatestTracker1st.db"),
connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_3_2_7/src/Alignment/MuonAlignment/test/Alignments.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedLargeErrors.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternalAlgorithm1st.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCorrectedInternal.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_7/src/Alignment/MuonAlignmentAlgorithms/test/Alignments6Deg.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment2/CMSSW_2_2_0/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsCRAFT/GlobalAlignment_ALL_V4LargeErrors.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/user/p/pivarski/ALCA_MUONALIGN/SWAlignment/MuonAlignmentFromReference/CMSSW_2_2_11/src/testpattern.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/Alignments1stOld.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsNewInternalSinDeltax.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsNewInternalSinDeltax1st.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/Alignments2ndOld.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsNewInternal.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/Alignments1stIteration.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentNewInternal.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_7/src/Alignment/MuonAlignmentAlgorithms/test/TrackerV4.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment2/CMSSW_2_2_0/src/Alignment/MuonAlignmentAlgorithms/test/AlignmentsNominalLargeErrors.db"),
DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd")),
                  cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd")),
                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignmentRcd")),
                  cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCAlignmentErrorRcd"))))
process.es_prefer_muonAlignment = cms.ESPrefer("PoolDBESSource", "muonAlignment")





###TrackerGeometry
#process.trackerAlignment = cms.ESSource("PoolDBESSource",
#connect = cms.string("frontier://FrontierProd/CMS_COND_21X_ALIGNMENT"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_7/src/Alignment/MuonAlignmentAlgorithms/test/TrackerV4.db"),
#connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_MUONALIGN/SWAlignment/Millepede/CRAFT/GlobalAlignment/CMSSW_2_2_10/src/Alignment/MuonAlignmentAlgorithms/test/TrackerAlignments.db"),
#DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
#toGet = cms.VPSet(
#cms.PSet(record = cms.string("TrackerAlignmentRcd"), tag = cms.string("Alignments")),
#cms.PSet(record = cms.string("TrackerAlignmentErrorRcd"), tag = cms.string("AlignmentErrors"))
#))
#
#process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "trackerAlignment")
#process.looper.applyDbAlignment = cms.untracked.bool(True)



process.myRECO = cms.OutputModule("PoolOutputModule",
     fileName = cms.untracked.string('reco.root')
)


process.outpath = cms.EndPath( process.myRECO )


