#include "Alignment/MuonAlignmentAlgorithms/interface/DTMuonLocalAlignment.h"



DTMuonLocalAlignment::DTMuonLocalAlignment(){}
DTMuonLocalAlignment::~DTMuonLocalAlignment(){}


void DTMuonLocalAlignment::initNTuples() {

  tali = new TChain("Residual1D");

  for(int n_file = 0; n_file < numberOfRootFiles; ++n_file) {
    char theNameOfTheFile[150];
    sprintf(theNameOfTheFile, "%sMyNtupleResidual_%d.root", ntuplePath.c_str(), n_file);
    tali->Add(theNameOfTheFile);
  }

  setBranchAddressTree();

}




void DTMuonLocalAlignment::setBranchAddressTree() {

  tali->SetBranchAddress("p", &p);
  tali->SetBranchAddress("pt", &pt);
  tali->SetBranchAddress("eta", &eta);
  tali->SetBranchAddress("phi", &phi);
  tali->SetBranchAddress("charge", &charge);
  tali->SetBranchAddress("nseg", &nseg);
  tali->SetBranchAddress("nhits", nhits);
  tali->SetBranchAddress("xSl", xSl);
  tali->SetBranchAddress("dxdzSl", dxdzSl);
  tali->SetBranchAddress("exSl", exSl);
  tali->SetBranchAddress("edxdzSl", edxdzSl);
  tali->SetBranchAddress("exdxdzSl", edxdzSl);
  tali->SetBranchAddress("ySl", ySl);
  tali->SetBranchAddress("dydzSl", dydzSl);
  tali->SetBranchAddress("eySl", eySl);
  tali->SetBranchAddress("edydzSl", edydzSl);
  tali->SetBranchAddress("eydydzSl", eydydzSl);
  tali->SetBranchAddress("xSlSL1", xSlSL1);
  tali->SetBranchAddress("dxdzSlSL1", dxdzSlSL1);
  tali->SetBranchAddress("exSlSL1", exSlSL1);
  tali->SetBranchAddress("edxdzSlSL1", edxdzSlSL1);
  tali->SetBranchAddress("xSL1SL3", xSL1SL3);
  tali->SetBranchAddress("xSlSL3", xSlSL3);
  tali->SetBranchAddress("dxdzSlSL3", dxdzSlSL3);
  tali->SetBranchAddress("exSlSL3", exSlSL3);
  tali->SetBranchAddress("edxdzSlSL3", edxdzSlSL3);
  tali->SetBranchAddress("xSL3SL1", xSL3SL1);
  tali->SetBranchAddress("xc", xc);
  tali->SetBranchAddress("yc", yc);
  tali->SetBranchAddress("zc", zc);
  tali->SetBranchAddress("ex", zc);
  tali->SetBranchAddress("xcp", xcp);
  tali->SetBranchAddress("ycp", ycp);
  tali->SetBranchAddress("exp", exp);
  tali->SetBranchAddress("eyp", eyp);
  tali->SetBranchAddress("wh", wh);
  tali->SetBranchAddress("st", st);
  tali->SetBranchAddress("sr", sr);
  tali->SetBranchAddress("sl", sl);
  tali->SetBranchAddress("la", la);

}
