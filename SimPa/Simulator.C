#include <iostream>
#include "TRandom3.h"
#include "TH1D.h"
#include "TLorentzVector.h"
#include "Particle.C"
#include "TFile.h"
#include "TTree.h"
#include "mt2_bisect.h"
using namespace std;




float getMT2(TLorentzVector, TLorentzVector, TLorentzVector);





//-----------------Data----------------------------
struct Event {
  
  float phiMED, etaMED, thetaMED, ptMED, pMED, mMED;
  float phiTOP1, etaTOP1, thetaTOP1, ptTOP1, pTOP1, mTOP1;
  float phiTOP2, etaTOP2, thetaTOP2, ptTOP2, pTOP2, mTOP2;
  
  float phiW1, etaW1, thetaW1, ptW1, pW1, mW1;
  float phiW2, etaW2, thetaW2, ptW2, pW2, mW2;
  
  float philep1, etalep1, thetalep1, ptlep1, plep1, mlep1;
  float philep2, etalep2, thetalep2, ptlep2, plep2, mlep2;
  
  float phinu1, etanu1, thetanu1, ptnu1, pnu1, mnu1;
  float phinu2, etanu2, thetanu2, ptnu2, pnu2, mnu2;
  
  float phib1, etab1, thetab1, ptb1, pb1, mb1;
  float phib2, etab2, thetab2, ptb2, pb2, mb2;
  
  float phiMET, etaMET, thetaMET, ptMET, pMET, mMET;

  float ptMETSmeared;

  float phiMETNu, etaMETNu, thetaMETNu, ptMETNu, pMETNu, mMETNu;
  float phiJets, etaJets, thetaJets, ptJets, pJets, mJets;
 
  float massTOP, massMED;

  float mt2, mt2Smeared;

  int nEvent;

} ev;



class Simulator {

  public:
  
  Simulator(size_t, float, float, string);
  ~Simulator();

  float angleT(TLorentzVector, TLorentzVector);
  float angle(TLorentzVector, TLorentzVector);
  float fix(float);
  void setBranches(TTree *);
  void ProduceDecay(Particle *, Particle *, Particle *, float, float, bool, float, bool, float);
  void Produce3Decay(Particle *, Particle *, Particle *, Particle *, float, float, float, bool, float, bool, float);
  float thetaToEta(float);
  
};
     

Simulator::~Simulator(){}


