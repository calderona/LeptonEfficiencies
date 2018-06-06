#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


// Data members
//------------------------------------------------------------------------------
enum        {noPU, PU200};

const Int_t nbinspt = 3;

Color_t     ptcolor[nbinspt] = {kRed+1, kBlue, kBlack};

Bool_t      doRebin     = false;
Bool_t      doSetRanges = false;
Bool_t      doSavePdf   = false;
Bool_t      doSavePng   = true;

TFile*      file_PU200 = NULL;
TFile*      file_noPU  = NULL;


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString type,
				  TString variable,
				  Int_t   PU,
				  Color_t color);

TGraphAsymmErrors* MakeFakes(TString variable,
			     Int_t   PU,
			     Color_t color);


void               DrawEfficiency(TString variable);

void               DrawFakes();

void               DrawResolution(TString muonType);

void               DrawTH2       (TString variable,
				  TString muonType);

void               Compare       (TString variable,
				  TString muonType,
				  Float_t xmax = -999);

TH1F*              AddOverflow   (TH1F*   h);


//------------------------------------------------------------------------------
//
// doEfficiencies
//
//------------------------------------------------------------------------------
void doEfficiencies()
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir("pdf", kTRUE);
  if (doSavePng) gSystem->mkdir("png", kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file_PU200 = TFile::Open("rootfiles/MyMuonPlots_PU200.root");
  file_noPU  = TFile::Open("rootfiles/MyMuonPlots_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  //  DrawEfficiency("vr");
  //DrawEfficiency("pt");
  DrawEfficiency("eta");
  DrawFakes();



  DrawResolution("Sta");
  DrawResolution("Trk");
  DrawResolution("Glb");

  DrawTH2("eta", "Sta");
  DrawTH2("eta", "Trk");
  DrawTH2("eta", "Glb");
  
  DrawTH2("phi", "Sta");
  DrawTH2("phi", "Trk");
  DrawTH2("phi", "Glb");

  Compare("iso", "all");
  Compare("charge", "all");
  Compare("neutral", "all");
  Compare("photon", "all");
  Compare("pu", "all");


  Compare("dR", "Tight");
  Compare("dR", "Sta");
  Compare("dR", "Trk");
  Compare("dR", "Glb");
  
  Compare("pt", "Sta");
  Compare("pt", "Trk");
  Compare("pt", "Glb");
  
  Compare("vr", "Gen", 50);
}


//------------------------------------------------------------------------------
//
// Make efficiency
//
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeFakes(TString variable,
				  Int_t   PU,
				  Color_t color)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + variable + "Muons_noGen_vr";
  TString den_name = "muonAnalysis/GenMuons_vr";

  TH1F* hnum = (TH1F*)(file->Get(num_name))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name))->Clone("hden");

  if (doRebin) hnum->Rebin(5);
  if (doRebin) hden->Rebin(5);

  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(hnum, hden);

  tgae->SetLineColor  (color);
  tgae->SetLineWidth  (    1);
  tgae->SetMarkerColor(color);
  tgae->SetMarkerStyle(style);

  return tgae;
}

TGraphAsymmErrors* MakeEfficiency(TString type,
				  TString variable,
				  Int_t   PU,
				  Color_t color)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + type + "Muons_" + variable;
  TString den_name = "muonAnalysis/GenMuons_" + variable;

  std::cout << num_name << "  " << den_name << std::endl;

  TH1F* hnum = (TH1F*)(file->Get(num_name))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name))->Clone("hden");

  if (doRebin) hnum->Rebin(5);
  if (doRebin) hden->Rebin(5);

  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(hnum, hden);

  tgae->SetLineColor  (color);
  tgae->SetLineWidth  (    1);
  tgae->SetMarkerColor(color);
  tgae->SetMarkerStyle(style);

  return tgae;
}

