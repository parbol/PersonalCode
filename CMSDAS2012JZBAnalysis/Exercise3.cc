#include "TProfile.h"
#include "TF1.h"




void makePrediction(TH1F *, TH1F *, TH1F *, TH1F *);





void exercise3() {


  //----------------First part: Drawing the raw jzb distribution----------------------
  TH1F *jzb_data_sfzp = new TH1F("jzb_data_sfzp", "", 40, -200, 400);
  TH1F *jzb_data_sfsb = new TH1F("jzb_data_sfsb", "", 40, -200, 400);
  TH1F *jzb_data_ofzp = new TH1F("jzb_data_ofzp", "", 40, -200, 400);
  TH1F *jzb_data_ofsb = new TH1F("jzb_data_ofsb", "", 40, -200, 400);

  THStack *jzb_mc_sfzp = new THStack("jzb_mc_sfzp", "");
  THStack *jzb_mc_sfsb = new THStack("jzb_mc_sfsb", "");
  THStack *jzb_mc_ofzp = new THStack("jzb_mc_ofzp", "");
  THStack *jzb_mc_ofsb = new THStack("jzb_mc_ofsb", "");

  //TCut sfzp("id1==id2&&abs(mll-91)<20.0"); 
  //TCut ofzp("id1!=id2&&abs(mll-91)<20.0"); 
  //TCut sfsb("id1==id2&&((mll>55&&mll<70)||(mll>112&&mll<160))"); 
  //TCut ofsb("id1!=id2&&((mll>55&&mll<70)||(mll>112&&mll<160))"); 

  string jzb_data = "jzb[1]+0.055*pt-3.108"; 
  string jzb_mc =   "jzb[1]+0.039*pt-1.920"; 

  //Produce the histograms
  data->Project(jzb_data_sfzp, jzb_data.c_str(), sfzp + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofzp, jzb_data.c_str(), ofzp + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_sfsb, jzb_data.c_str(), sfsb + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofsb, jzb_data.c_str(), ofsb + jets + "passed_triggers", "jzb [GeV]");

  cocktail->Project(jzb_mc_sfzp, jzb_mc.c_str(), sfzp + jets, 40, -200, 400, "jzb [GeV]");
  cocktail->Project(jzb_mc_ofzp, jzb_mc.c_str(), ofzp + jets, 40, -200, 400, "jzb [GeV]");
  cocktail->Project(jzb_mc_sfsb, jzb_mc.c_str(), sfsb + jets, 40, -200, 400, "jzb [GeV]");
  cocktail->Project(jzb_mc_ofsb, jzb_mc.c_str(), ofsb + jets, 40, -200, 400, "jzb [GeV]");


  //Produce the legend
  //makeLegend(mll_data, mll_mc, leg);
  
  TCanvas *can_jzb_regions = new TCanvas("can_jzb_regions");
  can_jzb_regions->Divide(2,2);
  can_jzb_regions->cd(1);
  can_jzb_regions->GetPad(1)->SetLogy(1);
  jzb_mc_sfzp->Draw("HIST");
  jzb_data_sfzp->Draw("SAME");
  drawPrelim(4700, false);
  can_jzb_regions->cd(2);
  can_jzb_regions->GetPad(2)->SetLogy(1);
  jzb_mc_ofzp->Draw("HIST");
  jzb_data_ofzp->Draw("SAME");
  drawPrelim(4700, false);
  can_jzb_regions->cd(3);
  can_jzb_regions->GetPad(3)->SetLogy(1);
  jzb_mc_sfsb->Draw("HIST");
  jzb_data_sfsb->Draw("SAME");
  drawPrelim(4700, false);
  can_jzb_regions->cd(4);
  can_jzb_regions->GetPad(4)->SetLogy(1);
  jzb_mc_ofsb->Draw("HIST");
  jzb_data_ofsb->Draw("SAME");
  drawPrelim(4700, false);
  can_jzb_regions->SaveAs("jzb_regions.png");

  delete can_jzb_regions;
  delete jzb_mc_sfzp;
  delete jzb_mc_ofzp;
  delete jzb_mc_sfsb;
  delete jzb_mc_ofsb;
  delete jzb_data_sfzp;
  delete jzb_data_ofzp;
  delete jzb_data_sfsb;
  delete jzb_data_ofsb;

  //----------------Second part: Building the prediction----------------------
  TH1F *jzb_data_sfzp_pos = new TH1F("jzb_data_sfzp_pos", "", 20, 0, 400);
  TH1F *jzb_data_sfsb_pos = new TH1F("jzb_data_sfsb_pos", "", 20, 0, 400);
  TH1F *jzb_data_ofzp_pos = new TH1F("jzb_data_ofzp_pos", "", 20, 0, 400);
  TH1F *jzb_data_ofsb_pos = new TH1F("jzb_data_ofsb_pos", "", 20, 0, 400);
  TH1F *jzb_mc_sfzp_pos = new TH1F("jzb_mc_sfzp_pos", "", 20, 0, 400);
  TH1F *jzb_mc_sfsb_pos = new TH1F("jzb_mc_sfsb_pos", "", 20, 0, 400);
  TH1F *jzb_mc_ofzp_pos = new TH1F("jzb_mc_ofzp_pos", "", 20, 0, 400);
  TH1F *jzb_mc_ofsb_pos = new TH1F("jzb_mc_ofsb_pos", "", 20, 0, 400);
  TH1F *jzb_mcc_sfzp_pos = new TH1F("jzb_mcc_sfzp_pos", "", 20, 0, 400);
  TH1F *jzb_mcc_sfsb_pos = new TH1F("jzb_mcc_sfsb_pos", "", 20, 0, 400);
  TH1F *jzb_mcc_ofzp_pos = new TH1F("jzb_mcc_ofzp_pos", "", 20, 0, 400);
  TH1F *jzb_mcc_ofsb_pos = new TH1F("jzb_mcc_ofsb_pos", "", 20, 0, 400);
  
  TH1F *jzb_data_sfzp_neg = new TH1F("jzb_data_sfzp_neg", "", 20, 0, 400);
  TH1F *jzb_data_sfsb_neg = new TH1F("jzb_data_sfsb_neg", "", 20, 0, 400);
  TH1F *jzb_data_ofzp_neg = new TH1F("jzb_data_ofzp_neg", "", 20, 0, 400);
  TH1F *jzb_data_ofsb_neg = new TH1F("jzb_data_ofsb_neg", "", 20, 0, 400);
  TH1F *jzb_mc_sfzp_neg = new TH1F("jzb_mc_sfzp_neg", "", 20, 0, 400);
  TH1F *jzb_mc_sfsb_neg = new TH1F("jzb_mc_sfsb_neg", "", 20, 0, 400);
  TH1F *jzb_mc_ofzp_neg = new TH1F("jzb_mc_ofzp_neg", "", 20, 0, 400);
  TH1F *jzb_mc_ofsb_neg = new TH1F("jzb_mc_ofsb_neg", "", 20, 0, 400);
  TH1F *jzb_mcc_sfzp_neg = new TH1F("jzb_mcc_sfzp_neg", "", 20, 0, 400);
  TH1F *jzb_mcc_sfsb_neg = new TH1F("jzb_mcc_sfsb_neg", "", 20, 0, 400);
  TH1F *jzb_mcc_ofzp_neg = new TH1F("jzb_mcc_ofzp_neg", "", 20, 0, 400);
  TH1F *jzb_mcc_ofsb_neg = new TH1F("jzb_mcc_ofsb_neg", "", 20, 0, 400);
  


  string jzb_data_pos = "jzb[1]+0.055*pt-3.108"; 
  string jzb_mc_pos =   "jzb[1]+0.039*pt-1.920"; 
  string jzb_data_neg = "-1.0*(jzb[1]+0.055*pt-3.108)"; 
  string jzb_mc_neg =   "-1.0*(jzb[1]+0.039*pt-1.920)"; 

  string jzb_data_poscut_ = jzb_data_pos + "> 0"; 
  string jzb_data_negcut_ = jzb_data_pos + "< 0"; 
  string jzb_mc_poscut_ = jzb_mc_pos + "> 0"; 
  string jzb_mc_negcut_ = jzb_mc_pos + "< 0"; 

  TCut jzb_data_poscut(jzb_data_poscut_.c_str());
  TCut jzb_data_negcut(jzb_data_negcut_.c_str());
  TCut jzb_mc_poscut(jzb_mc_poscut_.c_str());
  TCut jzb_mc_negcut(jzb_mc_negcut_.c_str());


  //Produce the histograms
  data->Project(jzb_data_sfzp_pos, jzb_data_pos.c_str(), sfzp + jzb_data_poscut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofzp_pos, jzb_data_pos.c_str(), ofzp + jzb_data_poscut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_sfsb_pos, jzb_data_pos.c_str(), sfsb + jzb_data_poscut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofsb_pos, jzb_data_pos.c_str(), ofsb + jzb_data_poscut + jets + "passed_triggers", "jzb [GeV]");
  cocktail->Project(jzb_mc_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mc_ofzp_pos, jzb_mc_pos.c_str(), ofzp + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mc_sfsb_pos, jzb_mc_pos.c_str(), sfsb + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mc_ofsb_pos, jzb_mc_pos.c_str(), ofsb + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_ofzp_pos, jzb_mc_pos.c_str(), ofzp + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_sfsb_pos, jzb_mc_pos.c_str(), sfsb + jzb_mc_poscut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_ofsb_pos, jzb_mc_pos.c_str(), ofsb + jzb_mc_poscut + jets, "jzb [GeV]");
  
  data->Project(jzb_data_sfzp_neg, jzb_data_neg.c_str(), sfzp + jzb_data_negcut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofzp_neg, jzb_data_neg.c_str(), ofzp + jzb_data_negcut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_sfsb_neg, jzb_data_neg.c_str(), sfsb + jzb_data_negcut + jets + "passed_triggers", "jzb [GeV]");
  data->Project(jzb_data_ofsb_neg, jzb_data_neg.c_str(), ofsb + jzb_data_negcut + jets + "passed_triggers", "jzb [GeV]");
  cocktail->Project(jzb_mc_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mc_ofzp_neg, jzb_mc_neg.c_str(), ofzp + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mc_sfsb_neg, jzb_mc_neg.c_str(), sfsb + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktail->Project(jzb_mcc_ofsb_neg, jzb_mc_neg.c_str(), ofsb + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_ofzp_neg, jzb_mc_neg.c_str(), ofzp + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_sfsb_neg, jzb_mc_neg.c_str(), sfsb + jzb_mc_negcut + jets, "jzb [GeV]");
  cocktails->Project(jzb_mcc_ofsb_neg, jzb_mc_neg.c_str(), ofsb + jzb_mc_negcut + jets, "jzb [GeV]");

  
  //Predictions
  TH1F *data_ofzp_pre = new TH1F("data_ofzp_pre", "", 20, 0, 400);
  TH1F *data_sfsb_pre = new TH1F("data_sfsb_pre", "", 20, 0, 400);
  TH1F *data_ofsb_pre = new TH1F("data_ofsb_pre", "", 20, 0, 400);
  TH1F *final_data_prediction = new TH1F("final_data_prediction", "", 20, 0, 400);
  
  TH1F *mc_ofzp_pre = new TH1F("mc_ofzp_pre", "", 20, 0, 400);
  TH1F *mc_sfsb_pre = new TH1F("mc_sfsb_pre", "", 20, 0, 400);
  TH1F *mc_ofsb_pre = new TH1F("mc_ofsb_pre", "", 20, 0, 400);
  TH1F *final_mc_prediction = new TH1F("final_mc_prediction", "", 20, 0, 400);
  TH1F *mcc_ofzp_pre = new TH1F("mcc_ofzp_pre", "", 20, 0, 400);
  TH1F *mcc_sfsb_pre = new TH1F("mcc_sfsb_pre", "", 20, 0, 400);
  TH1F *mcc_ofsb_pre = new TH1F("mcc_ofsb_pre", "", 20, 0, 400);
  TH1F *final_mcc_prediction = new TH1F("final_mcc_prediction", "", 20, 0, 400);


  makePrediction(data_ofzp_pre, jzb_data_sfzp_neg, jzb_data_ofzp_neg, jzb_data_ofzp_pos);
  makePrediction(data_sfsb_pre, jzb_data_sfzp_neg, jzb_data_sfsb_neg, jzb_data_sfsb_pos);
  makePrediction(data_ofsb_pre, jzb_data_sfzp_neg, jzb_data_ofsb_neg, jzb_data_ofsb_pos);
  
  makePrediction(mc_ofzp_pre, jzb_mc_sfzp_neg, jzb_mc_ofzp_neg, jzb_mc_ofzp_pos);
  makePrediction(mc_sfsb_pre, jzb_mc_sfzp_neg, jzb_mc_sfsb_neg, jzb_mc_sfsb_pos);
  makePrediction(mc_ofsb_pre, jzb_mc_sfzp_neg, jzb_mc_ofsb_neg, jzb_mc_ofsb_pos);
  makePrediction(mcc_ofzp_pre, jzb_mcc_sfzp_neg, jzb_mcc_ofzp_neg, jzb_mcc_ofzp_pos);
  makePrediction(mcc_sfsb_pre, jzb_mcc_sfzp_neg, jzb_mcc_sfsb_neg, jzb_mcc_sfsb_pos);
  makePrediction(mcc_ofsb_pre, jzb_mcc_sfzp_neg, jzb_mcc_ofsb_neg, jzb_mcc_ofsb_pos);

  final_data_prediction->GetXaxis()->SetTitle(data_ofzp_pre->GetTitle());
  final_data_prediction->Add(data_ofzp_pre, 1.0/3.0);
  final_data_prediction->Add(data_sfsb_pre, 1.0/3.0);
  final_data_prediction->Add(data_ofsb_pre, 1.0/3.0);
  

  final_mc_prediction->GetXaxis()->SetTitle(mc_ofzp_pre->GetTitle());
  final_mc_prediction->Add(mc_ofzp_pre, 1.0/3.0);
  final_mc_prediction->Add(mc_sfsb_pre, 1.0/3.0);
  final_mc_prediction->Add(mc_ofsb_pre, 1.0/3.0);
  
  final_mcc_prediction->GetXaxis()->SetTitle(mcc_ofzp_pre->GetTitle());
  final_mcc_prediction->Add(mcc_ofzp_pre, 1.0/3.0);
  final_mcc_prediction->Add(mcc_sfsb_pre, 1.0/3.0);
  final_mcc_prediction->Add(mcc_ofsb_pre, 1.0/3.0);


  TCanvas *can_pred_data = new TCanvas("can_pred_data");
  can_pred_data->Divide(2,1);
  can_pred_data->cd(1);
  can_pred_data->GetPad(1)->SetLogy(1);
  data_ofzp_pre->SetLineColor(kRed);
  data_ofsb_pre->SetLineColor(kBlue);
  data_sfsb_pre->SetLineColor(kGreen);
  data_ofzp_pre->Draw("HIST");
  data_ofsb_pre->Draw("SAME,HIST");
  data_sfsb_pre->Draw("SAME,HIST");
  jzb_data_sfzp_pos->Draw("SAME,E1");
  can_pred_data->cd(2);
  can_pred_data->GetPad(2)->SetLogy(1);
  final_data_prediction->Draw("HIST");
  jzb_data_sfzp_pos->Draw("SAME,E1");
  can_pred_data->SaveAs("jzb_pred_data.png");
  

  TCanvas *can_pred_mc = new TCanvas("can_pred_mc");
  can_pred_mc->GetPad(0)->SetLogy(1);
  can_pred_mc->Divide(2,1);
  can_pred_mc->cd(1);
  can_pred_mc->GetPad(1)->SetLogy(1);
  mc_ofzp_pre->Draw("HIST");
  mc_ofsb_pre->Draw("SAME,HIST");
  mc_sfsb_pre->Draw("SAME,HIST");
  jzb_mc_sfzp_pos->Draw("SAME,E1");
  can_pred_mc->cd(2);
  can_pred_mc->GetPad(2)->SetLogy(1);
  final_mc_prediction->Draw("HIST");
  jzb_mc_sfzp_pos->Draw("SAME");
  can_pred_mc->SaveAs("jzb_pred_mc.png");


  //----------------Third part: Closure test----------------------


  makePlot(final_mc_prediction, jzb_mc_sfzp_pos, "HIST", "E1,SAME", "jzb_closureTestMC.png");
  makePlot(final_mcc_prediction, jzb_mcc_sfzp_pos, "HIST", "E1,SAME", "jzb_closureTestMCS.png");

  delete can_pred_mc;
  delete data_ofzp_pre;
  delete data_sfsb_pre;
  delete data_ofsb_pre;
  delete final_data_prediction;
  delete mc_ofzp_pre;
  delete mc_sfsb_pre;
  delete mc_ofsb_pre;
  delete final_mc_prediction;
  delete mcc_ofzp_pre;
  delete mcc_sfsb_pre;
  delete mcc_ofsb_pre;
  delete final_mcc_prediction;
  delete jzb_data_sfzp_pos;
  delete jzb_data_sfsb_pos;
  delete jzb_data_ofzp_pos;
  delete jzb_data_ofsb_pos;
  delete jzb_mc_sfzp_pos;
  delete jzb_mc_sfsb_pos;
  delete jzb_mc_ofzp_pos;
  delete jzb_mc_ofsb_pos;
  delete jzb_mcc_sfzp_pos;
  delete jzb_mcc_sfsb_pos;
  delete jzb_mcc_ofzp_pos;
  delete jzb_mcc_ofsb_pos;
  delete jzb_data_sfzp_neg;
  delete jzb_data_sfsb_neg;
  delete jzb_data_ofzp_neg;
  delete jzb_data_ofsb_neg;
  delete jzb_mc_sfzp_neg;
  delete jzb_mc_sfsb_neg;
  delete jzb_mc_ofzp_neg;
  delete jzb_mc_ofsb_neg;
  delete jzb_mcc_sfzp_neg;
  delete jzb_mcc_sfsb_neg;
  delete jzb_mcc_ofzp_neg;
  delete jzb_mcc_ofsb_neg;


 


 
}



void makePrediction(TH1F *final, TH1F *z, TH1F *ofminus, TH1F *ofplus) {



  final->Add(z);
  final->Add(ofplus);
  final->Add(ofminus, -1.0);
  final->GetXaxis()->SetTitle(z->GetXaxis()->GetTitle());


}








