//#######################################################################################################
//########################### Name: Internal Analysis Fit           #####################################
//########################### Author: Pablo Martinez Ruiz del Arbol #####################################
//#######################################################################################################



#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"
#include "TF1.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TTree.h"
#include "TMatrixD.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TChain.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>

#define MAX_FILES 90


//Different functionalities
//start() is needed to read the root files
//run() will proceed with the analysis
//showPlots() prepares the different plots 



//Definition of the branch of the NTuple

typedef struct {
  Int_t wh, st, se;
  Float_t x04D, phi4D, z04D, theta04D;
  Float_t x0SL1up, phiSL1up, x0SL3up, phiSL3up;
  Float_t x0SL1down, phiSL1down, x0SL3down, phiSL3down;
  Float_t hitsPhi[8][3];
  Float_t hitsTheta[8][3];
} DTSegmentAn;



//Definition of the output branch of the NTuple

typedef struct {
  Int_t wh, st, se;
  float dx_f, dz_f, alpha_f;
  float ex_f, ez_f, ealpha_f;
  float corr_xz_f, corr_xalpha_f, corr_zalpha_f;
  float dx_b, dz_b, alpha_b;
  float ex_b, ez_b, ealpha_b;
  float corr_xz_b, corr_xalpha_b, corr_zalpha_b;
} DTSegmentResult; 




//Input to the TTree
DTSegmentAn my4DSegment;

//Output to the TTree
DTSegmentResult myResult;


//TTrees
TChain *tali;
TTree *outputTree;

//TFile 
TFile *myFile;

//Read the Ntuple
void start() {
  
  tali = new TChain("InternalAlignment");
  for(int n_file = 0; n_file < MAX_FILES; ++n_file) {
    char theNameOfTheFile[100];
    sprintf(theNameOfTheFile, 
	    "rfio:/castor/cern.ch/user/p/pablom/AnalysisCRAFTInternalAlignment/CRAFT_ALL_V4_V11_Survey/MyNtuple%d.root",
	    //"rfio:/castor/cern.ch/user/p/pablom/AnalysisCRAFTInternalAlignment/LastNominal/MyNtuple_%d.root",
            //"/tmp/pablom/job_%d/MyNtuple.root",
	    //"job_%d/MyNtuple.root",
	    n_file);
    tali->Add(theNameOfTheFile);
  }
  
  tali->SetBranchAddress("4DSegmentInfo", &my4DSegment);

  myFile = new TFile("InternalResultV4WithSurveyCorrected.root", "RECREATE");
  myFile->cd();
  outputTree = new TTree("InternalAlignmentResult", "InternalAlignmentResult"); 
  outputTree->Branch("4DSegmentResult", &myResult,"wh/I:st/I:se/I:dx_f/F:dz_f/F:alpha_f/F:ex_f/F:ez_f/F:ealpha_f/F:corr_xz_f/F:corr_xalpha_f/F:corr_zalpha_f/F:dx_b/F:dz_b/F:alpha_b/F:ex_b/F:ez_b/F:ealpha_b/F:corr_xz_b/F:corr_xalpha_b/F:corr_zalpha_b/F"); 

}




