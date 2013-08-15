#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include "TPaveText.h"
#include "TCanvas.h"
#include "TTreeUser.C"
#include "tdrstyle.C"
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include <string>


using namespace std;

//------------------------Methods-------------------------
void showMessage(string); 

void drawPrelim(float, bool);

void loadSamples(); 

void end();

float statErrorN(float);

float statErrorP(float);

float computeRatioError(float, float, float, float);

float computeProductError(float, float, float, float);

TGraphAsymmErrors * histRatio(TH1F *,TH1F *, int, bool);

void makePlot(TH1F *, TH1F *, THStack*, string, string, string);

void makePlot(TH1F *, TH1F *, string, string, string);

//-------------------Global variables---------------------
TTreeUser *data, *cocktail, *cocktails, *lm4, *lm8, *lm4eff, *lm8eff, *zjets, *zjets2, *ttbar, *sms1, *sms2, *sms3, *sms4;
float lumi; 
TCut sfzp("id1==id2&&abs(mll-91)<20.0"); 
TCut ofzp("id1!=id2&&abs(mll-91)<20.0"); 
TCut sfsb("id1==id2&&((mll>55&&mll<70)||(mll>112&&mll<160))"); 
TCut ofsb("id1!=id2&&((mll>55&&mll<70)||(mll>112&&mll<160))"); 
TCut jets("(pfJetGoodNum>2&&pfJetGoodID[0]!=0)&&(pfJetGoodNum>2&&pfJetGoodID[1]!=0)&&(pfJetGoodNum>2&&pfJetGoodID[2]!=0)");
//--------------------------------------------------------





//-----------------Method implementation------------------
template<class T>
std::string any2string(T i)
{
  std::ostringstream buffer;
  buffer << i;
  return buffer.str();
}



void showMessage(string msg) {
  cout << "[JZB Analysis] " << msg << endl;
}


void drawPrelim(float writelumi, bool isMC) {

  string barn="pb";
  if(writelumi>=1000)
  {
    writelumi/=1000;
    barn="fb";
  }

  stringstream prelimtext;
  if(writelumi == 0) {
    if(isMC) prelimtext << "CMS Simulation, #sqrt{s} = 7 TeV";
    else prelimtext << "CMS Preliminary, #sqrt{s} = 7 TeV";
  } else {
    if(isMC) prelimtext << "CMS Simulation, #sqrt{s} = 7 TeV, L = " << std::setprecision(2) <<writelumi<<" "<<barn<<"^{-1}";
    else prelimtext << "CMS Preliminary, #sqrt{s} = 7 TeV, L = " << std::setprecision(2) <<writelumi<<" "<<barn<<"^{-1}";
  }
  TPaveText *eventSelectionPaveText = new TPaveText(0.27, 0.93,0.77, 1.0,"blNDC");
  eventSelectionPaveText->SetFillStyle(4000);
  eventSelectionPaveText->SetBorderSize(0);
  eventSelectionPaveText->SetFillColor(kWhite);
  eventSelectionPaveText->SetTextFont(42);
  eventSelectionPaveText->SetTextSize(0.042);
  eventSelectionPaveText->AddText(prelimtext.str().c_str());
  eventSelectionPaveText->Draw();
}



