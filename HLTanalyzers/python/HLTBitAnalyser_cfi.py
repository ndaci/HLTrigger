import FWCore.ParameterSet.Config as cms

hltbitanalysis = cms.EDAnalyzer("HLTBitAnalyzer",
    ### Trigger objects
    #l1GctHFBitCounts                = cms.InputTag("hltGctDigis"),
    #l1GctHFRingSums                 = cms.InputTag("hltGctDigis"),
    #l1GtObjectMapRecord             = cms.InputTag("hltL1GtObjectMap::HLT"),
    l1results                       = cms.InputTag("hltGtStage2Digis"),                            
    #l1GtReadoutRecord               = cms.InputTag("hltGtDigis::HLT"),

    #l1extramc                       = cms.string('hltL1extraParticles'),
    #l1extramu                       = cms.string('hltL1extraParticles'),
    hltresults                      = cms.InputTag("TriggerResults::HLT"),
    HLTProcessName                  = cms.string("HLT"),

    ### GEN objects
    mctruth                         = cms.InputTag("genParticles::HLT"),
    genEventInfo                    = cms.InputTag("generator::SIM"),

    ### SIM objects
    simhits                         = cms.InputTag("g4SimHits"),
                                
    ## reco vertices
    OfflinePrimaryVertices0     = cms.InputTag('offlinePrimaryVertices'),

    ## HLT collections //ND
    hltPFJetLabel               = cms.InputTag('hltAK4PFJets::TEST'),
    hltPFJetCorrLabel           = cms.InputTag('hltAK4PFJetsCorrected::TEST'),
    hltCaloJetLabel             = cms.InputTag('hltAK4CaloJets::TEST'),
    hltCaloJetCorrLabel         = cms.InputTag('hltAK4CaloJetsCorrected::TEST'),
    hltPFMETLabel               = cms.InputTag('hltPFMETProducer::TEST'), 
    hltPFMETNoMuLabel           = cms.InputTag('hltPFMETNoMuProducer::TEST'), 
    hltPFMHTLabel               = cms.InputTag('hltPFMHTTightID::TEST'), 
    hltPFMHTNoMuLabel           = cms.InputTag('hltPFMHTNoMuTightID::TEST'), 
    hltCaloMETLabel             = cms.InputTag('hltMet::TEST'), 
    hltCaloMETCleanLabel        = cms.InputTag('hltMetClean::TEST'), 
    hltCaloMHTLabel             = cms.InputTag('hltMht::TEST'),

    ## RECO collections //ND
    recoPFJetLabel               = cms.InputTag('ak4PFJets::RECO'),
    recoPFJetCorrLabel           = cms.InputTag('ak4PFJetsCHS::RECO'),
    recoCaloJetLabel             = cms.InputTag('ak4CaloJets::RECO'),
    recoCaloJetCorrLabel         = cms.InputTag('ak4CaloJets::RECO'),
    recoPFMETLabel               = cms.InputTag('pfMet::RECO'), 
    recoMuonsLabel               = cms.InputTag('muons::RECO'),
    recoCaloMETLabel             = cms.InputTag('caloMet::RECO'), 
                                
    ### Run parameters
    RunParameters = cms.PSet(
        HistogramFile = cms.untracked.string('hltbitanalysis.root'),
        isData         = cms.untracked.bool(False),
        Monte          = cms.bool(True),
        GenTracks      = cms.bool(True),
        Debug          = cms.bool(True),
        RecoPFJets     = cms.untracked.bool(False),
        RecoCaloJets   = cms.untracked.bool(False),
        RecoPFMET      = cms.untracked.bool(False),
        RecoCaloMET    = cms.untracked.bool(False),
        RecoMuons      = cms.untracked.bool(False)
    )
                                
)
