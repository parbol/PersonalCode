#include "/mnt/t3nfs01/data01/shome/pablom/tdrstyle.C"




void run() {

  setTDRStyle();
  TFile *fnormal = new TFile("output.root");
  TTree *ev_normal = (TTree *)fnormal->Get("events");

  TH1F *mt2_normal = new TH1F("mt2_normal", "", 100, 0, 100);
  TH1F *mt2_smeared = new TH1F("mt2_smeared", "", 100, 0, 100);
  ev_normal->Project("mt2_normal", "mt2", "", "");
  ev_normal->Project("mt2_smeared", "mt2Smeared", "", "");
  mt2_normal->GetXaxis()->SetTitle("MT2 [GeV]");
  mt2_smeared->GetXaxis()->SetTitle("MT2 [GeV]");
  mt2_normal->GetYaxis()->SetTitle("# events");
  mt2_smeared->GetYaxis()->SetTitle("# events");
  mt2_normal->SetLineColor(kBlue);
  mt2_smeared->SetLineColor(kRed);

  TLegend *leg;
  leg  = new TLegend(0.4, 0.75, 0.8, 0.9);
  leg->AddEntry(mt2_normal, "Gen.", "L");
  leg->AddEntry(mt2_smeared, "Gen. + smeared", "L");
  leg->SetFillColor(kWhite);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->SetLineWidth(0);
  leg->SetBorderSize(0);


  TCanvas *mt2_can = new TCanvas("mt2_can");
  mt2_can->cd();
  mt2_can->SetLogy(1);
  mt2_normal->Draw();
  mt2_smeared->Draw("same");
  leg->Draw();
  mt2_can->SaveAs("mt2.pdf");

  TH1F *met_50 = new TH1F("met_50", "", 100, -80, 120);
  TH1F *met_50_80 = new TH1F("met_50_80", "", 100, -80, 120);
  TH1F *met_80 = new TH1F("met_80", "", 100, -80, 120);
  TH1F *met_100 = new TH1F("met_100", "", 100, -80, 120);
  ev_normal->Project("met_50", "-ptMET+ptMETSmeared", "mt2Smeared<50", "");
  ev_normal->Project("met_50_80", "-ptMET+ptMETSmeared", "mt2Smeared>50 && mt2Smeared<80", "");
  ev_normal->Project("met_80", "-ptMET+ptMETSmeared", "mt2Smeared>80 && mt2Smeared < 100", "");
  ev_normal->Project("met_100", "-ptMET+ptMETSmeared", "mt2Smeared>100", "");

  met_50->GetXaxis()->SetTitle("MET [RECO] - MET[Gen] [GeV]");
  met_50_80->GetXaxis()->SetTitle("MET [RECO] - MET[Gen] [GeV]");
  met_80->GetXaxis()->SetTitle("MET [RECO] - MET[Gen] [GeV]");
  met_100->GetXaxis()->SetTitle("MET [RECO] - MET[Gen] [GeV]");
  met_50->GetYaxis()->SetTitle("# events");
  met_50_80->GetYaxis()->SetTitle("# events");
  met_80->GetYaxis()->SetTitle("# events");
  met_100->GetYaxis()->SetTitle("# events");
  met_50->SetLineColor(kBlue);
  met_50_80->SetLineColor(kRed);
  met_80->SetLineColor(kGreen);
  met_100->SetLineColor(kOrange);
  met_50->Scale(1.0/met_50->Integral());
  met_50_80->Scale(1.0/met_50_80->Integral());
  met_80->Scale(1.0/met_80->Integral());
  met_100->Scale(1.0/met_100->Integral());
  

  TLegend *leg2;
  leg2  = new TLegend(0.55, 0.75, 0.8, 0.9);
  leg2->AddEntry(met_50, "MT2<50", "L");
  leg2->AddEntry(met_50_80, "50<MT2<80", "L");
  leg2->AddEntry(met_80, "80<MT2<100", "L");
  leg2->AddEntry(met_100, "MT2>100", "L");
  leg2->SetFillColor(kWhite);
  leg2->SetTextFont(42);
  leg2->SetTextSize(0.04);
  leg2->SetLineWidth(0);
  leg2->SetBorderSize(0);


  TCanvas *met_can = new TCanvas("met_can");
  met_can->cd();
  //met_can->SetLogy(1);
  met_50->Draw();
  met_50_80->Draw("same");
  met_80->Draw("same");
  met_100->Draw("same");
  leg2->Draw();
  met_can->SaveAs("met.pdf");







}