void loadSamples() {
  
  // Luminosity
  lumi = 4700.0;
  // Scale factors for MC samples: crosssections
  float ZJetsCrossSection         = 3048.0;       //NNLO
  float TTbarCrossSection         = 165.0;        //NLO
  float WJetsCrossSection         = 31314.0;      //NNLO
  float ZnunuCrossSection         = 5760.0;       //NNLO
  float SingleTopSCrossSection    = 4.63;         //NLO
  float SingleTopTCrossSection    = 64.57;        //NLO
  float SingleTopUCrossSection    = 15.74;        //NLO
  float VVJetsCrossSection        = 4.8;          //LO
  float LM4CrossSection           = 1.879*1.35;   //k*LO
  float LM8CrossSection           = 0.7300*1.41 ; //k*LO  
  // Scale factors for MC samples: number of events in the MC sample
  float totEventsZjets2            = 32005720.0;
  float totEventsZjets            = 3200572.0;
  float totEventsTTbar            = 3701872.0;
  float totEventsWJets            = 53739703.0;
  float totEventsZnunu            = 2106977.0;
  float totEventsVVJets           = 959076.0;
  float totEventsSingleTopS       = 493868.0;
  float totEventsSingleTopT       = 475460.0;
  float totEventsSingleTopU       = 489417.0;
  float totEventsLM4              =   218380;
  float totEventsLM8              =   180960;


  // Init data sample
  data = new TTreeUser();
  data->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/Data.root", "events", 1.0, 1.0, 1, "data");

  // Init MC cocktail
  cocktail = new TTreeUser();
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11_small.root", "events", lumi/totEventsZjets*ZJetsCrossSection, totEventsZjets, 5, "zjets");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TTJets_TuneZ2_7TeV-madgraph-tauola_Summer11_3.root", "events", lumi/totEventsTTbar*TTbarCrossSection, totEventsTTbar, 9, "ttbar");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Summer11_2.root", "events",  lumi/totEventsWJets*WJetsCrossSection, totEventsWJets, 4, "wjets");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_s-channel_7TeV-madgraph_2.root", "events", lumi/totEventsSingleTopS*SingleTopSCrossSection, totEventsSingleTopS, 2, "singlets");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_t-channel_7TeV-madgraph.root", "events", lumi/totEventsSingleTopT*SingleTopTCrossSection,totEventsSingleTopT, 2, "singlett");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph.root", "events", lumi/totEventsSingleTopU*SingleTopUCrossSection, totEventsSingleTopU, 2, "singlettw");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/ZinvisibleJets_7TeV-madgraph.root", "events", lumi/totEventsZnunu*ZnunuCrossSection, totEventsZnunu, 1, "zinv");
  cocktail->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/VVJetsTo4L_TuneD6T_7TeV-madgraph-tauola.root", "events", lumi/totEventsVVJets*VVJetsCrossSection, totEventsVVJets, 3, "VVjets");


  //Init MC cocktail with signal
  cocktails = new TTreeUser();
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11_small.root", "events", lumi/totEventsZjets*ZJetsCrossSection, totEventsZjets, 5, "zjets");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TTJets_TuneZ2_7TeV-madgraph-tauola_Summer11_3.root", "events", lumi/totEventsTTbar*TTbarCrossSection, totEventsTTbar, 9, "ttbar");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Summer11_2.root", "events",  lumi/totEventsWJets*WJetsCrossSection, totEventsWJets, 4, "wjets");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_s-channel_7TeV-madgraph_2.root", "events", lumi/totEventsSingleTopS*SingleTopSCrossSection, totEventsSingleTopS, 2, "singlets");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_t-channel_7TeV-madgraph.root", "events", lumi/totEventsSingleTopT*SingleTopTCrossSection,totEventsSingleTopT, 2, "singlett");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph.root", "events", lumi/totEventsSingleTopU*SingleTopUCrossSection, totEventsSingleTopU, 2, "singlettw");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/ZinvisibleJets_7TeV-madgraph.root", "events", lumi/totEventsZnunu*ZnunuCrossSection, totEventsZnunu, 1, "zinv");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/VVJetsTo4L_TuneD6T_7TeV-madgraph-tauola.root", "events", lumi/totEventsVVJets*VVJetsCrossSection, totEventsVVJets, 3, "VVjets");
  cocktails->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/LM4_SUSY_sftsht_7TeV-pythia6.root", "events", lumi/totEventsVVJets*VVJetsCrossSection, totEventsVVJets, 2, "lm4");




  lm4 = new TTreeUser();
  lm4->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/LM4_SUSY_sftsht_7TeV-pythia6.root", "events", lumi/totEventsLM4*LM4CrossSection, totEventsLM4, 6, "lm4"); 
  
  lm8 = new TTreeUser();
  lm8->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/LM8_SUSY_sftsht_7TeV-pythia6.root", "events", lumi/totEventsLM8*LM8CrossSection, totEventsLM8, 9, "lm8"); 

  zjets = new TTreeUser();
  zjets->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11_small.root", "events", lumi/totEventsZjets*ZJetsCrossSection, totEventsZjets, 5, "zjets");

  zjets2 = new TTreeUser();
  zjets2->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11.root", "events", lumi/totEventsZjets2*ZJetsCrossSection, totEventsZjets2, 5, "zjets");

  ttbar = new TTreeUser();
  ttbar->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/TTJets_TuneZ2_7TeV-madgraph-tauola_Summer11_3.root", "events", lumi/totEventsTTbar*TTbarCrossSection, totEventsTTbar, 9, "ttbar");

  sms1 = new TTreeUser();
  sms1->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/SMS_clean_splitup_1_14.root", "events",  1, 20000.0, 1, "sms1");
  
  sms2 = new TTreeUser();
  sms2->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/SMS_clean_splitup_10_13.root", "events",  1, 20000.0, 1, "sms2");
  
  sms3 = new TTreeUser();
  sms3->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/SMS_clean_splitup_2_9.root", "events",  1, 20000.0, 1, "sms3");
 
  sms4 = new TTreeUser();
  sms4->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/SMS_clean_splitup_0_3.root", "events",  1, 20000.0, 1, "sms4");

  lm4eff = new TTreeUser();
  lm4eff->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/LM4_SUSY_sftsht_7TeV-pythia6.root", "events", 1.0, totEventsLM4, 6, "lm4");

  lm8eff = new TTreeUser();
  lm8eff->Add("/gpfs/gpfsddn/srm/cms/store/user/cmsdas/2012/JZBAnalysis/LM8_SUSY_sftsht_7TeV-pythia6.root", "events", 1.0, totEventsLM8, 9, "lm8");




}


