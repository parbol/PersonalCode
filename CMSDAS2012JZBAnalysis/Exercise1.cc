


void exercise1() {



  //----------------First part: Drawing the mll distribution----------------------
  TH1F *data_mll_ee = new TH1F("data_mll_ee", "", 40, 60, 300);
  TH1F *data_mll_mm = new TH1F("data_mll_mm", "", 40, 60, 300);
  TH1F *data_mll_em = new TH1F("data_mll_em", "", 40, 60, 300);
  
  TH1F *data_mll_ee_inclusive = new TH1F("data_mll_ee_inclusive", "", 40, 60, 300);
  TH1F *data_mll_mm_inclusive = new TH1F("data_mll_mm_inclusive", "", 40, 60, 300);
  TH1F *data_mll_em_inclusive = new TH1F("data_mll_em_inclusive", "", 40, 60, 300);

  TH1F *data_njets_ee = new TH1F("data_njets_ee", "", 10, 0, 10);
  TH1F *data_njets_mm = new TH1F("data_njets_mm", "", 10, 0, 10);
  TH1F *data_njets_em = new TH1F("data_njets_em", "", 10, 0, 10);


  THStack *mc_mll_ee = new THStack("mc_mll_ee", "");
  THStack *mc_mll_mm = new THStack("mc_mll_mm", "");
  THStack *mc_mll_em = new THStack("mc_mll_em", "");

  THStack *mc_mll_ee_inclusive = new THStack("mc_mll_ee_inclusive", "");
  THStack *mc_mll_mm_inclusive = new THStack("mc_mll_mm_inclusive", "");
  THStack *mc_mll_em_inclusive = new THStack("mc_mll_em_inclusive", "");

  THStack *mc_njets_ee = new THStack("mc_njets_ee", "");
  THStack *mc_njets_mm = new THStack("mc_njets_mm", "");
  THStack *mc_njets_em = new THStack("mc_njets_em", "");


  TH1F *mc_mll_eer = new TH1F("mc_mll_eer", "", 40, 50, 300);
  TH1F *mc_mll_mmr = new TH1F("mc_mll_mmr", "", 40, 50, 300);
  TH1F *mc_mll_emr = new TH1F("mc_mll_emr", "", 40, 50, 300);
  
  TH1F *mc_mll_ee_inclusiver = new TH1F("mc_mll_ee_inclusiver", "", 40, 60, 300);
  TH1F *mc_mll_mm_inclusiver = new TH1F("mc_mll_mm_inclusiver", "", 40, 60, 300);
  TH1F *mc_mll_em_inclusiver = new TH1F("mc_mll_em_inclusiver", "", 40, 60, 300);

  TH1F *mc_njets_eer = new TH1F("mc_njets_eer", "", 10, 0, 10);
  TH1F *mc_njets_mmr = new TH1F("mc_njets_mmr", "", 10, 0, 10);
  TH1F *mc_njets_emr = new TH1F("mc_njets_emr", "", 10, 0, 10);

  TCut jets("(pfJetGoodNum>2&&pfJetGoodID[0]!=0)&&(pfJetGoodNum>2&&pfJetGoodID[1]!=0)&&(pfJetGoodNum>2&&pfJetGoodID[2]!=0)");

  //Produce the histograms
  data->Project(data_mll_ee, "mll", "id1==id2 && id1==0 && mll>50 && passed_triggers" + jets, "mll [GeV]");
  showMessage("Producing");
  data->Project(data_mll_mm, "mll", "id1==id2 && id1==1 && mll>50 && passed_triggers" + jets, "mll [GeV]");
  showMessage("Producing");
  data->Project(data_mll_em, "mll", "id1!=id2 && mll>50 && passed_triggers" + jets, "mll [GeV]");
  showMessage("Producing");
  data->Project(data_mll_ee_inclusive, "mll", "id1==id2 && id1==0 && mll>50 && passed_triggers", "mll [GeV]");
  showMessage("Producing");
  data->Project(data_mll_mm_inclusive, "mll", "id1==id2 && id1==1 && mll>50 && passed_triggers", "mll [GeV]");
  showMessage("Producing");
  data->Project(data_mll_em_inclusive, "mll", "id1!=id2 && mll>50 && passed_triggers", "mll [GeV]");
  showMessage("Producing");
  data->Project(data_njets_ee, "pfJetGoodNum", "id1==id2 && id1==0 && mll>50&&passed_triggers", "mll [GeV]");
  showMessage("Producing");
  data->Project(data_njets_mm, "pfJetGoodNum", "id1==id2 && id1==1 && mll>50&&passed_triggers", "mll [GeV]");
  showMessage("Producing");
  data->Project(data_njets_em, "pfJetGoodNum", "id1!=id2 && mll>50 && passed_triggers", "mll [GeV]");
  showMessage("Producing");

  cocktail->Project(mc_mll_ee, "mll", "id1==id2 && id1==0 && mll>50" + jets, 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_mm, "mll", "id1==id2 && id1==1 && mll>50" + jets, 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_em, "mll", "id1!=id2 && mll>50" + jets, 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_ee_inclusive, "mll", "id1==id2 && id1==0 && mll>50", 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_mm_inclusive, "mll", "id1==id2 && id1==1 && mll>50", 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_em_inclusive, "mll", "id1!=id2 && mll>50", 40, 60, 300, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_ee, "pfJetGoodNum", "id1==id2 && id1==0 && mll>50", 10, 0, 10, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_mm, "pfJetGoodNum", "id1==id2 && id1==1 && mll>50", 10, 0, 10, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_em, "pfJetGoodNum", "id1!=id2 && mll>50", 10, 0, 10, "mll [GeV]");
  showMessage("Producing");

  cocktail->Project(mc_mll_eer, "mll", "id1==id2 && id1==0 && mll>50" + jets, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_mmr, "mll", "id1==id2 && id1==1 && mll>50" + jets, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_emr, "mll", "id1!=id2 && mll>50" + jets, "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_ee_inclusiver, "mll", "id1==id2 && id1==0 && mll>50", "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_mm_inclusiver, "mll", "id1==id2 && id1==1 && mll>50", "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_mll_em_inclusiver, "mll", "id1!=id2 && mll>50", "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_eer, "pfJetGoodNum", "id1==id2 && id1==0&&mll>50", "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_mmr, "pfJetGoodNum", "id1==id2 && id1==1&&mll>50", "mll [GeV]");
  showMessage("Producing");
  cocktail->Project(mc_njets_emr, "pfJetGoodNum", "id1!=id2 && mll>50", "mll [GeV]");
  showMessage("Producing");


  makePlot(mc_mll_eer, data_mll_ee, mc_mll_ee, "HIST", "E1,SAME", "mll_ee.png");
  makePlot(mc_mll_mmr, data_mll_mm, mc_mll_mm, "HIST", "E1,SAME", "mll_mm.png");
  makePlot(mc_mll_emr, data_mll_em, mc_mll_em, "HIST", "E1,SAME", "mll_em.png");
  
  makePlot(mc_mll_ee_inclusiver, data_mll_ee_inclusive, mc_mll_ee_inclusive, "HIST", "E1,SAME", "mll_ee_inclusive.png");
  makePlot(mc_mll_mm_inclusiver, data_mll_mm_inclusive, mc_mll_mm_inclusive, "HIST", "E1,SAME", "mll_mm_inclusive.png");
  makePlot(mc_mll_em_inclusiver, data_mll_em_inclusive, mc_mll_em_inclusive, "HIST", "E1,SAME", "mll_em_inclusive.png");


  makePlot(mc_njets_eer, data_njets_ee, mc_njets_ee, "HIST", "E1,SAME", "njets_ee.png");
  makePlot(mc_njets_mmr, data_njets_mm, mc_njets_mm, "HIST", "E1,SAME", "njets_mm.png");
  makePlot(mc_njets_emr, data_njets_em, mc_njets_em, "HIST", "E1,SAME", "njets_em.png");



  delete mc_mll_eer;
  delete mc_mll_mmr;
  delete mc_mll_emr;
  delete mc_mll_ee;
  delete mc_mll_mm;
  delete mc_mll_em;
  
  delete mc_mll_ee_inclusiver;
  delete mc_mll_mm_inclusiver;
  delete mc_mll_em_inclusiver;
  delete mc_mll_ee_inclusive;
  delete mc_mll_mm_inclusive;
  delete mc_mll_em_inclusive;

  delete mc_njets_eer;
  delete mc_njets_mmr;
  delete mc_njets_emr;
  delete mc_njets_ee;
  delete mc_njets_mm;
  delete mc_njets_em;

}



