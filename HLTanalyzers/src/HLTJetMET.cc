#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <stdlib.h>
#include <string.h>

#include "HLTrigger/HLTanalyzers/interface/HLTJetMET.h"

HLTJetMET::HLTJetMET() {

  //set parameter defaults 
  _RecoPFJets   = false;
  _RecoCaloJets = false;
  _RecoPFMET    = false;
  _RecoCaloMET  = false;
  _RecoMuons    = false;
  _Debug=false;
}

void HLTJetMET::beginRun(const edm::Run& run, const edm::EventSetup& c){ 
}

/*  Setup the analysis to put the branch-variables into the tree. */
void HLTJetMET::setup(const edm::ParameterSet& pSet, TTree* HltTree) {

  processName_ = pSet.getParameter<std::string>("HLTProcessName") ;

  edm::ParameterSet myHltParams = pSet.getParameter<edm::ParameterSet>("RunParameters") ;
  std::vector<std::string> parameterNames = myHltParams.getParameterNames() ;
  
  for ( std::vector<std::string>::iterator iParam = parameterNames.begin();
        iParam != parameterNames.end(); iParam++ ){
    if ( (*iParam) == "Debug" )        _Debug        = myHltParams.getParameter<bool>( *iParam );
    if ( (*iParam) == "RecoPFJets" )   _RecoPFJets   = myHltParams.getUntrackedParameter<bool>( *iParam );
    if ( (*iParam) == "RecoCaloJets" ) _RecoCaloJets = myHltParams.getUntrackedParameter<bool>( *iParam );
    if ( (*iParam) == "RecoPFMET" )    _RecoPFMET    = myHltParams.getUntrackedParameter<bool>( *iParam );
    if ( (*iParam) == "RecoCaloMET" )  _RecoCaloMET  = myHltParams.getUntrackedParameter<bool>( *iParam );
    if ( (*iParam) == "RecoMuons" )    _RecoMuons    = myHltParams.getUntrackedParameter<bool>( *iParam );
  }

  // MET and MHT
  _hltPfMetPx = _hltPfMetPy = _hltPfMetPt = _hltPfMetPhi = 0;
  _hltPfMhtPx = _hltPfMhtPy = _hltPfMhtPt = _hltPfMhtPhi = 0;
  _hltPfMetNoMuPx = _hltPfMetNoMuPy = _hltPfMetNoMuPt = _hltPfMetNoMuPhi = 0;
  _hltPfMhtNoMuPx = _hltPfMhtNoMuPy = _hltPfMhtNoMuPt = _hltPfMhtNoMuPhi = 0;
  _hltCaloMetPx = _hltCaloMetPy = _hltCaloMetPt = _hltCaloMetPhi = 0;
  _hltCaloMetCleanPx = _hltCaloMetCleanPy = _hltCaloMetCleanPt = _hltCaloMetCleanPhi = 0;
  _hltCaloMhtPx = _hltCaloMhtPy = _hltCaloMhtPt = _hltCaloMhtPhi = 0;
  //
  HltTree->Branch("hltPfMetPx"     , &_hltPfMetPx     , "hltPfMetPx/F");
  HltTree->Branch("hltPfMetPy"     , &_hltPfMetPy     , "hltPfMetPy/F");
  HltTree->Branch("hltPfMetPt"     , &_hltPfMetPt     , "hltPfMetPt/F");
  HltTree->Branch("hltPfMetPhi"     , &_hltPfMetPhi     , "hltPfMetPhi/F");
  //
  HltTree->Branch("hltPfMhtPx"     , &_hltPfMhtPx     , "hltPfMhtPx/F");
  HltTree->Branch("hltPfMhtPy"     , &_hltPfMhtPy     , "hltPfMhtPy/F");
  HltTree->Branch("hltPfMhtPt"     , &_hltPfMhtPt     , "hltPfMhtPt/F");
  HltTree->Branch("hltPfMhtPhi"     , &_hltPfMhtPhi     , "hltPfMhtPhi/F");
  //
  HltTree->Branch("hltPfMetNoMuPx"     , &_hltPfMetNoMuPx     , "hltPfMetNoMuPx/F");
  HltTree->Branch("hltPfMetNoMuPy"     , &_hltPfMetNoMuPy     , "hltPfMetNoMuPy/F");
  HltTree->Branch("hltPfMetNoMuPt"     , &_hltPfMetNoMuPt     , "hltPfMetNoMuPt/F");
  HltTree->Branch("hltPfMetNoMuPhi"     , &_hltPfMetNoMuPhi     , "hltPfMetNoMuPhi/F");
  //
  HltTree->Branch("hltPfMhtNoMuPx"     , &_hltPfMhtNoMuPx     , "hltPfMhtNoMuPx/F");
  HltTree->Branch("hltPfMhtNoMuPy"     , &_hltPfMhtNoMuPy     , "hltPfMhtNoMuPy/F");
  HltTree->Branch("hltPfMhtNoMuPt"     , &_hltPfMhtNoMuPt     , "hltPfMhtNoMuPt/F");
  HltTree->Branch("hltPfMhtNoMuPhi"     , &_hltPfMhtNoMuPhi     , "hltPfMhtNoMuPhi/F");
  //
  HltTree->Branch("hltCaloMetPx"     , &_hltCaloMetPx     , "hltCaloMetPx/F");
  HltTree->Branch("hltCaloMetPy"     , &_hltCaloMetPy     , "hltCaloMetPy/F");
  HltTree->Branch("hltCaloMetPt"     , &_hltCaloMetPt     , "hltCaloMetPt/F");
  HltTree->Branch("hltCaloMetPhi"     , &_hltCaloMetPhi     , "hltCaloMetPhi/F");
  //
  HltTree->Branch("hltCaloMetCleanPx"     , &_hltCaloMetCleanPx     , "hltCaloMetCleanPx/F");
  HltTree->Branch("hltCaloMetCleanPy"     , &_hltCaloMetCleanPy     , "hltCaloMetCleanPy/F");
  HltTree->Branch("hltCaloMetCleanPt"     , &_hltCaloMetCleanPt     , "hltCaloMetCleanPt/F");
  HltTree->Branch("hltCaloMetCleanPhi"     , &_hltCaloMetCleanPhi     , "hltCaloMetCleanPhi/F");
  //
  HltTree->Branch("hltCaloMhtPx"     , &_hltCaloMhtPx     , "hltCaloMhtPx/F");
  HltTree->Branch("hltCaloMhtPy"     , &_hltCaloMhtPy     , "hltCaloMhtPy/F");
  HltTree->Branch("hltCaloMhtPt"     , &_hltCaloMhtPt     , "hltCaloMhtPt/F");
  HltTree->Branch("hltCaloMhtPhi"     , &_hltCaloMhtPhi     , "hltCaloMhtPhi/F");
  //

  // Jets 
  const uint kMaxJet = 10;

  _hltPfJetsPx     = new float[kMaxJet];
  _hltPfJetsPy     = new float[kMaxJet];
  _hltPfJetsPz     = new float[kMaxJet];
  _hltPfJetsPt     = new float[kMaxJet];
  _hltPfJetsE      = new float[kMaxJet];
  _hltPfJetsEta    = new float[kMaxJet];
  _hltPfJetsPhi    = new float[kMaxJet];
  _hltPfJetsElecF  = new float[kMaxJet];
  _hltPfJetsPhoF   = new float[kMaxJet];
  _hltPfJetsCEF    = new float[kMaxJet];
  _hltPfJetsCHF    = new float[kMaxJet];
  _hltPfJetsNEF    = new float[kMaxJet];
  _hltPfJetsNHF    = new float[kMaxJet];
  _hltPfJetsCMF    = new float[kMaxJet];
  _hltPfJetsNTOT   = new float[kMaxJet];
  _hltPfJetsNCH    = new float[kMaxJet];
  _hltPfJetsSpread = new float[kMaxJet];
  _hltPfJetsArea   = new float[kMaxJet];

  _hltPfJetsCorPx     = new float[kMaxJet];
  _hltPfJetsCorPy     = new float[kMaxJet];
  _hltPfJetsCorPz     = new float[kMaxJet];
  _hltPfJetsCorPt     = new float[kMaxJet];
  _hltPfJetsCorE      = new float[kMaxJet];
  _hltPfJetsCorEta    = new float[kMaxJet];
  _hltPfJetsCorPhi    = new float[kMaxJet];
  _hltPfJetsCorElecF  = new float[kMaxJet];
  _hltPfJetsCorPhoF   = new float[kMaxJet];
  _hltPfJetsCorCEF    = new float[kMaxJet];
  _hltPfJetsCorCHF    = new float[kMaxJet];
  _hltPfJetsCorNEF    = new float[kMaxJet];
  _hltPfJetsCorNHF    = new float[kMaxJet];
  _hltPfJetsCorCMF    = new float[kMaxJet];
  _hltPfJetsCorNTOT   = new float[kMaxJet];
  _hltPfJetsCorNCH    = new float[kMaxJet];
  _hltPfJetsCorSpread = new float[kMaxJet];
  _hltPfJetsCorArea   = new float[kMaxJet];

  _hltCaloJetsPx     = new float[kMaxJet];
  _hltCaloJetsPy     = new float[kMaxJet];
  _hltCaloJetsPz     = new float[kMaxJet];
  _hltCaloJetsPt     = new float[kMaxJet];
  _hltCaloJetsE      = new float[kMaxJet];
  _hltCaloJetsEta    = new float[kMaxJet];
  _hltCaloJetsPhi    = new float[kMaxJet];
  _hltCaloJetsEF     = new float[kMaxJet];
  _hltCaloJetsHF     = new float[kMaxJet];
  _hltCaloJetsNTOT   = new float[kMaxJet];
  _hltCaloJetsSpread = new float[kMaxJet];
  _hltCaloJetsArea   = new float[kMaxJet];

  _hltCaloJetsCorPx     = new float[kMaxJet];
  _hltCaloJetsCorPy     = new float[kMaxJet];
  _hltCaloJetsCorPz     = new float[kMaxJet];
  _hltCaloJetsCorPt     = new float[kMaxJet];
  _hltCaloJetsCorE      = new float[kMaxJet];
  _hltCaloJetsCorEta    = new float[kMaxJet];
  _hltCaloJetsCorPhi    = new float[kMaxJet];
  _hltCaloJetsCorEF     = new float[kMaxJet];
  _hltCaloJetsCorHF     = new float[kMaxJet];
  _hltCaloJetsCorNTOT   = new float[kMaxJet];
  _hltCaloJetsCorSpread = new float[kMaxJet];
  _hltCaloJetsCorArea   = new float[kMaxJet];
 
  HltTree->Branch("nHltPfJet"     , &_nHltPfJet     , "nHltPfJet/I");
  HltTree->Branch("nHltPfJetCor"  , &_nHltPfJetCor  , "nHltPfJetCor/I");
  HltTree->Branch("nHltCaloJet"   , &_nHltCaloJet   , "nHltCaloJet/I");
  HltTree->Branch("nHltCaloJetCor", &_nHltCaloJetCor, "nHltCaloJetCor/I");

  HltTree->Branch("hltPfJetsPx"    , _hltPfJetsPx     , "hltPfJetsPx[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsPy"    , _hltPfJetsPy     , "hltPfJetsPy[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsPz"    , _hltPfJetsPz     , "hltPfJetsPz[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsPt"    , _hltPfJetsPt     , "hltPfJetsPt[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsE"     , _hltPfJetsE      , "hltPfJetsE[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsEta"   , _hltPfJetsEta    , "hltPfJetsEta[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsPhi"   , _hltPfJetsPhi    , "hltPfJetsPhi[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsElecF"   , _hltPfJetsElecF    , "hltPfJetsElecF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsPhoF"   , _hltPfJetsPhoF    , "hltPfJetsPhoF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsCEF"   , _hltPfJetsCEF    , "hltPfJetsCEF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsCHF"   , _hltPfJetsCHF    , "hltPfJetsCHF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsNEF"   , _hltPfJetsNEF    , "hltPfJetsNEF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsNHF"   , _hltPfJetsNHF    , "hltPfJetsNHF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsCMF"   , _hltPfJetsCMF    , "hltPfJetsCMF[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsNTOT"  , _hltPfJetsNTOT   , "hltPfJetsNTOT[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsNCH"   , _hltPfJetsNCH    , "hltPfJetsNCH[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsSpread", _hltPfJetsSpread , "hltPfJetsSpread[nHltPfJet]/F");
  HltTree->Branch("hltPfJetsArea"  , _hltPfJetsArea   , "hltPfJetsArea[nHltPfJet]/F");

  HltTree->Branch("hltPfJetsCorPx" , _hltPfJetsCorPx , "hltPfJetsCorPx[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorPy" , _hltPfJetsCorPy , "hltPfJetsCorPy[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorPz" , _hltPfJetsCorPz , "hltPfJetsCorPz[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorPt" , _hltPfJetsCorPt , "hltPfJetsCorPt[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorE" , _hltPfJetsCorE , "hltPfJetsCorE[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorEta" , _hltPfJetsCorEta , "hltPfJetsCorEta[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorPhi" , _hltPfJetsCorPhi , "hltPfJetsCorPhi[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorElecF" , _hltPfJetsCorElecF , "hltPfJetsCorElecF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorPhoF" , _hltPfJetsCorPhoF , "hltPfJetsCorPhoF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorCEF" , _hltPfJetsCorCEF , "hltPfJetsCorCEF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorCHF" , _hltPfJetsCorCHF , "hltPfJetsCorCHF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorNEF" , _hltPfJetsCorNEF , "hltPfJetsCorNEF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorNHF" , _hltPfJetsCorNHF , "hltPfJetsCorNHF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorCMF" , _hltPfJetsCorCMF , "hltPfJetsCorCMF[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorNTOT" , _hltPfJetsCorNTOT , "hltPfJetsCorNTOT[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorNCH" , _hltPfJetsCorNCH , "hltPfJetsCorNCH[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorSpread" , _hltPfJetsCorSpread , "hltPfJetsCorSpread[nHltPfJetCor]/F");
  HltTree->Branch("hltPfJetsCorArea" , _hltPfJetsCorArea , "hltPfJetsCorArea[nHltPfJetCor]/F");

  HltTree->Branch("hltCaloJetsPx" , _hltCaloJetsPx , "hltCaloJetsPx[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsPy" , _hltCaloJetsPy , "hltCaloJetsPy[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsPz" , _hltCaloJetsPz , "hltCaloJetsPz[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsPt" , _hltCaloJetsPt , "hltCaloJetsPt[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsE" , _hltCaloJetsE , "hltCaloJetsE[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsEta" , _hltCaloJetsEta , "hltCaloJetsEta[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsPhi" , _hltCaloJetsPhi , "hltCaloJetsPhi[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsEF" , _hltCaloJetsEF , "hltCaloJetsEF[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsHF" , _hltCaloJetsHF , "hltCaloJetsHF[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsNTOT" , _hltCaloJetsNTOT , "hltCaloJetsNTOT[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsSpread" , _hltCaloJetsSpread , "hltCaloJetsSpread[nHltCaloJet]/F");
  HltTree->Branch("hltCaloJetsArea" , _hltCaloJetsArea , "hltCaloJetsArea[nHltCaloJet]/F");

  HltTree->Branch("hltCaloJetsCorPx" , _hltCaloJetsCorPx , "hltCaloJetsCorPx[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorPy" , _hltCaloJetsCorPy , "hltCaloJetsCorPy[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorPz" , _hltCaloJetsCorPz , "hltCaloJetsCorPz[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorPt" , _hltCaloJetsCorPt , "hltCaloJetsCorPt[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorE" , _hltCaloJetsCorE , "hltCaloJetsCorE[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorEta" , _hltCaloJetsCorEta , "hltCaloJetsCorEta[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorPhi" , _hltCaloJetsCorPhi , "hltCaloJetsCorPhi[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorEF" , _hltCaloJetsCorEF , "hltCaloJetsCorEF[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorHF" , _hltCaloJetsCorHF , "hltCaloJetsCorHF[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorNTOT" , _hltCaloJetsCorNTOT , "hltCaloJetsCorNTOT[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorSpread" , _hltCaloJetsCorSpread , "hltCaloJetsCorSpread[nHltCaloJetCor]/F");
  HltTree->Branch("hltCaloJetsCorArea" , _hltCaloJetsCorArea , "hltCaloJetsCorArea[nHltCaloJetCor]/F");

  // RECO JETS //ND
  if(_RecoPFJets) {
    _recoPfJetsPx     = new float[kMaxJet];
    _recoPfJetsPy     = new float[kMaxJet];
    _recoPfJetsPz     = new float[kMaxJet];
    _recoPfJetsPt     = new float[kMaxJet];
    _recoPfJetsE      = new float[kMaxJet];
    _recoPfJetsEta    = new float[kMaxJet];
    _recoPfJetsPhi    = new float[kMaxJet];
    _recoPfJetsElecF  = new float[kMaxJet];
    _recoPfJetsPhoF   = new float[kMaxJet];
    _recoPfJetsCEF    = new float[kMaxJet];
    _recoPfJetsCHF    = new float[kMaxJet];
    _recoPfJetsNEF    = new float[kMaxJet];
    _recoPfJetsNHF    = new float[kMaxJet];
    _recoPfJetsCMF    = new float[kMaxJet];
    _recoPfJetsNTOT   = new float[kMaxJet];
    _recoPfJetsNCH    = new float[kMaxJet];
    _recoPfJetsSpread = new float[kMaxJet];
    _recoPfJetsArea   = new float[kMaxJet];

    _recoPfJetsCorPx     = new float[kMaxJet];
    _recoPfJetsCorPy     = new float[kMaxJet];
    _recoPfJetsCorPz     = new float[kMaxJet];
    _recoPfJetsCorPt     = new float[kMaxJet];
    _recoPfJetsCorE      = new float[kMaxJet];
    _recoPfJetsCorEta    = new float[kMaxJet];
    _recoPfJetsCorPhi    = new float[kMaxJet];
    _recoPfJetsCorElecF  = new float[kMaxJet];
    _recoPfJetsCorPhoF   = new float[kMaxJet];
    _recoPfJetsCorCEF    = new float[kMaxJet];
    _recoPfJetsCorCHF    = new float[kMaxJet];
    _recoPfJetsCorNEF    = new float[kMaxJet];
    _recoPfJetsCorNHF    = new float[kMaxJet];
    _recoPfJetsCorCMF    = new float[kMaxJet];
    _recoPfJetsCorNTOT   = new float[kMaxJet];
    _recoPfJetsCorNCH    = new float[kMaxJet];
    _recoPfJetsCorSpread = new float[kMaxJet];
    _recoPfJetsCorArea   = new float[kMaxJet];

    HltTree->Branch("nRecoPfJet"     , &_nRecoPfJet     , "nRecoPfJet/I");
    HltTree->Branch("nRecoPfJetCor"  , &_nRecoPfJetCor  , "nRecoPfJetCor/I");

    HltTree->Branch("recoPfJetsPx"    , _recoPfJetsPx     , "recoPfJetsPx[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsPy"    , _recoPfJetsPy     , "recoPfJetsPy[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsPz"    , _recoPfJetsPz     , "recoPfJetsPz[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsPt"    , _recoPfJetsPt     , "recoPfJetsPt[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsE"     , _recoPfJetsE      , "recoPfJetsE[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsEta"   , _recoPfJetsEta    , "recoPfJetsEta[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsPhi"   , _recoPfJetsPhi    , "recoPfJetsPhi[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsElecF"   , _recoPfJetsElecF    , "recoPfJetsElecF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsPhoF"   , _recoPfJetsPhoF    , "recoPfJetsPhoF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsCEF"   , _recoPfJetsCEF    , "recoPfJetsCEF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsCHF"   , _recoPfJetsCHF    , "recoPfJetsCHF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsNEF"   , _recoPfJetsNEF    , "recoPfJetsNEF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsNHF"   , _recoPfJetsNHF    , "recoPfJetsNHF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsCMF"   , _recoPfJetsCMF    , "recoPfJetsCMF[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsNTOT"  , _recoPfJetsNTOT   , "recoPfJetsNTOT[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsNCH"   , _recoPfJetsNCH    , "recoPfJetsNCH[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsSpread", _recoPfJetsSpread , "recoPfJetsSpread[nRecoPfJet]/F");
    HltTree->Branch("recoPfJetsArea"  , _recoPfJetsArea   , "recoPfJetsArea[nRecoPfJet]/F");

    HltTree->Branch("recoPfJetsCorPx" , _recoPfJetsCorPx , "recoPfJetsCorPx[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorPy" , _recoPfJetsCorPy , "recoPfJetsCorPy[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorPz" , _recoPfJetsCorPz , "recoPfJetsCorPz[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorPt" , _recoPfJetsCorPt , "recoPfJetsCorPt[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorE" , _recoPfJetsCorE , "recoPfJetsCorE[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorEta" , _recoPfJetsCorEta , "recoPfJetsCorEta[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorPhi" , _recoPfJetsCorPhi , "recoPfJetsCorPhi[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorElecF" , _recoPfJetsCorElecF , "recoPfJetsCorElecF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorPhoF" , _recoPfJetsCorPhoF , "recoPfJetsCorPhoF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorCEF" , _recoPfJetsCorCEF , "recoPfJetsCorCEF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorCHF" , _recoPfJetsCorCHF , "recoPfJetsCorCHF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorNEF" , _recoPfJetsCorNEF , "recoPfJetsCorNEF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorNHF" , _recoPfJetsCorNHF , "recoPfJetsCorNHF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorCMF" , _recoPfJetsCorCMF , "recoPfJetsCorCMF[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorNTOT" , _recoPfJetsCorNTOT , "recoPfJetsCorNTOT[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorNCH" , _recoPfJetsCorNCH , "recoPfJetsCorNCH[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorSpread" , _recoPfJetsCorSpread , "recoPfJetsCorSpread[nRecoPfJetCor]/F");
    HltTree->Branch("recoPfJetsCorArea" , _recoPfJetsCorArea , "recoPfJetsCorArea[nRecoPfJetCor]/F");

  }

  if(_RecoCaloJets) {
    _recoCaloJetsPx     = new float[kMaxJet];
    _recoCaloJetsPy     = new float[kMaxJet];
    _recoCaloJetsPz     = new float[kMaxJet];
    _recoCaloJetsPt     = new float[kMaxJet];
    _recoCaloJetsE      = new float[kMaxJet];
    _recoCaloJetsEta    = new float[kMaxJet];
    _recoCaloJetsPhi    = new float[kMaxJet];
    _recoCaloJetsEF     = new float[kMaxJet];
    _recoCaloJetsHF     = new float[kMaxJet];
    _recoCaloJetsNTOT   = new float[kMaxJet];
    _recoCaloJetsSpread = new float[kMaxJet];
    _recoCaloJetsArea   = new float[kMaxJet];

    _recoCaloJetsCorPx     = new float[kMaxJet];
    _recoCaloJetsCorPy     = new float[kMaxJet];
    _recoCaloJetsCorPz     = new float[kMaxJet];
    _recoCaloJetsCorPt     = new float[kMaxJet];
    _recoCaloJetsCorE      = new float[kMaxJet];
    _recoCaloJetsCorEta    = new float[kMaxJet];
    _recoCaloJetsCorPhi    = new float[kMaxJet];
    _recoCaloJetsCorEF     = new float[kMaxJet];
    _recoCaloJetsCorHF     = new float[kMaxJet];
    _recoCaloJetsCorNTOT   = new float[kMaxJet];
    _recoCaloJetsCorSpread = new float[kMaxJet];
    _recoCaloJetsCorArea   = new float[kMaxJet];

    HltTree->Branch("nRecoCaloJet"   , &_nRecoCaloJet   , "nRecoCaloJet/I");
    HltTree->Branch("nRecoCaloJetCor", &_nRecoCaloJetCor, "nRecoCaloJetCor/I");

    HltTree->Branch("recoCaloJetsPx" , _recoCaloJetsPx , "recoCaloJetsPx[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsPy" , _recoCaloJetsPy , "recoCaloJetsPy[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsPz" , _recoCaloJetsPz , "recoCaloJetsPz[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsPt" , _recoCaloJetsPt , "recoCaloJetsPt[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsE" , _recoCaloJetsE , "recoCaloJetsE[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsEta" , _recoCaloJetsEta , "recoCaloJetsEta[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsPhi" , _recoCaloJetsPhi , "recoCaloJetsPhi[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsEF" , _recoCaloJetsEF , "recoCaloJetsEF[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsHF" , _recoCaloJetsHF , "recoCaloJetsHF[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsNTOT" , _recoCaloJetsNTOT , "recoCaloJetsNTOT[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsSpread" , _recoCaloJetsSpread , "recoCaloJetsSpread[nRecoCaloJet]/F");
    HltTree->Branch("recoCaloJetsArea" , _recoCaloJetsArea , "recoCaloJetsArea[nRecoCaloJet]/F");

    HltTree->Branch("recoCaloJetsCorPx" , _recoCaloJetsCorPx , "recoCaloJetsCorPx[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorPy" , _recoCaloJetsCorPy , "recoCaloJetsCorPy[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorPz" , _recoCaloJetsCorPz , "recoCaloJetsCorPz[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorPt" , _recoCaloJetsCorPt , "recoCaloJetsCorPt[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorE" , _recoCaloJetsCorE , "recoCaloJetsCorE[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorEta" , _recoCaloJetsCorEta , "recoCaloJetsCorEta[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorPhi" , _recoCaloJetsCorPhi , "recoCaloJetsCorPhi[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorEF" , _recoCaloJetsCorEF , "recoCaloJetsCorEF[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorHF" , _recoCaloJetsCorHF , "recoCaloJetsCorHF[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorNTOT" , _recoCaloJetsCorNTOT , "recoCaloJetsCorNTOT[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorSpread" , _recoCaloJetsCorSpread , "recoCaloJetsCorSpread[nRecoCaloJetCor]/F");
    HltTree->Branch("recoCaloJetsCorArea" , _recoCaloJetsCorArea , "recoCaloJetsCorArea[nRecoCaloJetCor]/F");

  } 

  // MET and MHT
  _recPfMetPx = _recPfMetPy = _recPfMetPt = _recPfMetPhi = 0;
  _recPfMhtPx = _recPfMhtPy = _recPfMhtPt = _recPfMhtPhi = 0;
  _recPfMetNoMuPx = _recPfMetNoMuPy = _recPfMetNoMuPt = _recPfMetNoMuPhi = 0;
  _recPfMhtNoMuPx = _recPfMhtNoMuPy = _recPfMhtNoMuPt = _recPfMhtNoMuPhi = 0;
  _recCaloMetPx = _recCaloMetPy = _recCaloMetPt = _recCaloMetPhi = 0;
  _recCaloMhtPx = _recCaloMhtPy = _recCaloMhtPt = _recCaloMhtPhi = 0;
  //
  if(_RecoPFMET) {
    HltTree->Branch("recPfMetPx"     , &_recPfMetPx     , "recPfMetPx/F");
    HltTree->Branch("recPfMetPy"     , &_recPfMetPy     , "recPfMetPy/F");
    HltTree->Branch("recPfMetPt"     , &_recPfMetPt     , "recPfMetPt/F");
    HltTree->Branch("recPfMetPhi"     , &_recPfMetPhi     , "recPfMetPhi/F");
    if(_RecoMuons) {
      HltTree->Branch("recPfMetNoMuPx"     , &_recPfMetNoMuPx     , "recPfMetNoMuPx/F");
      HltTree->Branch("recPfMetNoMuPy"     , &_recPfMetNoMuPy     , "recPfMetNoMuPy/F");
      HltTree->Branch("recPfMetNoMuPt"     , &_recPfMetNoMuPt     , "recPfMetNoMuPt/F");
      HltTree->Branch("recPfMetNoMuPhi"     , &_recPfMetNoMuPhi     , "recPfMetNoMuPhi/F");
    }
  }
  //
  if(_RecoPFJets) {
    HltTree->Branch("recPfMhtPx"     , &_recPfMhtPx     , "recPfMhtPx/F");
    HltTree->Branch("recPfMhtPy"     , &_recPfMhtPy     , "recPfMhtPy/F");
    HltTree->Branch("recPfMhtPt"     , &_recPfMhtPt     , "recPfMhtPt/F");
    HltTree->Branch("recPfMhtPhi"     , &_recPfMhtPhi     , "recPfMhtPhi/F");
    if(_RecoMuons) {
      HltTree->Branch("recPfMhtNoMuPx"     , &_recPfMhtNoMuPx     , "recPfMhtNoMuPx/F");
      HltTree->Branch("recPfMhtNoMuPy"     , &_recPfMhtNoMuPy     , "recPfMhtNoMuPy/F");
      HltTree->Branch("recPfMhtNoMuPt"     , &_recPfMhtNoMuPt     , "recPfMhtNoMuPt/F");
      HltTree->Branch("recPfMhtNoMuPhi"     , &_recPfMhtNoMuPhi     , "recPfMhtNoMuPhi/F");
    }
  }
  //
  if(_RecoCaloMET) {
    HltTree->Branch("recCaloMetPx"     , &_recCaloMetPx     , "recCaloMetPx/F");
    HltTree->Branch("recCaloMetPy"     , &_recCaloMetPy     , "recCaloMetPy/F");
    HltTree->Branch("recCaloMetPt"     , &_recCaloMetPt     , "recCaloMetPt/F");
    HltTree->Branch("recCaloMetPhi"     , &_recCaloMetPhi     , "recCaloMetPhi/F");
  }
  //
  if(_RecoCaloJets) {
    HltTree->Branch("recCaloMhtPx"     , &_recCaloMhtPx     , "recCaloMhtPx/F");
    HltTree->Branch("recCaloMhtPy"     , &_recCaloMhtPy     , "recCaloMhtPy/F");
    HltTree->Branch("recCaloMhtPt"     , &_recCaloMhtPt     , "recCaloMhtPt/F");
    HltTree->Branch("recCaloMhtPhi"     , &_recCaloMhtPhi     , "recCaloMhtPhi/F");
  }

  // END BRANCHES //

}

