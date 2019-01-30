#include "TLorentzVector.h"
#include "TVector2.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TFile.h"
#include "../interface/MassVariations.h"

#include<iostream>
#include <vector>

#define debug 0


MassVariations::MassVariations() {

    massSolver = new MassSolver();
    random = new TRandom3();

}


MassVariations::~MassVariations() {

    delete massSolver;
    delete random;

}


void MassVariations::performAllVariations(Int_t NtoysJec, Int_t NtoysMassTop, Int_t NtoysMassW, TLorentzVector & lep1, TLorentzVector & lep2, 
                                     std::vector<TLorentzVector> & jets, std::vector<Float_t> & unc, TVector2 & MET, TVector2 & METnu, std::vector<TLorentzVector> &nu1, std::vector<TLorentzVector> &nu2) {

    long double maxCost = 1e100;
    int indexjet1 = -1, indexjet2 = -1;
    int theJet1 = 0, theJet2 = 1;

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::vector<Float_t>::iterator unc1 = unc.begin();
    for(std::vector<TLorentzVector>::iterator jet1 = jets.begin(); jet1 != jets.end(); jet1++, unc1++) {    

        indexjet1++;
        std::vector<Float_t>::iterator unc2 = unc.begin();
        indexjet2 = -1;

        for(std::vector<TLorentzVector>::iterator jet2 = jets.begin(); jet2 != jets.end(); jet2++, unc2++) {

            indexjet2++;
            if(jet1 == jet2) continue;     
            double d = massSolver->cost(MET, *jet1, *jet2, lep1, lep2, 80.385, 80.385, 173.34, 173.34);
            if(d < maxCost) {
                theJet1 = indexjet1;
                theJet2 = indexjet2;
                maxCost = d;
            }
            std::cout << "Trying jet " << indexjet1 << " with jet " << indexjet2 << ". Score = " << maxCost << std::endl;  
        }
    }  


    std::cout << "Jet " << theJet1 << " and jet " << theJet2 << " were selected." << std::endl;  
    vector<double> metx;
    vector<double> mety;
    TVector2 separator(20, 20);
    TVector2 newMET = MET;
    double lambda = 1;
    int solutionFound = 0;
    long double last = 1e300;
    for(unsigned int i = 0; i < 600; i++) {
        long double d = massSolver->cost(newMET, jets[theJet1], jets[theJet2], lep1, lep2, 80.385, 80.385, 173.34, 173.34);
        std::cout << "Iteration " << i << " with real MET:(" << METnu.X() << ", " << METnu.Y() << ") and MET:(" << newMET.X() << ", " << newMET.Y() << "), cost " << d << " and lambda: " << lambda << std::endl;
        nu1.clear();
        nu2.clear();
        massSolver->solve(newMET, jets[theJet1], jets[theJet2], lep1, lep2, 80.385, 80.385, 173.34, 173.34, nu1, nu2);
        if(nu1.size() != 0) {
            solutionFound = 1;
            std::cout << "Solution found!" << std::endl;
            break;
        }
        //if(d > last) newMET += separator;
        metx.push_back(newMET.X());
        mety.push_back(newMET.X());
        if(d > last) lambda = lambda * 0.7;
        vector<double> grad;
        massSolver->gradient(newMET, jets[theJet1], jets[theJet2], lep1, lep2, 80.385, 80.385, 173.34, 173.34, grad, 0.05, lambda);
        TVector2 gradi(grad[0], grad[1]);
        newMET = newMET + gradi; 
        last = d;
    }
    double *met_x = new double[metx.size()];
    double *met_y = new double[mety.size()];
    for(unsigned int i = 0; i < metx.size(); i++) {
        met_x[i] = metx[i];
        met_y[i] = mety[i];
    }
    TGraph *graph = new TGraph(metx.size(), met_x, met_y);
    TFile f("output2.root", "recreate");
    f.cd();
    graph->Write();
    f.Close();
}




Int_t MassVariations::variations(Int_t NtoysJec, Int_t NtoysMassTop, Int_t NtoysMassW, TLorentzVector & lep1, TLorentzVector & lep2, TLorentzVector & jet1, TLorentzVector & jet2, 
                                Float_t unc1, Float_t unc2, TVector2 &MET, std::vector<TLorentzVector> &nu1, std::vector<TLorentzVector> &nu2) {



    if(debug == 1) {
        std::cout << "[MassVariation] Starting combination:" << std::endl;;
        std::cout << "[MassVariation] Lepton 1 "; lep1.Print();
        std::cout << "[MassVariation] Jet 1 "; jet1.Print();
        std::cout << "[MassVariation] Lepton 2 "; lep2.Print();
        std::cout << "[MassVariation] Jet 2 "; jet2.Print();
        std::cout << "[MassVariation] MET "; MET.Print();
    }

    for(Int_t masswcounter = 0; masswcounter < NtoysMassW; masswcounter++) {
    
        for(Int_t masstopcounter = 0; masstopcounter < NtoysMassTop; masstopcounter++) {

            for(Int_t massjetcounter = 0; massjetcounter < NtoysJec; massjetcounter++) {

                Float_t theMassW1   = random->BreitWigner(80.385, 0.015);
                Float_t theMassTop1 = random->BreitWigner(173.34, 1);
                Float_t theMassW2   = random->BreitWigner(80.385, 0.015);
                Float_t theMassTop2 = random->BreitWigner(173.34, 1);
                Float_t theUncJet1  = random->Gaus(0, unc1);
                Float_t theUncJet2  = random->Gaus(0, unc2);

                if(debug == 1) {
                    std::cout << "[MassVariation] Trying: M(W1): " << theMassW1 << " M(W2): " << theMassW2 << " M(top1): " << theMassTop1 << " M(top2): " << theMassTop2;
                    std::cout << " Unc(Jet 1): " << theUncJet1 << " Unc(Jet 2): " << theUncJet2 << std::endl;
                }
                
                TLorentzVector newJet1(0,0,0,0);
                TLorentzVector newJet2(0,0,0,0);
                Float_t pCorrection1 = sqrt((jet1.E() + theUncJet1)*(jet1.E() + theUncJet1) - jet1.M()*jet1.M())/jet1.P();
                Float_t pCorrection2 = sqrt((jet2.E() + theUncJet2)*(jet2.E() + theUncJet2) - jet2.M()*jet2.M())/jet2.P();   
                newJet1.SetPtEtaPhiM(jet1.Pt()*pCorrection1, jet1.Eta(), jet1.Phi(), jet1.M());
                newJet2.SetPtEtaPhiM(jet2.Pt()*pCorrection2, jet2.Eta(), jet2.Phi(), jet2.M());
                
                TVector2 DeltaP1(newJet1.Px() - jet1.Px(), newJet1.Py() - jet1.Py());
                TVector2 DeltaP2(newJet2.Px() - jet2.Px(), newJet2.Py() - jet2.Py());
                TVector2 newMET = MET + DeltaP1 + DeltaP2;

                nu1.clear(); 
		nu2.clear();		

                massSolver->solve(newMET, newJet1, newJet2, lep1, lep2, theMassW1, theMassW2, theMassTop1, theMassTop2, nu1, nu2);

                if(nu1.size() != 0) return 1; 
           
           }

      }

   } 
   return 0;

}