//________________________________
void end() {

  delete cocktail;
  delete data;
  delete cocktails;
  delete lm4;
  delete lm8;
  delete lm4eff;
  delete lm8eff;
  delete zjets;
  delete zjets2;
  delete ttbar;
  delete sms1;
  delete sms2;
  delete sms3;
  delete sms4;


}

//________________________________
float computeRatioError(float a, float da, float b, float db) {
  float val=0.;
  float errorSquare = (a/b)*(a/b)*( (da/a)*(da/a) + (db/b)*(db/b));
  val = TMath::Sqrt(errorSquare);
  return val;

}


//________________________________
float computeProductError(float a, float da, float b, float db) {
  float val=0.;
  float errorSquare = (a*b)*(a*b)*( (da/a)*(da/a) + (db/b)*(db/b));
  val = TMath::Sqrt(errorSquare);
  return val;
}

//________________________________
float statErrorN(float x){return x - 0.5*TMath::ChisquareQuantile(0.3173/2,2*x);}

//________________________________
float statErrorP(float x){return 0.5*TMath::ChisquareQuantile(1-0.3173/2,2*(x+1))-x;}


//________________________________
TGraphAsymmErrors *histRatio(TH1F *h1,TH1F *h2, int id, bool precise) {
  int absJZBbinsNumber = h1->GetNbinsX()-1;
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(absJZBbinsNumber);
  string name1(h1->GetName());
  string name2(h2->GetName());
  string name3 = name1 + name2;
  graph->SetName(name3.c_str());
  for(unsigned int i=0;i<absJZBbinsNumber;i++) {
    float xCenter=h1->GetBinCenter(i+1);
    float xWidth=(h1->GetBinWidth(i+1))*0.5;
    float nominatorError = h1->GetBinError(i+1);
    float nominator=h1->GetBinContent(i+1);
    float denominatorError=h2->GetBinError(i+1);
    float denominator=h2->GetBinContent(i+1);
    float errorN = 0;
    float errorP = computeRatioError(nominator,nominatorError,denominator,denominatorError);
    if(id==1) {
      if(!precise) errorP = computeRatioError(nominator,statErrorP(nominator),denominator,statErrorP(denominator));
      else         errorP = computeRatioError(nominator,nominatorError,denominator,denominatorError);
      errorN = errorP; // symmetrize using statErrorP
     } else {
      errorN = computeRatioError(nominator,nominatorError,denominator,denominatorError);
      errorP = errorN;
     }
     if(denominator!=0) {
       graph->SetPoint(i, xCenter, nominator/denominator);
       graph->SetPointError(i,xWidth,xWidth,errorN,errorP);
     } else {
       graph->SetPoint(i, xCenter, -999);
       graph->SetPointError(i,xWidth,xWidth,0,0);
     }
  }
  return graph;
}


