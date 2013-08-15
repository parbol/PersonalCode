#include "Alignment/MuonAlignmentAlgorithms/interface/DTMuonLocalAlignment.h"
#include "Alignment/MuonAlignmentAlgorithms/interface/ReadPGInfo.h"
#include "TMatrixD.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include "TChain.h"
#include "math.h"


class DTMuonMillepede : public DTMuonLocalAlignment {
  
  public:

  DTMuonMillepede(std::string, int, float, float, TFile *);

  ~DTMuonMillepede(); 

  void calculationMillepede();

  TMatrixD getCqcMatrix(int, int, int);
  
  TMatrixD getbqcMatrix(int, int, int);
  
  TMatrixD getCsurveyMatrix(int, int, int);
  
  TMatrixD getbsurveyMatrix(int, int, int);

  TMatrixD getLagMatrix(int, int, int);
  
  TMatrixD prepareForLagrange(TMatrixD );

  void setBranchTree();

  private:

  ReadPGInfo *myPG;
  
  TTree *ttreeOutput;

  float ptMax, ptMin;

  //Variables for the output tree
  //---------------------------------------------------------
  int whC, stC, srC;
  int slC[12], laC[12];
  float dx[12], dy[12], dz[12], phix[12], phiy[12], phiz[12];
  float cov[60][60];
  //---------------------------------------------------------
   

};