//Simulator::Simulator(size_t num=1000, string name="output.root", float massGlu = 700, float massLSP = 500, float x = 0.5) {
Simulator::Simulator(size_t num, float massTOP_, float massMED_, string name) {

  
  

  TRandom3 *b = new TRandom3(0);

  float s_eff = 900.0 + fabs(b->Gaus(0, 400));
  float massTOP = massTOP_;
  float massMED = massMED_;
  float massJets = 3;

  ev.nEvent = 1;
  ev.massTOP = massTOP_;
  ev.massMED = massMED_;


  TFile *f = new TFile(name.c_str(), "RECREATE");
  f->cd();
  
  TTree *s = new TTree("events","events");
  setBranches(s);  


  for(size_t i = 0; i < num; ++i) {

    
    float phi1 = b->Uniform(0, 2.0*TMath::Pi());
    float u1 = b->Uniform(0, 1.0);
    float theta1 = TMath::ACos(2.0*u1 - 1.0);
    float phi2, theta2;
    if(massMED > 0) {
        phi2 = b->Uniform(0, 2.0*TMath::Pi());
        float u2 = b->Uniform(0, 1.0);
        theta2 = TMath::ACos(2.0*u2 - 1.0);
    } else {
        phi2 = phi1 + TMath::Pi();
        theta2 = TMath::Pi() - theta1;
    }

    float E1 = 0;
    float E2 = 0;
    while(1) {
        if(massMED > 0) {
            E1 = b->Uniform(0, 1.0) * s_eff;
            E2 = b->Uniform(0, 1.0) * (s_eff-E1);
            if(E1 > massTOP && E2 > massTOP && (s_eff-E1-E2) > ev.massMED) break;
        } else { 
            E1 = s_eff/2.0;
            E2 = s_eff/2.0;
            break;
        }
    }


    float p1 = sqrt(E1*E1-massTOP*massTOP);
    float p2 = sqrt(E2*E2-massTOP*massTOP);
    float pt1 = p1 * sin(theta1); 
    float pt2 = p2 * sin(theta2); 
  
    TLorentzVector zero(0, 0, 0, 0);
    zero.SetPtEtaPhiM(0, 0, 0, s_eff);
    TLorentzVector topt1(0, 0, 0, 0);
    topt1.SetPtEtaPhiM(pt1, thetaToEta(theta1), phi1, massTOP);
    TLorentzVector topt2(0, 0, 0, 0);
    topt2.SetPtEtaPhiM(pt2, thetaToEta(theta2), phi2, massTOP);
    TLorentzVector medt = (zero-topt1-topt2);

    Particle top1(pt1, theta1, phi1, massTOP);
    Particle top2(pt2, theta2, phi2, massTOP);
    Particle med(medt.Pt(), medt.Theta(), medt.Phi(), massMED);

    ev.phiTOP1 = top1.Phi();
    ev.etaTOP1 = top1.Eta();
    ev.thetaTOP1 = top1.Theta();
    ev.pTOP1 = top1.p().P();
    ev.ptTOP1 = top1.Pt();
    ev.mTOP1 = top1.Mass();
    ev.phiTOP2 = top2.Phi();
    ev.etaTOP2 = top2.Eta();
    ev.thetaTOP2 = top2.Theta();
    ev.pTOP2 = top2.p().P();
    ev.ptTOP2 = top2.Pt();
    ev.mTOP2 = top2.Mass();
    ev.phiMED = med.Phi();
    ev.etaMED = med.Eta();
    ev.thetaMED = med.Theta();
    ev.pMED = med.p().P();
    ev.ptMED = med.Pt();
    ev.mMED = med.Mass();
 
    Particle W1(0.1, 0.1, 0.1, 0.1); 
    Particle W2(0.1, 0.1, 0.1, 0.1); 
    Particle lep1(0.1, 0.1, 0.1, 0.1); 
    Particle lep2(0.1, 0.1, 0.1, 0.1); 
    Particle nu1(0.1, 0.1, 0.1, 0.1); 
    Particle nu2(0.1, 0.1, 0.1, 0.1); 
    Particle b1(0.1, 0.1, 0.1, 0.1); 
    Particle b2(0.1, 0.1, 0.1, 0.1); 


    ProduceDecay(&top1, &W1, &b1, 80.4, 3, false, 0, false, TMath::Pi()/2.0);
    ProduceDecay(&top2, &W2, &b2, 80.4, 3, false, 0, false, TMath::Pi()/2.0);

  
    ev.phiW1 = fix(W1.Phi());
    ev.etaW1 = W1.Eta();
    ev.thetaW1 = W1.Theta();
    ev.pW1 = W1.p().P();
    ev.ptW1 = W1.Pt();
    ev.mW1 = W1.Mass();

    ev.phib1 = fix(b1.Phi());
    ev.etab1 = b1.Eta();
    ev.thetab1 = b1.Theta();
    ev.pb1 = b1.p().P();
    ev.ptb1 = b1.Pt();
    ev.mb1 = b1.Mass();
   
    ev.phiW2 = fix(W2.Phi());
    ev.etaW2 = W2.Eta();
    ev.thetaW2 = W2.Theta();
    ev.pW2 = W2.p().P();
    ev.ptW2 = W2.Pt();
    ev.mW2 = W2.Mass();

    ev.phib2 = fix(b2.Phi());
    ev.etab2 = b2.Eta();
    ev.thetab2 = b2.Theta();
    ev.pb2 = b2.p().P();
    ev.ptb2 = b2.Pt();
    ev.mb2 = b2.Mass();


    ProduceDecay(&W1, &lep1, &nu1, 0.02, 0.02, false, 0, false, TMath::Pi()/2.0);
    ProduceDecay(&W2, &lep2, &nu2, 0.02, 0.02, false, 0, false, TMath::Pi()/2.0);


    ev.philep1 = fix(lep1.Phi());
    ev.etalep1 = lep1.Eta();
    ev.thetalep1 = lep1.Theta();
    ev.plep1 = lep1.p().P();
    ev.ptlep1 = lep1.Pt();
    ev.mlep1 = lep1.Mass();

    ev.phinu1 = fix(nu1.Phi());
    ev.etanu1 = nu1.Eta();
    ev.thetanu1 = nu1.Theta();
    ev.pnu1 = nu1.p().P();
    ev.ptnu1 = nu1.Pt();
    ev.mnu1 = nu1.Mass();

    ev.philep2 = fix(lep2.Phi());
    ev.etalep2 = lep2.Eta();
    ev.thetalep2 = lep2.Theta();
    ev.plep2 = lep2.p().P();
    ev.ptlep2 = lep2.Pt();
    ev.mlep2 = lep2.Mass();

    ev.phinu2 = fix(nu2.Phi());
    ev.etanu2 = nu2.Eta();
    ev.thetanu2 = nu2.Theta();
    ev.pnu2 = nu2.p().P();
    ev.ptnu2 = nu2.Pt();
    ev.mnu2 = nu2.Mass();


    TLorentzVector theMET;
    if(massMED > 0) {
        theMET = med.p() + nu1.p() + nu2.p();
    } else {
        theMET = nu1.p() + nu2.p();
    }


    TLorentzVector theMETSmeared = theMET;
    Float_t corr1 = b1.Pt() * fabs(b->Gaus(0, 0.2));
    Float_t corr2 = b2.Pt() * fabs(b->Gaus(0, 0.2));
    float_t corr1x = corr1 * cos(b1.Phi());
    float_t corr2x = corr2 * cos(b2.Phi());
    float_t corr1y = corr1 * sin(b1.Phi());
    float_t corr2y = corr2 * sin(b2.Phi());

    theMETSmeared.SetPx(theMET.Px() + corr1x + corr2x);
    theMETSmeared.SetPy(theMET.Py() + corr1y + corr2y);

    ev.phiMET = fix(theMET.Phi());
    ev.etaMET = theMET.Eta();
    ev.thetaMET = theMET.Theta();
    ev.pMET = theMET.P();
    ev.ptMET = theMET.Pt();
    ev.ptMETSmeared = theMETSmeared.Pt();
    ev.mMET = theMET.M();

    TLorentzVector theMETNu;
    theMETNu = nu1.p() + nu2.p();
    ev.phiMETNu = fix(theMETNu.Phi());
    ev.etaMETNu = theMETNu.Eta();
    ev.thetaMETNu = theMETNu.Theta();
    ev.pMETNu = theMETNu.P();
    ev.ptMETNu = theMETNu.Pt();
    ev.mMETNu = theMETNu.M();

    TLorentzVector theJets = b1.p() + b2.p();
    ev.phiJets = fix(theJets.Phi());
    ev.etaJets = theJets.Eta();
    ev.thetaJets = theJets.Theta();
    ev.pJets = theJets.P();
    ev.ptJets = theJets.Pt();
    ev.mJets = theJets.M();

    TLorentzVector theZ = lep1.p() + lep2.p();
 
    ev.mt2 = getMT2(lep1.p(), lep2.p(), theMET);
    ev.mt2Smeared = getMT2(lep1.p(), lep2.p(), theMETSmeared);

    s->Fill();
    ev.nEvent++;

  }

  f->Write();
  f->Close();
  
  delete b;

}


