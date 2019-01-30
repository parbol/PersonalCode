#include "TLorentzVector.h"
#include "TVector2.h"
#include "TRandom3.h"
#include "../interface/MassReconstructor.h"
#include "TFile.h"
#include "TH2F.h"

#include<iostream>
#include <vector>

#define THEDEBUG 1


//------------------------------------------------------------------------------------------------------------------------------------------------//
MassReconstructor::MassReconstructor(Int_t Ntoys_, TH1F *mlbshape_) {

    Ntoys      = Ntoys_;
    massSolver = new MassSolver();
    random = new TRandom3();
    mlbshape = mlbshape_;

}
//------------------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------------------------------//
MassReconstructor::~MassReconstructor() {

    delete massSolver;
    delete random;

}
//------------------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------------------------------//
void MassReconstructor::startVariations(TLorentzVector & lep1, TLorentzVector & lep2, std::vector<TLorentzVector> & bjets, std::vector<TLorentzVector> & jets, 
                                     TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w) {

        
    if(bjets.size() == 2) {
        //if(THEDEBUG) std::cout << "Going for only b-jets" << std::endl;
        startVariations(lep1, lep2, bjets, MET, top1, top2, w);
    }
    if((top1.Mod() == 0 && top2.Mod() == 0) || bjets.size() < 2) {
        //if(THEDEBUG) std::cout << "Trying all jets" << std::endl;
        startVariations(lep1, lep2, bjets[0], jets, MET, top1, top2, w);
    } 

}
//------------------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------------------------------//
void MassReconstructor::startVariations(TLorentzVector & lep1, TLorentzVector & lep2, std::vector<TLorentzVector> & bjets, TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w) {

    Float_t max_w = -1; 
    TVector2 theTop1, theTop2;

    for(std::vector<TLorentzVector>::iterator jet1 = bjets.begin(); jet1 != bjets.end(); jet1++) {    
        for(std::vector<TLorentzVector>::iterator jet2 = bjets.begin(); jet2 != bjets.end(); jet2++) {
            if(jet1 == jet2) continue;     
            Float_t localW;
            TVector2 localTop1, localTop2;
            //if(THEDEBUG) {
            //   std::cout << "Trying the following combination:" << std::endl;
            //   std::cout << "Lepton 1 ";  lep1.Print();
            //   std::cout << "Lepton 2 "; lep2.Print();
            //   std::cout << "Jet 1 "; jet1->Print();
            //   std::cout << "Jet 2 "; jet2->Print();
            //   std::cout << "MET "; MET.Print();
            //}     
            makePair(lep1, lep2, *jet1, *jet2, MET, localTop1, localTop2, localW);
            //if(THEDEBUG) std::cout << "W achieved is " << localW << std::endl;
            if(localW > max_w) {
                max_w = localW;
                theTop1 = localTop1;
                theTop2 = localTop2;
            }
        }
    }
    w = max_w;
    top1 = theTop1;
    top2 = theTop2;
    //if(THEDEBUG) std::cout << "Final W achieved is " << w << std::endl;

}            
//------------------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------------------------------//
void MassReconstructor::startVariations(TLorentzVector & lep1, TLorentzVector & lep2, TLorentzVector &bjet, std::vector<TLorentzVector> &jets, TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w) {

    Float_t max_w = -1; 
    TVector2 theTop1, theTop2;

    for(std::vector<TLorentzVector>::iterator jet = jets.begin(); jet != jets.end(); jet++) {    
        Float_t localW;
        TVector2 localTop1, localTop2;
        makePair(lep1, lep2, bjet, *jet, MET, localTop1, localTop2, localW);
        if(localW > max_w) {
            max_w = localW;
            theTop1 = localTop1;
            theTop2 = localTop2;
        }
    }
    for(std::vector<TLorentzVector>::iterator jet = jets.begin(); jet != jets.end(); jet++) {    
        Float_t localW;
        TVector2 localTop1, localTop2;
        makePair(lep1, lep2, *jet, bjet, MET, localTop1, localTop2, localW);
        if(localW > max_w) {
            max_w = localW;
            theTop1 = localTop1;
            theTop2 = localTop2;
        }
    } 

    w = max_w;
    top1 = theTop1;
    top2 = theTop2;

}            
//------------------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------------------------------------------------//
void MassReconstructor::makePair(TLorentzVector &lep1, TLorentzVector &lep2, TLorentzVector &jet1, TLorentzVector &jet2, TVector2 &MET, TVector2 &top1, TVector2 &top2, Float_t &w) {

     Float_t theW = 0;
     TVector2 theTop1(0, 0);
     TVector2 theTop2(0, 0);
     
     for(unsigned int i = 0; i < Ntoys; i++) {

        Float_t theUncJet1 = random->Gaus(0, 0.3) * jet1.E(); 
        Float_t theUncJet2 = random->Gaus(0, 0.3) * jet2.E(); 
        Float_t theMassTop1 = random->BreitWigner(173.34, 1);
        Float_t theMassTop2 = random->BreitWigner(173.34, 1);
        Float_t theMassW1   = random->BreitWigner(80.385, 0.015);
        Float_t theMassW2   = random->BreitWigner(80.385, 0.015);
        TLorentzVector newJet1(0,0,0,0);
        TLorentzVector newJet2(0,0,0,0);
        Float_t pCorrection1 = sqrt((jet1.E() + theUncJet1)*(jet1.E() + theUncJet1) - jet1.M()*jet1.M())/jet1.P();
        Float_t pCorrection2 = sqrt((jet2.E() + theUncJet2)*(jet2.E() + theUncJet2) - jet2.M()*jet2.M())/jet2.P();
        newJet1.SetPtEtaPhiM(jet1.Pt()*pCorrection1, jet1.Eta(), jet1.Phi(), jet1.M());
        newJet2.SetPtEtaPhiM(jet2.Pt()*pCorrection2, jet2.Eta(), jet2.Phi(), jet2.M());
        TVector2 DeltaP1(newJet1.Px() - jet1.Px(), newJet1.Py() - jet1.Py());
        TVector2 DeltaP2(newJet2.Px() - jet2.Px(), newJet2.Py() - jet2.Py());
        TVector2 newMET = MET + DeltaP1 + DeltaP2;
        //if(THEDEBUG) {
        //       std::cout << "After updating the energy:" << std::endl;
        //       std::cout << "Lepton 1 ";  lep1.Print();
        //       std::cout << "Lepton 2 "; lep2.Print();
        //       std::cout << "Jet 1 "; newJet1.Print();
        //       std::cout << "Jet 2 "; newJet2.Print();
        //       std::cout << "MET "; newMET.Print();
        //}  
        std::vector<TLorentzVector> nu1, nu2;
        //massSolver->solve(newMET, newJet1, newJet2, lep1, lep2, theMassW1, theMassW2, theMassTop1, theMassTop2, nu1, nu2);
        massSolver->solve(MET, newJet1, newJet2, lep1, lep2, theMassW1, theMassW2, theMassTop1, theMassTop2, nu1, nu2);
        
        if(nu1.size() == 0) {
            continue;
        }

        Float_t localW;
        TVector2 localTop1, localTop2;

        getWeight(lep1, lep2, newJet1, newJet2, nu1, nu2, localTop1, localTop2, localW);
        theW += localW;
        theTop1 += (localW * localTop1);
        theTop2 += (localW * localTop2);
     }

     w = theW;
     if(w != 0) {
         top1 = theTop1/theW;
         top2 = theTop2/theW;
     }

}
//------------------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------------------------------------------------//
void MassReconstructor::getWeight(TLorentzVector &lep1, TLorentzVector &lep2, TLorentzVector &jet1, TLorentzVector &jet2, std::vector<TLorentzVector> nu1, std::vector<TLorentzVector> nu2, 
                               TVector2 &top1, TVector2 &top2, Float_t &w) {


    //First we see which is the combination that gives the smallest (t1+t2).M() as suggested in the literature
    Float_t max_mass = 1e8;
    unsigned int index = -1;
    
    for(unsigned int i = 0; i < nu1.size(); i++) {
        Float_t mass = (lep1 + jet1 + nu1[i] + lep2 + jet2 + nu2[i]).M();
        if(mass < max_mass) {
            max_mass = mass;
            index = i;
        } 
    }

    //Once we have the index we calculate the weight and the top vector
    TLorentzVector theTop1 = (lep1 + jet1 + nu1[index]);
    TLorentzVector theTop2 = (lep2 + jet2 + nu2[index]);
    Float_t massW1 = (lep1 + nu1[index]).M();
    Float_t massW2 = (lep2 + nu2[index]).M();
    Float_t massb1 = jet1.M();
    Float_t massb2 = jet2.M();
    Float_t masst1 = theTop1.M(); 
    Float_t masst2 = theTop2.M();
    Float_t mtE1 = theTop1 * lep1; 
    Float_t mtE2 = theTop2 * lep2; 
    Float_t mlb1 = (lep1 + jet1).M();
    Float_t mlb2 = (lep2 + jet2).M();

    w = l_weight(mlb1) * l_weight(mlb2);
    
    top1.SetX(theTop1.Px()); top1.SetY(theTop1.Py());
    top2.SetX(theTop2.Px()); top2.SetY(theTop2.Py());

}
//------------------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------------------------------------------------//
Float_t MassReconstructor::l_weight(Float_t mtE, Float_t mb, Float_t mw, Float_t mt) {
    return 4.0 * mtE * (mt*mt - mb*mb - 2.0*mtE) / ((mt*mt - mb*mb)*(mt*mt - mb*mb) + mw*mw*(mt*mt - mb*mb)-2.0*mw*mw*mw*mw);
}
//------------------------------------------------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------------------------------------------------//
Float_t MassReconstructor::l_weight(Float_t mlb) {
    return mlbshape->GetBinContent(mlbshape->FindBin(mlb));
}
//------------------------------------------------------------------------------------------------------------------------------------------------//