void run2(void) {
 

  ofstream myout("values.out");
 
  float c11[5][4][14];
  float c12[5][4][14];
  float c21[5][4][14];
  float c22[5][4][14];
  float b1[5][4][14];
  float b2[5][4][14];

  TProfile ****xphi = new TProfile *** [5];
  TH1D ****phi = new TH1D *** [5];
  
  //Put all values to 0
  for(int wheel = -2; wheel < 3; ++wheel) {
    xphi[wheel+2] = new TProfile **[4];
    phi[wheel+2] = new TH1D **[4];
    for(int station = 1; station < 5; ++station) {
      xphi[wheel+2][station-1] = new TProfile * [14];
      phi[wheel+2][station-1] = new TH1D * [14];
      for(int sector = 1; sector < 15; ++sector) {
        if(sector > 12 && station != 4) continue;
        char name[30], namephi[30];
        sprintf(name, "xphi_%d_%d_%d", wheel, station, sector);
        sprintf(namephi, "phi_%d_%d_%d", wheel, station, sector);
        xphi[wheel+2][station-1][sector-1] = new TProfile(name, name, 30, -1.2, 1.2);
        phi[wheel+2][station-1][sector-1] = new TH1D(namephi, namephi, 30, -5, 5);
        //xphi[wheel+2][station-1][sector-1]->GetYaxis()->SetRangeUser(-0.3, 0.3);
        c11[wheel+2][station-1][sector-1] = 0;
        c12[wheel+2][station-1][sector-1] = 0;
        c21[wheel+2][station-1][sector-1] = 0;
        c22[wheel+2][station-1][sector-1] = 0;
        b1[wheel+2][station-1][sector-1] = 0;
        b2[wheel+2][station-1][sector-1] = 0;
      }
    }
  }


  // all entries and fill the histograms
  Int_t nentries = (Int_t)tali->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
  //for (Int_t i=0;i<1000;i++) {
    tali->GetEntry(i);

    if(my4DSegment.hitsPhi[0][0] == my4DSegment.hitsPhi[4][0] || my4DSegment.hitsPhi[1][0] == my4DSegment.hitsPhi[5][0] ||
       my4DSegment.hitsPhi[2][0] == my4DSegment.hitsPhi[6][0] || my4DSegment.hitsPhi[3][0] == my4DSegment.hitsPhi[7][0]) {
       continue;
    }   


    //cout << "-------------------" << endl;
    //for(int k = 0; k < 8; k++) {
   
    //   cout << my4DSegment.hitsPhi[k][0] << " " << my4DSegment.hitsPhi[k][1] << " " << my4DSegment.hitsPhi[k][2] << std::endl;
    // }


    float x1 = (my4DSegment.hitsPhi[0][0]+my4DSegment.hitsPhi[1][0]+my4DSegment.hitsPhi[2][0]+my4DSegment.hitsPhi[3][0])/4.0;
    float x2 = (my4DSegment.hitsPhi[4][0]+my4DSegment.hitsPhi[5][0]+my4DSegment.hitsPhi[6][0]+my4DSegment.hitsPhi[7][0])/4.0;
    float z1 = (my4DSegment.hitsPhi[0][2]+my4DSegment.hitsPhi[1][2]+my4DSegment.hitsPhi[2][2]+my4DSegment.hitsPhi[3][2])/4.0;
    float z2 = (my4DSegment.hitsPhi[4][2]+my4DSegment.hitsPhi[5][2]+my4DSegment.hitsPhi[6][2]+my4DSegment.hitsPhi[7][2])/4.0;

    
    float tanphi = my4DSegment.phi4D;
    
    int wheel = my4DSegment.wh;
    int station = my4DSegment.st;
    int sector = my4DSegment.se;

    //cout << wheel << " " << station << " " << sector << " " << z2-z1 << std::endl;
    //std::cout << wheel << " " << station << " " << sector << std::endl; 

    //for(int s = 0; s < 8; s++) {
    //  std::cout << my4DSegment.hitsPhi[s][0] << std::endl; 
    // }
    //std::cout << tanphi << std::endl;
    xphi[wheel+2][station-1][sector-1]->Fill(tanphi, x2-x1);
    phi[wheel+2][station-1][sector-1]->Fill(tanphi);
   
 
       
    if(isnan(x2) || isnan(x1) || isnan(x1-x2)) continue;
    if(fabs(my4DSegment.phiSL1up-my4DSegment.phiSL3up)>0.06) continue;
    if(x2 > 160 || x2 < -160) continue;
    if(x1 > 160 || x1 < -160) continue;
 
    if(fabs(tanphi) > 0.6) continue;
    if(fabs(x2-x1)>40) continue;

    c11[wheel+2][station-1][sector-1] += 1;
    c12[wheel+2][station-1][sector-1] += tanphi;
    c21[wheel+2][station-1][sector-1] += tanphi;
    c22[wheel+2][station-1][sector-1] += tanphi*tanphi;
    b1[wheel+2][station-1][sector-1] -= (x2-x1);
    b2[wheel+2][station-1][sector-1] -= (x2-x1)*tanphi;
    
  }  
  for(int wheel = -2; wheel < 3; ++wheel) {
    for(int station = 1; station < 5; ++station) {
      for(int sector = 1; sector < 15; ++sector) {
        if(sector > 12 && station != 4) continue;
        char canvasName[40], canvas2[40];
        sprintf(canvasName, "xphi_%d_%d_%d.eps", wheel, station, sector);
        sprintf(canvas2, "phi_%d_%d_%d.eps", wheel, station, sector);
        TCanvas *mcan = new TCanvas(canvasName);
        mcan->cd();
        TF1 *linxphi = new TF1("linxphi", "pol1");
        xphi[wheel+2][station-1][sector-1]->Draw();
        xphi[wheel+2][station-1][sector-1]->Fit(linxphi, "", "SAMES");
        cout << "copy " << wheel << " " << station << " " << sector << " " << linxphi->GetParameter(0) << " " << linxphi->GetParameter(1) << endl;
        mcan->SaveAs(canvasName);
        TCanvas *mcan2 = new TCanvas(canvas2);
        mcan2->cd();
        phi[wheel+2][station-1][sector-1]->Draw();
        mcan2->SaveAs(canvas2);
        TMatrixD Cov(2,2);
        Cov(0,0) = c11[wheel+2][station-1][sector-1];
        Cov(0,1) = c12[wheel+2][station-1][sector-1];
        Cov(1,0) = c21[wheel+2][station-1][sector-1];
        Cov(1,1) = c22[wheel+2][station-1][sector-1];
        TMatrixD vec(2,1);
        vec(0,0) = b1[wheel+2][station-1][sector-1];
        vec(1,0) = b2[wheel+2][station-1][sector-1];

        //Cov.Print();
        //vec.Print();
        Cov.Invert();

        TMatrixD solution = Cov * vec;
        //cout << "copy " << wheel << " " << station << " " << sector << " " << solution(0,0) << " " << solution(1,0) << endl;
        myout << wheel << " " << station << " " << sector << " " <<  phi[wheel+2][station-1][sector-1]->GetMean() << " " << phi[wheel+2][station-1][sector-1]->GetRMS()  << endl;

        //Fill the tree 
        myResult.wh = wheel;
        myResult.st = station;
        myResult.se = sector;
        myResult.dx_f = solution(0,0);
        myResult.dz_f = solution(1,0);
        myResult.alpha_f = 0.0;
        myResult.ex_f = sqrt(Cov(0,0));
        myResult.ez_f = sqrt(Cov(1,1));
        myResult.ealpha_f = 0.0;
        myResult.corr_xz_f = Cov(0,1)/sqrt(Cov(0,0)*Cov(1,1));
        myResult.corr_xalpha_f = 0.0;
        myResult.corr_zalpha_f = 0.0;
        
        myResult.dx_b = 0.0;
        myResult.dz_b = 0.0;
        myResult.alpha_b = 0.0;
        myResult.ex_b = 0.0;
        myResult.ez_b = 0.0;
        myResult.ealpha_b = 0.0;
        myResult.corr_xz_b = 0.0;
        myResult.corr_xalpha_b = 0.0;
        myResult.corr_zalpha_b = 0.0;
       
        outputTree->Fill();
      }
    }
  }
  myFile->Write();
  myFile->Close();
  myout.close();
}