//------------------------------------------------------------------------------
//
// Draw efficiency
//
//------------------------------------------------------------------------------
void DrawEfficiency(TString variable)
{
  TGraphAsymmErrors* sta_efficiency = MakeEfficiency("Sta", variable , PU200, kBlack);
  TGraphAsymmErrors* trk_efficiency = MakeEfficiency("Trk", variable, PU200, kRed+1);
  TGraphAsymmErrors* glb_efficiency = MakeEfficiency("Glb", variable, PU200, kBlue);
  TGraphAsymmErrors* tight_efficiency = MakeEfficiency("Tight", variable, PU200, kGreen+2);

  TGraphAsymmErrors* sta_efficiency_noPU = MakeEfficiency("Sta", variable, noPU, kBlack);
  TGraphAsymmErrors* trk_efficiency_noPU = MakeEfficiency("Trk", variable, noPU, kRed+1);
  TGraphAsymmErrors* glb_efficiency_noPU = MakeEfficiency("Glb", variable, noPU, kBlue);
  TGraphAsymmErrors* tight_efficiency_noPU = MakeEfficiency("Tight", variable, noPU, kGreen+2);

  TCanvas* canvas = new TCanvas("efficiency", "efficiency", 600, 600);

  canvas->SetLeftMargin (0.13);
  canvas->SetRightMargin(0.24);

  TMultiGraph* mg = new TMultiGraph();

  mg->Add(sta_efficiency);
  mg->Add(sta_efficiency_noPU);
  mg->Add(trk_efficiency);
  mg->Add(trk_efficiency_noPU);
  mg->Add(glb_efficiency);
  mg->Add(glb_efficiency_noPU);
  mg->Add(tight_efficiency);
  mg->Add(tight_efficiency_noPU);

  mg->Draw("apz");

  // Ranges
  if (doSetRanges)
    {
      mg->GetXaxis()->SetRangeUser(0, 49);

      mg->SetMinimum(-0.05);
      mg->SetMaximum( 1.05);

    

    }
  //  mg->GetYaxis()->SetRangeUser(0.8, 1.02);
  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");
  if (variable == "vr") {
    mg->GetXaxis()->SetTitle("production radius [cm]");
  } else if (variable == "pt") {
    mg->GetXaxis()->SetTitle("pT (GeV)");
  } else if (variable == "eta") {
     mg->GetXaxis()->SetTitle("eta");
  }
 
  mg->GetYaxis()->SetTitle("reconstruction efficiency");
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.5);

  // Legend
  TLegend* legend = new TLegend(0.775, 0.5, 0.95, 0.91);

  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (0.025);

  legend->AddEntry(sta_efficiency_noPU, "(no PU) sta",  "lp");
  legend->AddEntry(trk_efficiency_noPU, "(no PU) trk",  "lp");
  legend->AddEntry(glb_efficiency_noPU, "(no PU) glb",  "lp");
  legend->AddEntry(tight_efficiency_noPU, "(no PU) Medium",  "lp"); 
  legend->AddEntry(sta_efficiency,      "(200 PU) sta", "lp");
  legend->AddEntry(trk_efficiency,      "(200 PU) trk", "lp");
  legend->AddEntry(glb_efficiency,      "(200 PU) glb", "lp");
  legend->AddEntry(tight_efficiency,      "(200 PU) Medium", "lp");


  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePdf) { 
    if (variable == "vr") {
      canvas->SaveAs("pdf/efficiency_vr.pdf");
  } else if (variable == "pt") {
      canvas->SaveAs("pdf/efficiency_pt.pdf");
  } else if (variable == "eta") {
      canvas->SaveAs("pdf/efficiency_eta.pdf");
    }
  }
    
  if (doSavePng) {
    if (variable == "vr") {
      canvas->SaveAs("png/efficiency_vr.png");
    } else if (variable == "pt") {
      canvas->SaveAs("png/efficiency_pt.png");
    } else if (variable == "eta") {
      canvas->SaveAs("png/efficiency_eta.png");
    }
  }


}




//------------------------------------------------------------------------------
//
// Draw efficiency
//
//------------------------------------------------------------------------------
void DrawFakes()
{
  TGraphAsymmErrors* sta_efficiency = MakeFakes("Sta", PU200, kBlack);
  TGraphAsymmErrors* trk_efficiency = MakeFakes("Trk", PU200, kRed+1);
  TGraphAsymmErrors* glb_efficiency = MakeFakes("Glb", PU200, kBlue);
  TGraphAsymmErrors* tight_efficiency = MakeFakes("ID", PU200, kGreen+2);

  TGraphAsymmErrors* sta_efficiency_noPU = MakeFakes("Sta", noPU, kBlack);
  TGraphAsymmErrors* trk_efficiency_noPU = MakeFakes("Trk", noPU, kRed+1);
  TGraphAsymmErrors* glb_efficiency_noPU = MakeFakes("Glb", noPU, kBlue);
  TGraphAsymmErrors* tight_efficiency_noPU = MakeFakes("ID", noPU, kGreen+2);

  TCanvas* canvas = new TCanvas("efficiency", "efficiency", 600, 600);

  canvas->SetLeftMargin (0.13);
  canvas->SetRightMargin(0.24);

  TMultiGraph* mg = new TMultiGraph();

  mg->Add(sta_efficiency);
  mg->Add(sta_efficiency_noPU);
  mg->Add(trk_efficiency);
  mg->Add(trk_efficiency_noPU);
  mg->Add(glb_efficiency);
  mg->Add(glb_efficiency_noPU);
  mg->Add(tight_efficiency);
  mg->Add(tight_efficiency_noPU);

  mg->Draw("apz");

  // Ranges
  if (doSetRanges)
    {
      mg->GetXaxis()->SetRangeUser(0, 49);

      mg->SetMinimum(-0.05);
      mg->SetMaximum( 1.05);

    

    }
  //  mg->GetYaxis()->SetRangeUser(0.8, 1.02);
  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");
  mg->GetXaxis()->SetTitle("production radius [cm]");
  mg->GetYaxis()->SetTitle("reconstruction fake rate");
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.5);

  // Legend
  TLegend* legend = new TLegend(0.775, 0.5, 0.95, 0.91);

  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (0.025);

  legend->AddEntry(sta_efficiency_noPU, "(no PU) sta",  "lp");
  legend->AddEntry(trk_efficiency_noPU, "(no PU) trk",  "lp");
  legend->AddEntry(glb_efficiency_noPU, "(no PU) glb",  "lp");
  legend->AddEntry(tight_efficiency_noPU, "(no PU) Medium",  "lp"); 
  legend->AddEntry(sta_efficiency,      "(200 PU) sta", "lp");
  legend->AddEntry(trk_efficiency,      "(200 PU) trk", "lp");
  legend->AddEntry(glb_efficiency,      "(200 PU) glb", "lp");
  legend->AddEntry(tight_efficiency,      "(200 PU) Medium", "lp");


  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs("pdf/fakes.pdf");
  if (doSavePng) canvas->SaveAs("png/fakes.png");
}