/* **Analyze the event** */
void HLTJetMET::analyze(const edm::Handle<reco::PFJetCollection>   & H_HltPfJets,        //ND
			const edm::Handle<reco::PFJetCollection>   & H_HltPfJetsCorr,    //ND
			const edm::Handle<reco::CaloJetCollection> & H_HltCaloJets,      //ND
			const edm::Handle<reco::CaloJetCollection> & H_HltCaloJetsCorr,  //ND
			const edm::Handle<reco::PFMETCollection>   & H_HltPFMET   , 
			const edm::Handle<reco::PFMETCollection>   & H_HltPFMETNoMu ,
			const edm::Handle<reco::METCollection>     & H_HltPFMHT   , 
			const edm::Handle<reco::METCollection>     & H_HltPFMHTNoMu ,
			const edm::Handle<reco::CaloMETCollection> & H_HltCaloMET , 
			const edm::Handle<reco::CaloMETCollection> & H_HltCaloMETClean ,
			const edm::Handle<reco::METCollection>     & H_HltCaloMHT ,
			const edm::Handle<reco::PFJetCollection>   & H_RecoPfJets,        //ND
			const edm::Handle<reco::PFJetCollection>   & H_RecoPfJetsCorr,    //ND
			const edm::Handle<reco::CaloJetCollection> & H_RecoCaloJets,      //ND
			const edm::Handle<reco::CaloJetCollection> & H_RecoCaloJetsCorr,  //ND
			const edm::Handle<reco::PFMETCollection>   & H_RecoPFMET   ,
			const edm::Handle<reco::CaloMETCollection> & H_RecoCaloMET ,
			const edm::Handle<reco::MuonCollection>    & H_RecoMuons ,
			edm::EventSetup const& eventSetup,
			edm::Event const& iEvent,
			TTree* HltTree) {

  //   std::cout << " Beginning HLTJetMET " << std::endl;

  /////////// Analyzing HLT Collections ////////// //ND

  if (_Debug) std::cout << "%HLTJetMET --  Start analyzing HLT collections //ND" << std::endl
			<< "%HLTJetMET --  default values"                       << std::endl;

  // Default values (initialize)
  const uint kMaxJet = 10;
  InitHltJets(kMaxJet);
  InitMET();

  // HLT PFMET
  if (_Debug) std::cout << "%HLTJetMET --  HLT PFMET" << std::endl;
  if( H_HltPFMET.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPFMET Handle is valid" << std::endl;
    FillPFMET(H_HltPFMET, "HLTPFMET");
  }

  // HLT PFMHT
  if (_Debug) std::cout << "%HLTJetMET --  HLT PFMHT" << std::endl;
  if( H_HltPFMHT.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPFMHT Handle is valid" << std::endl;
    FillMET(H_HltPFMHT, "HLTPFMHT");
  }

  // HLT PFMETNoMu
  if (_Debug) std::cout << "%HLTJetMET --  HLT PFMETNoMu" << std::endl;
  if( H_HltPFMETNoMu.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPFMETNoMu Handle is valid" << std::endl;
    FillPFMET(H_HltPFMETNoMu, "HLTPFMETNoMu");
  }

  // HLT PFMHTNoMu
  if (_Debug) std::cout << "%HLTJetMET --  HLT PFMHTNoMu" << std::endl;
  if( H_HltPFMHTNoMu.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPFMHTNoMu Handle is valid" << std::endl;
    FillMET(H_HltPFMHTNoMu, "HLTPFMHTNoMu");
  }

  // HLT CaloMET
  if (_Debug) std::cout << "%HLTJetMET --  HLT CaloMET" << std::endl;
  if( H_HltCaloMET.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltCaloMET Handle is valid" << std::endl;
    FillCaloMET(H_HltCaloMET, "HLTCaloMET");
  }

  // HLT CaloMETClean
  if (_Debug) std::cout << "%HLTJetMET --  HLT CaloMETClean" << std::endl;
  if( H_HltCaloMETClean.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltCaloMETClean Handle is valid" << std::endl;
    FillCaloMET(H_HltCaloMETClean, "HLTCaloMETClean");
  }

  // HLT CaloMHT
  if (_Debug) std::cout << "%HLTJetMET --  HLT CaloMHT" << std::endl;
  if( H_HltCaloMHT.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltCaloMHT Handle is valid" << std::endl;
    FillMET(H_HltCaloMHT, "HLTCaloMHT");
  }


  // HLT ak4 PFJets
  if (_Debug) std::cout << "%HLTJetMET --  HLT ak4 PFJets" << std::endl;
  if( H_HltPfJets.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPfJets Handle is valid" << std::endl;
    FillHltPfJets(kMaxJet, H_HltPfJets, "uncorr");
  }

  // HLT ak4 PFJets corrected
  if (_Debug) std::cout << "%HLTJetMET --  HLT ak4 PFJets corrected" << std::endl;
  if( H_HltPfJetsCorr.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltPfJetsCorr Handle is valid" << std::endl;
    FillHltPfJets(kMaxJet, H_HltPfJetsCorr, "corr");
  }

  // HLT ak4 CaloJets
  if (_Debug) std::cout << "%HLTJetMET --  HLT ak4 CaloJets" << std::endl;
  if( H_HltCaloJets.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltCaloJets Handle is valid" << std::endl;
    FillHltCaloJets(kMaxJet, H_HltCaloJets, "uncorr");
  }

  // HLT ak4 CaloJets corrected
  if (_Debug) std::cout << "%HLTJetMET --  HLT ak4 CaloJets corrected" << std::endl;
  if( H_HltCaloJetsCorr.isValid() ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_HltCaloJetsCorr Handle is valid" << std::endl;
    FillHltCaloJets(kMaxJet, H_HltCaloJetsCorr, "corr");
  }

  // OFFLINE RECO MET //ND
  // RECO PFMET
  if (_Debug) std::cout << "%HLTJetMET --  RECO PFMET" << std::endl;
  if( H_RecoPFMET.isValid() && _RecoPFMET ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_RecoPFMET Handle is valid" << std::endl;
    FillPFMET(H_RecoPFMET, "RECOPFMET");
  }

  // RECO PFMETNoMu --> use PFMET and Muons
  if (_Debug) std::cout << "%HLTJetMET --  RECO PFMETNoMu" << std::endl;
  if( H_RecoPFMET.isValid() && H_RecoMuons.isValid() && _RecoPFMET && _RecoMuons ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_RecoPFMET and H_RecoMuons Handles is valid" << std::endl;
    FillPFMETNoMu(H_RecoPFMET, H_RecoMuons, "RECOPFMETNoMu");
  }

  // RECO PFMHT and PFMHTNoMu --> use PFJets and Muons
  if (_Debug) std::cout << "%HLTJetMET --  RECO PFMHT and PFMHTNoMu" << std::endl;
  if( H_RecoPfJetsCorr.isValid() && H_RecoMuons.isValid() && _RecoPFJets && _RecoMuons ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_RecoPfJetsCorr and H_RecoMuons Handles are valid" << std::endl;
    FillPFMHT(H_RecoPfJetsCorr, H_RecoMuons, "RECOPFMHT_AndNoMu");
  }

  // RECO CaloMET
  if (_Debug) std::cout << "%HLTJetMET --  RECO CaloMET" << std::endl;
  if( H_RecoCaloMET.isValid() && _RecoCaloMET ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_RecoCaloMET Handle is valid" << std::endl;
    FillCaloMET(H_RecoCaloMET, "RECOCaloMET");
  }

  // RECO CaloMHT --> use CaloJets
  if (_Debug) std::cout << "%HLTJetMET --  RECO CaloMHT" << std::endl;
  if( H_RecoCaloJets.isValid() && _RecoCaloJets ) {
    if (_Debug) std::cout << "%HLTJetMET --  H_RecoCaloJets Handle is valid" << std::endl;
    FillCaloMHT(H_RecoCaloJets, "RECOCaloMHT");
  }

  // OFFLINE RECO JETS //ND
  if(_Debug) std::cout << "%HLTJetMET -- InitRecoJets..." << std::endl;
  if(_RecoPFJets || _RecoCaloJets) {
    if(_Debug) std::cout << "%HLTJetMET -- InitRecoJets(" << kMaxJet << ")... " << std::endl;
    InitRecoJets(kMaxJet);
    if(_Debug) std::cout << "%HLTJetMET --  ...done!" << std::endl;
  }
  else {
    if(_Debug) std::cout << "%HLTJetMET --  not launched!" << std::endl;
  }

  if(_RecoPFJets) {

    // RECO ak4 PFJets
    if (_Debug) std::cout << "%HLTJetMET --  RECO ak4 PFJets" << std::endl;
    if( H_RecoPfJets.isValid() ) {
      if (_Debug) std::cout << "%HLTJetMET --  H_RecoPfJets Handle is valid" << std::endl;
      FillRecoPfJets(kMaxJet, H_RecoPfJets, "uncorr");
    }

    // RECO ak4 PFJets corrected
    if (_Debug) std::cout << "%HLTJetMET --  RECO ak4 PFJets corrected" << std::endl;
    if( H_RecoPfJetsCorr.isValid() ) {
      if (_Debug) std::cout << "%HLTJetMET --  H_RecoPfJetsCorr Handle is valid" << std::endl;
      FillRecoPfJets(kMaxJet, H_RecoPfJetsCorr, "corr");
    }
  }

  if(_RecoCaloJets) {
    // RECO ak4 CaloJets
    if (_Debug) std::cout << "%HLTJetMET --  RECO ak4 CaloJets" << std::endl;
    if( H_RecoCaloJets.isValid() ) {
      if (_Debug) std::cout << "%HLTJetMET --  H_RecoCaloJets Handle is valid" << std::endl;
      FillRecoCaloJets(kMaxJet, H_RecoCaloJets, "uncorr");
    }

    // RECO ak4 CaloJets corrected
    if (_Debug) std::cout << "%HLTJetMET --  RECO ak4 CaloJets corrected" << std::endl;
    if( H_RecoCaloJetsCorr.isValid() ) {
      if (_Debug) std::cout << "%HLTJetMET --  H_RecoCaloJetsCorr Handle is valid" << std::endl;
      FillRecoCaloJets(kMaxJet, H_RecoCaloJetsCorr, "corr");
    }

  }

  // END COLLECTIONS //                   
  if(_Debug) std::cout << "%HLTJetMET -- End reading collections" << std::endl;

}
      
