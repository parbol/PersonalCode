


void printEffInfo(string, float, float, float, float, float, float, float, float, float, float, float, string); 


void exercise6() {


  string searchRegionsm[] = {"45", "90", "135", "180", "225"};
  string searchRegionsp[] = {"55", "110", "165", "220", "275"};
  string searchRegions[] = {"50", "100", "150", "200", "250"};

  for(int i = 0; i < 5; ++i) {
    //----------------First part: Calculating the efficiency----------------------
    TH1F *jzb_lm4_sfzp_pos = new TH1F("jzb_lm4_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_lm4_sfzp_neg = new TH1F("jzb_lm4_sfzp_neg", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_pos = new TH1F("jzb_lm8_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_neg = new TH1F("jzb_lm8_sfzp_neg", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_pos = new TH1F("jzb_sms1_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_neg = new TH1F("jzb_sms1_sfzp_neg", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_pos = new TH1F("jzb_sms2_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_neg = new TH1F("jzb_sms2_sfzp_neg", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_pos = new TH1F("jzb_sms3_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_neg = new TH1F("jzb_sms3_sfzp_neg", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_pos = new TH1F("jzb_sms4_sfzp_pos", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_neg = new TH1F("jzb_sms4_sfzp_neg", "", 20, 0, 400);
  
 
    TH1F *jzb_lm4_sfzp_pospjzb = new TH1F("jzb_lm4_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_lm4_sfzp_negpjzb = new TH1F("jzb_lm4_sfzp_negpjzb", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_pospjzb = new TH1F("jzb_lm8_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_negpjzb = new TH1F("jzb_lm8_sfzp_negpjzb", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_pospjzb = new TH1F("jzb_sms1_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_negpjzb = new TH1F("jzb_sms1_sfzp_negpjzb", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_pospjzb = new TH1F("jzb_sms2_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_negpjzb = new TH1F("jzb_sms2_sfzp_negpjzb", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_pospjzb = new TH1F("jzb_sms3_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_negpjzb = new TH1F("jzb_sms3_sfzp_negpjzb", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_pospjzb = new TH1F("jzb_sms4_sfzp_pospjzb", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_negpjzb = new TH1F("jzb_sms4_sfzp_negpjzb", "", 20, 0, 400);

    TH1F *jzb_lm4_sfzp_posnjzb = new TH1F("jzb_lm4_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_lm4_sfzp_negnjzb = new TH1F("jzb_lm4_sfzp_negnjzb", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_posnjzb = new TH1F("jzb_lm8_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_negnjzb = new TH1F("jzb_lm8_sfzp_negnjzb", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_posnjzb = new TH1F("jzb_sms1_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_negnjzb = new TH1F("jzb_sms1_sfzp_negnjzb", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_posnjzb = new TH1F("jzb_sms2_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_negnjzb = new TH1F("jzb_sms2_sfzp_negnjzb", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_posnjzb = new TH1F("jzb_sms3_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_negnjzb = new TH1F("jzb_sms3_sfzp_negnjzb", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_posnjzb = new TH1F("jzb_sms4_sfzp_posnjzb", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_negnjzb = new TH1F("jzb_sms4_sfzp_negnjzb", "", 20, 0, 400);

    TH1F *jzb_lm4_sfzp_pospjes = new TH1F("jzb_lm4_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_lm4_sfzp_negpjes = new TH1F("jzb_lm4_sfzp_negpjes", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_pospjes = new TH1F("jzb_lm8_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_negpjes = new TH1F("jzb_lm8_sfzp_negpjes", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_pospjes = new TH1F("jzb_sms1_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_negpjes = new TH1F("jzb_sms1_sfzp_negpjes", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_pospjes = new TH1F("jzb_sms2_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_negpjes = new TH1F("jzb_sms2_sfzp_negpjes", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_pospjes = new TH1F("jzb_sms3_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_negpjes = new TH1F("jzb_sms3_sfzp_negpjes", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_pospjes = new TH1F("jzb_sms4_sfzp_pospjes", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_negpjes = new TH1F("jzb_sms4_sfzp_negpjes", "", 20, 0, 400);

    TH1F *jzb_lm4_sfzp_posnjes = new TH1F("jzb_lm4_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_lm4_sfzp_negnjes = new TH1F("jzb_lm4_sfzp_negnjes", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_posnjes = new TH1F("jzb_lm8_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_lm8_sfzp_negnjes = new TH1F("jzb_lm8_sfzp_negnjes", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_posnjes = new TH1F("jzb_sms1_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_sms1_sfzp_negnjes = new TH1F("jzb_sms1_sfzp_negnjes", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_posnjes = new TH1F("jzb_sms2_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_sms2_sfzp_negnjes = new TH1F("jzb_sms2_sfzp_negnjes", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_posnjes = new TH1F("jzb_sms3_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_sms3_sfzp_negnjes = new TH1F("jzb_sms3_sfzp_negnjes", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_posnjes = new TH1F("jzb_sms4_sfzp_posnjes", "", 20, 0, 400);
    TH1F *jzb_sms4_sfzp_negnjes = new TH1F("jzb_sms4_sfzp_negnjes", "", 20, 0, 400);

    string jzb_data_pos = "jzb[1]";
    string jzb_mc_pos = "jzb[1]";
    string jzb_data_neg = "-1.0*(jzb[1])";
    string jzb_mc_neg = "-1.0*(jzb[1])";

    string jzb_data_poscut_ = jzb_data_pos + "> " + searchRegions[i]; 
    string jzb_data_negcut_ = jzb_data_pos + "< -" + searchRegions[i]; 
    string jzb_mc_poscut_ = jzb_mc_pos + "> " + searchRegions[i]; 
    string jzb_mc_negcut_ = jzb_mc_pos + "< -" + searchRegions[i]; 
    string jzb_data_poscutp_ = jzb_data_pos + "> " + searchRegionsp[i]; 
    string jzb_data_negcutp_ = jzb_data_pos + "< -" + searchRegionsp[i]; 
    string jzb_mc_poscutp_ = jzb_mc_pos + "> " + searchRegionsp[i]; 
    string jzb_mc_negcutp_ = jzb_mc_pos + "< -" + searchRegionsp[i]; 
    string jzb_data_poscutm_ = jzb_data_pos + "> " + searchRegionsm[i]; 
    string jzb_data_negcutm_ = jzb_data_pos + "< -" + searchRegionsm[i]; 
    string jzb_mc_poscutm_ = jzb_mc_pos + "> " + searchRegionsm[i]; 
    string jzb_mc_negcutm_ = jzb_mc_pos + "< -" + searchRegionsm[i]; 


    TCut jzb_data_poscut(jzb_data_poscut_.c_str());
    TCut jzb_data_negcut(jzb_data_negcut_.c_str());
    TCut jzb_mc_poscut(jzb_mc_poscut_.c_str());
    TCut jzb_mc_negcut(jzb_mc_negcut_.c_str());
    TCut jzb_data_poscutp(jzb_data_poscutp_.c_str());
    TCut jzb_data_negcutp(jzb_data_negcutp_.c_str());
    TCut jzb_mc_poscutp(jzb_mc_poscutp_.c_str());
    TCut jzb_mc_negcutp(jzb_mc_negcutp_.c_str());
    TCut jzb_data_poscutn(jzb_data_poscutm_.c_str());
    TCut jzb_data_negcutn(jzb_data_negcutm_.c_str());
    TCut jzb_mc_poscutn(jzb_mc_poscutm_.c_str());
    TCut jzb_mc_negcutn(jzb_mc_negcutm_.c_str());


    //Produce the histograms
    lm4eff->Project(jzb_lm4_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
    lm4eff->Project(jzb_lm4_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + jets, "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_pos, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_neg, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");

 
    lm4eff->Project(jzb_lm4_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp + jets, "jzb [GeV]");
    lm4eff->Project(jzb_lm4_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp + jets, "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_pospjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutp +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_negpjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutp +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");

  
    lm4eff->Project(jzb_lm4_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn + jets, "jzb [GeV]");
    lm4eff->Project(jzb_lm4_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn + jets, "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn + jets, "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn +  jets + " MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn +  jets + " MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn +  jets + " MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_posnjzb, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscutn +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_negnjzb, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcutn +  jets + " MassGlu==250&&MassLSP==75", "jzb [GeV]");


    lm4eff->Project(jzb_lm4_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2", "jzb [GeV]");
    lm4eff->Project(jzb_lm4_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2", "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2", "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2 && MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2 && MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2 && MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2 && MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2 && MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2 && MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_pospjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNump1sigma > 2 && MassGlu==250&&MassLSP==75", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_negpjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNump1sigma > 2 && MassGlu==250&&MassLSP==75", "jzb [GeV]");


    lm4eff->Project(jzb_lm4_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2", "jzb [GeV]"); 
    lm4eff->Project(jzb_lm4_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2", "jzb [GeV]"); 
    lm8eff->Project(jzb_lm8_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2", "jzb [GeV]");
    lm8eff->Project(jzb_lm8_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2 && MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms1->Project(jzb_sms1_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2 && MassGlu==1200&&MassLSP==100", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2 && MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms2->Project(jzb_sms2_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2 && MassGlu==1050&&MassLSP==850", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2 && MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms3->Project(jzb_sms3_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2 && MassGlu==800&&MassLSP==200", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_posnjes, jzb_mc_pos.c_str(), sfzp + jzb_mc_poscut + "pfJetGoodNumn1sigma > 2 && MassGlu==250&&MassLSP==75", "jzb [GeV]");
    sms4->Project(jzb_sms4_sfzp_negnjes, jzb_mc_neg.c_str(), sfzp + jzb_mc_negcut + "pfJetGoodNumn1sigma > 2 && MassGlu==250&&MassLSP==75", "jzb [GeV]");


    float lm4p = calculate_yields(jzb_lm4_sfzp_pos);
    float lm4n = calculate_yields(jzb_lm4_sfzp_neg);
    float lm8p = calculate_yields(jzb_lm8_sfzp_pos);
    float lm8n = calculate_yields(jzb_lm8_sfzp_neg);
    float sms1p = calculate_yields(jzb_sms1_sfzp_pos);
    float sms1n = calculate_yields(jzb_sms1_sfzp_neg);
    float sms2p = calculate_yields(jzb_sms2_sfzp_pos);
    float sms2n = calculate_yields(jzb_sms2_sfzp_neg);
    float sms3p = calculate_yields(jzb_sms3_sfzp_pos);
    float sms3n = calculate_yields(jzb_sms3_sfzp_neg);
    float sms4p = calculate_yields(jzb_sms4_sfzp_pos);
    float sms4n = calculate_yields(jzb_sms4_sfzp_neg);

    float lm4ppjes = calculate_yields(jzb_lm4_sfzp_pospjes);
    float lm4npjes = calculate_yields(jzb_lm4_sfzp_negpjes);
    float lm8ppjes = calculate_yields(jzb_lm8_sfzp_pospjes);
    float lm8npjes = calculate_yields(jzb_lm8_sfzp_negpjes);
    float sms1ppjes = calculate_yields(jzb_sms1_sfzp_pospjes);
    float sms1npjes = calculate_yields(jzb_sms1_sfzp_negpjes);
    float sms2ppjes = calculate_yields(jzb_sms2_sfzp_pospjes);
    float sms2npjes = calculate_yields(jzb_sms2_sfzp_negpjes);
    float sms3ppjes = calculate_yields(jzb_sms3_sfzp_pospjes);
    float sms3npjes = calculate_yields(jzb_sms3_sfzp_negpjes);
    float sms4ppjes = calculate_yields(jzb_sms4_sfzp_pospjes);
    float sms4npjes = calculate_yields(jzb_sms4_sfzp_negpjes);


    float lm4pnjes = calculate_yields(jzb_lm4_sfzp_posnjes);
    float lm4nnjes = calculate_yields(jzb_lm4_sfzp_negnjes);
    float lm8pnjes = calculate_yields(jzb_lm8_sfzp_posnjes);
    float lm8nnjes = calculate_yields(jzb_lm8_sfzp_negnjes);
    float sms1pnjes = calculate_yields(jzb_sms1_sfzp_posnjes);
    float sms1nnjes = calculate_yields(jzb_sms1_sfzp_negnjes);
    float sms2pnjes = calculate_yields(jzb_sms2_sfzp_posnjes);
    float sms2nnjes = calculate_yields(jzb_sms2_sfzp_negnjes);
    float sms3pnjes = calculate_yields(jzb_sms3_sfzp_posnjes);
    float sms3nnjes = calculate_yields(jzb_sms3_sfzp_negnjes);
    float sms4pnjes = calculate_yields(jzb_sms4_sfzp_posnjes);
    float sms4nnjes = calculate_yields(jzb_sms4_sfzp_negnjes);

    float lm4ppjzb = calculate_yields(jzb_lm4_sfzp_pospjzb);
    float lm4npjzb = calculate_yields(jzb_lm4_sfzp_negpjzb);
    float lm8ppjzb = calculate_yields(jzb_lm8_sfzp_pospjzb);
    float lm8npjzb = calculate_yields(jzb_lm8_sfzp_negpjzb);
    float sms1ppjzb = calculate_yields(jzb_sms1_sfzp_pospjzb);
    float sms1npjzb = calculate_yields(jzb_sms1_sfzp_negpjzb);
    float sms2ppjzb = calculate_yields(jzb_sms2_sfzp_pospjzb);
    float sms2npjzb = calculate_yields(jzb_sms2_sfzp_negpjzb);
    float sms3ppjzb = calculate_yields(jzb_sms3_sfzp_pospjzb);
    float sms3npjzb = calculate_yields(jzb_sms3_sfzp_negpjzb);
    float sms4ppjzb = calculate_yields(jzb_sms4_sfzp_pospjzb);
    float sms4npjzb = calculate_yields(jzb_sms4_sfzp_negpjzb);

    float lm4pnjzb = calculate_yields(jzb_lm4_sfzp_posnjzb);
    float lm4nnjzb = calculate_yields(jzb_lm4_sfzp_negnjzb);
    float lm8pnjzb = calculate_yields(jzb_lm8_sfzp_posnjzb);
    float lm8nnjzb = calculate_yields(jzb_lm8_sfzp_negnjzb);
    float sms1pnjzb = calculate_yields(jzb_sms1_sfzp_posnjzb);
    float sms1nnjzb = calculate_yields(jzb_sms1_sfzp_negnjzb);
    float sms2pnjzb = calculate_yields(jzb_sms2_sfzp_posnjzb);
    float sms2nnjzb = calculate_yields(jzb_sms2_sfzp_negnjzb);
    float sms3pnjzb = calculate_yields(jzb_sms3_sfzp_posnjzb);
    float sms3nnjzb = calculate_yields(jzb_sms3_sfzp_negnjzb);
    float sms4pnjzb = calculate_yields(jzb_sms4_sfzp_posnjzb);
    float sms4nnjzb = calculate_yields(jzb_sms4_sfzp_negnjzb);



    printEffInfo("LM4", lm4eff->getNumberOfEvents(), lm4p, lm4n, lm4ppjzb, lm4pnjzb, lm4npjzb, lm4nnjzb, lm4ppjes, lm4pnjes, lm4npjes, lm4nnjes, searchRegions[i]);
    printEffInfo("LM8", lm8eff->getNumberOfEvents(), lm8p, lm8n, lm8ppjzb, lm8pnjzb, lm8npjzb, lm8nnjzb, lm8ppjes, lm8pnjes, lm8npjes, lm8nnjes,  searchRegions[i]);
    printEffInfo("SMS1", 20000, sms1p, sms1n, sms1ppjzb, sms1pnjzb, sms1npjzb, sms1nnjzb, sms1ppjes, sms1pnjes, sms1npjes, sms1nnjes,  searchRegions[i]);
    printEffInfo("SMS2", 20000, sms2p, sms2n, sms2ppjzb, sms2pnjzb, sms2npjzb, sms2nnjzb, sms2ppjes, sms2pnjes, sms2npjes, sms2nnjes,  searchRegions[i]);
    printEffInfo("SMS3", 20000, sms3p, sms3n, sms3ppjzb, sms3pnjzb, sms3npjzb, sms3nnjzb, sms3ppjes, sms3pnjes, sms3npjes, sms3nnjes,  searchRegions[i]);
    printEffInfo("SMS4", 20000, sms4p, sms4n, sms4ppjzb, sms4pnjzb, sms4npjzb, sms4nnjzb, sms4ppjes, sms4pnjes, sms4npjes, sms4nnjes, searchRegions[i]);
  



    delete jzb_lm4_sfzp_pos;
    delete jzb_lm4_sfzp_neg;
    delete jzb_lm8_sfzp_pos;
    delete jzb_lm8_sfzp_neg;
    delete jzb_sms1_sfzp_pos;
    delete jzb_sms1_sfzp_neg;
    delete jzb_sms2_sfzp_pos;
    delete jzb_sms2_sfzp_neg;
    delete jzb_sms3_sfzp_pos;
    delete jzb_sms3_sfzp_neg;
    delete jzb_sms4_sfzp_pos;
    delete jzb_sms4_sfzp_neg;
    delete jzb_lm4_sfzp_pospjzb;
    delete jzb_lm4_sfzp_negpjzb;
    delete jzb_lm8_sfzp_pospjzb;
    delete jzb_lm8_sfzp_negpjzb;
    delete jzb_sms1_sfzp_pospjzb;
    delete jzb_sms1_sfzp_negpjzb;
    delete jzb_sms2_sfzp_pospjzb;
    delete jzb_sms2_sfzp_negpjzb;
    delete jzb_sms3_sfzp_pospjzb;
    delete jzb_sms3_sfzp_negpjzb;
    delete jzb_sms4_sfzp_pospjzb;
    delete jzb_sms4_sfzp_negpjzb;
    delete jzb_lm4_sfzp_posnjzb;
    delete jzb_lm4_sfzp_negnjzb;
    delete jzb_lm8_sfzp_posnjzb;
    delete jzb_lm8_sfzp_negnjzb;
    delete jzb_sms1_sfzp_posnjzb;
    delete jzb_sms1_sfzp_negnjzb;
    delete jzb_sms2_sfzp_posnjzb;
    delete jzb_sms2_sfzp_negnjzb;
    delete jzb_sms3_sfzp_posnjzb;
    delete jzb_sms3_sfzp_negnjzb;
    delete jzb_sms4_sfzp_posnjzb;
    delete jzb_sms4_sfzp_negnjzb;
    delete jzb_lm4_sfzp_pospjes;
    delete jzb_lm4_sfzp_negpjes;
    delete jzb_lm8_sfzp_pospjes;
    delete jzb_lm8_sfzp_negpjes;
    delete jzb_sms1_sfzp_pospjes;
    delete jzb_sms1_sfzp_negpjes;
    delete jzb_sms2_sfzp_pospjes;
    delete jzb_sms2_sfzp_negpjes;
    delete jzb_sms3_sfzp_pospjes;
    delete jzb_sms3_sfzp_negpjes;
    delete jzb_sms4_sfzp_pospjes;
    delete jzb_sms4_sfzp_negpjes;
    delete jzb_lm4_sfzp_posnjes;
    delete jzb_lm4_sfzp_negnjes;
    delete jzb_lm8_sfzp_posnjes;
    delete jzb_lm8_sfzp_negnjes;
    delete jzb_sms1_sfzp_posnjes;
    delete jzb_sms1_sfzp_negnjes;
    delete jzb_sms2_sfzp_posnjes;
    delete jzb_sms2_sfzp_negnjes;
    delete jzb_sms3_sfzp_posnjes;
    delete jzb_sms3_sfzp_negnjes;
    delete jzb_sms4_sfzp_posnjes;
    delete jzb_sms4_sfzp_negnjes;

  }


  
}



void printEffInfo(string tag, float n, float pos, float neg, float pospjzb, float posnjzb, float negpjzb, float negnjzb,
                                               float pospjes, float posnjes, float negpjes, float negnjes, string reg) {


  cout << "Efficiency of " << tag << " for JZB > " << reg << " GeV" << endl;

  float eff = (pos-neg)/n;
  float eeff = (1.0/n)*sqrt((pos-neg)*(pos-neg)/n + pos + neg);
  float effpjzb = (pospjzb-negpjzb)/n;
  float effnjzb = (pospjzb-negnjzb)/n;
  float effpjes = (pospjes-negpjes)/n;
  float effnjes = (pospjes-negnjes)/n;
  float ejzb = max(fabs(effpjzb-eff), fabs(effnjzb-eff));
  float ejes = max(fabs(effpjes-eff), fabs(effnjes-eff));
  float totes = sqrt((0.02*0.02*eff*eff)+(0.02*0.02*eff*eff)+(ejzb*ejzb)+(ejes*ejes)+0.03*0.03*eff*eff+0.13*0.13*eff*eff);
  
  float toterror = sqrt(totes*totes+eeff*eeff);
  cout << eff << " +- " << toterror << " +- " << eeff << " (stat) +- " << totes << " (sys)" << endl;



}



