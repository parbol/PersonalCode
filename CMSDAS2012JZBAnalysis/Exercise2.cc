#include "TProfile.h"
#include "TF1.h"



float get_Gaussian_peak(TH1F *, float &, float &, float, float,bool,int); 




void exercise2() {



  //----------------First part: Drawing the raw jzb distribution----------------------
  THStack *jzb_lm4 = new THStack("jzb_lm4", "");
  THStack *jzb_zjets = new THStack("jzb_zjets", "");
  THStack *jzb_ttbar = new THStack("jzb_ttbar", "");
  

  //FIX ME: trigger

  //Produce the histograms
  lm4->Project(jzb_lm4, "jzb[1]", "id1==id2 && mll>40 && mll<200 && abs(mll-91.2)<20.0" + jets, 40, -200, 400, "jzb [GeV]");
  zjets->Project(jzb_zjets, "jzb[1]", "id1==id2 && mll>40 && mll<200 && abs(mll-91.2)<20.0" + jets, 40, -200, 400, "jzb [GeV]");
  ttbar->Project(jzb_ttbar, "jzb[1]", "id1==id2 && mll>40 && mll<200 && abs(mll-91.2)<20.0" + jets, 40, -200, 400, "jzb [GeV]");


  //Produce the legend
  //makeLegend(mll_data, mll_mc, leg);
  TCanvas *can_jzb = new TCanvas("can_jzb");
  can_jzb->cd();
  can_jzb->GetPad(0)->SetLogy(1);
  jzb_zjets->Draw("HIST");
  jzb_ttbar->Draw("HIST,SAME");
  jzb_lm4->Draw("HIST,SAME");
  can_jzb->SaveAs("jzb.png");

  delete can_jzb;
  delete jzb_lm4;
  delete jzb_zjets;
  delete jzb_ttbar;

  //----------------Second part: Response correction--------------------------------
  
  TH2F *response_data = new TH2F("response_data", "", 100, 0, 600, 100, 0, 5);
  TH2F *response_mc = new TH2F("response_mc", "", 100, 0, 600, 100, 0, 5);
  
  data->Project(response_data, "sumJetPt[1]/pt:pt", "id1==id2&&abs(mll-91.2)<20.0&&pt<600&&(sumJetPt[1]/pt)<5.0", "Z pt [GeV]", "Response");
  cocktail->Project(response_mc, "sumJetPt[1]/pt:pt", "id1==id2&&abs(mll-91.2)<20.0&&pt<600&&(sumJetPt[1]/pt)<5.0", "Z pt [GeV]", "Response");
   
  TProfile *response_prof_data = (TProfile*) response_data->ProfileX();
  TProfile *response_prof_mc = (TProfile*) response_mc->ProfileX();

  TCanvas *can_response = new TCanvas("can_response"); 
  can_response->Divide(2,1);
  can_response->cd(1);
  response_data->Draw("COLZ");
  response_prof_data->Draw("SAME");
  response_prof_data->Fit("pol0","","same,e1",30,400);
  drawPrelim(4700, false);
  TText* title_data = new TText(400.,3.,"Data");
  title_data->SetTextAlign(12);
  title_data->Draw();
  TF1 *datapol=(TF1*)response_prof_data->GetFunction("pol0");
  float datacorrection=datapol->GetParameter(0);
  can_response->cd(2);
  response_mc->Draw("COLZ");
  response_prof_mc->Draw("SAME");
  response_prof_mc->Fit("pol0","","same,e1",30,400);
  drawPrelim(4700, true);
  TText* title_mc = new TText(400.,3.,"MC");
  title_mc->SetTextAlign(12);
  title_mc->Draw();
  TF1 *mcpol=(TF1*)response_prof_mc->GetFunction("pol0");
  float mccorrection = mcpol->GetParameter(0);
  can_response->SaveAs("response_correction.png"); 

  delete mcpol;
  delete title_mc;
  delete datapol;
  delete title_data;
  delete can_response;
  delete response_prof_mc;
  delete response_prof_data;
  delete response_mc;
  delete response_data;
  
  //----------------Third part: Peak finding--------------------------------
  TH1F *jzb_data = new TH1F("jzb_data", "", 40, -200, 200);
  TH1F *jzb_mc = new TH1F("jzb_mc", "", 40, -200, 200);

  data->Project(jzb_data, "jzb[1]+0.055*pt", "id1==id2 && mll>40 && mll<200 && abs(mll-91.2)<20.0"+jets, "jzb [GeV]");
  cocktail->Project(jzb_mc, "jzb[1]+0.039*pt", "id1==id2 && mll>40 && mll<200 && abs(mll-91.2)<20.0"+jets, "jzb [GeV]");

  float error_data, sigma_data, peak_data, error_mc, sigma_mc, peak_mc;
  peak_data = get_Gaussian_peak(jzb_data, error_data, sigma_data, -40., 40., true, 2.0); 
  peak_mc =  get_Gaussian_peak(jzb_mc, error_data, sigma_data, -40., 40., false, 2.0); 
 
}