int HLTJetMET::FillHltPfJets(uint kMaxJet, const edm::Handle<reco::PFJetCollection> & H_HltPfJets, std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillHltPfJets(" << kMaxJet << " , H_HltPfJets , " << mode << ")" << std::endl;

  if (_Debug) std::cout << "%HLTJetMET --  get and sort collection...";  
  reco::PFJetCollection Jets = *H_HltPfJets;
  sort( Jets.begin() , Jets.end() , _jetsorter );
  if (_Debug) std::cout << " sorted!" << std::endl;  

  // Fill output counter and arrays //

  // Check size
  uint vsize = Jets.size();
  if (_Debug) std::cout << "%HLTJetMET --  fill output counter and arrays: loop over sorted PFJets (size=" << vsize << ")" << std::endl;

  // Initialize counters
  if(     mode=="uncorr") _nHltPfJet    = 0 ;
  else if(mode=="corr"  ) _nHltPfJetCor = 0 ;

  // Loop over sorted jets
  uint i=-1;
  for(reco::PFJetCollection::const_iterator iter = Jets.begin() ; iter != Jets.end() ; iter++) {

    i++ ;
    if(i>=vsize || i>=kMaxJet) break;

    if (_Debug && i==0) std::cout << "%HLTJetMET --  HLT ak4 PFJet #0: pT=" << iter->pt() << std::endl;
    
    if(mode=="uncorr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: uncorr" << std::endl;

      _hltPfJetsPx[i]     = iter->px(); 
      _hltPfJetsPy[i]     = iter->py(); 
      _hltPfJetsPz[i]     = iter->pz(); 
      _hltPfJetsPt[i]     = iter->pt(); 
      _hltPfJetsE[i]      = iter->energy(); 
      _hltPfJetsEta[i]    = iter->eta(); 
      _hltPfJetsPhi[i]    = iter->phi(); 
      _hltPfJetsElecF[i]  = iter->electronEnergyFraction(); 
      _hltPfJetsPhoF[i]   = iter->photonEnergyFraction(); 
      _hltPfJetsCEF[i]    = iter->chargedEmEnergyFraction(); 
      _hltPfJetsCHF[i]    = iter->chargedHadronEnergyFraction(); 
      _hltPfJetsNEF[i]    = iter->neutralEmEnergyFraction(); 
      _hltPfJetsNHF[i]    = iter->neutralHadronEnergyFraction(); 
      _hltPfJetsCMF[i]    = iter->chargedMuEnergyFraction(); 
      _hltPfJetsNTOT[i]   = iter->numberOfDaughters(); 
      _hltPfJetsNCH[i]    = iter->chargedMultiplicity(); 
      _hltPfJetsSpread[i] = iter->constituentEtaPhiSpread(); 
      _hltPfJetsArea[i]   = iter->jetArea(); 
      _nHltPfJet++ ;
    }

    else if(mode=="corr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: corr" << std::endl;

      _hltPfJetsCorPx[i]     = iter->px(); 
      _hltPfJetsCorPy[i]     = iter->py(); 
      _hltPfJetsCorPz[i]     = iter->pz(); 
      _hltPfJetsCorPt[i]     = iter->pt(); 
      _hltPfJetsCorE[i]      = iter->energy(); 
      _hltPfJetsCorEta[i]    = iter->eta(); 
      _hltPfJetsCorPhi[i]    = iter->phi(); 
      _hltPfJetsCorElecF[i]  = iter->electronEnergyFraction(); 
      _hltPfJetsCorPhoF[i]   = iter->photonEnergyFraction(); 
      _hltPfJetsCorCEF[i]    = iter->chargedEmEnergyFraction(); 
      _hltPfJetsCorCHF[i]    = iter->chargedHadronEnergyFraction(); 
      _hltPfJetsCorNEF[i]    = iter->neutralEmEnergyFraction(); 
      _hltPfJetsCorNHF[i]    = iter->neutralHadronEnergyFraction(); 
      _hltPfJetsCorCMF[i]    = iter->chargedMuEnergyFraction(); 
      _hltPfJetsCorNTOT[i]   = iter->numberOfDaughters(); 
      _hltPfJetsCorNCH[i]    = iter->chargedMultiplicity(); 
      _hltPfJetsCorSpread[i] = iter->constituentEtaPhiSpread(); 
      _hltPfJetsCorArea[i]   = iter->jetArea(); 
      _nHltPfJetCor++ ;
    }

  }

  if (_Debug) std::cout << "%HLTJetMET --  end loop over sorted PFJets" << std::endl;

  return 0;
}

