import FWCore.ParameterSet.Config as cms

SegAnal = cms.EDAnalyzer("SegAnal",
    recHits2DLabel = cms.string('dt2DSegments'),
    recHits4DLabel = cms.string('dt4DSegments::runMuonMillepedeAlgorithm'),
    recHits1DLabel = cms.string('dt1DRecHits')
)