void run(void) {
  
  
  
  //The minimization leads to a matrix of the form (for one indivual case)
  //
  //    |1         m        x*m  |          |   Res   |
  //    |                        |          |         |
  //    |m        m^2      m^2*x | delta =  |  Res*m  |
  //    |                        |          |         |
  //    |x*m      m^2*x   m^2*x^2|          | Res*x*m |
  //We will call cij_f, bi_f, cij_b, bi_b to the components depending (forward, and backward)
 


 
  //Backward
  float c11_b[5][4][14];
  float c12_b[5][4][14];
  float c13_b[5][4][14];
  float c22_b[5][4][14];
  float c23_b[5][4][14];
  float c33_b[5][4][14];
  float b1_b[5][4][14];
  float b2_b[5][4][14];
  float b3_b[5][4][14];
  //Forward
  float c11_f[5][4][14];
  float c12_f[5][4][14];
  float c13_f[5][4][14];
  float c22_f[5][4][14];
  float c23_f[5][4][14];
  float c33_f[5][4][14];
  float b1_f[5][4][14];
  float b2_f[5][4][14];
  float b3_f[5][4][14];

  float mean[5][4][14];
  float rms[5][4][14];

  ifstream a("values.out");
  while(!a.eof()) {
    int wh, st, se;
    float m, r;
    a >> wh >> st >> se >> m >> r;
    if(a.eof()) break;
    mean[wh+2][st-1][se-1] = m;
    rms[wh+2][st-1][se-1] = r;
  }  
  a.close();

  TProfile ****xphi = new TProfile *** [5];
  TProfile ****xphi_b = new TProfile *** [5];
  TProfile ****xphi_f = new TProfile *** [5];
  
  //Put all values to 0
  for(int wheel = -2; wheel < 3; ++wheel) {
    xphi[wheel+2] = new TProfile **[4];
    xphi_b[wheel+2] = new TProfile **[4];
    xphi_f[wheel+2] = new TProfile **[4];
    for(int station = 1; station < 5; ++station) {
      xphi[wheel+2][station-1] = new TProfile * [14];
      xphi_b[wheel+2][station-1] = new TProfile * [14];
      xphi_f[wheel+2][station-1] = new TProfile * [14];
      for(int sector = 1; sector < 15; ++sector) {
        if(sector > 12 && station != 4) continue;
        char name[30], name_b[30], name_f[30];
        sprintf(name, "xphi_%d_%d_%d", wheel, station, sector);
        sprintf(name_b, "xphi_b_%d_%d_%d", wheel, station, sector);
        sprintf(name_f, "xphi_f_%d_%d_%d", wheel, station, sector);
        xphi[wheel+2][station-1][sector-1] = new TProfile(name, name, 30, mean[wheel+2][station-1][sector-1]-rms[wheel+2][station-1][sector-1],  mean[wheel+2][station-1][sector-1]-rms[wheel+2][station-1][sector-1]);
        xphi_b[wheel+2][station-1][sector-1] = new TProfile(name_b, name_b, 30, -1.2, 1.2);
        xphi_f[wheel+2][station-1][sector-1] = new TProfile(name_f, name_f, 30, -1.2, 1.2);
        xphi_b[wheel+2][station-1][sector-1]->SetLineColor(kRed);
        xphi_f[wheel+2][station-1][sector-1]->SetLineColor(kBlue);
        xphi[wheel+2][station-1][sector-1]->GetYaxis()->SetRangeUser(-0.3, 0.3);
        xphi_b[wheel+2][station-1][sector-1]->GetYaxis()->SetRangeUser(-0.3, 0.3);
        xphi_f[wheel+2][station-1][sector-1]->GetYaxis()->SetRangeUser(-0.3, 0.3);
	c11_b[wheel+2][station-1][sector-1] = c11_f[wheel+2][station-1][sector-1] = 0;
	c12_b[wheel+2][station-1][sector-1] = c12_f[wheel+2][station-1][sector-1] = 0;
	c13_b[wheel+2][station-1][sector-1] = c13_f[wheel+2][station-1][sector-1] = 0;
	c22_b[wheel+2][station-1][sector-1] = c22_f[wheel+2][station-1][sector-1] = 0;
	c23_b[wheel+2][station-1][sector-1] = c23_f[wheel+2][station-1][sector-1] = 0;
	c33_b[wheel+2][station-1][sector-1] = c33_f[wheel+2][station-1][sector-1] = 0;
	b1_b[wheel+2][station-1][sector-1] = b1_f[wheel+2][station-1][sector-1] = 0;
	b2_b[wheel+2][station-1][sector-1] = b2_f[wheel+2][station-1][sector-1] = 0;
	b3_b[wheel+2][station-1][sector-1] = b3_f[wheel+2][station-1][sector-1] = 0;
      }
    }
  }
  
  // all entries and fill the histograms
  Int_t nentries = (Int_t)tali->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    tali->GetEntry(i);
    if(my4DSegment.hitsPhi[0][0] == my4DSegment.hitsPhi[4][0] || my4DSegment.hitsPhi[1][0] == my4DSegment.hitsPhi[5][0] ||
       my4DSegment.hitsPhi[2][0] == my4DSegment.hitsPhi[6][0] || my4DSegment.hitsPhi[3][0] == my4DSegment.hitsPhi[7][0]) {
       continue;
    }
  
    float x_f = my4DSegment.x0SL3up;
    float xp_f = my4DSegment.x0SL1up;
    float x_b = my4DSegment.x0SL3down;
    float xp_b = my4DSegment.x0SL1down;
    float tanphi = my4DSegment.phi4D;
    //float tanphi_f = my4DSegment.phiSL1up;
    //float tanphi_b = my4DSegment.phiSL1down;
    float tanphi_f = tanphi;
    float tanphi_b = tanphi;
    int wheel = my4DSegment.wh;
    int station = my4DSegment.st;
    int sector = my4DSegment.se;
    //if(tanphi == nan) {
    //  std::cout << wheel << " " << station << " " << sector << " Produced a nan in tanphi " << std::endl;
    //}
    if(tanphi >  mean[wheel+2][station-1][sector-1]+0.7*rms[wheel+2][station-1][sector-1] || 
       tanphi < mean[wheel+2][station-1][sector-1]-0.7*rms[wheel+2][station-1][sector-1]) continue; 
    //if(tanphi >  0.6 || tanphi < -0.6) continue; 
    
   
    if(fabs(x_f-xp_f)< 3 && fabs(my4DSegment.phiSL1up-my4DSegment.phiSL3up)<0.06) {
    
      xphi[wheel+2][station-1][sector-1]->Fill(tanphi, x_f-xp_f);
      xphi_b[wheel+2][station-1][sector-1]->Fill(tanphi_b, x_b-xp_b);
      xphi_f[wheel+2][station-1][sector-1]->Fill(tanphi_f, x_f-xp_f);
      
      c11_f[wheel+2][station-1][sector-1] += 1;
      c12_f[wheel+2][station-1][sector-1] += tanphi_f;
      c13_f[wheel+2][station-1][sector-1] += tanphi_f*xp_f;
      c22_f[wheel+2][station-1][sector-1] += tanphi_f*tanphi_f;
      c23_f[wheel+2][station-1][sector-1] += tanphi_f*tanphi_f*xp_f;
      //c33_f[wheel+2][station-1][sector-1] += xp_f*tanphi_f*xp_f*tanphi_f + 1.0;
      c33_f[wheel+2][station-1][sector-1] += xp_f*tanphi_f*xp_f*tanphi_f;
      b1_f[wheel+2][station-1][sector-1] -= (x_f-xp_f);
      b2_f[wheel+2][station-1][sector-1] -= (x_f-xp_f)*tanphi_f;
      //b3_f[wheel+2][station-1][sector-1] -= (x_f-xp_f)*xp_f*tanphi_f + my4DSegment.phiSL1up-my4DSegment.phiSL3up;
      b3_f[wheel+2][station-1][sector-1] -= (x_f-xp_f)*xp_f*tanphi_f;
   
    }
    
    if(fabs(x_b-xp_b)< 3 && fabs(my4DSegment.phiSL1down-my4DSegment.phiSL3down)<0.04) {
      
      c11_b[wheel+2][station-1][sector-1] += 1;
      c12_b[wheel+2][station-1][sector-1] += tanphi_b;
      c13_b[wheel+2][station-1][sector-1] += tanphi_b*xp_b;
      c22_b[wheel+2][station-1][sector-1] += tanphi_b*tanphi_b;
      c23_b[wheel+2][station-1][sector-1] += tanphi_b*tanphi_b*xp_b;
      //c33_b[wheel+2][station-1][sector-1] += xp_b*tanphi_b*xp_b*tanphi_b + 1.0;
      c33_b[wheel+2][station-1][sector-1] += xp_b*tanphi_b*xp_b*tanphi_b;
      b1_b[wheel+2][station-1][sector-1] -= (x_b-xp_b);
      b2_b[wheel+2][station-1][sector-1] -= (x_b-xp_b)*tanphi_b;
      //b3_b[wheel+2][station-1][sector-1] -= (x_b-xp_b)*xp_b*tanphi_b + my4DSegment.phiSL1down-my4DSegment.phiSL3down;
      b3_b[wheel+2][station-1][sector-1] -= (x_b-xp_b)*xp_b*tanphi_b;
    }
    
  }
  
  for(int wheel = -2; wheel < 3; ++wheel) {
    for(int station = 1; station < 5; ++station) {
      for(int sector = 1; sector < 15; ++sector) {
	if(sector > 12 && station != 4) continue;
	char canvasName[40];
        sprintf(canvasName, "xphi_%d_%d_%d.eps", wheel, station, sector);
        TCanvas *mcan = new TCanvas(canvasName); 
        mcan->cd();
        TF1 *linxphi = new TF1("linxphi", "pol1");
        TF1 *linxphi_b = new TF1("linxphi_b", "pol1");
        TF1 *linxphi_f = new TF1("linxphi_f", "pol1");
        linxphi_b->SetLineColor(kRed);
        linxphi_f->SetLineColor(kBlue);
        xphi[wheel+2][station-1][sector-1]->Draw();
        xphi[wheel+2][station-1][sector-1]->Fit(linxphi, "", "SAMES");
        std::cout << "Output " << wheel << " " << station << " " << sector << " "
                  <<  linxphi->GetParameter(0) <<  " " << linxphi->GetParameter(1) << " " 
                  <<  linxphi->GetParError(0) <<  " " << linxphi->GetParError(1) << std::endl;
        //xphi_b[wheel+2][station-1][sector-1]->Draw("SAMES");
        //xphi_b[wheel+2][station-1][sector-1]->Fit(linxphi_b, "", "SAMES");
        //xphi_f[wheel+2][station-1][sector-1]->Draw("SAMES");
        //xphi_f[wheel+2][station-1][sector-1]->Fit(linxphi_f, "", "SAMES");
        mcan->SaveAs(canvasName);
        //Forward
	TMatrixD Cov_f(3,3);
	Cov_f(0,0) = c11_f[wheel+2][station-1][sector-1];
	Cov_f(1,0) = c12_f[wheel+2][station-1][sector-1];
	Cov_f(2,0) = c13_f[wheel+2][station-1][sector-1];
	Cov_f(0,1) = c12_f[wheel+2][station-1][sector-1];
	Cov_f(0,2) = c13_f[wheel+2][station-1][sector-1];
	Cov_f(1,1) = c22_f[wheel+2][station-1][sector-1];
	Cov_f(1,2) = c23_f[wheel+2][station-1][sector-1];
	Cov_f(2,1) = c23_f[wheel+2][station-1][sector-1];
	Cov_f(2,2) = c33_f[wheel+2][station-1][sector-1];
	TMatrixD vec_f(3,1);
	vec_f(0,0) = b1_f[wheel+2][station-1][sector-1]; 
	vec_f(1,0) = b2_f[wheel+2][station-1][sector-1];
	vec_f(2,0) = b3_f[wheel+2][station-1][sector-1];
	//Backward
	TMatrixD Cov_b(3,3);
	Cov_b(0,0) = c11_b[wheel+2][station-1][sector-1];
	Cov_b(1,0) = c12_b[wheel+2][station-1][sector-1];
	Cov_b(2,0) = c13_b[wheel+2][station-1][sector-1];
	Cov_b(0,1) = c12_b[wheel+2][station-1][sector-1];
	Cov_b(0,2) = c13_b[wheel+2][station-1][sector-1];
	Cov_b(1,1) = c22_b[wheel+2][station-1][sector-1];
	Cov_b(1,2) = c23_b[wheel+2][station-1][sector-1];
	Cov_b(2,1) = c23_b[wheel+2][station-1][sector-1];
	Cov_b(2,2) = c33_b[wheel+2][station-1][sector-1];
	TMatrixD vec_b(3,1);
	vec_b(0,0) = b1_b[wheel+2][station-1][sector-1]; 
	vec_b(1,0) = b2_b[wheel+2][station-1][sector-1];
	vec_b(2,0) = b3_b[wheel+2][station-1][sector-1];
	
	//Cov_f.Print();
        //Cov_b.Print();

	Cov_f.Invert();
	Cov_b.Invert();
	
	TMatrixD solution_f = Cov_f * vec_f;
	TMatrixD solution_b = Cov_b * vec_b;

        //Fill the tree	
        myResult.wh = wheel;
        myResult.st = station;
        myResult.se = sector;
        myResult.dx_f = solution_f(0,0);
        myResult.dz_f = solution_f(1,0);
        myResult.alpha_f = solution_f(2,0);
        myResult.ex_f = sqrt(Cov_f(0,0));
        myResult.ez_f = sqrt(Cov_f(1,1));
        myResult.ealpha_f = sqrt(Cov_f(2,2));
        myResult.corr_xz_f = Cov_f(0,1)/sqrt(Cov_f(0,0)*Cov_f(1,1));
        myResult.corr_xalpha_f = Cov_f(0,2)/sqrt(Cov_f(0,0)*Cov_f(2,2));
        myResult.corr_zalpha_f = Cov_f(1,2)/sqrt(Cov_f(1,1)*Cov_f(2,2));
        myResult.dx_b = solution_b(0,0);
        myResult.dz_b = solution_b(1,0);
        myResult.alpha_b = solution_b(2,0);
        myResult.ex_b = sqrt(Cov_b(0,0));
        myResult.ez_b = sqrt(Cov_b(1,1));
        myResult.ealpha_b = sqrt(Cov_b(2,2));
        myResult.corr_xz_b = Cov_b(0,1)/sqrt(Cov_b(0,0)*Cov_b(1,1));
        myResult.corr_xalpha_b = Cov_b(0,2)/sqrt(Cov_b(0,0)*Cov_b(2,2));
        myResult.corr_zalpha_b = Cov_b(1,2)/sqrt(Cov_b(1,1)*Cov_b(2,2));

       outputTree->Fill();
	
      }
    }
  }

  myFile->Write();
  myFile->Close();
  
}