int HLTJetMET::FillHltCaloJets(uint kMaxJet, const edm::Handle<reco::CaloJetCollection> & H_HltCaloJets, std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillHltCaloJets(" << kMaxJet << " , H_HltCaloJets , " << mode << ")" << std::endl;

  // Sort collection
  if (_Debug) std::cout << "%HLTJetMET --  get and sort collection...";
  reco::CaloJetCollection Jets = *H_HltCaloJets;
  sort( Jets.begin() , Jets.end() , _jetsorter );
  if (_Debug) std::cout << " sorted!" << std::endl;

  // Fill output counter and arrays //

  // Check size
  uint vsize = Jets.size();
  if (_Debug) std::cout << "%HLTJetMET --  fill output counter and arrays: loop over sorted CaloJets (size=" << vsize << ")" << std::endl;

  // Initialize counters
  if(     mode=="uncorr") _nHltCaloJet    = 0 ;
  else if(mode=="corr"  ) _nHltCaloJetCor = 0 ;

  // Loop over sorted jets
  uint i=-1;
  for(reco::CaloJetCollection::const_iterator iter = Jets.begin() ; iter != Jets.end() ; iter++) {

    i++ ;
    if(i>=vsize || i>=kMaxJet) break;

    if (_Debug && i==0) std::cout << "%HLTJetMET --  HLT ak4 CaloJet #0: pT=" << iter->pt() << std::endl;
    
    if(mode=="uncorr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: uncorr" << std::endl;

      _hltCaloJetsPx[i]     = iter->px(); 
      _hltCaloJetsPy[i]     = iter->py(); 
      _hltCaloJetsPz[i]     = iter->pz(); 
      _hltCaloJetsPt[i]     = iter->pt(); 
      _hltCaloJetsE[i]      = iter->energy(); 
      _hltCaloJetsEta[i]    = iter->eta(); 
      _hltCaloJetsPhi[i]    = iter->phi(); 
      _hltCaloJetsEF[i]     = iter->emEnergyFraction(); 
      _hltCaloJetsHF[i]     = iter->energyFractionHadronic(); 
      _hltCaloJetsNTOT[i]   = iter->numberOfDaughters(); 
      _hltCaloJetsSpread[i] = iter->constituentEtaPhiSpread(); 
      _hltCaloJetsArea[i]   = iter->jetArea(); 

      _nHltCaloJet++ ;
    }

    else if(mode=="corr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: corr" << std::endl;

      _hltCaloJetsCorPx[i]     = iter->px(); 
      _hltCaloJetsCorPy[i]     = iter->py(); 
      _hltCaloJetsCorPz[i]     = iter->pz(); 
      _hltCaloJetsCorPt[i]     = iter->pt(); 
      _hltCaloJetsCorE[i]      = iter->energy(); 
      _hltCaloJetsCorEta[i]    = iter->eta(); 
      _hltCaloJetsCorPhi[i]    = iter->phi(); 
      _hltCaloJetsCorEF[i]     = iter->emEnergyFraction(); 
      _hltCaloJetsCorHF[i]     = iter->energyFractionHadronic(); 
      _hltCaloJetsCorNTOT[i]   = iter->numberOfDaughters(); 
      _hltCaloJetsCorSpread[i] = iter->constituentEtaPhiSpread(); 
      _hltCaloJetsCorArea[i]   = iter->jetArea(); 

      _nHltCaloJetCor++ ;
    }

  }

  if (_Debug) std::cout << "%HLTJetMET --  end loop over sorted CaloJets" << std::endl;

  return 0;
}