void makePlot(TH1F * h1, TH1F * h2, THStack *hs, string s1, string s2, string name) {

  float bottommargin=gStyle->GetPadBottomMargin();
  float canvas_height=gStyle->GetCanvasDefH();
  float canvas_width=gStyle->GetCanvasDefW();
  float ratiospace=0.25;// space the ratio should take up (relative to original pad)
  float ratiobottommargin=0.3;
  float ratiotopmargin=0.1;
  float xstretchfactor=((1-ratiospace)*(1-gStyle->GetPadTopMargin()))/((1)*ratiospace);

  TCanvas *main_canvas = new TCanvas("main_canvas","main_canvas",(Int_t)canvas_width,(Int_t)(canvas_height*(1+ratiospace)));
  TPad *mainpad = new TPad("mainpad","mainpad",0,1-(1.0/(1+ratiospace)),1,1);//top (main) pad
  TPad *coverpad = new TPad("coverpad","coverpad",gStyle->GetPadLeftMargin()-0.008,1-(1.0/(1+ratiospace))-0.04,1,1-(1.0/(1+ratiospace))+0.103);//pad covering up the x scale
  TPad *bottompad = new TPad("bottompad", "Ratio Pad",0,0,1,(1-(1-bottommargin)/(1+ratiospace))-0.015); //bottom pad

  main_canvas->Range(0,0,1,1);
  main_canvas->SetBorderSize(0);
  main_canvas->SetFrameFillColor(0);

  TH1F *haux = new TH1F("haux", "", h1->GetXaxis()->GetNbins(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
  mainpad->Draw();
  mainpad->cd();
  mainpad->Range(0,0,1,1);
  mainpad->SetFillColor(kWhite);
  mainpad->SetBorderSize(0);
  mainpad->SetFrameFillColor(0);
  mainpad->SetLogy(1);
  hs->Draw(s1.c_str());
  h2->Draw(s2.c_str());
  mainpad->Modified();
  main_canvas->cd();
  coverpad->Draw();
  coverpad->cd();
  coverpad->Range(0,0,1,1);
  coverpad->SetFillColor(kWhite);
  coverpad->SetBorderSize(0);
  coverpad->SetFrameFillColor(0);
  coverpad->Modified();
  main_canvas->cd();
  bottompad->SetTopMargin(ratiotopmargin);
  bottompad->SetBottomMargin(ratiobottommargin);
  bottompad->Draw();
  bottompad->cd();
  bottompad->Range(0,0,1,1);
  bottompad->SetFillColor(kWhite);
  TGraphAsymmErrors *eratio = histRatio(h2, h1, 0, false);
  haux->GetYaxis()->SetRangeUser(0, 5);
  haux->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
  haux->Draw();
  eratio->Draw("P");
  bottompad->Modified();

  main_canvas->SaveAs(name.c_str());

  delete haux;
  delete eratio;
  delete bottompad;
  delete coverpad;
  delete mainpad;
  delete main_canvas;

}


void makePlot(TH1F * h1, TH1F * h2, string s1, string s2, string name) {

  float bottommargin=gStyle->GetPadBottomMargin();
  float canvas_height=gStyle->GetCanvasDefH();
  float canvas_width=gStyle->GetCanvasDefW();
  float ratiospace=0.25;// space the ratio should take up (relative to original pad)
  float ratiobottommargin=0.3;
  float ratiotopmargin=0.1;
  float xstretchfactor=((1-ratiospace)*(1-gStyle->GetPadTopMargin()))/((1)*ratiospace);

  TCanvas *main_canvas = new TCanvas("main_canvas","main_canvas",(Int_t)canvas_width,(Int_t)(canvas_height*(1+ratiospace)));
  TPad *mainpad = new TPad("mainpad","mainpad",0,1-(1.0/(1+ratiospace)),1,1);//top (main) pad
  TPad *coverpad = new TPad("coverpad","coverpad",gStyle->GetPadLeftMargin()-0.008,1-(1.0/(1+ratiospace))-0.04,1,1-(1.0/(1+ratiospace))+0.103);//pad covering up the x scale
  TPad *bottompad = new TPad("bottompad", "Ratio Pad",0,0,1,(1-(1-bottommargin)/(1+ratiospace))-0.015); //bottom pad
  TH1F *haux = new TH1F("haux", "", h1->GetXaxis()->GetNbins(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());

  main_canvas->Range(0,0,1,1);
  main_canvas->SetBorderSize(0);
  main_canvas->SetFrameFillColor(0);

  mainpad->Draw();
  mainpad->cd();
  mainpad->Range(0,0,1,1);
  mainpad->SetFillColor(kWhite);
  mainpad->SetBorderSize(0);
  mainpad->SetLogy(1);
  mainpad->SetFrameFillColor(0);
  h1->Draw(s1.c_str());
  h2->Draw(s2.c_str());
  mainpad->Modified();
  main_canvas->cd();
  coverpad->Draw();
  coverpad->cd();
  coverpad->Range(0,0,1,1);
  coverpad->SetFillColor(kWhite);
  coverpad->SetBorderSize(0);
  coverpad->SetFrameFillColor(0);
  coverpad->Modified();
  main_canvas->cd();
  bottompad->SetTopMargin(ratiotopmargin);
  bottompad->SetBottomMargin(ratiobottommargin);
  bottompad->Draw();
  bottompad->cd();
  bottompad->Range(0,0,1,1);
  bottompad->SetFillColor(kWhite);
  TGraphAsymmErrors *eratio = histRatio(h2, h1, 0, false);
  haux->GetYaxis()->SetRangeUser(0, 5);
  haux->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
  haux->Draw();
  eratio->Draw("P");
  bottompad->Modified();

  main_canvas->SaveAs(name.c_str());

  delete haux;
  delete eratio;
  delete bottompad;
  delete coverpad;
  delete mainpad;
  delete main_canvas;


 
}