//Prepare the histograms
void prepareHisto(TH1D *histo, int col, float min, float max, char *labelY, char *title) {

  histo->SetLineColor(col);
  histo->SetMarkerColor(col);
  histo->SetMarkerStyle(21);
  histo->SetMarkerSize(0.5);
  histo->SetTitle(title);

  histo->GetXaxis()->SetBinLabel(1, "Sector 1");
  histo->GetXaxis()->SetBinLabel(2, "Sector 2");
  histo->GetXaxis()->SetBinLabel(3, "Sector 3");
  histo->GetXaxis()->SetBinLabel(4, "Sector 4");
  histo->GetXaxis()->SetBinLabel(5, "Sector 5");
  histo->GetXaxis()->SetBinLabel(6, "Sector 6");
  histo->GetXaxis()->SetBinLabel(7, "Sector 7");
  histo->GetXaxis()->SetBinLabel(8, "Sector 8");
  histo->GetXaxis()->SetBinLabel(9, "Sector 9");
  histo->GetXaxis()->SetBinLabel(10, "Sector 10");
  histo->GetXaxis()->SetBinLabel(11, "Sector 11");
  histo->GetXaxis()->SetBinLabel(12, "Sector 12");
  histo->GetXaxis()->SetBinLabel(13, "Sector 13");
  histo->GetXaxis()->SetBinLabel(14, "Sector 14");

  histo->GetYaxis()->SetRangeUser(min, max);
  histo->GetYaxis()->SetTitle(labelY);

}