int HLTJetMET::InitHltJets(uint kMaxJet)
{

  _nHltPfJet = _nHltPfJetCor = _nHltCaloJet = _nHltCaloJetCor = 0;

  for(uint i=0 ; i<kMaxJet ; i++) {

    _hltPfJetsPx[i] = _hltPfJetsPy[i] = _hltPfJetsPz[i] = _hltPfJetsPt[i] = _hltPfJetsE[i] = _hltPfJetsEta[i] = _hltPfJetsPhi[i] = _hltPfJetsCEF[i] = 
      _hltPfJetsCHF[i] = _hltPfJetsElecF[i] = _hltPfJetsPhoF[i] = _hltPfJetsNEF[i] = _hltPfJetsNHF[i] = _hltPfJetsCMF[i] = _hltPfJetsNTOT[i] = _hltPfJetsNCH[i] = 
      _hltPfJetsSpread[i] = _hltPfJetsArea[i] = -999999 ; 

    _hltPfJetsCorPx[i] = _hltPfJetsCorPy[i] = _hltPfJetsCorPz[i] = _hltPfJetsCorPt[i] = _hltPfJetsCorE[i] = _hltPfJetsCorEta[i] = _hltPfJetsCorPhi[i] = 
      _hltPfJetsCorCEF[i] = _hltPfJetsCorCHF[i] = _hltPfJetsCorNEF[i] = _hltPfJetsCorNHF[i] = _hltPfJetsCorCMF[i] = _hltPfJetsCorNTOT[i] = _hltPfJetsCorNCH[i] =
      _hltPfJetsCorElecF[i] = _hltPfJetsCorPhoF[i] = _hltPfJetsCorSpread[i] = _hltPfJetsCorArea[i] = -999999 ;

    _hltCaloJetsPx[i] = _hltCaloJetsPy[i] = _hltCaloJetsPz[i] = _hltCaloJetsPt[i] = _hltCaloJetsE[i] = _hltCaloJetsEta[i] = _hltCaloJetsPhi[i] = 
      _hltCaloJetsEF[i] = _hltCaloJetsHF[i] = _hltCaloJetsNTOT[i] = _hltCaloJetsSpread[i] = _hltCaloJetsArea[i] = -999999 ;

    _hltCaloJetsCorPx[i] = _hltCaloJetsCorPy[i] = _hltCaloJetsCorPz[i] = _hltCaloJetsCorPt[i] = _hltCaloJetsCorE[i] = _hltCaloJetsCorEta[i] = 
      _hltCaloJetsCorPhi[i] = _hltCaloJetsCorEF[i] = _hltCaloJetsCorHF[i] = _hltCaloJetsCorNTOT[i] = _hltCaloJetsCorSpread[i] = _hltCaloJetsCorArea[i] = -999999 ;
  }

  return 0;
}

