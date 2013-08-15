/** \class MuonStandaloneAlgorithm
*  
*  This class contains all the useful information needed by the Alignment Algorithm.
*   
*  $Date: 2009/10/16 09:41:05 $
*  $Revision: 1.1 $
*  \author P. Martinez Ruiz del Arbol, IFCA (CSIC-UC)  <Pablo.Martinez@cern.ch>
*/
#include "PointForInternalAlignment.cc"
#include <vector>
#include "TMatrixD.h"


class TrackForAlignment {
  
 public:
  

  
  TrackForAlignment(int, int, int, int);
  
  virtual ~TrackForAlignment(){};
  
  void setPerigeeParameters(double, double, double, double, double);
  
  void setP(double, double);
  
  void insert(PointForInternalAlignment );
  
  int pointNumber();

  double transverseCurvature();

  double theta();

  double phi();
  
  double trans();

  double longi();
  
  double p();
  
  double pt();
  
  void isValid();
  
  void printMatrix(TMatrixD &, char *);
  
  void CalculateMatrizes();
  
  TMatrixD CpMatrix() const;
  
  TMatrixD BMatrix() const;

  bool trackValid;

  std::vector<PointForInternalAlignment> pointsAlignment() const;

 private:

  void CalculateCoeffMatrices();  
  
  void CalculateC();
  
  void CalculateG();
  
  void CalculateGamma();
  
  void CalculateCp();
  
  void CalculateB();
  

  double thep, thept;
  
  double theTransverseCurvature, theTheta, thePhi, theTrans, theLongi;
  
  std::vector<PointForInternalAlignment> thePoints;
  
  TMatrixD C, G, Gt, Gamma, Cp, b, A, At, E, B, Bt, R;  
  
  int NDOFTrack;
  int NDOFAlign;
  int NDOFChamber;
  int NDOFCoor;
  
};