float get_Gaussian_peak(TH1F *hist, float &error, float &sigma, float lowlimit, float highlimit,bool is_data,int numsig) {
    
    float minfit = -100;
    float maxfit =  100;


    TCanvas *fitcanvas = new TCanvas("fitcanvas","fitcanvas");
    TF1 *fitFunc = new TF1("fitFunc","gaus",minfit,maxfit);
    
    float mean = hist->GetBinCenter( hist->GetMaximumBin());
    float rms = hist->GetRMS();

    fitFunc->SetParameter(1,mean);

    hist->Fit(fitFunc,"QLL0","",mean-10,mean+10);

    mean=fitFunc->GetParameter(1);
    rms=fitFunc->GetParameter(2);
    error=fitFunc->GetParError(1);

    // --- perform iterations
    int numIterations=5;

    for(int i=1;i<numIterations+1;i++) //--- modify the number of iterations until peak is stable 
    {
        hist->Fit(fitFunc,"QLLN","same",mean - numsig*rms, mean + numsig*rms);  // fit -2 +1 sigma from previous iteration
        mean=fitFunc->GetParameter(1);
        fitFunc->SetRange(mean - numsig*rms, mean + numsig*rms);
    }
    
    mean=fitFunc->GetParameter(1);
    sigma=fitFunc->GetParameter(2);
    error=1.0*fitFunc->GetParError(1);
    
    fitcanvas->cd();
    hist->SetMinimum(0);
    if(is_data) hist->Draw("e1");
    else hist->Draw("histo");
    fitFunc->SetLineColor(kBlue);
    fitFunc->SetLineWidth(1);
    fitFunc->Draw("same");
    hist->SetStats(0);

    TText *ftitle= new TText(50,4000,"Fit results:");
    ftitle->SetTextSize(0.03);
    ftitle->SetTextAlign(11);
    stringstream fitresult;
    fitresult << "#mu=" << std::setprecision(4) <<  mean << "+/-" << std::setprecision(4) <<  error;
    
    TText *title1 = new TText(50,3000,fitresult.str().c_str());
    title1->SetTextSize(0.03);
    title1->SetTextAlign(11);
    stringstream sigmainfo;
    sigmainfo << "#sigma=" << std::setprecision(4) << fitFunc->GetParameter(2) << "+/-" << std::setprecision(4) << fitFunc->GetParError(2);
    
    TText *sigmatext = new TText(50,2500,sigmainfo.str().c_str());
    sigmatext->SetTextSize(0.03);
    sigmatext->SetTextAlign(11);

    ftitle->Draw();
    title1->Draw();
    sigmatext->Draw();
    if(!is_data) fitcanvas->SaveAs("fit_summary_mc.png");
    if(is_data) fitcanvas->SaveAs("fit_summary_data.png");

    return mean;
}




