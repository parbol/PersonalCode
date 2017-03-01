#include "TLorentzVector.h"
#include "TRandom3.h"
#include <iostream>
#include <string>

using namespace std;

class Particle {

  public:
  
  Particle(float, float, float, float);
  
  void Set(float, float, float, float);

  ~Particle();
  
  float Pt();

  float Eta();
  
  float Theta();

  float Phi();

  float Mass();

  float thetaToEta(float);


  void Print(string);

  TLorentzVector p();

  float fix(float);
  
  private:

  TLorentzVector theParticle;

  TRandom3 *ran;


};



Particle::Particle(float pt, float theta, float phi, float mass) {
  ran = new TRandom3(0);
  Set(pt, thetaToEta(theta), phi, mass);
}


void Particle::Set(float pt, float theta, float phi, float mass) {
  theParticle.SetPtEtaPhiM(pt, thetaToEta(theta), phi, mass);
}

float Particle::fix(float a) {

  if(a < 0) return a + 2.0*TMath::Pi();
  return a;

}

Particle::~Particle() {
  delete ran;
}


float Particle::thetaToEta(float theta) {

  TLorentzVector a(1, 0, 0, 1);
  a.SetTheta(theta);
  return a.Eta();

}


TLorentzVector Particle::p(){
  return theParticle;
}

float Particle::Pt() {
  return theParticle.Pt();
}


float Particle::Eta() {
  return theParticle.Eta();
}


float Particle::Theta() {
  return theParticle.Theta();
}


float Particle::Phi() {
  return theParticle.Phi();
}


float Particle::Mass() {
  return theParticle.M();
}


void Particle::Print(string id) {

  cout << "---- " << id << " ----" << endl;
  cout << "Mass: " << Mass() << endl;
  cout << "Phi: " << Phi() << endl;
  cout << "Eta: " << Eta() << endl;
  cout << "Pt: " << Pt() << endl;
  cout << "---------------------" << endl;

}



