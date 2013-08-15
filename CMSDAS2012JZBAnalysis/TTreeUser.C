#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TTree.h"
#include "TCut.h"
#include <vector>
#include <string>
#include <iostream>

class TTreeUser {

public:

  TTreeUser();

  ~TTreeUser();

  void Add(std::string, std::string, float, float, int color, std::string);

  void Project(TH1F *, std::string, TCut, std::string);
  
  void Project(TH2F *, std::string, TCut, std::string, std::string);
  
  void Project(THStack *, std::string, TCut, int, float, float, std::string);

  float getNumberOfEvents();

  std::vector<std::string> getNames();

private:

  std::vector<TFile *> files;
  std::vector<TTree *> trees;
  std::vector<float > scaleFactors;
  std::vector<float > numberOfEvents;
  std::vector<int> theColors;
  std::vector<std::string> names;

};




TTreeUser::TTreeUser() {

}



TTreeUser::~TTreeUser() {

  trees.erase(trees.begin(), trees.end());
  files.erase(files.begin(), files.end());
  scaleFactors.clear();

}


std::vector<std::string> TTreeUser::getNames() {return names;}


void TTreeUser::Add(std::string nameFile, std::string nameTree, float scale, float numEvents, int color, std::string name) {

  TFile *f = new TFile(nameFile.c_str());
  TTree *s = (TTree *) f->Get(nameTree.c_str());
  
  files.push_back(f);
  trees.push_back(s);
  scaleFactors.push_back(scale);  
  numberOfEvents.push_back(numEvents);
  theColors.push_back(color);
  names.push_back(name);
  //f->Close();


}


float TTreeUser::getNumberOfEvents() {

  std::vector<float>::iterator sca = scaleFactors.begin();
  std::vector<float>::iterator num = numberOfEvents.begin();
  float val = 0;
  for(std::vector<TTree *>::iterator t = trees.begin(); t != trees.end(); ++t, ++sca, ++num) {
    val += (*num) * (*sca);
  }
  return val;
}


void TTreeUser::Project(TH1F *h, std::string var, TCut cuts, std::string labelX) {

  h->Sumw2();
  h->GetXaxis()->SetTitle(labelX.c_str());
  std::vector<float>::iterator sca = scaleFactors.begin();
  for(std::vector<TTree *>::iterator t = trees.begin(); t != trees.end(); ++t, ++sca) {
    TH1F *haux = new TH1F("haux", "", h->GetXaxis()->GetNbins(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    haux->Sumw2();
    (*t)->Project("haux", var.c_str(), cuts, "goff");
    haux->GetXaxis()->SetTitle(labelX.c_str());
    haux->Scale(*sca);
    h->Add(haux);
    delete haux;
  }
}


void TTreeUser::Project(TH2F *h, std::string var, TCut cuts, std::string labelX, std::string labelY) {

  h->Sumw2();
  h->GetXaxis()->SetTitle(labelX.c_str());
  h->GetYaxis()->SetTitle(labelY.c_str());
  std::vector<float>::iterator sca = scaleFactors.begin();
  for(std::vector<TTree *>::iterator t = trees.begin(); t != trees.end(); ++t, ++sca) {
    TH2F *haux = new TH2F("haux", "", h->GetXaxis()->GetNbins(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax(), 
                                      h->GetYaxis()->GetNbins(), h->GetYaxis()->GetXmin(), h->GetYaxis()->GetXmax());
    (*t)->Project("haux", var.c_str(), cuts, "goff");
    haux->GetXaxis()->SetTitle(labelX.c_str());
    haux->GetYaxis()->SetTitle(labelY.c_str());
    haux->Sumw2();
    haux->Scale(*sca);
    h->Add(haux);
    delete haux;
  }
}


void TTreeUser::Project(THStack *h, std::string var, TCut cuts, int bin, float min_, float max_, std::string labelX) {

  std::vector<float>::iterator sca = scaleFactors.begin();
  std::vector<int>::iterator col = theColors.begin();
  std::vector<std::string>::iterator nam = names.begin();
  for(std::vector<TTree *>::iterator t = trees.begin(); t != trees.end(); ++t, ++sca, ++col, ++nam) {
    std::string nameOfOriginalHisto(h->GetName());
    std::string nameOfHisto = var + "_" + *nam + "_" + nameOfOriginalHisto;
    TH1F *haux = new TH1F(nameOfHisto.c_str(), "", bin, min_, max_);
    (*t)->Project(nameOfHisto.c_str(), var.c_str(), cuts, "goff");
    haux->GetXaxis()->SetTitle(labelX.c_str());
    haux->Sumw2();
    haux->Scale(*sca);
    haux->SetFillColor(*col);
    h->Add(haux);
    //delete haux;
  }



}
