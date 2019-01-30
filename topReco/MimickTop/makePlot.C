#include "/afs/cern.ch/user/p/pablom/tdrstyle.C"


void makePlot() {

    setTDRStyle();

    TFile f("output.root");
    TH1F *dm300_w = (TH1F *)f.Get("dm300_w");
    TH1F *ttbar_w = (TH1F *)f.Get("ttbar_w");
    TH1F *dm300_pt = (TH1F *)f.Get("dm300_pt");
    TH1F *ttbar_pt = (TH1F *)f.Get("ttbar_pt");
    TH1F *dm300_comp = (TH1F *)f.Get("dm300_comp");
    TH1F *ttbar_comp = (TH1F *)f.Get("ttbar_comp");
    dm300_w->GetXaxis()->SetTitle("weight");
    ttbar_w->GetXaxis()->SetTitle("weight");
    dm300_pt->GetXaxis()->SetTitle("p_{T} [GeV]");
    ttbar_pt->GetXaxis()->SetTitle("p_{T} [GeV]");
    dm300_comp->GetXaxis()->SetTitle("#Delta p_{T}/p_{T}");
    ttbar_comp->GetXaxis()->SetTitle("#Delta p_{T}/p_{T}");
    dm300_w->GetXaxis()->CenterTitle();
    ttbar_w->GetXaxis()->CenterTitle();
    dm300_w->SetLineColor(kBlue);
    ttbar_w->SetLineColor(kRed);
    dm300_pt->GetXaxis()->CenterTitle();
    ttbar_pt->GetXaxis()->CenterTitle();
    dm300_pt->SetLineColor(kBlue);
    ttbar_pt->SetLineColor(kRed);
    dm300_comp->GetXaxis()->CenterTitle();
    ttbar_comp->GetXaxis()->CenterTitle();
    dm300_comp->SetLineColor(kBlue);
    ttbar_comp->SetLineColor(kRed);
    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.9);
    leg->AddEntry(dm300_w, "DM(300,1)", "L");
    leg->AddEntry(ttbar_w, "ttbar", "L");
    leg->SetFillColor(kWhite);
    leg->SetTextFont(42);
    leg->SetTextSize(0.04);
    leg->SetLineWidth(0);
    leg->SetBorderSize(0);

    TCanvas *m_w = new TCanvas("m_w");
    m_w->cd();
    m_w->SetLogy(1);
    dm300_w->Draw();
    ttbar_w->Draw("SAME");
    leg->Draw();
    m_w->SaveAs("weights.png"); 

    TCanvas *m_pt = new TCanvas("m_pt");
    m_pt->cd();
    m_pt->SetLogy(1);
    dm300_pt->Draw();
    ttbar_pt->Draw("SAME");
    leg->Draw();
    m_pt->SaveAs("pt_med.png"); 

    TCanvas *m_comp = new TCanvas("m_comp");
    m_comp->cd();
    m_comp->SetLogy(1);
    ttbar_comp->Draw();
    dm300_comp->Draw("SAME");
    leg->Draw();
    m_comp->SaveAs("pt_comp.png"); 

 


}