// RECO JETS METHODS //
int HLTJetMET::FillRecoPfJets(uint kMaxJet, const edm::Handle<reco::PFJetCollection> & H_RecoPfJets, std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillRecoPfJets(" << kMaxJet << " , H_RecoPfJets , " << mode << ")" << std::endl;

  if (_Debug) std::cout << "%HLTJetMET --  get and sort collection...";  
  reco::PFJetCollection Jets = *H_RecoPfJets;
  sort( Jets.begin() , Jets.end() , _jetsorter );
  if (_Debug) std::cout << " sorted!" << std::endl;  

  // Fill output counter and arrays //

  // Check size
  uint vsize = Jets.size();
  if (_Debug) std::cout << "%HLTJetMET --  fill output counter and arrays: loop over sorted PFJets (size=" << vsize << ")" << std::endl;

  // Initialize counters
  if(     mode=="uncorr") _nRecoPfJet    = 0 ;
  else if(mode=="corr"  ) _nRecoPfJetCor = 0 ;

  // Loop over sorted jets
  uint i=-1;
  for(reco::PFJetCollection::const_iterator iter = Jets.begin() ; iter != Jets.end() ; iter++) {

    i++ ;
    if(i>=vsize || i>=kMaxJet) break;

    if (_Debug && i==0) std::cout << "%HLTJetMET --  RECO ak4 PFJet #0: pT=" << iter->pt() << std::endl;
    
    if(mode=="uncorr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: uncorr" << std::endl;

      _recoPfJetsPx[i]     = iter->px(); 
      _recoPfJetsPy[i]     = iter->py(); 
      _recoPfJetsPz[i]     = iter->pz(); 
      _recoPfJetsPt[i]     = iter->pt(); 
      _recoPfJetsE[i]      = iter->energy(); 
      _recoPfJetsEta[i]    = iter->eta(); 
      _recoPfJetsPhi[i]    = iter->phi(); 
      _recoPfJetsElecF[i]  = iter->electronEnergyFraction(); 
      _recoPfJetsPhoF[i]   = iter->photonEnergyFraction(); 
      _recoPfJetsCEF[i]    = iter->chargedEmEnergyFraction(); 
      _recoPfJetsCHF[i]    = iter->chargedHadronEnergyFraction(); 
      _recoPfJetsNEF[i]    = iter->neutralEmEnergyFraction(); 
      _recoPfJetsNHF[i]    = iter->neutralHadronEnergyFraction(); 
      _recoPfJetsCMF[i]    = iter->chargedMuEnergyFraction(); 
      _recoPfJetsNTOT[i]   = iter->numberOfDaughters(); 
      _recoPfJetsNCH[i]    = iter->chargedMultiplicity(); 
      _recoPfJetsSpread[i] = iter->constituentEtaPhiSpread(); 
      _recoPfJetsArea[i]   = iter->jetArea(); 
      _nRecoPfJet++ ;
    }

    else if(mode=="corr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: corr" << std::endl;

      _recoPfJetsCorPx[i]     = iter->px(); 
      _recoPfJetsCorPy[i]     = iter->py(); 
      _recoPfJetsCorPz[i]     = iter->pz(); 
      _recoPfJetsCorPt[i]     = iter->pt(); 
      _recoPfJetsCorE[i]      = iter->energy(); 
      _recoPfJetsCorEta[i]    = iter->eta(); 
      _recoPfJetsCorPhi[i]    = iter->phi(); 
      _recoPfJetsCorElecF[i]     = iter->electronEnergyFraction(); 
      _recoPfJetsCorPhoF[i]      = iter->photonEnergyFraction(); 
      _recoPfJetsCorCEF[i]    = iter->chargedEmEnergyFraction(); 
      _recoPfJetsCorCHF[i]    = iter->chargedHadronEnergyFraction(); 
      _recoPfJetsCorNEF[i]    = iter->neutralEmEnergyFraction(); 
      _recoPfJetsCorNHF[i]    = iter->neutralHadronEnergyFraction(); 
      _recoPfJetsCorCMF[i]    = iter->chargedMuEnergyFraction(); 
      _recoPfJetsCorNTOT[i]   = iter->numberOfDaughters(); 
      _recoPfJetsCorNCH[i]    = iter->chargedMultiplicity(); 
      _recoPfJetsCorSpread[i] = iter->constituentEtaPhiSpread(); 
      _recoPfJetsCorArea[i]   = iter->jetArea(); 
      _nRecoPfJetCor++ ;
    }

  }

  if (_Debug) std::cout << "%HLTJetMET --  end loop over sorted PFJets" << std::endl;

  return 0;
}

