#ifndef MassReconstructor_class_h
#define MassReconstructor_class_h
#include "TLorentzVector.h"
#include "TVector2.h"
#include "../src/MassSolver.cc"
#include "TRandom3.h"
#include "TH1F.h"
#include<iostream>
#include <vector>

using namespace std;


class MassReconstructor 
{
    public:

        //Constructor: input is the number of toys to simulate per event  
        MassReconstructor(Int_t, TH1F *mlbshape);
        //Destructor
        ~MassReconstructor();
        //General call to start variations it decides whether we are in the 2 bjet case or 1 bjet case
        void startVariations(TLorentzVector & lep1, TLorentzVector & lep2, std::vector<TLorentzVector> & bjets, std::vector<TLorentzVector> & jets, TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w);
        //Call to startVariations in the case that we have 2 or more b-jets (only those are considered)
        void startVariations(TLorentzVector & lep1, TLorentzVector & lep2, std::vector<TLorentzVector> & bjets, TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w);
        //General case where we only have 1 b-jet
        void startVariations(TLorentzVector & lep1, TLorentzVector & lep2, TLorentzVector &bjet, std::vector<TLorentzVector> &jets, TVector2 & MET, TVector2 &top1, TVector2 &top2, Float_t &w);
        //Solves the system for a given combination of jets 
        void makePair(TLorentzVector &lep1, TLorentzVector &lep2, TLorentzVector &jet1, TLorentzVector &jet2, TVector2 &MET, TVector2 &top1, TVector2 &top2, Float_t &w);
        //Calculates top1 and top2 transverse momenta and also the weights
        void getWeight(TLorentzVector &lep1, TLorentzVector &lep2, TLorentzVector &jet1, TLorentzVector &jet2, std::vector<TLorentzVector> nu1, std::vector<TLorentzVector> nu2, 
                    TVector2 &top1, TVector2 &top2, Float_t &w);
        //Getting the weight for the lepton
        Float_t l_weight(Float_t mtE, Float_t mb, Float_t mw, Float_t mt);
        //Getting the weight for the lepton
        Float_t l_weight(Float_t mlb);
        
        MassSolver *massSolver;
  
    private: 

        Int_t Ntoys;
        TH1F *mlbshape;
        TRandom3   *random;


}; //end class MassReconstructor  

#endif