void Simulator::setBranches(TTree *s) {
  
  s->Branch("massTOP", &ev.massTOP, "massTOP/F");
  s->Branch("massMED", &ev.massMED, "massMED/F");
  s->Branch("nEvent", &ev.nEvent, "nEvent/I");

  s->Branch("phiTOP1", &ev.phiTOP1, "phiTOP1/F");
  s->Branch("etaTOP1", &ev.etaTOP1, "etaTOP1/F");
  s->Branch("thetaTOP1", &ev.thetaTOP1, "thetaTOP1/F");
  s->Branch("ptTOP1", &ev.ptTOP1, "ptTOP1/F");
  s->Branch("pTOP1", &ev.pTOP1, "pTOP1/F");
  s->Branch("mTOP1", &ev.mTOP1, "mTOP1/F");

  s->Branch("phiTOP2", &ev.phiTOP2, "phiTOP2/F");
  s->Branch("etaTOP2", &ev.etaTOP2, "etaTOP2/F");
  s->Branch("thetaTOP2", &ev.thetaTOP2, "thetaTOP2/F");
  s->Branch("ptTOP2", &ev.ptTOP2, "ptTOP2/F");
  s->Branch("pTOP2", &ev.pTOP2, "pTOP2/F");
  s->Branch("mTOP2", &ev.mTOP2, "mTOP2/F");

  s->Branch("phiMED", &ev.phiMED, "phiMED/F");
  s->Branch("etaMED", &ev.etaMED, "etaMED/F");
  s->Branch("thetaMED", &ev.thetaMED, "thetaMED/F");
  s->Branch("ptMED", &ev.ptMED, "ptMED/F");
  s->Branch("pMED", &ev.pMED, "pMED/F");
  s->Branch("mMED", &ev.mMED, "mMED/F");

  s->Branch("phiW1", &ev.phiW1, "phiW1/F");
  s->Branch("etaW1", &ev.etaW1, "etaW1/F");
  s->Branch("thetaW1", &ev.thetaW1, "thetaW1/F");
  s->Branch("ptW1", &ev.ptW1, "ptW1/F");
  s->Branch("pW1", &ev.pW1, "pW1/F");
  s->Branch("mW1", &ev.mW1, "mW1/F");
  
  s->Branch("phiW2", &ev.phiW2, "phiW2/F");
  s->Branch("etaW2", &ev.etaW2, "etaW2/F");
  s->Branch("thetaW2", &ev.thetaW2, "thetaW2/F");
  s->Branch("ptW2", &ev.ptW2, "ptW2/F");
  s->Branch("pW2", &ev.pW2, "pW2/F");
  s->Branch("mW2", &ev.mW2, "mW2/F");


  s->Branch("phib1", &ev.phib1, "phib1/F");
  s->Branch("etab1", &ev.etab1, "etab1/F");
  s->Branch("thetab1", &ev.thetab1, "thetab1/F");
  s->Branch("ptb1", &ev.ptb1, "ptb1/F");
  s->Branch("pb1", &ev.pb1, "pb1/F");
  s->Branch("mb1", &ev.mb1, "mb1/F");
  
  s->Branch("phib2", &ev.phib2, "phib2/F");
  s->Branch("etab2", &ev.etab2, "etab2/F");
  s->Branch("thetab2", &ev.thetab2, "thetab2/F");
  s->Branch("ptb2", &ev.ptb2, "ptb2/F");
  s->Branch("pb2", &ev.pb2, "pb2/F");
  s->Branch("mb2", &ev.mb2, "mb2/F");

  s->Branch("philep1", &ev.philep1, "philep1/F");
  s->Branch("etalep1", &ev.etalep1, "etalep1/F");
  s->Branch("thetalep1", &ev.thetalep1, "thetalep1/F");
  s->Branch("ptlep1", &ev.ptlep1, "ptlep1/F");
  s->Branch("plep1", &ev.plep1, "plep1/F");
  s->Branch("mlep1", &ev.mlep1, "mlep1/F");

  s->Branch("philep2", &ev.philep2, "philep2/F");
  s->Branch("etalep2", &ev.etalep2, "etalep2/F");
  s->Branch("thetalep2", &ev.thetalep2, "thetalep2/F");
  s->Branch("ptlep2", &ev.ptlep2, "ptlep2/F");
  s->Branch("plep2", &ev.plep2, "plep2/F");
  s->Branch("mlep2", &ev.mlep2, "mlep2/F");

  s->Branch("phinu1", &ev.phinu1, "phinu1/F");
  s->Branch("etanu1", &ev.etanu1, "etanu1/F");
  s->Branch("thetanu1", &ev.thetanu1, "thetanu1/F");
  s->Branch("ptnu1", &ev.ptnu1, "ptnu1/F");
  s->Branch("pnu1", &ev.pnu1, "pnu1/F");
  s->Branch("mnu1", &ev.mnu1, "mnu1/F");

  s->Branch("phinu2", &ev.phinu2, "phinu2/F");
  s->Branch("etanu2", &ev.etanu2, "etanu2/F");
  s->Branch("thetanu2", &ev.thetanu2, "thetanu2/F");
  s->Branch("ptnu2", &ev.ptnu2, "ptnu2/F");
  s->Branch("pnu2", &ev.pnu2, "pnu2/F");
  s->Branch("mnu2", &ev.mnu2, "mnu2/F");


  s->Branch("phiMET", &ev.phiMET, "phiMET/F");
  s->Branch("etaMET", &ev.etaMET, "etaMET/F");
  s->Branch("thetaMET", &ev.thetaMET, "thetaMET/F");
  s->Branch("ptMET", &ev.ptMET, "ptMET/F");
  s->Branch("ptMETSmeared", &ev.ptMETSmeared, "ptMETSmeared/F");
  s->Branch("pMET", &ev.pMET, "pMET/F");
  s->Branch("mMET", &ev.mMET, "mMET/F");
  
  s->Branch("phiMETNu", &ev.phiMETNu, "phiMETNu/F");
  s->Branch("etaMETNu", &ev.etaMETNu, "etaMETNu/F");
  s->Branch("thetaMETNu", &ev.thetaMETNu, "thetaMETNu/F");
  s->Branch("ptMETNu", &ev.ptMETNu, "ptMETNu/F");
  s->Branch("pMETNu", &ev.pMETNu, "pMETNu/F");
  s->Branch("mMETNu", &ev.mMETNu, "mMETNu/F");
  
  s->Branch("phiJets", &ev.phiJets, "phiJets/F");
  s->Branch("etaJets", &ev.etaJets, "etaJets/F");
  s->Branch("thetaJets", &ev.thetaJets, "thetaJets/F");
  s->Branch("ptJets", &ev.ptJets, "ptJets/F");
  s->Branch("pJets", &ev.pJets, "pJets/F");
  s->Branch("mJets", &ev.mJets, "mJets/F");

  s->Branch("mt2", &ev.mt2, "mt2/F");
  s->Branch("mt2Smeared", &ev.mt2Smeared, "mt2Smeared/F");

}