//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType)
{
  TCanvas* c2 = new TCanvas("resolution " + muonType, "resolution " + muonType);

  TH1F* hStaMuons_res[nbinspt];

  Float_t ymax = 0;

  for (Int_t i=0; i<nbinspt; i++) {

    hStaMuons_res[i] = (TH1F*)file_PU200->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    if (hStaMuons_res[i]->GetMaximum() > ymax) ymax = hStaMuons_res[i]->GetMaximum();

    hStaMuons_res[i]->SetLineColor(ptcolor[i]);
    hStaMuons_res[i]->SetLineWidth(3);

    TString option = (i == 0) ? "" : "same";

    hStaMuons_res[i]->Draw(option);
  }

  hStaMuons_res[0]->SetTitle("");
  hStaMuons_res[0]->SetXTitle(muonType + " #Deltaq/p_{T} / (q/p_{T})");
  hStaMuons_res[0]->SetYTitle("entries / bin");
  hStaMuons_res[0]->SetMaximum(1.1 * ymax);
  hStaMuons_res[0]->GetXaxis()->SetTitleOffset(1.5);
  hStaMuons_res[0]->GetYaxis()->SetTitleOffset(2.0);

  // Legend
  TLegend* legend = new TLegend(0.59, 0.72, 0.85, 0.88);

  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (0.035);

  legend->AddEntry(hStaMuons_res[0], "10 < p_{T} < 20 GeV", "l");
  legend->AddEntry(hStaMuons_res[1], "20 < p_{T} < 35 GeV", "l");
  legend->AddEntry(hStaMuons_res[2], "35 < p_{T} < 50 GeV", "l");

  legend->Draw();

  c2->GetFrame()->DrawClone();

  if (doSavePdf) c2->SaveAs("pdf/resolution_" + muonType + ".pdf");
  if (doSavePng) c2->SaveAs("png/resolution_" + muonType + ".png");
}


