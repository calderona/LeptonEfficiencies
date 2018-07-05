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
  hGenMuons_phi = fileService->make<TH1F>("GenMuons_phi", "gen muons phi", 100, -3.2, 3.2);
  hGenMuons_pt  = fileService->make<TH1F>("GenMuons_pt",  "gen muons pt",  100,    0, 100); 
  hGenMuons_vx  = fileService->make<TH1F>("GenMuons_vx",  "gen muons vx",  150,    0, 750);
  hGenMuons_vy  = fileService->make<TH1F>("GenMuons_vy",  "gen muons vy",  150,    0, 750);
  hGenMuons_vz  = fileService->make<TH1F>("GenMuons_vz",  "gen muons vz",  150,    0, 750);
  hGenMuons_vr  = fileService->make<TH1F>("GenMuons_vr",  "gen muons vr",  750,    0, 750);

  hStaMuons_eta = fileService->make<TH1F>("StaMuons_eta", "sta-gen dR-matched eta",         100, -2.5, 2.5);
  hStaMuons_phi = fileService->make<TH1F>("StaMuons_phi", "sta muons phi",         100, -3.2, 3.2);
  hStaMuons_dR  = fileService->make<TH1F>("StaMuons_dR",  "sta-gen dR",            100,    0,   4);
  hStaMuons_pt  = fileService->make<TH1F>("StaMuons_pt",  "sta-gen dR-matched pt", 100,    0, 100);
  hStaMuons_vr  = fileService->make<TH1F>("StaMuons_vr",  "sta-gen dR-matched vr", 750,    0, 750);

  hStaMuons_noGen_eta = fileService->make<TH1F>("StaMuons_noGen_eta", "sta-gen NO dR-matched eta",         100, -2.5, 2.5);
  hStaMuons_noGen_vr  = fileService->make<TH1F>("StaMuons_noGen_vr",  "sta-gen NO dR-matched vr", 750,    0, 750);
  hStaMuons_noGen_pt  = fileService->make<TH1F>("StaMuons_noGen_pt",  "sta-gen NO dR-matched pt", 100,    0, 100);

  hTrkMuons_eta = fileService->make<TH1F>("TrkMuons_eta", "trk-gen dR-matched eta",         100, -2.5, 2.5);
  hTrkMuons_phi = fileService->make<TH1F>("TrkMuons_phi", "trk muons phi",         100, -3.2, 3.2);
  hTrkMuons_dR  = fileService->make<TH1F>("TrkMuons_dR",  "trk-gen dR",            100,    0,   4);
  hTrkMuons_pt  = fileService->make<TH1F>("TrkMuons_pt",  "trk-gen dR-matched pt", 100,    0, 100);
  hTrkMuons_vr  = fileService->make<TH1F>("TrkMuons_vr",  "trk-gen dR-matched vr", 750,    0, 750);

  hTrkMuons_noGen_eta = fileService->make<TH1F>("TrkMuons_noGen_eta", "trk-gen NO dR-matched eta",         100, -2.5, 2.5);
  hTrkMuons_noGen_vr  = fileService->make<TH1F>("TrkMuons_noGen_vr",  "trk-gen NO dR-matched vr", 750,    0, 750);
  hTrkMuons_noGen_pt  = fileService->make<TH1F>("TrkMuons_noGen_pt",  "trk-gen NO dR-matched pt", 100,    0, 100);


  hGlbMuons_eta = fileService->make<TH1F>("GlbMuons_eta", "glb-gen dR-matched eta",         100, -2.5, 2.5);
  hGlbMuons_phi = fileService->make<TH1F>("GlbMuons_phi", "glb muons phi",         100, -3.2, 3.2);
  hGlbMuons_dR  = fileService->make<TH1F>("GlbMuons_dR",  "glb-gen dR",            100,    0,   4);
  hGlbMuons_pt  = fileService->make<TH1F>("GlbMuons_pt",  "glb-gen dR-matched pt", 100,    0, 100);
  hGlbMuons_vr  = fileService->make<TH1F>("GlbMuons_vr",  "glb-gen dR-matched vr", 750,    0, 750);

  hGlbMuons_noGen_eta = fileService->make<TH1F>("GlbMuons_noGen_eta", "glb-gen NO dR-matched eta",         100, -2.5, 2.5);
  hGlbMuons_noGen_vr  = fileService->make<TH1F>("GlbMuons_noGen_vr",  "glb-gen NO dR-matched vr", 750,    0, 750);
  hGlbMuons_noGen_pt  = fileService->make<TH1F>("GlbMuons_noGen_pt",  "glb-gen NO dR-matched pt", 100,    0, 100);

  hTightMuons_eta = fileService->make<TH1F>("TightMuons_eta", "ID-gen dR-matched eta",         100, -2.5, 2.5);
  hTightMuons_phi = fileService->make<TH1F>("TightMuons_phi", "ID muons phi",         100, -3.2, 3.2);
  hTightMuons_dR  = fileService->make<TH1F>("TightMuons_dR",  "ID-gen dR",            100,    0,   4);
  hTightMuons_pt  = fileService->make<TH1F>("TightMuons_pt",  "ID-gen dR-matched pt", 100,    0, 100);
  hTightMuons_vr  = fileService->make<TH1F>("TightMuons_vr",  "ID-gen dR-matched vr", 750,    0, 750);

  hIDMuons_noGen_eta = fileService->make<TH1F>("IDMuons_noGen_eta", "glb-gen NO dR-matched eta",         100, -2.5, 2.5);
  hIDMuons_noGen_vr  = fileService->make<TH1F>("IDMuons_noGen_vr",  "glb-gen NO dR-matched vr", 750,    0, 750);
  hIDMuons_noGen_pt  = fileService->make<TH1F>("IDMuons_noGen_pt",  "glb-gen NO dR-matched pt", 100,    0, 100);



  for (Int_t i=0; i<nbinspt; i++) {
    hStaMuons_res[i] = fileService->make<TH1F>(Form("StaMuons_res_%d", i), "#Deltaq/p_{T} / q/p_{T}", 60, -1, 1);
    hTrkMuons_res[i] = fileService->make<TH1F>(Form("TrkMuons_res_%d", i), "#Deltaq/p_{T} / q/p_{T}", 60, -0.1, 0.1);
    hGlbMuons_res[i] = fileService->make<TH1F>(Form("GlbMuons_res_%d", i), "#Deltaq/p_{T} / q/p_{T}", 60, -0.1, 0.1);
  }

  // TH2 histograms
  hGenStaMuons_eta = fileService->make<TH2F>("GenStaMuons_eta", "sta-gen dR-matched eta", 50, -2.5, 2.5, 50, -2.5, 2.5);
  hGenTrkMuons_eta = fileService->make<TH2F>("GenTrkMuons_eta", "trk-gen dR-matched eta", 50, -2.5, 2.5, 50, -2.5, 2.5);
  hGenGlbMuons_eta = fileService->make<TH2F>("GenGlbMuons_eta", "glb-gen dR-matched eta", 50, -2.5, 2.5, 50, -2.5, 2.5);

  hGenStaMuons_phi = fileService->make<TH2F>("GenStaMuons_phi", "sta-gen dR-matched phi", 50, -3.2, 3.2, 50, -3.2, 3.2);
  hGenTrkMuons_phi = fileService->make<TH2F>("GenTrkMuons_phi", "trk-gen dR-matched phi", 50, -3.2, 3.2, 50, -3.2, 3.2);
  hGenGlbMuons_phi = fileService->make<TH2F>("GenGlbMuons_phi", "glb-gen dR-matched phi", 50, -3.2, 3.2, 50, -3.2, 3.2);

  // Isolation                                                                                                                     
  hMuPFChargeIso = fileService->make<TH1F>("MuPFChargeIso","Isolation #Delta(R)=0.4: PFCharge",200,0,1);   
  hMuPFNeutralIso = fileService->make<TH1F>("MuPFNeutralIso","Isolation #Delta(R)=0.4: PFNeutral",200,0,1);
  hMuPFPhotonIso = fileService->make<TH1F>("MuPFPhotonIso","Isolation #Delta(R)=0.4: PFPhoton",200,0,1);
  hMuPFPUIso = fileService->make<TH1F>("MuPFPUIso","Isolation #Delta(R)=0.4: PFPU",200,0,1);
  hMuPFIso = fileService->make<TH1F>("MuPFIso","Isolation #Delta(R)=0.4: SumPt",200,0,1); 
  hMuPFIso_R = fileService->make<TH2F>("MuPFIso_R","Isolation #Delta(R)=0.4: SumPt vs R",100,0, 15, 100,0,0.5);  
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



  // =================================================================================
  // Look for the Primary Vertex (and use the BeamSpot instead, if you can't find it):
  reco::Vertex::Point posVtx;
  reco::Vertex::Error errVtx;
  unsigned int theIndexOfThePrimaryVertex = 999.;
  if (vertices.isValid()){
    for (unsigned int ind=0; ind<vertices->size(); ++ind) {
      if ( (*vertices)[ind].isValid() && !((*vertices)[ind].isFake()) ) {
	theIndexOfThePrimaryVertex = ind;
	break;
      }
    }
  }
  
  if (theIndexOfThePrimaryVertex<100) {
    posVtx = ((*vertices)[theIndexOfThePrimaryVertex]).position();
    errVtx = ((*vertices)[theIndexOfThePrimaryVertex]).error();
  }   
  else {
    LogInfo("RecoMuonValidator") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
    
    edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
    event.getByToken(beamSpotToken,recoBeamSpotHandle);
    reco::BeamSpot bs = *recoBeamSpotHandle;
    
    posVtx = bs.position();
    errVtx(0,0) = bs.BeamWidthX();
    errVtx(1,1) = bs.BeamWidthY();
    errVtx(2,2) = bs.sigmaZ();
  }
    
  const reco::Vertex thePrimaryVertex(posVtx,errVtx);
  
  // ==========================================================


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
    Float_t sta_eta        = -999;
    Float_t sta_phi        = -999;
    Float_t sta_pt         = -999;
    Float_t sta_res        = -999;
    
    Float_t tight_min_deltaR =  999;   
    Float_t tight_eta        = -999;
    Float_t tight_phi        = -999;
    Float_t tight_pt         = -999;
    
    Float_t trk_min_deltaR =  999; 
    Float_t trk_eta        = -999;
    Float_t trk_phi        = -999;
    Float_t trk_pt         = -999;
    Float_t trk_res        = -999;

    Float_t glb_min_deltaR =  999; 
    Float_t glb_eta        = -999;
    Float_t glb_phi        = -999;
    Float_t glb_pt         = -999;
    Float_t glb_res        = -999;


    for (pat::MuonCollection::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon) {

      Float_t iso =  (muon->pfIsolationR04().sumChargedHadronPt + max(0., muon->pfIsolationR04().sumNeutralHadronEt + muon->pfIsolationR04().sumPhotonEt - 0.5*muon->pfIsolationR04().sumPUPt))/muon->pt();

      float chargeIso = (muon->pfIsolationR04().sumChargedHadronPt)/muon->pt();
      float neutralIso = (muon->pfIsolationR04().sumNeutralHadronEt)/muon->pt();
      float photonIso = (muon->pfIsolationR04().sumPhotonEt)/muon->pt();
      float puIso = (muon->pfIsolationR04().sumPUPt)/muon->pt();

      hMuPFChargeIso->Fill(chargeIso);
      hMuPFNeutralIso->Fill(neutralIso);
      hMuPFPhotonIso->Fill(photonIso);
      hMuPFPUIso->Fill(puIso);

      hMuPFIso->Fill(iso);

      hMuPFIso_R->Fill(vr, iso);

      //if ( chargeIso > 0.15) continue;

      /* 
      bool passID = false; 
      float nValidHits =  0;
      float nValidPixelHits = 0; 

      if (muon->isGlobalMuon()) nValidHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits();
      float nMatched = muon->numberOfMatchedStations();
      if (muon->isTrackerMuon()) nValidPixelHits =  muon->innerTrack()->hitPattern().numberOfValidPixelHits();

      if ( nValidHits > 1 && nMatched > 1 && nValidPixelHits > 0) passID = true; 
      */

      // isTightMuon
      //------------------------------------------------------------------------
      if (muon::isTightMuon(*muon, thePrimaryVertex)) {
      //if ( passID) { 
      // Test Soft muons:: bool muon::isSoftMuon(const reco::Muon & recoMu, const reco::Vertex & vtx);

      //if ( muon::isSoftMuon(*muon, thePrimaryVertex)) {
     
	//if (muon::isMediumMuon(*muon)) {
	
	  Float_t muEta    = muon->eta();
	Float_t muPhi    = muon->phi();
	Float_t muPt     = muon->pt();
	Float_t muCharge = muon->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
 

	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < tight_min_deltaR) {
	  tight_min_deltaR = dR;
	  tight_eta        = muEta;
	  tight_phi        = muPhi;
	  tight_pt         = muPt;
	}
      }


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
	  trk_eta        = muEta;
	  trk_phi        = muPhi;
	  trk_pt         = muPt;
	  trk_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
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
	
	if (dR < glb_min_deltaR) {
	  glb_min_deltaR = dR;
	  glb_eta        = muEta;
	  glb_phi        = muPhi;
	  glb_pt         = muPt;
	  glb_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }
    }  // for..muons


  if (vr > max_vr) continue;
  

    // Fill gen histograms
    //--------------------------------------------------------------------------
    hGenMuons_eta->Fill(eta);
    hGenMuons_phi->Fill(phi);
    hGenMuons_pt ->Fill(pt);
    hGenMuons_vx ->Fill(fabs(vx));
    hGenMuons_vy ->Fill(fabs(vy));
    hGenMuons_vz ->Fill(fabs(vz));
    hGenMuons_vr ->Fill(vr);



    // Fill sta histograms
    //--------------------------------------------------------------------------
    
    if (tight_min_deltaR < 999)
      {

	hTightMuons_phi->Fill(tight_phi);
	hTightMuons_dR ->Fill(tight_min_deltaR);
	
	if (tight_min_deltaR < max_deltaR)
	  {
	    hTightMuons_eta->Fill(tight_eta);	
	    hTightMuons_pt->Fill(tight_pt);
	    hTightMuons_vr->Fill(vr);
	  } else {

	  hIDMuons_noGen_eta->Fill(tight_eta);	
	  hIDMuons_noGen_pt->Fill(tight_pt);
	  hIDMuons_noGen_vr->Fill(vr);
	}
      }

    if (sta_min_deltaR < 999)
      {
	//hStaMuons_eta->Fill(sta_eta);
	hStaMuons_phi->Fill(sta_phi);
	hStaMuons_dR ->Fill(sta_min_deltaR);

	if (sta_min_deltaR < max_deltaR)
	  {
	    hStaMuons_pt->Fill(sta_pt);
	    hStaMuons_vr->Fill(vr);
	    hStaMuons_eta->Fill(sta_eta);

	    hGenStaMuons_eta->Fill(eta, sta_eta);
	    hGenStaMuons_phi->Fill(phi, sta_phi);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt > ptbins[i] && pt < ptbins[i+1]) hStaMuons_res[i]->Fill(sta_res);
	  } else {
	  
	  hStaMuons_noGen_pt->Fill(sta_pt);
	  hStaMuons_noGen_vr->Fill(vr);
	  hStaMuons_noGen_eta->Fill(sta_eta);
	}
      }


    // Fill trk histograms
    //--------------------------------------------------------------------------
    if (trk_min_deltaR < 999)
      {

	hTrkMuons_phi->Fill(trk_phi);
	hTrkMuons_dR ->Fill(trk_min_deltaR);

	if (trk_min_deltaR < max_deltaR)
	  {
	    hTrkMuons_eta->Fill(trk_eta);
	    hTrkMuons_pt->Fill(trk_pt);
	    hTrkMuons_vr->Fill(vr);
	
	    hGenTrkMuons_eta->Fill(eta, trk_eta);
	    hGenTrkMuons_phi->Fill(phi, trk_phi);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt > ptbins[i] && pt < ptbins[i+1]) hTrkMuons_res[i]->Fill(trk_res);
	  } else {

	  hTrkMuons_noGen_pt->Fill(trk_pt);
	  hTrkMuons_noGen_vr->Fill(vr);
	  hTrkMuons_noGen_eta->Fill(trk_eta);
	}

      }


    // Fill glb histograms
    //--------------------------------------------------------------------------
    if (glb_min_deltaR < 999)
      {
	
	hGlbMuons_phi->Fill(glb_phi);
	hGlbMuons_dR ->Fill(glb_min_deltaR);

	if (glb_min_deltaR < max_deltaR)
	  {
	    hGlbMuons_eta->Fill(glb_eta);	
	    hGlbMuons_pt->Fill(glb_pt);
	    hGlbMuons_vr->Fill(vr);

	    hGenGlbMuons_eta->Fill(eta, glb_eta);
	    hGenGlbMuons_phi->Fill(phi, glb_phi);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt > ptbins[i] && pt < ptbins[i+1]) hGlbMuons_res[i]->Fill(glb_res);
	  } else {

	  hGlbMuons_noGen_eta->Fill(glb_eta);	
	  hGlbMuons_noGen_pt->Fill(glb_pt);
	  hGlbMuons_noGen_vr->Fill(vr);
	}
      }

    // } else  {



      //    }




  } // for..pruned
} 




DEFINE_FWK_MODULE(ExampleMuonAnalyzer);