float Simulator::angleT(TLorentzVector aa, TLorentzVector bb) {

  float aX = aa.Px();
  float aY = aa.Py();
  float bX = bb.Px();
  float bY = bb.Py();
  float aT = aa.Pt();
  float bT = bb.Pt();

  float tangle = acos((aX*bX+aY*bY)/(aT*bT)); 
  return tangle;

}



float Simulator::angle(TLorentzVector aa, TLorentzVector bb) {

  float aX = aa.Px();
  float aY = aa.Py();
  float aZ = aa.Pz();
  float bX = bb.Px();
  float bY = bb.Py();
  float bZ = bb.Pz();

  float aT = aa.P();
  float bT = bb.P();

  float tangle = acos((aX*bX+aY*bY+aZ*bZ)/(aT*bT)); 
  return tangle;

}


float Simulator::fix(float a) {   
  if(a < 0) return a + 2.0*TMath::Pi();
  return a;
}


float Simulator::thetaToEta(float theta) {

  TLorentzVector a(1, 0, 0, 1);
  a.SetTheta(theta);
  return a.Eta();

}




void Simulator::ProduceDecay(Particle *mom, Particle *d1, Particle *d2, float mass1, float mass2, bool phifix, float phiFix, bool thetafix, float thetaFix) {
  
  TRandom3 *ran = new TRandom3(0);

  float m_mass = mom->Mass();
  
  float aux1 = (m_mass*m_mass-mass1*mass1-mass2*mass2)*(m_mass*m_mass-mass1*mass1-mass2*mass2) - 4.0*mass1*mass1*mass2*mass2;
  float aux2 = 4.0*(m_mass*m_mass);
  float pk = TMath::Sqrt(aux1/aux2);

  float phi1;
  if(phifix) {
    phi1 = phiFix;
  } else {
    phi1 = ran->Uniform(0, 2.0*TMath::Pi());
  }

  float u1, theta1;
  if(thetafix) {
    theta1 = thetaFix;
  } else {
    u1 = ran->Uniform(0, 1.0);
    theta1 = TMath::ACos(2.0*u1 - 1.0);
  }

  float phi2 = phi1 + TMath::Pi();
  float theta2 = TMath::Pi() - theta1;

  float eta1 = thetaToEta(theta1);
  float eta2 = thetaToEta(theta2);
  float pt1 = fabs(pk*sin(theta1));
  float pt2 = fabs(pk*sin(theta2));

  TLorentzVector son1_Mother;
  TLorentzVector son2_Mother;
  son1_Mother.SetPtEtaPhiM(pt1, eta1, phi1, mass1);
  son2_Mother.SetPtEtaPhiM(pt2, eta2, phi2, mass2);

  TVector3 boostvector = mom->p().BoostVector();
  son1_Mother.Boost(boostvector);
  son2_Mother.Boost(boostvector);

  d1->Set(son1_Mother.Pt(), fix(son1_Mother.Theta()), fix(son1_Mother.Phi()), mass1);
  d2->Set(son2_Mother.Pt(), fix(son2_Mother.Theta()), fix(son2_Mother.Phi()), mass2);

  delete ran;
}




