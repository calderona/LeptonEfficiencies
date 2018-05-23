
#include "MuonAnalyzer.h" 

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageService/interface/MessageServicePresence.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"

using namespace edm;
using namespace reco;
using namespace std;


ExampleMuonAnalyzer::ExampleMuonAnalyzer(const ParameterSet& pset)
{
  beamSpotToken  = consumes<reco::BeamSpot>(pset.getParameter<InputTag>("beamSpot"));
  muonToken      = consumes<pat::MuonCollection>(pset.getParameter<InputTag>("MuonCollection"));
  packedGenToken = consumes<edm::View<pat::PackedGenParticle>>(pset.getParameter<InputTag>("packed"));
  prunedGenToken = consumes<edm::View<reco::GenParticle>>(pset.getParameter<InputTag>("pruned"));
  vtxToken       = consumes<reco::VertexCollection>(pset.getParameter<InputTag>("vertices"));
}


ExampleMuonAnalyzer::~ExampleMuonAnalyzer() {}


void ExampleMuonAnalyzer::beginJob()
{
  cout << "\n [ExampleMuonAnalyzer::beginJob]\n" << endl;

  edm::Service<TFileService> fileService;

  // TH1 histograms
  hGenMuons_eta = fileService->make<TH1F>("GenMuons_eta", "gen muons eta", 100, -2.5, 2.5);
  hGenMuons_pt  = fileService->make<TH1F>("GenMuons_pt",  "gen muons pt",  100,    0, 100); 
  hGenMuons_vx  = fileService->make<TH1F>("GenMuons_vx",  "gen muons vx",  150,    0, 750);
  hGenMuons_vy  = fileService->make<TH1F>("GenMuons_vy",  "gen muons vy",  150,    0, 750);
  hGenMuons_vz  = fileService->make<TH1F>("GenMuons_vz",  "gen muons vz",  150,    0, 750);
  hGenMuons_vr  = fileService->make<TH1F>("GenMuons_vr",  "gen muons vr",  750,    0, 750);

  hStaMuons_dR = fileService->make<TH1F>("StaMuons_dR", "sta-gen dR-matched dR", 100, 0,   4);
  hStaMuons_pt = fileService->make<TH1F>("StaMuons_pt", "sta-gen dR-matched pt", 100, 0, 100);
  hStaMuons_vr = fileService->make<TH1F>("StaMuons_vr", "sta-gen dR-matched vr", 750, 0, 750);

  hTrkMuons_dR  = fileService->make<TH1F>("TrkMuons_dR", "trk-gen dR-matched dR", 100, 0,   4);
  hTrkMuons_pt  = fileService->make<TH1F>("TrkMuons_pt", "trk-gen dR-matched pt", 100, 0, 100);
  hTrkMuons_vr  = fileService->make<TH1F>("TrkMuons_vr", "trk-gen dR-matched vr", 750, 0, 750);

  hGlbMuons_dR = fileService->make<TH1F>("GlbMuons_dR", "glb-gen dR-matched dR", 100, 0,   4);
  hGlbMuons_pt = fileService->make<TH1F>("GlbMuons_pt", "glb-gen dR-matched pt", 100, 0, 100);
  hGlbMuons_vr = fileService->make<TH1F>("GlbMuons_vr", "glb-gen dR-matched vr", 750, 0, 750);

  hGlbStaMuons_dR = fileService->make<TH1F>("GlbStaMuons_dR", "glbsta-gen dR-matched dR", 100, 0,   4);
  hGlbStaMuons_pt = fileService->make<TH1F>("GlbStaMuons_pt", "glbsta-gen dR-matched pt", 100, 0, 100);
  hGlbStaMuons_vr = fileService->make<TH1F>("GlbStaMuons_vr", "glbsta-gen dR-matched vr", 750, 0, 750);

  for (Int_t i=0; i<nbinspt; i++) {
    hStaMuons_res   [i] = fileService->make<TH1F>(Form("StaMuons_res_%d",    i), "#Deltaq/p_{T} / q/p_{T}", 60, -3, 3);
    hTrkMuons_res   [i] = fileService->make<TH1F>(Form("TrkMuons_res_%d",    i), "#Deltaq/p_{T} / q/p_{T}", 60, -3, 3);
    hGlbMuons_res   [i] = fileService->make<TH1F>(Form("GlbMuons_res_%d",    i), "#Deltaq/p_{T} / q/p_{T}", 60, -3, 3);
    hGlbStaMuons_res[i] = fileService->make<TH1F>(Form("GlbStaMuons_res_%d", i), "#Deltaq/p_{T} / q/p_{T}", 60, -3, 3);
  }

  // TH2 histograms
  hGenStaMuons_eta = fileService->make<TH2F>("GenStaMuons_eta", "sta-gen dR-matched eta", 50, -2.5, 2.5, 50, -2.5, 2.5);
  hGenStaMuons_phi = fileService->make<TH2F>("GenStaMuons_phi", "sta-gen dR-matched phi", 50, -3.2, 3.2, 50, -3.2, 3.2);
}


