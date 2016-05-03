#ifndef HLTJETMET_H
#define HLTJETMET_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNamed.h"
#include <vector>
#include <map>
#include "TROOT.h"
#include "TChain.h"
#include "TLorentzVector.h"

// CMSSW
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Common/interface/Provenance.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/MuonReco/interface/Muon.h"             //ND
#include "DataFormats/MuonReco/interface/MuonFwd.h"          //ND
#include "DataFormats/METReco/interface/CaloMET.h"           //ND
#include "DataFormats/METReco/interface/PFMET.h"             //ND
#include "DataFormats/METReco/interface/GenMET.h"            //ND
#include "DataFormats/JetReco/interface/PFJetCollection.h"   //ND
#include "DataFormats/JetReco/interface/CaloJetCollection.h" //ND
#include "DataFormats/METReco/interface/PFMETCollection.h"   //ND
#include "DataFormats/METReco/interface/METCollection.h"     //ND
#include "DataFormats/METReco/interface/GenMETCollection.h"  //ND
#include "DataFormats/METReco/interface/CaloMETCollection.h" //ND

namespace edm {
  class ConsumesCollector;
  class ParameterSet;
}

typedef std::vector<std::string> MyStrings;

#ifndef GETPTGREATER
#define GETPTGREATER
class GetPtGreater {
public:
    template <typename T> bool operator () (const T& i, const T& j) {
        return (i.pt() > j.pt());
    }
};
#endif

/** \class HLTJetMET
  *  
  * $Date: November 2006
  * $Revision: 
  * \author P. Bargassa - Rice U.
  * $Date: April 2016                                                                                                                                             
  * $Revision:     
  * \author G. Karapostoli - ULB    
  */
class HLTJetMET {
public:
  HLTJetMET();
  
  void setup(const edm::ParameterSet& pSet, TTree* tree);
  void beginRun(const edm::Run& , const edm::EventSetup& );

  /** Analyze the Data */
  void analyze(const edm::Handle<reco::PFJetCollection>               & H_HltPfJets,        //ND
	       const edm::Handle<reco::PFJetCollection>               & H_HltPfJetsCorr,    //ND
	       const edm::Handle<reco::CaloJetCollection>             & H_HltCaloJets,      //ND
	       const edm::Handle<reco::CaloJetCollection>             & H_HltCaloJetsCorr,  //ND
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
	       TTree* tree);

  int FillHltPfJets(  uint kMaxJet, const edm::Handle<reco::PFJetCollection>   & H_HltPfJets,   std::string mode);
  int FillHltCaloJets(uint kMaxJet, const edm::Handle<reco::CaloJetCollection> & H_HltCaloJets, std::string mode);
  int InitHltJets(    uint kMaxJet);

  int FillPFMETNoMu(const edm::Handle<reco::PFMETCollection>   & H_RecoPFMET,
		    const edm::Handle<reco::MuonCollection>    & H_RecoMuons,
		    std::string mode);

  int FillPFMHT(const edm::Handle<reco::PFJetCollection>   & H_RecoPfJets,
		const edm::Handle<reco::MuonCollection>    & H_RecoMuons,
		std::string mode);

  int FillCaloMHT(const edm::Handle<reco::CaloJetCollection> & H_RecoCaloJets, std::string mode);

  int FillPFMET(   const edm::Handle<reco::PFMETCollection>   & H_PFMET  , std::string mode);
  int FillMET(     const edm::Handle<reco::METCollection>     & H_MET    , std::string mode);
  int FillCaloMET( const edm::Handle<reco::CaloMETCollection> & H_CaloMET, std::string mode);
  int InitMET();

  int FillRecoPfJets(  uint kMaxJet, const edm::Handle<reco::PFJetCollection>   & H_RecoPfJets,   std::string mode);
  int FillRecoCaloJets(uint kMaxJet, const edm::Handle<reco::CaloJetCollection> & H_RecoCaloJets, std::string mode);
  int InitRecoJets(uint kMaxJet);

private:

  // utility
  std::string processName_;
  bool _Debug, _RecoPFJets, _RecoCaloJets, _RecoPFMET, _RecoCaloMET, _RecoMuons;
  GetPtGreater _jetsorter;

  // For branches //

