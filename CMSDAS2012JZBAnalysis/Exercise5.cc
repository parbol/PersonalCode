


float calculate_yields(TH1F *);






void exercise5() {


  //----------------Second part: Building the prediction----------------------
  string searchRegions[] = {"50", "100", "150", "200", "250"};

  for(int i = 0; i < 5; ++i) {
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


    string jzb_data_poscut_ = jzb_data_pos + "> " + searchRegions[i]; 
    string jzb_data_negcut_ = jzb_data_pos + "< -" + searchRegions[i]; 
    string jzb_mc_poscut_ = jzb_mc_pos + "> " + searchRegions[i]; 
    string jzb_mc_negcut_ = jzb_mc_pos + "< -" + searchRegions[i]; 

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
    float datasfzpp = calculate_yields(jzb_data_sfzp_pos);
    float dataofzpp = calculate_yields(jzb_data_ofzp_pos);
    float datasfsbp = calculate_yields(jzb_data_sfsb_pos);
    float dataofsbp = calculate_yields(jzb_data_ofsb_pos);
    float datasfzpn = calculate_yields(jzb_data_sfzp_neg);
    float dataofzpn = calculate_yields(jzb_data_ofzp_neg);
    float datasfsbn = calculate_yields(jzb_data_sfsb_neg);
    float dataofsbn = calculate_yields(jzb_data_ofsb_neg);

    float datazjets = datasfzpn - 1.0/3.0*(dataofzpn+datasfsbn+dataofsbn);
    float datastatzjets = sqrt(datasfzpn + 1.0/9.0*(dataofzpn+datasfsbn+dataofsbn));
    float datasystzjets = sqrt(1.0/16.0*datasfzpn*datasfzpn+1.0/9.0*1.0/16.0*(dataofzpn*dataofzpn+datasfsbn*datasfsbn+dataofsbn*dataofsbn));

    float datattbar = 1.0/3.0*(dataofzpp + datasfsbp + dataofsbp);
    float datastatttbar = sqrt(1.0/9.0*(dataofzpp + datasfsbp + dataofsbp));
    float datasystttbar = sqrt(1.0/9.0*1.0/16.0*(dataofzpp*dataofzpp+datasfsbp*datasfsbp+dataofsbp*dataofsbp));

    float datapredicted = datazjets + datattbar;
    float datastatpredicted = sqrt(datastatzjets*datastatzjets+datastatttbar*datastatttbar);
    float datasystpredicted = sqrt(datasystzjets*datasystzjets+datasystttbar*datasystttbar);

    cout << "_____________________DATA______________________________" << endl;
    cout << "JZB > " << searchRegions[i] << endl;
    cout << "Data Observed: " << datasfzpp << " +- " << sqrt(datasfzpp) << endl;
    cout << "Data Z+jets prediction: 1/3*(" << datasfzpn << " - " << dataofzpn << ") + " << endl;
    cout << "                        1/3*(" << datasfzpn << " - " << datasfsbn << ") + " << endl;
    cout << "                        1/3*(" << datasfzpn << " - " << dataofsbn << ") " << endl;
    cout << "                        = " << datazjets << " +- " << datastatzjets << " +- " << datasystzjets << endl;
    cout << "Data ttbar prediction:  1/3*(" << dataofzpp << " + " << endl;
    cout << "                             " << datasfsbp << " + " << endl;
    cout << "                             " << dataofsbp << ")" << endl;
    cout << "                        = " << datattbar << " +- " << datastatttbar << " +- " << datasystttbar << endl;
    cout << "Total prediction:       = " << datapredicted << " +- " << datastatpredicted << " +- " << datasystpredicted << endl;

  


    float mcsfzpp = calculate_yields(jzb_mc_sfzp_pos);
    float mcofzpp = calculate_yields(jzb_mc_ofzp_pos);
    float mcsfsbp = calculate_yields(jzb_mc_sfsb_pos);
    float mcofsbp = calculate_yields(jzb_mc_ofsb_pos);
    float mcsfzpn = calculate_yields(jzb_mc_sfzp_neg);
    float mcofzpn = calculate_yields(jzb_mc_ofzp_neg);
    float mcsfsbn = calculate_yields(jzb_mc_sfsb_neg);
    float mcofsbn = calculate_yields(jzb_mc_ofsb_neg);

    float mczjets = mcsfzpn - 1.0/3.0*(mcofzpn+mcsfsbn+mcofsbn);
    float mcstatzjets = sqrt(mcsfzpn + 1.0/9.0*(mcofzpn+mcsfsbn+mcofsbn));
    float mcsystzjets = sqrt(1.0/16.0*mcsfzpn*mcsfzpn+1.0/9.0*1.0/16.0*(mcofzpn*mcofzpn+mcsfsbn*mcsfsbn+mcofsbn*mcofsbn));

    float mcttbar = 1.0/3.0*(mcofzpp + mcsfsbp + mcofsbp);
    float mcstatttbar = sqrt(1.0/9.0*(mcofzpp + mcsfsbp + mcofsbp));
    float mcsystttbar = sqrt(1.0/9.0*1.0/16.0*(mcofzpp*mcofzpp+mcsfsbp*mcsfsbp+mcofsbp*mcofsbp));

    float mcpredicted = mczjets + mcttbar;
    float mcstatpredicted = sqrt(mcstatzjets*mcstatzjets+mcstatttbar*mcstatttbar); 
    float mcsystpredicted = sqrt(mcsystzjets*mcsystzjets+mcsystttbar*mcsystttbar); 

    cout << "_____________________MC______________________________" << endl;
    cout << "JZB > " << searchRegions[i] << endl; 
    cout << "Data Observed: " << mcsfzpp << " +- " << sqrt(mcsfzpp) << endl;
    cout << "Data Z+jets prediction: 1/3*(" << mcsfzpn << " - " << mcofzpn << ") + " << endl;
    cout << "                        1/3*(" << mcsfzpn << " - " << mcsfsbn << ") + " << endl;
    cout << "                        1/3*(" << mcsfzpn << " - " << mcofsbn << ") " << endl;
    cout << "                        = " << mczjets << " +- " << mcstatzjets << " +- " << mcsystzjets << endl;
    cout << "Data ttbar prediction:  1/3*(" << mcofzpp << " + " << endl;
    cout << "                             " << mcsfsbp << " + " << endl;
    cout << "                             " << mcofsbp << ")" << endl;
    cout << "                        = " << mcttbar << " +- " << mcstatttbar << " +- " << mcsystttbar << endl;
    cout << "Total prediction:       = " << mcpredicted << " +- " << mcstatpredicted << " +- " << mcsystpredicted << endl;    


    float mccsfzpp = calculate_yields(jzb_mcc_sfzp_pos);
    float mccofzpp = calculate_yields(jzb_mcc_ofzp_pos);
    float mccsfsbp = calculate_yields(jzb_mcc_sfsb_pos);
    float mccofsbp = calculate_yields(jzb_mcc_ofsb_pos);
    float mccsfzpn = calculate_yields(jzb_mcc_sfzp_neg);
    float mccofzpn = calculate_yields(jzb_mcc_ofzp_neg);
    float mccsfsbn = calculate_yields(jzb_mcc_sfsb_neg);
    float mccofsbn = calculate_yields(jzb_mcc_ofsb_neg);

    float mcczjets = mccsfzpn - 1.0/3.0*(mccofzpn+mccsfsbn+mccofsbn);
    float mccstatzjets = sqrt(mccsfzpn + 1.0/9.0*(mccofzpn+mccsfsbn+mccofsbn));
    float mccsystzjets = sqrt(1.0/16.0*mccsfzpn*mccsfzpn+1.0/9.0*1.0/16.0*(mccofzpn*mccofzpn+mccsfsbn*mccsfsbn+mccofsbn*mccofsbn));

    float mccttbar = 1.0/3.0*(mccofzpp + mccsfsbp + mccofsbp);
    float mccstatttbar = sqrt(1.0/9.0*(mccofzpp + mccsfsbp + mccofsbp));
    float mccsystttbar = sqrt(1.0/9.0*1.0/16.0*(mccofzpp*mccofzpp+mccsfsbp*mccsfsbp+mccofsbp*mccofsbp));

    float mccpredicted = mcczjets + mccttbar;
    float mccstatpredicted = sqrt(mccstatzjets*mccstatzjets+mccstatttbar*mccstatttbar); 
    float mccsystpredicted = sqrt(mccsystzjets*mccsystzjets+mccsystttbar*mccsystttbar); 

    cout << "_____________________MC+S______________________________" << endl;
    cout << "JZB > " << searchRegions[i] << endl;
    cout << "Data Observed: " << mccsfzpp << " +- " << sqrt(mccsfzpp) << endl;
    cout << "Data Z+jets prediction: 1/3*(" << mccsfzpn << " - " << mccofzpn << ") + " << endl;
    cout << "                        1/3*(" << mccsfzpn << " - " << mccsfsbn << ") + " << endl;
    cout << "                        1/3*(" << mccsfzpn << " - " << mccofsbn << ") " << endl;
    cout << "                        = " << mcczjets << " +- " << mccstatzjets << " +- " << mccsystzjets << endl; 
    cout << "Data ttbar prediction:  1/3*(" << mccofzpp << " + " << endl;
    cout << "                             " << mccsfsbp << " + " << endl;
    cout << "                             " << mccofsbp << ")" << endl;
    cout << "                        = " << mccttbar << " +- " << mccstatttbar << " +- " << mccsystttbar << endl; 
    cout << "Total prediction:       = " << mccpredicted << " +- " << mccstatpredicted << " +- " << mccsystpredicted << endl;    



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


  
}



float calculate_yields(TH1F *h) {

  float cumulative=0;

  for(int i=h->GetNbinsX();i>=1;i--) {
    cumulative+=h->GetBinContent(i);
  }

  return cumulative;
}