void ExampleMuonAnalyzer::endJob() {}


void ExampleMuonAnalyzer::analyze(const Event& event, const EventSetup& eventSetup)
{
  // BeamSpot
  edm::Handle<reco::BeamSpot> beamSpot;
  event.getByToken(beamSpotToken, beamSpot);
  reco::BeamSpot bs;

  if (beamSpot.isValid()) bs = *beamSpot;    


  // Vertex collection
  edm::Handle<reco::VertexCollection> vertices;
  event.getByToken(vtxToken, vertices);


  // Pruned particles are the ones containing "important" stuff
  Handle<edm::View<reco::GenParticle> > pruned;
  event.getByToken(prunedGenToken, pruned);


  // Packed particles are all the status 1, so usable to remake jets
  // The navigation from status 1 to pruned is possible (the other direction should be made by hand)
  Handle<edm::View<pat::PackedGenParticle> > packed;
  event.getByToken(packedGenToken, packed);


  // Get the muon collection
  Handle<pat::MuonCollection> muons;
  event.getByToken(muonToken, muons);


  // Loop over pruned particles
  //----------------------------------------------------------------------------
  for (size_t i=0; i<pruned->size(); i++) {

    if (abs((*pruned)[i].pdgId()) != 13)    continue;
    if (!(*pruned)[i].isPromptFinalState()) continue;
    if (!(*pruned)[i].isLastCopy())         continue;

    Float_t charge = (*pruned)[i].charge();
    Float_t eta    = (*pruned)[i].eta();
    Float_t phi    = (*pruned)[i].phi();
    Float_t pt     = (*pruned)[i].pt();
    Float_t vx     = (*pruned)[i].vx();
    Float_t vy     = (*pruned)[i].vy();
    Float_t vz     = (*pruned)[i].vz();

    if (fabs(eta) > 2.4) continue;
    if (pt < ptbins[0])  continue;

    Float_t vr = sqrt(vx*vx + vy*vy + vz*vz);


    // Loop over reconstructed muons
    //--------------------------------------------------------------------------
    Float_t sta_min_deltaR =  999;   
    Float_t sta_pt         = -999;
    Float_t sta_res        = -999;

    Float_t trk_min_deltaR =  999; 
    Float_t trk_pt         = -999;
    Float_t trk_res        = -999;

    Float_t glb_min_deltaR =  999; 
    Float_t glb_pt         = -999;
    Float_t glb_res        = -999;

    Float_t glbsta_min_deltaR =  999; 
    Float_t glbsta_pt         = -999;
    Float_t glbsta_res        = -999;

    Float_t sta_eta = -999;  // So far we are only drawing standalone vs. gen eta
    Float_t sta_phi = -999;  // So far we are only drawing standalone vs. gen phi


    for (pat::MuonCollection::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon) {
      

      // isStandAloneMuon
      //------------------------------------------------------------------------
      if (muon->isStandAloneMuon()) {
     
	Float_t muEta    = muon->standAloneMuon()->eta();
	Float_t muPhi    = muon->standAloneMuon()->phi();
	Float_t muPt     = muon->standAloneMuon()->pt();
	Float_t muCharge = muon->standAloneMuon()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
	
	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < sta_min_deltaR) {
	  sta_min_deltaR = dR;
	  sta_eta        = muEta;
	  sta_phi        = muPhi;
	  sta_pt         = muPt;
	  sta_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }

      
      // isTrackerMuon
      //------------------------------------------------------------------------
      if (muon->isTrackerMuon()) {

	float muEta    = muon->innerTrack()->eta();
	float muPhi    = muon->innerTrack()->phi();
	float muPt     = muon->innerTrack()->pt();
	float muCharge = muon->innerTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < trk_min_deltaR) {
	  trk_min_deltaR = dR;
	  trk_pt         = muPt;
	  trk_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isGlobalMuon
      //------------------------------------------------------------------------
      if (muon->isGlobalMuon()) {

	float muEta    = muon->globalTrack()->eta();
	float muPhi    = muon->globalTrack()->phi();
	float muPt     = muon->globalTrack()->pt();
	float muCharge = muon->globalTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < glb_min_deltaR) {
	  glb_min_deltaR = dR;
	  glb_pt         = muPt;
	  glb_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isGlobalMuon && isStandAloneMuon
      //------------------------------------------------------------------------
      if (muon->isGlobalMuon() && muon->isStandAloneMuon()) {

	float muEta    = muon->globalTrack()->eta();
	float muPhi    = muon->globalTrack()->phi();
	float muPt     = muon->globalTrack()->pt();
	float muCharge = muon->globalTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < glbsta_min_deltaR) {
	  glbsta_min_deltaR = dR;
	  glbsta_pt         = muPt;
	  glbsta_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }
    }  // for..muons


    if (vr > max_vr) continue;


    // Fill gen histograms
    //--------------------------------------------------------------------------
    hGenMuons_eta->Fill(eta);
    hGenMuons_pt ->Fill(pt);
    hGenMuons_vx ->Fill(fabs(vx));
    hGenMuons_vy ->Fill(fabs(vy));
    hGenMuons_vz ->Fill(fabs(vz));
    hGenMuons_vr ->Fill(vr);


    // Fill sta histograms
    //--------------------------------------------------------------------------
    hStaMuons_dR->Fill(sta_min_deltaR);

    if (sta_min_deltaR < max_deltaR)
      {
	hStaMuons_pt->Fill(sta_pt);
	hStaMuons_vr->Fill(vr);

	hGenStaMuons_eta->Fill(eta, sta_eta);
	hGenStaMuons_phi->Fill(phi, sta_phi);

	for (Int_t i=0; i<nbinspt; i++)
	  if (pt > ptbins[i] && pt < ptbins[i+1]) hStaMuons_res[i]->Fill(sta_res);
      }


    // Fill trk histograms
    //--------------------------------------------------------------------------
    hTrkMuons_dR->Fill(trk_min_deltaR);

    if (trk_min_deltaR < max_deltaR)
      {
	hTrkMuons_pt->Fill(trk_pt);
	hTrkMuons_vr->Fill(vr);
	
	for (Int_t i=0; i<nbinspt; i++)
	  if (pt > ptbins[i] && pt < ptbins[i+1]) hTrkMuons_res[i]->Fill(trk_res);
      }


    // Fill glb histograms
    //--------------------------------------------------------------------------
    hGlbMuons_dR->Fill(glb_min_deltaR);

    if (glb_min_deltaR < max_deltaR)
      {
	hGlbMuons_pt->Fill(glb_pt);
	hGlbMuons_vr->Fill(vr);

	for (Int_t i=0; i<nbinspt; i++)
	  if (pt > ptbins[i] && pt < ptbins[i+1]) hGlbMuons_res[i]->Fill(glb_res);
      }


    // Fill glbsta histograms
    //--------------------------------------------------------------------------
    hGlbStaMuons_dR->Fill(glbsta_min_deltaR);

    if (glbsta_min_deltaR < max_deltaR)
      {
	hGlbStaMuons_pt->Fill(glbsta_pt);
	hGlbStaMuons_vr->Fill(vr);

	for (Int_t i=0; i<nbinspt; i++)
	  if (pt > ptbins[i] && pt < ptbins[i+1]) hGlbStaMuons_res[i]->Fill(glbsta_res);
      }
  }  // for..pruned
}


DEFINE_FWK_MODULE(ExampleMuonAnalyzer);