void showPlots(char *name) {

  TFile *file = new TFile(name);
  TTree *t = (TTree *)file->Get("InternalAlignmentResult");
  t->SetBranchAddress("4DSegmentResult", &myResult);

  TH1D **** correctPlotdx = new TH1D *** [5];
  TH1D **** correctPlotdz = new TH1D *** [5];
  TH1D **** correctPlotalpha = new TH1D *** [5];

  for(int wheel = -2; wheel < 3; wheel++) {
    correctPlotdx[wheel+2] = new TH1D ** [4];
    correctPlotdz[wheel+2] = new TH1D ** [4];
    correctPlotalpha[wheel+2] = new TH1D ** [4];
    for(int station = 1; station < 5; station++) {
      correctPlotdx[wheel+2][station-1] = new TH1D * [2];
      correctPlotdz[wheel+2][station-1] = new TH1D * [2];
      correctPlotalpha[wheel+2][station-1] = new TH1D * [2];
      char nameFdx[50];
      sprintf(nameFdx, "DeltaXF_%d_%d", wheel, station);
      char nameBdx[50];
      sprintf(nameBdx, "DeltaXB_%d_%d", wheel, station);
      char nameFdz[50];
      sprintf(nameFdz, "DeltaZF_%d_%d", wheel, station);
      char nameBdz[50];
      sprintf(nameBdz, "DeltaZB_%d_%d", wheel, station);
      char nameFalpha[50];
      sprintf(nameFalpha, "AlphaF_%d_%d", wheel, station);
      char nameBalpha[50];
      sprintf(nameBalpha, "AlphaZB_%d_%d", wheel, station);

      int colF = 30;
      int colB = 50;
      correctPlotdx[wheel+2][station-1][0] = new TH1D(nameFdx, "name", 15, 0, 14);
      prepareHisto(correctPlotdx[wheel+2][station-1][0], colF , -0.4, 0.4, "#delta_{x} (cm)", "#Delta_{X} from superlayer 1 to superlayer 3");
      correctPlotdx[wheel+2][station-1][1] = new TH1D(nameBdx, "name", 15, 0, 14);
      prepareHisto(correctPlotdx[wheel+2][station-1][1], colB , -0.4, 0.4, "#delta_{x} (cm)", "#Delta_{X} from superlayer 3 to superlayer 1");
      
      correctPlotdz[wheel+2][station-1][0] = new TH1D(nameFdz, "name", 15, 0, 14);
      prepareHisto(correctPlotdz[wheel+2][station-1][0], colF , -0.4, 0.4, "#delta_{z} (cm)", "#Delta_{Z} from superlayer 1 to superlayer 3");
      correctPlotdz[wheel+2][station-1][1] = new TH1D(nameBdz, "name", 15, 0, 14);
      prepareHisto(correctPlotdz[wheel+2][station-1][1], colB , -0.4, 0.4, "#delta_{z} (cm)", "#Delta_{Z} from superlayer 3 to superlayer 1");

      correctPlotalpha[wheel+2][station-1][0] = new TH1D(nameFalpha, "name", 15, 0, 14);
      prepareHisto(correctPlotalpha[wheel+2][station-1][0], colF , -3, 3, "#alpha (mrad)", "#alpha from superlayer 1 to superlayer 3");
      correctPlotalpha[wheel+2][station-1][1] = new TH1D(nameBalpha, "name", 15, 0, 14);
      prepareHisto(correctPlotalpha[wheel+2][station-1][1], colB , -3, 3, "#alpha (mrad)", "#alpha from superlayer 3 to superlayer 1");
 
    }
  }


  
  Int_t nentries = (Int_t)t->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    t->GetEntry(i);
    correctPlotdx[myResult.wh+2][myResult.st-1][0]->SetBinContent(myResult.se, myResult.dx_f);
    correctPlotdx[myResult.wh+2][myResult.st-1][0]->SetBinError(myResult.se, myResult.ex_f);
    correctPlotdx[myResult.wh+2][myResult.st-1][1]->SetBinContent(myResult.se, -myResult.dx_b);
    correctPlotdx[myResult.wh+2][myResult.st-1][1]->SetBinError(myResult.se, myResult.ex_b);
    
    correctPlotdz[myResult.wh+2][myResult.st-1][0]->SetBinContent(myResult.se, myResult.dz_f);
    correctPlotdz[myResult.wh+2][myResult.st-1][0]->SetBinError(myResult.se, myResult.ez_f);
    correctPlotdz[myResult.wh+2][myResult.st-1][1]->SetBinContent(myResult.se, -myResult.dz_b);
    correctPlotdz[myResult.wh+2][myResult.st-1][1]->SetBinError(myResult.se, myResult.ez_b);

    float errormeandz1 = sqrt(myResult.ez_f*myResult.ez_f+myResult.ez_b*myResult.ez_b);
    float errormeandz2 = fabs(myResult.dz_f + myResult.dz_b);
    float errormeandx1 = sqrt(myResult.ex_f*myResult.ex_f+myResult.ex_b*myResult.ex_b);
    float errormeandx2 = fabs(myResult.dx_f + myResult.dx_b);
    float errordz = 0;
    if(errormeandz1 > errormeandz2) {
      errordz = errormeandz1;
    } else {
      errordz = errormeandz2;
    }
    float errordx = 0;
    if(errormeandx1 > errormeandx2) {
      errordx = errormeandx1;
    } else {
      errordx = errormeandx2;
    }

    //cout << myResult.wh << " " << myResult.st << " " << myResult.se << " " 
    //     << (myResult.dx_f - myResult.dx_b)/2.0 << " " << errordx << " " 
    //     << (myResult.dz_f - myResult.dz_b)/2.0 << " " << errordz << endl;
    cout << myResult.wh << " " << myResult.st << " " << myResult.se << " " 
         << myResult.dx_f << " " << errormeandx1 << " " 
         << myResult.dz_f << " " << errormeandz1 << " "
         << myResult.alpha_f << " " << myResult.ealpha_f << endl;

    correctPlotalpha[myResult.wh+2][myResult.st-1][0]->SetBinContent(myResult.se, 1000.0*myResult.alpha_f);
    correctPlotalpha[myResult.wh+2][myResult.st-1][0]->SetBinError(myResult.se, 1000.0*myResult.alpha_f);
    correctPlotalpha[myResult.wh+2][myResult.st-1][1]->SetBinContent(myResult.se, 1000.0*myResult.ealpha_b);
    correctPlotalpha[myResult.wh+2][myResult.st-1][1]->SetBinError(myResult.se, 1000.0*myResult.ealpha_b);

  }

  for(int wheel = -2; wheel < 3; wheel++) {
    for(int station = 1; station < 5; station++) {
      char nameCanvasX[50];
      sprintf(nameCanvasX, "canvasX_%d_%d.eps", wheel, station);
      char nameCanvasZ[50];
      sprintf(nameCanvasZ, "canvasZ_%d_%d.eps", wheel, station);
      char nameCanvasAlpha[50];
      sprintf(nameCanvasAlpha, "canvasAlpha_%d_%d.eps", wheel, station);
      TCanvas *mX = new TCanvas(nameCanvasX);
      TCanvas *mZ = new TCanvas(nameCanvasZ);
      TCanvas *mAlpha = new TCanvas(nameCanvasAlpha);
      mX->cd();
      correctPlotdx[wheel+2][station-1][0]->Draw();
      correctPlotdx[wheel+2][station-1][1]->Draw("SAMES");
      mX->SaveAs(nameCanvasX);
      mZ->cd();
      correctPlotdz[wheel+2][station-1][0]->Draw();
      correctPlotdz[wheel+2][station-1][1]->Draw("SAMES");
      mZ->SaveAs(nameCanvasZ);
      mAlpha->cd();
      correctPlotalpha[wheel+2][station-1][0]->Draw();
      correctPlotalpha[wheel+2][station-1][1]->Draw("SAMES");
      mAlpha->SaveAs(nameCanvasAlpha);
    }
  }

  file->Close();  



     

}