//------------------------------------------------------------------------------
//
// Draw TH2
//
//------------------------------------------------------------------------------
void DrawTH2(TString variable,
	     TString muonType)
{
  TH2F* h2 = (TH2F*)file_noPU->Get("muonAnalysis/Gen" + muonType + "Muons_" + variable);

  TCanvas* canvas = new TCanvas("Gen vs. " + muonType + " " + variable,
				"Gen vs. " + muonType + " " + variable);

  h2->SetTitle("");
  h2->GetXaxis()->SetTitle("gen #" + variable);
  h2->GetYaxis()->SetTitle("(no PU) " + muonType + " #" + variable);
  h2->GetXaxis()->SetTitleOffset(1.5);
  h2->GetYaxis()->SetTitleOffset(2.0);

  h2->Draw("colz");

  if (doSavePdf) canvas->SaveAs("pdf/Gen_vs_" + muonType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/Gen_vs_" + muonType + "_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Compare
//
//------------------------------------------------------------------------------
void Compare(TString variable,
	     TString muonType,
	     Float_t xmax)
{


  TH1F* h_noPU  = NULL;
  TH1F* h_PU200 = NULL;

  if ((variable.Contains("iso"))){
 
    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/MuPFIso"))->Clone("h_"+ muonType + "_noPU_"  + variable);
    h_PU200 =  (TH1F*)(file_PU200 ->Get("muonAnalysis/MuPFIso"))->Clone("h_"+ muonType + "_PU200_"  + variable);

  } else  if ((variable.Contains("charge"))){
 
    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/MuPFChargeIso"))->Clone("h_"+ muonType + "_noPU_"  + variable);
    h_PU200 =  (TH1F*)(file_PU200 ->Get("muonAnalysis/MuPFChargeIso"))->Clone("h_"+ muonType + "_PU200_"  + variable);
 
  }   else  if ((variable.Contains("neutral"))){
 
    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/MuPFNeutralIso"))->Clone("h_"+ muonType + "_noPU_"  + variable);
    h_PU200 =  (TH1F*)(file_PU200 ->Get("muonAnalysis/MuPFNeutralIso"))->Clone("h_"+ muonType + "_PU200_"  + variable);
  }  else  if ((variable.Contains("photon"))){
 
    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/MuPFPhotonIso"))->Clone("h_"+ muonType + "_noPU_"  + variable);
    h_PU200 =  (TH1F*)(file_PU200 ->Get("muonAnalysis/MuPFPhotonIso"))->Clone("h_"+ muonType + "_PU200_"  + variable);
 
  }   else  if ((variable.Contains("pu"))){
 
    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/MuPFPUIso"))->Clone("h_"+ muonType + "_noPU_"  + variable);
    h_PU200 =  (TH1F*)(file_PU200 ->Get("muonAnalysis/MuPFPUIso"))->Clone("h_"+ muonType + "_PU200_"  + variable);
 
  } else {

    h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_noPU_"  + variable);
    h_PU200 = (TH1F*)(file_PU200->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_PU200_" + variable);

  }

  if (!muonType.Contains("Gen"))
    {
      h_noPU ->Rebin(2);
      h_PU200->Rebin(2);
    }

                                                                                                                 
  
  h_noPU ->Scale(1. / h_noPU ->Integral(-1, -1));
  h_PU200->Scale(1. / h_PU200->Integral(-1, -1));

  h_noPU ->SetLineColor(kBlack);
  h_PU200->SetLineColor(kRed+1);

  h_noPU ->SetLineWidth(2);
  h_PU200->SetLineWidth(2);




  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("compare " + muonType + " " + variable,
				"compare " + muonType + " " + variable);

  if (variable.Contains("dR")) canvas->SetLogy();
  if (variable.Contains("vr")) canvas->SetLogy();
  if (variable.Contains("iso") || variable.Contains("charge")  || variable.Contains("photon")  || variable.Contains("neutral")  || variable.Contains("pu")  ) canvas->SetLogy(); 

  TH1F* h_noPU_overflow  = AddOverflow(h_noPU);
  TH1F* h_PU200_overflow = AddOverflow(h_PU200);

  
  if (h_noPU_overflow->GetMaximum() > h_PU200_overflow->GetMaximum())
    {
      h_noPU_overflow ->Draw("hist");
      h_PU200_overflow->Draw("hist,same");
      h_noPU_overflow ->GetXaxis()->SetTitle(muonType + " " + variable);
      
      if (xmax > -999) h_noPU_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  else
    {
      h_PU200_overflow->Draw("hist");
      h_noPU_overflow ->Draw("hist,same");
      h_PU200_overflow->GetXaxis()->SetTitle(muonType + " " + variable);

      if (xmax > -999) h_PU200_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  

  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs("pdf/compare_" + muonType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/compare_" + muonType + "_" + variable + ".png");
  
}


//------------------------------------------------------------------------------
// Add overflow
//------------------------------------------------------------------------------
TH1F* AddOverflow(TH1F* h)
{
  TString  name = h->GetName();
  Int_t    nx   = h->GetNbinsX()+1;
  Double_t bw   = h->GetBinWidth(nx);
  Double_t x1   = h->GetBinLowEdge(1);
  Double_t x2   = h->GetBinLowEdge(nx) + bw;
  
  // Book a new histogram having an extra bin for overflows
  TH1F* htmp = new TH1F(name + "_overflow", "", nx, x1, x2);

  // Fill the new hisogram including the extra bin for overflows
  for (Int_t i=1; i<=nx; i++) {
    htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i));
  }

  // Fill the underflow
  htmp->Fill(x1-1, h->GetBinContent(0));

  // Restore the number of entries
  htmp->SetEntries(h->GetEntries());

  // Cosmetics
  htmp->SetLineColor(h->GetLineColor());
  htmp->SetLineWidth(h->GetLineWidth());
  htmp->GetXaxis()->SetTitleOffset(1.5);

  return htmp;
}
