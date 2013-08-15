
import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.source = cms.Source("EmptySource")

#################################### Geometry ##########################################
process.load("Configuration.StandardSequences.Geometry_cff")

##process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
##process.load("Geometry.DTGeometry.dtGeometry_cfi")
##process.load("Geometry.CSCGeometry.cscGeometry_cfi")
##process.load("Alignment.CommonAlignmentProducer.GlobalPosition_Frontier_cff")
##process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

#process.load("Alignment.MuonAlignment.ReadDBToRootFile_cfi")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "DESIGN_31X_V4::All"
process.GlobalTag.globaltag = "GR10_P_V7::All"

####Muon Geometry
import CondCore.DBCommon.CondDBSetup_cfi
process.muonAlignment = cms.ESSource("PoolDBESSource",
                                     
#connect = cms.string("sqlite_file:./SQLites/Design.db"),                              
#connect = cms.string("sqlite_file:./SQLites/BarrelPG2009Corrections_trp.db"),
#connect = cms.string("sqlite_file:./SQLites/CSCRecord_HWAlignment_AND_PG_CurvedEndcaps_Final_April_29_09.db"),
#connect = cms.string("sqlite_file:./SQLites/Barrel_1125_PG_20100313_LINKFIT_NOOMRON_BOTH_PLUS_PG_Err.db"),
#connect = cms.string("sqlite_file:./SQLites/MAY16_CSC_beamhalo-PG-diskXYphiZ-diskZ.db"),
#connect = cms.string("sqlite_file:./SQLites/MAY16_DT_withMotionPolicy.db"),
connect = cms.string("sqlite_file:./SQLites/MuAli_August2010.db"),

# standard tag names
#DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
#toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd")),
#                cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd")),
#                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignmentRcd")),
#                cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCAlignmentErrorRcd"))
#                  ))

# MC simulation Summer 2010 tag names
#DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
#toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignmentRcd_CRAFT08Realistic-noME13_3XY_v1_mc")),
#                cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentErrorRcd_CRAFT08Realistic-noME13_3XY_v1_mc")),
#                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCCRAFTScenario310me42_v2_mc")),
#                cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCIdealGeometryErrors310me42_mc"))
#                  ))

# Data August 2010 tag names
DBParameters = CondCore.DBCommon.CondDBSetup_cfi.CondDBSetup.DBParameters,
toGet = cms.VPSet(cms.PSet(record = cms.string("DTAlignmentRcd"),       tag = cms.string("DTAlignment_2009_v4_offline_100902V0")),
                cms.PSet(record = cms.string("DTAlignmentErrorRcd"),  tag = cms.string("DTAlignmentError_2009_v4_offline_100902V0")),
                  cms.PSet(record = cms.string("CSCAlignmentRcd"),      tag = cms.string("CSCAlignment_2009_v4_offline_100902V0")),
                cms.PSet(record = cms.string("CSCAlignmentErrorRcd"), tag = cms.string("CSCAlignmentError_2009_v3_offline_100902V0"))
                  ))


process.es_prefer_muonAlignment = cms.ESPrefer("PoolDBESSource", "muonAlignment")

process.ReadDBToRootFile = cms.EDAnalyzer('ReadDBToRootFile',
        nameOfTheFile = cms.string("./Output/Geometries/MuAli_August2010.root")
)

process.allPath = cms.Path( process.ReadDBToRootFile )