int HLTJetMET::FillRecoCaloJets(uint kMaxJet, const edm::Handle<reco::CaloJetCollection> & H_RecoCaloJets, std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillRecoCaloJets(" << kMaxJet << " , H_RecoCaloJets , " << mode << ")" << std::endl;

  // Sort collection
  if (_Debug) std::cout << "%HLTJetMET --  get and sort collection...";
  reco::CaloJetCollection Jets = *H_RecoCaloJets;
  sort( Jets.begin() , Jets.end() , _jetsorter );
  if (_Debug) std::cout << " sorted!" << std::endl;

  // Fill output counter and arrays //

  // Check size
  uint vsize = Jets.size();
  if (_Debug) std::cout << "%HLTJetMET --  fill output counter and arrays: loop over sorted CaloJets (size=" << vsize << ")" << std::endl;

  // Initialize counters
  if(     mode=="uncorr") _nRecoCaloJet    = 0 ;
  else if(mode=="corr"  ) _nRecoCaloJetCor = 0 ;

  // Loop over sorted jets
  uint i=-1;
  for(reco::CaloJetCollection::const_iterator iter = Jets.begin() ; iter != Jets.end() ; iter++) {

    i++ ;
    if(i>=vsize || i>=kMaxJet) break;

    if (_Debug && i==0) std::cout << "%HLTJetMET --  RECO ak4 CaloJet #0: pT=" << iter->pt() << std::endl;
    
    if(mode=="uncorr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: uncorr" << std::endl;

      _recoCaloJetsPx[i]     = iter->px(); 
      _recoCaloJetsPy[i]     = iter->py(); 
      _recoCaloJetsPz[i]     = iter->pz(); 
      _recoCaloJetsPt[i]     = iter->pt(); 
      _recoCaloJetsE[i]      = iter->energy(); 
      _recoCaloJetsEta[i]    = iter->eta(); 
      _recoCaloJetsPhi[i]    = iter->phi(); 
      _recoCaloJetsEF[i]     = iter->emEnergyFraction(); 
      _recoCaloJetsHF[i]     = iter->energyFractionHadronic(); 
      _recoCaloJetsNTOT[i]   = iter->numberOfDaughters(); 
      //_recoCaloJetsSpread[i] = iter->constituentEtaPhiSpread(); // induces cmsRun failure
      _recoCaloJetsArea[i]   = iter->jetArea(); 

      _nRecoCaloJet++ ;
    }

    else if(mode=="corr") {

      if(_Debug && i==0) std::cout << "%HLTJetMET --  fill branch arrays; mode: corr" << std::endl;

      _recoCaloJetsCorPx[i]     = iter->px(); 
      _recoCaloJetsCorPy[i]     = iter->py(); 
      _recoCaloJetsCorPz[i]     = iter->pz(); 
      _recoCaloJetsCorPt[i]     = iter->pt(); 
      _recoCaloJetsCorE[i]      = iter->energy(); 
      _recoCaloJetsCorEta[i]    = iter->eta(); 
      _recoCaloJetsCorPhi[i]    = iter->phi(); 
      _recoCaloJetsCorEF[i]     = iter->emEnergyFraction(); 
      _recoCaloJetsCorHF[i]     = iter->energyFractionHadronic(); 
      _recoCaloJetsCorNTOT[i]   = iter->numberOfDaughters(); 
      //_recoCaloJetsCorSpread[i] = iter->constituentEtaPhiSpread(); // fail
      _recoCaloJetsCorArea[i]   = iter->jetArea(); 

      _nRecoCaloJetCor++ ;
    }

  }

  if (_Debug) std::cout << "%HLTJetMET --  end loop over sorted CaloJets" << std::endl;

  return 0;
}

int HLTJetMET::InitRecoJets(uint kMaxJet)
{

  if(_Debug) std::cout << "%HLTJetMET::InitRecoJets(" << kMaxJet << ") starts" << std::endl;

  _nRecoPfJet = _nRecoPfJetCor = _nRecoCaloJet = _nRecoCaloJetCor = 0;

  for(uint i=0 ; i<kMaxJet ; i++) {

    if(_RecoPFJets) {
      _recoPfJetsPx[i] = _recoPfJetsPy[i] = _recoPfJetsPz[i] = _recoPfJetsPt[i] = _recoPfJetsE[i] = _recoPfJetsEta[i] = _recoPfJetsPhi[i] = _recoPfJetsCEF[i] = 
	_recoPfJetsCHF[i] = _recoPfJetsElecF[i] = _recoPfJetsPhoF[i] = _recoPfJetsNEF[i] = _recoPfJetsNHF[i] = _recoPfJetsCMF[i] = _recoPfJetsNTOT[i] = _recoPfJetsNCH[i] = 
	_recoPfJetsSpread[i] = _recoPfJetsArea[i] = -999999 ; 
    
      _recoPfJetsCorPx[i] = _recoPfJetsCorPy[i] = _recoPfJetsCorPz[i] = _recoPfJetsCorPt[i] = _recoPfJetsCorE[i] = _recoPfJetsCorEta[i] = _recoPfJetsCorPhi[i] = 
	_recoPfJetsCorCEF[i] = _recoPfJetsCorCHF[i] = _recoPfJetsCorNEF[i] = _recoPfJetsCorNHF[i] = _recoPfJetsCorCMF[i] = _recoPfJetsCorNTOT[i] = _recoPfJetsCorNCH[i] =
	_recoPfJetsCorElecF[i] = _recoPfJetsCorPhoF[i] = _recoPfJetsCorSpread[i] = _recoPfJetsCorArea[i] = -999999 ;
    }

    if(_RecoCaloJets) {
      _recoCaloJetsPx[i] = _recoCaloJetsPy[i] = _recoCaloJetsPz[i] = _recoCaloJetsPt[i] = _recoCaloJetsE[i] = _recoCaloJetsEta[i] = _recoCaloJetsPhi[i] = 
	_recoCaloJetsEF[i] = _recoCaloJetsHF[i] = _recoCaloJetsNTOT[i] = _recoCaloJetsSpread[i] = _recoCaloJetsArea[i] = -999999 ;

      _recoCaloJetsCorPx[i] = _recoCaloJetsCorPy[i] = _recoCaloJetsCorPz[i] = _recoCaloJetsCorPt[i] = _recoCaloJetsCorE[i] = _recoCaloJetsCorEta[i] = 
	_recoCaloJetsCorPhi[i] = _recoCaloJetsCorEF[i] = _recoCaloJetsCorHF[i] = _recoCaloJetsCorNTOT[i] = _recoCaloJetsCorSpread[i] = _recoCaloJetsCorArea[i] = -999999 ;
    }
  }

  return 0;
}

int HLTJetMET::InitMET()
{

  _hltPfMetPx = _hltPfMetPy = _hltPfMetPt = _hltPfMetPhi = 0;
  _hltPfMhtPx = _hltPfMhtPy = _hltPfMhtPt = _hltPfMhtPhi = 0;
  _hltPfMetNoMuPx = _hltPfMetNoMuPy = _hltPfMetNoMuPt = _hltPfMetNoMuPhi = 0;
  _hltPfMhtNoMuPx = _hltPfMhtNoMuPy = _hltPfMhtNoMuPt = _hltPfMhtNoMuPhi = 0;
  _hltCaloMetPx = _hltCaloMetPy = _hltCaloMetPt = _hltCaloMetPhi = 0;
  _hltCaloMetCleanPx = _hltCaloMetCleanPy = _hltCaloMetCleanPt = _hltCaloMetCleanPhi = 0;
  _hltCaloMhtPx = _hltCaloMhtPy = _hltCaloMhtPt = _hltCaloMhtPhi = 0;

  _recPfMetPx = _recPfMetPy = _recPfMetPt = _recPfMetPhi = 0;
  _recPfMhtPx = _recPfMhtPy = _recPfMhtPt = _recPfMhtPhi = 0;
  _recPfMetNoMuPx = _recPfMetNoMuPy = _recPfMetNoMuPt = _recPfMetNoMuPhi = 0;
  _recPfMhtNoMuPx = _recPfMhtNoMuPy = _recPfMhtNoMuPt = _recPfMhtNoMuPhi = 0;
  _recCaloMetPx = _recCaloMetPy = _recCaloMetPt = _recCaloMetPhi = 0;
  _recCaloMhtPx = _recCaloMhtPy = _recCaloMhtPt = _recCaloMhtPhi = 0;

  return 0;
}

int HLTJetMET::FillPFMET(   const edm::Handle<reco::PFMETCollection>   & H_PFMET  , std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillPFMET(" << " , H_PFMET , " << mode << ")" << std::endl
		      << "%HLTJetMET --  get collection...";

  if(     mode=="HLTPFMET") {
    _hltPfMetPx  = H_PFMET->begin()->px();
    _hltPfMetPy  = H_PFMET->begin()->py();
    _hltPfMetPt  = H_PFMET->begin()->pt();
    _hltPfMetPhi = H_PFMET->begin()->phi();
  }
  //
  else if(mode=="HLTPFMETNoMu") {
    _hltPfMetNoMuPx  = H_PFMET->begin()->px();
    _hltPfMetNoMuPy  = H_PFMET->begin()->py();
    _hltPfMetNoMuPt  = H_PFMET->begin()->pt();
    _hltPfMetNoMuPhi = H_PFMET->begin()->phi();
  }
  //
  else if(mode=="RECOPFMET") {
    _recPfMetPx  = H_PFMET->begin()->px();
    _recPfMetPy  = H_PFMET->begin()->py();
    _recPfMetPt  = H_PFMET->begin()->pt();
    _recPfMetPhi = H_PFMET->begin()->phi();
  }

  return 0;
}

int HLTJetMET::FillMET(     const edm::Handle<reco::METCollection>     & H_MET    , std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillMET(" << " , H_MET , " << mode << ")" << std::endl
		      << "%HLTJetMET --  get collection...";

  if(     mode=="HLTPFMHT") {
    _hltPfMhtPx  = H_MET->begin()->px();
    _hltPfMhtPy  = H_MET->begin()->py();
    _hltPfMhtPt  = H_MET->begin()->pt();
    _hltPfMhtPhi = H_MET->begin()->phi();
  }
  //
  else if(mode=="HLTPFMHTNoMu") {
    _hltPfMhtNoMuPx  = H_MET->begin()->px();
    _hltPfMhtNoMuPy  = H_MET->begin()->py();
    _hltPfMhtNoMuPt  = H_MET->begin()->pt();
    _hltPfMhtNoMuPhi = H_MET->begin()->phi();
  }
  //
  else if(mode=="HLTCaloMHT") {
    _hltCaloMhtPx  = H_MET->begin()->px();
    _hltCaloMhtPy  = H_MET->begin()->py();
    _hltCaloMhtPt  = H_MET->begin()->pt();
    _hltCaloMhtPhi = H_MET->begin()->phi();
  }

  return 0;
}