void Simulator::Produce3Decay(Particle *mom, Particle *d1, Particle *d2, Particle *d3, float mass1, float mass2, float mass3, bool phifix, float phiFix, bool thetafix, float thetaFix) {
  
  TRandom3 *ran = new TRandom3(0);
  
  float m_mass = mom->Mass();
  float E1 = ran->Uniform(mass1, (m_mass*m_mass+mass1*mass1-(mass2+mass3)*(mass2+mass3))/(2.0*m_mass));
  float P1 = sqrt(E1*E1-mass1*mass1);
  float Phi1 = ran->Uniform(0, 2.0*TMath::Pi());
  float Theta1 = ran->Uniform(0, TMath::Pi());
  float P23 = P1;
  float Phi23 = Phi1 + TMath::Pi();
  float Theta23 = TMath::Pi()-Theta1;
  float m232 = (-2.0*m_mass*E1 + m_mass*m_mass + mass1*mass1);
  float E2 = sqrt(m232+P23*P23);

  TLorentzVector Particle1;
  Particle1.SetPtEtaPhiM(P1*sin(Theta1), thetaToEta(Theta1), Phi1, mass1);
  TLorentzVector Particle23;
  Particle23.SetPtEtaPhiM(P23*sin(Theta23), thetaToEta(Theta23), Phi23, TMath::Sqrt(m232));
  
  float aux1 = (m232-mass2*mass2-mass3*mass3)*(m232-mass2*mass2-mass3*mass3) - 4.0*mass2*mass2*mass3*mass3;
  float aux2 = 4.0*(m232);
  float pk = TMath::Sqrt(aux1/aux2);
  float Theta2 = ran->Uniform(0, TMath::Pi());
  float Theta3 = TMath::Pi() - Theta2;
  float Phi2 = ran->Uniform(0, 2.0*TMath::Pi());
  float Phi3 = TMath::Pi() + Phi2;
  
  TLorentzVector Particle2, Particle3;
  Particle2.SetPtEtaPhiM(pk*sin(Theta2), thetaToEta(Theta2), Phi2, mass2);
  Particle3.SetPtEtaPhiM(pk*sin(Theta3), thetaToEta(Theta3), Phi3, mass3);
  
  TVector3 boostvector23 = Particle23.BoostVector();
  Particle2.Boost(boostvector23);
  Particle3.Boost(boostvector23);
  
  TVector3 boostvector = mom->p().BoostVector();
  Particle1.Boost(boostvector);
  Particle2.Boost(boostvector);
  Particle3.Boost(boostvector);
  
  d1->Set(Particle1.Pt(), fix(Particle1.Theta()), fix(Particle1.Phi()), mass1);
  d2->Set(Particle2.Pt(), fix(Particle2.Theta()), fix(Particle2.Phi()), mass2);
  d3->Set(Particle3.Pt(), fix(Particle3.Theta()), fix(Particle3.Phi()), mass3);
  
  delete ran;
    
} 





float getMT2(TLorentzVector l1, TLorentzVector l2, TLorentzVector MET) {

  double pa[3] = {0.106, l1.Px(), l1.Py()};
  double pb[3] = {0.106, l2.Px(), l2.Py()};
  double pmiss[3] = {0, MET.Px(), MET.Py()};
  double mn = 0.0;
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pa,pb,pmiss);
  mt2_event.set_mn(mn);
  return (float) mt2_event.get_mt2();

}


