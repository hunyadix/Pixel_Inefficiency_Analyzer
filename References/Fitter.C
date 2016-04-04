#include <map>
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "TLegend.h"
#include "TStyle.h"

#include "custom_can_macro.h"

TH1D* getplot(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH1D *h= (TH1D*)f.Get(hname.Data());
  TH1D *c=(TH1D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

TH2D* getplot2d(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH2D *h= (TH2D*)f.Get(hname.Data());
  TH2D *c=(TH2D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

TH3D* getplot3d(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH3D *h= (TH3D*)f.Get(hname.Data());
  TH3D *c=(TH3D*)h->Clone();
  c->SetDirectory(0);
  return c;
}

void error(TH1D* eff, TH1D* dist) {
  double z = 1; // 1 Sigma confidence
  for (int i=1; i<=eff->GetNbinsX(); i++) if (dist->GetBinContent(i)!=0)
  {
    double p = eff->GetBinContent(i);
    double n = dist->GetBinContent(i);
    double cen = (p+(z*z/(2*n))) / (1.0 + (z*z/n));
    double halfwidth = z*sqrt( p*(1.0-p)/n + (z*z/(4*n*n)) ) / (1.0 + (z*z/n));
    double err = halfwidth + fabs(cen-p); // Assymmetric error -> Choose larger for a conservative error estimate
    eff->SetBinError(i,err);
  }
}

void rebin_eff(TH1D* eff, TH1D* dist, int n=2) {
  eff->Multiply(dist);
  dist->Add(eff,-1);
  eff->Rebin(n);
  dist->Rebin(n);
  dist->Add(eff);
  eff->Divide(dist);
  error(eff,dist);
}

int get_online_ladder(int layer, int ladder) {
  // copied exactly from TimingStudy::getModuleData
  int on_ladder = -9999;
  if (layer==1) {
    if (ladder<=5 && ladder>=1) on_ladder = 6-ladder;
    else if (ladder<=15 && ladder>=6) on_ladder = 5-ladder;
    else if (ladder<=20 && ladder>=16) on_ladder = 26-ladder;
  } else if (layer==2) {
    if (ladder<=8 && ladder>=1) on_ladder = 9-ladder;
    else if (ladder<=24 && ladder>=9) on_ladder = 8-ladder;
    else if (ladder<=32 && ladder>=25) on_ladder = 41-ladder;
  } if (layer==3) {
    if (ladder<=11 && ladder>=1) on_ladder = 12-ladder;
    else if (ladder<=33 && ladder>=12) on_ladder = 11-ladder;
    else if (ladder<=44 && ladder>=34) on_ladder = 56-ladder;
  }
  return on_ladder;
}

void Fitter(bool draw_fits = 1, std::string print_factors = "db") {

  if (print_factors != "cfg" && print_factors != "db") std::cout<<"Wrong argument for print_factors! it should be \"cfg\" or \"db\"."<<std::endl;

  // ______________________________________________
  //           Load histos from input files

  //std::string data_input_file = "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/PHM_out/2015_25ns_Data.root";
  //std::string data_input_file = "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/DataMCComp.root";
  std::string data_input_file = "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/PHM_out/DynIneff_2015.root";
  std::string mc_input_file = "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/PHM_out/DcolScan.root";

  // Data
  TH1D *data_instlumi_eff[3];
  std::string layname[3] = {"_Lay1", "_Lay2","_Lay3"};
  for (int lay=0; lay<3; lay++)
    //data_instlumi_eff[lay]=getplot(data_input_file,(std::string("HitEfficiency_vs_InstLumi/NBx2225")+layname[lay]+"_Mod12_Data"));
    data_instlumi_eff[lay]=getplot(data_input_file,(std::string("HitEfficiency_vs_InstLumi/2232b")+layname[lay]+"_Mod12_Data"));

  // MC
  TH1D *mc_det_eff[10];
  double      dcol_eff[10] = { 1, 0.999, 0.996, 0.993, 0.99, 0.98, 0.97, 0.95, 0.90, 0.80 };
  std::string dcolname[10] = {"100", "99p9", "99p6", "99p3", "99", "98", "97", "95", "90", "80"};
  for (int i=0; i<10; i++)
    mc_det_eff[i] = getplot(mc_input_file,(std::string("HitEfficiency_vs_LayersDisks/Dcol")+dcolname[i]));

  // Add Predicted 2016 Points
  // 35 PU -> 97.5% (instl = 0.37935 * 35 = 13.28)
  // 50 PU -> 96.5% (instl = 0.37935 * 50 = 18.97)
  // 35 PU -> 97.5% (instl = 0.313596 * 35 = 10.976)
  // 50 PU -> 96.5% (instl = 0.313596 * 50 = 15.68)
  //for (int i=1; i<data_instlumi_eff[0]->GetNbinsX(); ++i) 
  //  data_instlumi_eff[0]->SetBinError(i, data_instlumi_eff[0]->GetBinError(i)*3);
  //int bin1=data_instlumi_eff[0]->FindBin(13.28);
  //int bin2=data_instlumi_eff[0]->FindBin(18.97);
  int bin1=data_instlumi_eff[0]->FindBin(10.976);
  int bin2=data_instlumi_eff[0]->FindBin(15.68);
  data_instlumi_eff[0]->SetBinContent(bin1, 0.975);
  data_instlumi_eff[0]->SetBinContent(bin2, 0.965);
  data_instlumi_eff[0]->SetBinError(bin1, 0.003);
  data_instlumi_eff[0]->SetBinError(bin2, 0.003);

  // ______________________________________________
  //         Fill hiteff vs dcol histos

  const char* name[3] = {"hiteff_vs_dcol_l1", "hiteff_vs_dcol_l2", "hiteff_vs_dcol_l3"};
  TH1D *hiteff_vs_dcol[3];
  TF1 *avg_fit[3];
  for (int lay=0; lay<3; lay++) {
    // Hit Eff vs Dcol Eff Fitting
    hiteff_vs_dcol[lay] = new TH1D(name[lay], ";DCol Efficiency;Hit Efficiency", 251,0.7495,1.0005);
    for (int i=0; i<10; i++) {
      int bin = hiteff_vs_dcol[lay]->FindBin(dcol_eff[i]);
      hiteff_vs_dcol[lay]->SetBinContent(bin, mc_det_eff[i]->GetBinContent(lay+1));
      hiteff_vs_dcol[lay]->SetBinError(bin,   mc_det_eff[i]->GetBinError(lay+1));
    }
    avg_fit[lay] = new TF1(name[lay],"pol2",0.75,1);
    hiteff_vs_dcol[lay]->Fit(name[lay],"RQ0");
  }

  // ______________________________________________
  //   Convert hit efficiency to Dcol efficiency
  //          and fit with a function

  int npar[3] = { 2, 2, 2 };
  const char* fitfunc[3] = { "pol1", "pol1", "pol1" };
  TH1D *instlumi_dcol[3];
  TF1 *instlumi_fit[3];
  for (int lay=0; lay<3; lay++) {    
    // instlumi scaling
    data_instlumi_eff[lay]->SetMarkerStyle(20+lay);
    instlumi_dcol[lay]=(TH1D*)data_instlumi_eff[lay]->Clone();
    for (int i=1, n=data_instlumi_eff[lay]->GetNbinsX(); i<n; ++i) {
      double eff = data_instlumi_eff[lay]->GetBinContent(i);
      double dcol = avg_fit[lay]->GetX(eff);
      instlumi_dcol[lay]->SetBinContent(i,dcol);
    }
    instlumi_fit[lay] = new TF1((std::string("instlumi_fit")+layname[lay]).c_str(), fitfunc[lay], 0,20);
    
    //// Fix 1st parameter so that dcol(0PU) = 0.997
    //instlumi_fit[lay]->FixParameter(0,0.997);
    
    // Fix 1st parameter so that dcol(0PU) = 1
    instlumi_fit[lay]->FixParameter(0,1);
    
    instlumi_dcol[lay]->Fit((std::string("instlumi_fit")+layname[lay]).c_str(), "RQ0");
    //instlumi_fit[lay]->Print();
    std::cout<<"Layer "<<lay+1<<" (const pu[0]=0.997) chi^2/Ndof = "<<instlumi_fit[lay]->GetChisquare()/instlumi_fit[lay]->GetNDF()<<std::endl;
  }

  // ______________________________________________
  //              Print Fit parameters

  if (print_factors=="cfg") {
    for (int lay=0; lay<3; lay++) {   
      std::cout<<"    thePUEfficiency_BPix"<<(lay+1)<<" = cms.vdouble("<<std::endl;
      for (int par=0; par<npar[lay]; par++) std::cout<<"        "<<instlumi_fit[lay]->GetParameter(par)<<",\n";
      std::cout<<"    ),\n";
    }
  }
  if (print_factors=="db"){
    std::cout<<"    thePUEfficiency = cms.untracked.VPSet(\n";
    for (int lay=0; lay<3; lay++) {   
      std::cout<<"        cms.PSet(\n";
      std::cout<<"            det = cms.string(\"bpix\"),\n";
      std::cout<<"            layer = cms.uint32("<<lay+1<<"),\n";
      std::cout<<"            factor = cms.vdouble(\n";
      for (int par=0; par<npar[lay]; par++)
	std::cout<<"              "<<instlumi_fit[lay]->GetParameter(par)<<",\n";
      std::cout<<"            ),\n";
      std::cout<<"        ),\n";
    }
    std::cout<<"    ),\n";
  }

  // ______________________________________________
  //                  Draw plots

  gStyle->SetOptTitle(0);
  TCanvas* c = custom_can_(hiteff_vs_dcol[0], "HitEfficiency_vs_DCol", 1,1, 500,500, 80,20);
  hiteff_vs_dcol[0]->SetAxisRange(0.85, 1, "Y");
  hiteff_vs_dcol[0]->SetMarkerColor(633);
  hiteff_vs_dcol[0]->SetMarkerStyle(20);
  hiteff_vs_dcol[0]->Draw("P");
  hiteff_vs_dcol[1]->SetMarkerColor(618);
  hiteff_vs_dcol[1]->SetMarkerStyle(21);
  hiteff_vs_dcol[1]->Draw("SAMEP");
  hiteff_vs_dcol[2]->SetMarkerColor(601);
  hiteff_vs_dcol[2]->SetMarkerStyle(22);
  hiteff_vs_dcol[2]->Draw("SAMEP");
  avg_fit[0]->SetLineColor(633);
  avg_fit[1]->SetLineColor(618);
  avg_fit[2]->SetLineColor(601);
  avg_fit[0]->Draw("SAME");
  avg_fit[1]->Draw("SAME");
  avg_fit[2]->Draw("SAME");
  TLegend* leg = new TLegend(0.35,0.2,0.5,0.4,"");
  leg->AddEntry(hiteff_vs_dcol[0],   "Layer 1", "P");
  leg->AddEntry(hiteff_vs_dcol[1],   "Layer 2", "P");
  leg->AddEntry(hiteff_vs_dcol[2],   "Layer 3", "P");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAMEP1");

  c = custom_can_(data_instlumi_eff[0], "HitEfficiency_vs_InstLumi_With2016Pred", 1,1, 500,500, 90,20);
  data_instlumi_eff[0]->SetTitle("");
  data_instlumi_eff[0]->GetYaxis()->SetTitle("Hit Efficiency");
  data_instlumi_eff[0]->GetXaxis()->SetRangeUser(0,20);
  data_instlumi_eff[0]->GetYaxis()->SetRangeUser(0.95,1);
  for (int lay=0; lay<3; ++lay) {
    data_instlumi_eff[lay]->SetMarkerColor(lay==0 ? 633 : lay==1 ? 618 : 601);
    data_instlumi_eff[lay]->Draw(lay? "P SAME" : "P");
  }
  leg = new TLegend(0.35,0.2,0.55,0.4,"2232 Colliding Bunches");
  leg->AddEntry(data_instlumi_eff[0],   "Layer 1", "P");
  leg->AddEntry(data_instlumi_eff[1],   "Layer 2", "P");
  leg->AddEntry(data_instlumi_eff[2],   "Layer 3", "P");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAMEP1");

  c = custom_can_(instlumi_dcol[0], "DColEff_vs_InstLumi_With2016Pred", 1,1, 500,500, 80,20);
  instlumi_dcol[0]->SetTitle("2016 Prediciton");
  instlumi_dcol[0]->GetXaxis()->SetRangeUser(0,20);
  instlumi_dcol[0]->GetYaxis()->SetTitle("DCol factor");
  instlumi_dcol[0]->GetYaxis()->SetRangeUser(0.86,1);
  for (int lay=0; lay<3; ++lay) {
    instlumi_dcol[lay]->SetMarkerColor(lay==0 ? 633 : lay==1 ? 618 : 601);
    instlumi_fit[lay]->SetLineColor(lay==0 ? 633 : lay==1 ? 618 : 601);
    instlumi_dcol[lay]->Draw(lay? "P SAME" : "P");
    instlumi_fit[lay]->Draw("SAME");
  }
  leg = new TLegend(0.35,0.2,0.5,0.4,"");
  leg->AddEntry(instlumi_dcol[0],   "Layer 1", "P");
  leg->AddEntry(instlumi_dcol[1],   "Layer 2", "P");
  leg->AddEntry(instlumi_dcol[2],   "Layer 3", "P");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAMEP1");


}
