





void exercise4() {


  //----------------First part: Systematics of Z+jets prediction----------------------
  TH1F *jzb_zjets_sfzp_pos = new TH1F("jzb_zjets_sfzp_pos", "", 6, 0, 100);
  TH1F *jzb_zjets_sfzp_neg = new TH1F("jzb_zjets_sfzp_neg", "", 6, 0, 100);


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
  zjets2->Project(jzb_zjets_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
  zjets2->Project(jzb_zjets_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + jets, "jzb [GeV]");


  TCanvas *mierda = new TCanvas("mierda");
  mierda->Divide(2,1);
  mierda->cd(1);
  jzb_zjets_sfzp_pos->Draw("HIST");
  mierda->cd(2);
  jzb_zjets_sfzp_neg->Draw("HIST");
  mierda->SaveAs("mierda.png");


  TH1F *aux = new TH1F("auxiliar", "", 6, 0, 100);
  TH1F *aux2 = new TH1F("auxiliar2", "", 10, 0, 120);
  TGraphAsymmErrors *zjets_systematics =  histRatio(jzb_zjets_sfzp_pos, jzb_zjets_sfzp_neg, 0, false);
  
  TCanvas *can_zjets_systematics = new TCanvas("can_zjets_systematics");
  can_zjets_systematics->cd();
  aux->GetYaxis()->SetRangeUser(0, 3);
  aux->Draw();
  zjets_systematics->Draw("P");
  can_zjets_systematics->SaveAs("zjetsystematics.png");


  delete can_zjets_systematics;
  delete zjets_systematics;
  delete jzb_zjets_sfzp_pos;
  delete jzb_zjets_sfzp_neg;



  //----------------Second part: Systematics for ttbar prediction----------------------
  TH1F *jzb_ttbar_sfzp_pos = new TH1F("jzb_ttbar_sfzp_pos", "", 10, 0, 120);
  TH1F *jzb_ttbar_ofzp_pos = new TH1F("jzb_ttbar_ofzp_pos", "", 10, 0, 120);
  TH1F *jzb_ttbar_sfsb_pos = new TH1F("jzb_ttbar_sfsb_pos", "", 10, 0, 120);
  TH1F *jzb_ttbar_ofsb_pos = new TH1F("jzb_ttbar_ofsb_pos", "", 10, 0, 120);


  ttbar->Project(jzb_ttbar_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
  ttbar->Project(jzb_ttbar_ofzp_pos, jzb_mc_pos.c_str(), ofzp + jzb_mc_poscut + jets, "jzb [GeV]");
  ttbar->Project(jzb_ttbar_sfsb_pos, jzb_mc_pos.c_str(), sfsb + jzb_mc_poscut + jets, "jzb [GeV]");
  ttbar->Project(jzb_ttbar_ofsb_pos, jzb_mc_pos.c_str(), ofsb + jzb_mc_poscut + jets, "jzb [GeV]");
  
  
  TGraphAsymmErrors *ttbar_systematics_ofzp =  histRatio(jzb_ttbar_ofzp_pos, jzb_ttbar_sfzp_pos, 0, false);
  TGraphAsymmErrors *ttbar_systematics_sfsb =  histRatio(jzb_ttbar_sfsb_pos, jzb_ttbar_sfzp_pos, 0, false);
  TGraphAsymmErrors *ttbar_systematics_ofsb =  histRatio(jzb_ttbar_ofsb_pos, jzb_ttbar_sfzp_pos, 0, false);

  TCanvas *can_ttbar_systematics = new TCanvas("can_ttbar_systematics");
  can_ttbar_systematics->cd();
  aux2->GetYaxis()->SetRangeUser(0, 3);
  aux2->Draw();
  ttbar_systematics_ofzp->SetLineColor(kRed);
  ttbar_systematics_sfsb->SetLineColor(kBlue);
  ttbar_systematics_ofsb->SetLineColor(kGreen);
  ttbar_systematics_ofzp->Draw("P");
  ttbar_systematics_sfsb->Draw("P");
  ttbar_systematics_ofsb->Draw("P");
  can_ttbar_systematics->SaveAs("ttbarsystematics.png");


  delete can_ttbar_systematics;
  delete ttbar_systematics_ofzp;
  delete ttbar_systematics_sfsb;
  delete ttbar_systematics_ofsb;
  delete jzb_ttbar_sfzp_pos;
  delete jzb_ttbar_ofzp_pos;
  delete jzb_ttbar_sfsb_pos;
  delete jzb_ttbar_ofsb_pos;
  delete aux;
  delete aux2;



  
}







