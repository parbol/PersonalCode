#include "/afs/cern.ch/user/p/pablom/tdrstyle.C"


void run() {

   setTDRStyle();
   TFile f("output2.root");
   TFile f2("../temp/output.root");
   TGraph *gr = (TGraph *)f.Get("Graph");
   TH2F   *cost = (TH2F *)f2.Get("cost");
   TH2F   *meth = (TH2F *)f2.Get("meth");
   TCanvas *m = new TCanvas("m");
   m->cd();
   cost->GetXaxis()->SetTitle("MET X [GeV]");
   cost->GetYaxis()->SetTitle("MET Y [GeV]");
   gr->GetXaxis()->SetTitle("MET X [GeV]");
   gr->GetYaxis()->SetTitle("MET Y [GeV]");
   cost->Draw("COLZ");
   gr->Draw("P");
   m->SaveAs("caca.png");
   TCanvas *m2 = new TCanvas("m2");
   m2->cd();
   meth->GetXaxis()->SetTitle("MET X [GeV]");
   meth->GetYaxis()->SetTitle("MET Y [GeV]");
   cost->GetXaxis()->SetTitle("MET X [GeV]");
   cost->GetYaxis()->SetTitle("MET Y [GeV]");
   gr->GetXaxis()->SetTitle("MET X [GeV]");
   gr->GetYaxis()->SetTitle("MET Y [GeV]");
   meth->Draw("COLZ");
   gr->Draw("P");
   m2->SaveAs("caca2.png");

}