int HLTJetMET::FillCaloMET( const edm::Handle<reco::CaloMETCollection> & H_CaloMET, std::string mode)
{

  if(_Debug) std::cout<< "%HLTJetMET --  ::FillCaloMET(" << " , H_CaloMET , " << mode << ")" << std::endl
		      << "%HLTJetMET --  get collection...";

  if(     mode=="HLTCaloMET") {
    _hltCaloMetPx  = H_CaloMET->begin()->px();
    _hltCaloMetPy  = H_CaloMET->begin()->py();
    _hltCaloMetPt  = H_CaloMET->begin()->pt();
    _hltCaloMetPhi = H_CaloMET->begin()->phi();
  }
  //
  else if(mode=="HLTCaloCaloMETClean") {
    _hltCaloMetCleanPx  = H_CaloMET->begin()->px();
    _hltCaloMetCleanPy  = H_CaloMET->begin()->py();
    _hltCaloMetCleanPt  = H_CaloMET->begin()->pt();
    _hltCaloMetCleanPhi = H_CaloMET->begin()->phi();
  }
  //
  else if(mode=="RECOCaloMET") {
    _recCaloMetPx  = H_CaloMET->begin()->px();
    _recCaloMetPy  = H_CaloMET->begin()->py();
    _recCaloMetPt  = H_CaloMET->begin()->pt();
    _recCaloMetPhi = H_CaloMET->begin()->phi();
  }

  return 0;
}

int HLTJetMET::FillPFMETNoMu(const edm::Handle<reco::PFMETCollection>   & H_PFMET,
			   const edm::Handle<reco::MuonCollection>    & H_Muons,
			   std::string mode)
{

  double met_px, met_py, met_pt, met_phi;
  TLorentzVector VMET;
  
  met_px = H_PFMET->begin()->px();
  met_py = H_PFMET->begin()->py();

  reco::MuonCollection::const_iterator it;
  for(it=H_Muons->begin(); it!=H_Muons->end(); it++) {
    met_px += it->px();
    met_py += it->py();
  }

  VMET.SetXYZM(met_px, met_py, 0, 0);
  met_pt  = VMET.Pt();
  met_phi = VMET.Phi();

  if(mode=="RECOPFMETNoMu") {
    _recPfMetNoMuPx  = met_px;
    _recPfMetNoMuPy  = met_py;
    _recPfMetNoMuPt  = met_pt;
    _recPfMetNoMuPhi = met_phi;
  }

  return 0;
}

int HLTJetMET::FillPFMHT(const edm::Handle<reco::PFJetCollection>   & H_PFJets,
		       const edm::Handle<reco::MuonCollection>    & H_Muons,
		       std::string mode)
{

  // utility variables
  TLorentzVector VMHT, VMHTNoMu;
  double mht_px, mht_py, mht_pt, mht_phi;
  double mhtnomu_px, mhtnomu_py, mhtnomu_pt, mhtnomu_phi;

  double jet_Px     ;//= iter->px(); 
  double jet_Py     ;//= iter->py(); 
  double jet_Pt     ;//= iter->pt(); 
  double jet_Eta    ;//= iter->eta(); 
  double jet_CEF    ;//= iter->chargedEmEnergyFraction(); 
  double jet_CHF    ;//= iter->chargedHadronEnergyFraction(); 
  double jet_NEF    ;//= iter->neutralEmEnergyFraction(); 
  double jet_NHF    ;//= iter->neutralHadronEnergyFraction(); 
  double jet_NTOT   ;//= iter->numberOfDaughters(); 
  double jet_NCH    ;//= iter->chargedMultiplicity(); 

  ///
  // initialize
  mht_px = mht_py = mht_pt = mht_phi = 0;
  mhtnomu_px = mhtnomu_py = mhtnomu_pt = mhtnomu_phi = 0;
  ///
  jet_Px     = 0; //iter->px(); 
  jet_Py     = 0; //iter->py(); 
  jet_Pt     = 0; //iter->pt(); 
  jet_Eta    = 0; //iter->eta(); 
  jet_CEF    = 0; //iter->chargedEmEnergyFraction(); 
  jet_CHF    = 0; //iter->chargedHadronEnergyFraction(); 
  jet_NEF    = 0; //iter->neutralEmEnergyFraction(); 
  jet_NHF    = 0; //iter->neutralHadronEnergyFraction(); 
  jet_NTOT   = 0; //iter->numberOfDaughters(); 
  jet_NCH    = 0; //iter->chargedMultiplicity(); 

  // Loop over jets to build MHT
  for(reco::PFJetCollection::const_iterator iter = H_PFJets->begin() ; iter != H_PFJets->end() ; iter++) {

    // Get PFJet variables
    jet_Px     = iter->px(); 
    jet_Py     = iter->py(); 
    jet_Pt     = iter->pt(); 
    jet_Eta    = iter->eta(); 
    jet_CEF    = iter->chargedEmEnergyFraction(); 
    jet_CHF    = iter->chargedHadronEnergyFraction(); 
    jet_NEF    = iter->neutralEmEnergyFraction(); 
    jet_NHF    = iter->neutralHadronEnergyFraction(); 
    jet_NTOT   = iter->numberOfDaughters(); 
    jet_NCH    = iter->chargedMultiplicity(); 

    // Definition similar to online TightID + PFMHT definition
    if(std::abs(jet_Eta)>=5.2) continue;
    if(jet_Pt<=20)             continue;
    if(jet_NTOT<=1)            continue;
    if(jet_NEF>=0.99)          continue;
    if(jet_NHF>=0.9  && std::abs(jet_Eta)<2.4)  continue;
    if(jet_CEF>=0.99 && std::abs(jet_Eta)<2.4)  continue;
    if(jet_CHF<=0    && std::abs(jet_Eta)<2.4)  continue;
    if(jet_NCH<=0    && std::abs(jet_Eta)<2.4)  continue;

    // Subtract momentum from jets that pass online ID + PFMHT definition
    mht_px -= jet_Px;
    mht_py -= jet_Py;
  }

  // Loop over muons to add back muon pT to PFMHT
  mhtnomu_px = mht_px;
  mhtnomu_py = mht_py;
  //
  reco::MuonCollection::const_iterator it;
  for(it=H_Muons->begin(); it!=H_Muons->end(); it++) {
    mhtnomu_px += it->px();
    mhtnomu_py += it->py();
  }

  // Build TLorentzVectors
  VMHT    .SetXYZM(mht_px    , mht_py    , 0, 0);
  VMHTNoMu.SetXYZM(mhtnomu_px, mhtnomu_py, 0, 0);

  // Get pT and phi from the TLorentzVector
  mht_pt      = VMHT.Pt();
  mht_phi     = VMHT.Phi();
  mhtnomu_pt  = VMHTNoMu.Pt();
  mhtnomu_phi = VMHTNoMu.Phi();

  // Fill branches' arrays
  if(mode=="RECOPFMHT_AndNoMu") {

    _recPfMhtPx  = mht_px;
    _recPfMhtPy  = mht_py;
    _recPfMhtPt  = mht_pt;
    _recPfMhtPhi = mht_phi;

    _recPfMhtNoMuPx  = mhtnomu_px;
    _recPfMhtNoMuPy  = mhtnomu_py;
    _recPfMhtNoMuPt  = mhtnomu_pt;
    _recPfMhtNoMuPhi = mhtnomu_phi;

  }

  return 0;
}

int HLTJetMET::FillCaloMHT(const edm::Handle<reco::CaloJetCollection>             & H_CaloJets,
			 std::string mode)
{

  if(_Debug) std::cout << "%HLTJetMET::FillCaloMHT() starts." << std::endl;

  // utility variables
  TLorentzVector VMHT;
  double mht_px, mht_py, mht_pt, mht_phi;
  //
  double jet_Px     ;
  double jet_Py     ;
  double jet_Pt     ;
  double jet_Eta    ;
  double jet_N90 ;
  //double jet_N90hits ;
  double jet_EMF ;
  ///
  // initialize
  mht_px = mht_py = mht_pt = mht_phi = 0;
  ///
  jet_Px     = 0; 
  jet_Py     = 0; 
  jet_Pt     = 0; 
  jet_Eta    = 0; 
  jet_N90    = 0;
  //jet_N90hits= 0;
  jet_EMF    = 0;

  // Loop over jets to build MHT
  for(reco::CaloJetCollection::const_iterator iter = H_CaloJets->begin() ; iter != H_CaloJets->end() ; iter++) {

    // Get CaloJet variables
    jet_Px     = iter->px(); 
    jet_Py     = iter->py(); 
    jet_Pt     = iter->pt(); 
    jet_Eta    = iter->eta(); 
    jet_N90    = iter->n90(); 
    //jet_N90hits= iter->n90Hits();  // FIXME : needs a JetIDHelper
    jet_EMF    = iter->emEnergyFraction(); 

    // Definition similar to online TightID + CaloMHT definition
    if(std::abs(jet_Eta)>=5.2) continue;
    if(jet_Pt <= 20)           continue;
    if(jet_N90 < -2)           continue;
    //if(jet_N90hits <= )      continue;
    if(jet_EMF < 1.0e-6)       continue;
    if(jet_EMF > 999)          continue;
    
    // Subtract momentum from jets that pass online ID + CaloMHT definition
    mht_px -= jet_Px;
    mht_py -= jet_Py;
  }

  // Build TLorentzVector
  VMHT    .SetXYZM(mht_px    , mht_py    , 0, 0);

  // Get pT and phi from the TLorentzVector
  mht_pt      = VMHT.Pt();
  mht_phi     = VMHT.Phi();

  // Fill branches' arrays
  if(mode=="RECOCaloMHT") {
    _recPfMhtPx  = mht_px;
    _recPfMhtPy  = mht_py;
    _recPfMhtPt  = mht_pt;
    _recPfMhtPhi = mht_phi;
  }

  return 0;
}