  // HLT objects branches //ND
  ///
  // Met
  float _hltPfMetPx , _hltPfMetPy , _hltPfMetPt , _hltPfMetPhi ;
  float _hltPfMhtPx , _hltPfMhtPy , _hltPfMhtPt , _hltPfMhtPhi ;
  float _hltPfMetNoMuPx , _hltPfMetNoMuPy , _hltPfMetNoMuPt , _hltPfMetNoMuPhi ;
  float _hltPfMhtNoMuPx , _hltPfMhtNoMuPy , _hltPfMhtNoMuPt , _hltPfMhtNoMuPhi ;
  float _hltCaloMetPx , _hltCaloMetPy , _hltCaloMetPt , _hltCaloMetPhi ;
  float _hltCaloMetCleanPx , _hltCaloMetCleanPy , _hltCaloMetCleanPt , _hltCaloMetCleanPhi ;
  float _hltCaloMhtPx , _hltCaloMhtPy , _hltCaloMhtPt , _hltCaloMhtPhi ;
  ///
  // Jets
  int _nHltPfJet, _nHltPfJetCor, _nHltCaloJet, _nHltCaloJetCor;
  float *_hltPfJetsPx, *_hltPfJetsPy, *_hltPfJetsPz, *_hltPfJetsPt, *_hltPfJetsE, *_hltPfJetsEta, *_hltPfJetsPhi, *_hltPfJetsCEF, *_hltPfJetsElecF, *_hltPfJetsPhoF, 
    *_hltPfJetsCHF, *_hltPfJetsNEF, *_hltPfJetsNHF, *_hltPfJetsCMF, *_hltPfJetsNTOT, *_hltPfJetsNCH, *_hltPfJetsSpread, *_hltPfJetsArea; 
  float *_hltPfJetsCorPx, *_hltPfJetsCorPy, *_hltPfJetsCorPz, *_hltPfJetsCorPt, *_hltPfJetsCorE, *_hltPfJetsCorEta, *_hltPfJetsCorPhi, *_hltPfJetsCorElecF, 
    *_hltPfJetsCorPhoF, *_hltPfJetsCorCEF, *_hltPfJetsCorCHF, *_hltPfJetsCorNEF, *_hltPfJetsCorNHF, *_hltPfJetsCorCMF, *_hltPfJetsCorNTOT, *_hltPfJetsCorNCH,
    *_hltPfJetsCorSpread, *_hltPfJetsCorArea;
  float *_hltCaloJetsPx, *_hltCaloJetsPy, *_hltCaloJetsPz, *_hltCaloJetsPt, *_hltCaloJetsE, *_hltCaloJetsEta, *_hltCaloJetsPhi, 
    *_hltCaloJetsEF, *_hltCaloJetsHF, *_hltCaloJetsNTOT, *_hltCaloJetsSpread, *_hltCaloJetsArea;
  float *_hltCaloJetsCorPx, *_hltCaloJetsCorPy, *_hltCaloJetsCorPz, *_hltCaloJetsCorPt, *_hltCaloJetsCorE, *_hltCaloJetsCorEta, 
    *_hltCaloJetsCorPhi, *_hltCaloJetsCorEF, *_hltCaloJetsCorHF, *_hltCaloJetsCorNTOT, *_hltCaloJetsCorSpread, *_hltCaloJetsCorArea;

  // Offline objects branches //ND
  ///
  float _recPfMetPx , _recPfMetPy , _recPfMetPt , _recPfMetPhi;
  float _recPfMhtPx , _recPfMhtPy , _recPfMhtPt , _recPfMhtPhi;
  float _recPfMetNoMuPx , _recPfMetNoMuPy , _recPfMetNoMuPt , _recPfMetNoMuPhi;
  float _recPfMhtNoMuPx , _recPfMhtNoMuPy , _recPfMhtNoMuPt , _recPfMhtNoMuPhi;
  float _recCaloMetPx , _recCaloMetPy , _recCaloMetPt , _recCaloMetPhi;
  float _recCaloMetCleanPx , _recCaloMetCleanPy , _recCaloMetCleanPt , _recCaloMetCleanPhi;
  float _recCaloMhtPx , _recCaloMhtPy , _recCaloMhtPt , _recCaloMhtPhi;
  ///
  int _nRecoPfJet, _nRecoPfJetCor, _nRecoCaloJet, _nRecoCaloJetCor;
  float *_recoPfJetsPx, *_recoPfJetsPy, *_recoPfJetsPz, *_recoPfJetsPt, *_recoPfJetsE, *_recoPfJetsEta, *_recoPfJetsPhi, *_recoPfJetsCEF, *_recoPfJetsElecF, *_recoPfJetsPhoF, 
    *_recoPfJetsCHF, *_recoPfJetsNEF, *_recoPfJetsNHF, *_recoPfJetsCMF, *_recoPfJetsNTOT, *_recoPfJetsNCH, *_recoPfJetsSpread, *_recoPfJetsArea; 
  float *_recoPfJetsCorPx, *_recoPfJetsCorPy, *_recoPfJetsCorPz, *_recoPfJetsCorPt, *_recoPfJetsCorE, *_recoPfJetsCorEta, *_recoPfJetsCorPhi, *_recoPfJetsCorElecF, 
    *_recoPfJetsCorPhoF, *_recoPfJetsCorCEF, *_recoPfJetsCorCHF, *_recoPfJetsCorNEF, *_recoPfJetsCorNHF, *_recoPfJetsCorCMF, *_recoPfJetsCorNTOT, *_recoPfJetsCorNCH,
    *_recoPfJetsCorSpread, *_recoPfJetsCorArea;
  float *_recoCaloJetsPx, *_recoCaloJetsPy, *_recoCaloJetsPz, *_recoCaloJetsPt, *_recoCaloJetsE, *_recoCaloJetsEta, *_recoCaloJetsPhi, 
    *_recoCaloJetsEF, *_recoCaloJetsHF, *_recoCaloJetsNTOT, *_recoCaloJetsSpread, *_recoCaloJetsArea;
  float *_recoCaloJetsCorPx, *_recoCaloJetsCorPy, *_recoCaloJetsCorPz, *_recoCaloJetsCorPt, *_recoCaloJetsCorE, *_recoCaloJetsCorEta, 
    *_recoCaloJetsCorPhi, *_recoCaloJetsCorEF, *_recoCaloJetsCorHF, *_recoCaloJetsCorNTOT, *_recoCaloJetsCorSpread, *_recoCaloJetsCorArea;


};

#endif
