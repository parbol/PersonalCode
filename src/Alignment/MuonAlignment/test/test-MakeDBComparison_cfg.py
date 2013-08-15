
import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

#MessageLogging
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("WARNING"))



process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.source = cms.Source("EmptySource")

#################################### Geometry ##########################################
#################################### Geometry ##########################################
process.load("Configuration.StandardSequences.Geometry_cff")

##process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
##process.load("Geometry.DTGeometry.dtGeometry_cfi")
##process.load("Geometry.CSCGeometry.cscGeometry_cfi")
##process.load("Alignment.CommonAlignmentProducer.GlobalPosition_Frontier_cff")
##process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

#process.load("Alignment.MuonAlignment.ReadDBToRootFile_cfi")

##rocess.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "DESIGN_31X_V4::All"
#process.GlobalTag.globaltag = "GR10_P_V7::All"

####Muon Geometry
import CondCore.DBCommon.CondDBSetup_cfi
process.muonAlignment = cms.ESSource("PoolDBESSource",


#connect = cms.string("frontier://FrontierProd/CMS_COND_21X_ALIGNMENT"),
#connect = cms.string("sqlite_file:./SQLites/Barrel_1125_PG_20100313_LINKFIT_NOOMRON_BOTH_PG_Err.db"),
connect = cms.string("sqlite_file:./SQLites/BarrelPG2009Corrections_trp.db"),
#connect = cms.string("sqlite_file:./SQLites/CSC_HWAlignment_Rphi_May_06_2010_STARTUP.db"),
#connect = cms.string("sqlite_file:./SQLites/MAY7_CSC_beamhalo-PG.db"),
#connect = cms.string("sqlite_file:./SQLites/Barrel_1125_PG_20100313_LINKFIT_NOOMRON_BOTH_PLUS_PG.db"),
#connect = cms.string("sqlite_file:./SQLites/Design.db"),

# Standard tag names
#DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
#toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd")),
#                  cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd")),
#                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignmentRcd")),
#                  cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCAlignmentErrorRcd"))))

# MC simulation Summer 2010 tag names
#DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
#toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd_CRAFT08Realistic-noME13_3XY_v1_mc")),
#                  cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd_CRAFT08Realistic-noME13_3XY_v1_mc")),
#                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCCRAFTScenario310me42_v2_mc")),
#                  cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCIdealGeometryErrors310me42_mc"))))

# Data August 2010 tag names
DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignment_2009_v4_offline_100902V0")),
                  cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentError_2009_v4_offline_100902V0")),
                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignment_2009_v4_offline_100902V0")),
                  cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCIdealGeometryErrors310me42_mc"))))


process.es_prefer_muonAlignment = cms.ESPrefer("PoolDBESSource", "muonAlignment")

#read in a geom from root file (output root file is ROOT geom minus DB geom)
process.MakeDBComparison = cms.EDAnalyzer('MakeDBComparison',
        nameOfTheInputFile = cms.string("./Output/Geometries/MAY16_DT_withMotionPolicy.root"),
        nameOfTheOutputFile = cms.string("./Output/Differences/MAY16_DT_withMotionPolicy-MINUS-PG2009.root")
)

process.allPath = cms.Path( process.MakeDBComparison )

