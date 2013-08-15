/** \class TrackForAlignment
*
*
*  $Date: 2009/10/16 09:41:05 $
*  $Revision: 1.1 $
*  \author P. Martinez Ruiz del Arbol, IFCA (CSIC-UC)  <Pablo.Martinez@cern.ch>
*/

#include "TrackForAlignment.h"
#include "TMath.h"
#include <iostream>



TrackForAlignment::TrackForAlignment(int track, int align, int chamber, int coor){
  NDOFTrack = track;
  NDOFAlign = align;
  NDOFChamber = chamber;
  NDOFCoor = coor;
}

std::vector<PointForInternalAlignment> TrackForAlignment::pointsAlignment() const {return thePoints;}

void TrackForAlignment::setP(double p_, double pt_) {
  thep = p_;
  thept = pt_;
}

void TrackForAlignment::setPerigeeParameters(double transverseCurvature_, double theta_, double phi_, double trans_, double longi_) {
 
  theTransverseCurvature = transverseCurvature_;
  theTheta = theta_;
  thePhi = phi_;
  theTrans = trans_;
  theLongi = longi_;
  
}

void TrackForAlignment::insert(PointForInternalAlignment a) {thePoints.push_back(a);}

int TrackForAlignment::pointNumber(){return thePoints.size();}

double TrackForAlignment::p(){return thep;}

double TrackForAlignment::pt(){return thept;}

double TrackForAlignment::transverseCurvature() {return theTransverseCurvature;}

double TrackForAlignment::theta() {return theTheta;}

double TrackForAlignment::phi() {return thePhi;}

double TrackForAlignment::trans() {return theTrans;}

double TrackForAlignment::longi() {return theLongi;}

TMatrixD TrackForAlignment::CpMatrix() const {return Cp;}

TMatrixD TrackForAlignment::BMatrix() const {return b;}

void TrackForAlignment::isValid() {
  
  if(thePoints.size() != NDOFChamber) {
    trackValid = false;
    return;
  }	
  
  std::vector<PointForInternalAlignment> mPoints = this->pointsAlignment();
  int quality = 0;
  for(std::vector<PointForInternalAlignment>::iterator points = mPoints.begin(); points != mPoints.end(); ++points) {
    if(!(*points).pointIsValid()) {
      trackValid = false;
      return;
    }
    quality++;
  }
  if(quality == NDOFChamber) {
    trackValid = true;
    return;
  }
  trackValid = false;
  return;
  
}


void TrackForAlignment::CalculateCoeffMatrices () {
  
  B.ResizeTo(NDOFChamber*NDOFCoor, NDOFAlign*NDOFChamber);
  Bt.ResizeTo(NDOFChamber*NDOFCoor, NDOFAlign*NDOFChamber);
  A.ResizeTo(NDOFChamber*NDOFCoor, NDOFTrack);
  At.ResizeTo(NDOFChamber*NDOFCoor, NDOFTrack);
  E.ResizeTo(NDOFChamber*NDOFCoor, NDOFChamber*NDOFCoor);
  R.ResizeTo(NDOFChamber*NDOFCoor, 1);
  
  int counter = 0;
  std::vector<PointForInternalAlignment> mPoints = this->pointsAlignment();
  for(std::vector<PointForInternalAlignment>::iterator points = mPoints.begin(); points != mPoints.end(); ++points) {
    TMatrixD Errors = (*points).errors();
    TMatrixD Jacobian = (*points).derivatives();
    TMatrixD Residuals = (*points).residual();
    TMatrixD AlignmentMatrix = (*points).alignmentMatrix();
    for(int coor = 0; coor < NDOFCoor; ++coor) {
      R(counter*NDOFCoor+coor, 0) = Residuals(coor, 0);
      for(int track = 0; track < NDOFTrack; ++track) {
	A(counter*NDOFCoor+coor, track) = Jacobian(coor, track);
      }
      for(int alig = 0; alig < NDOFAlign; ++alig) {
	B(counter*NDOFCoor+coor, counter*NDOFAlign+alig) = AlignmentMatrix(coor, alig);
      }
      for(int coor2 = 0; coor2 < NDOFCoor; ++coor2) {
      	E(counter*NDOFCoor+coor, counter*NDOFCoor+coor2) = Errors(coor, coor2);
      }
    }
    counter++;
  }
  
  Bt = B;
  Bt.T();
  At = A;
  At.T();
  
}


void TrackForAlignment::CalculateC() {
  C.ResizeTo(NDOFChamber*NDOFAlign,NDOFChamber*NDOFAlign);
  C = Bt*E*B;
}



void TrackForAlignment::CalculateG() {
  
  G.ResizeTo(NDOFChamber*NDOFAlign, NDOFTrack);
  Gt.ResizeTo(NDOFChamber*NDOFAlign, NDOFTrack);
  G = Bt*E*A; 
  Gt = G;
  Gt.T();
}


void TrackForAlignment::CalculateGamma() {
  
  TMatrixD kk;
  kk.ResizeTo(NDOFTrack, NDOFTrack); 
  Gamma.ResizeTo(NDOFTrack,NDOFTrack);
  Gamma = At*E*A;
  kk = Gamma;
  Gamma.Invert();
  TMatrixD unit, unit2;
  unit.ResizeTo(NDOFTrack, NDOFTrack);
  unit2.ResizeTo(NDOFTrack, NDOFTrack);
  unit2.UnitMatrix();
  unit = unit2-kk*Gamma;
  //std::cout << "Inversion check partial: " << unit.E2Norm();
 
}


void TrackForAlignment::printMatrix(TMatrixD &m, char *name) {

  std::cout << "Matrix: " << name << std::endl;
  for(int n = 0; n < m.GetNrows(); n++) {
    for(int s = 0; s < m.GetNcols(); s++) {
      std::cout << m(n,s) << " ";
    }
    std::cout << std::endl;
  }
  
}


void TrackForAlignment::CalculateCp() {
  Cp.ResizeTo(C.GetNrows(), C.GetNcols());
  Cp = C - G*Gamma*Gt;
  
} 


void TrackForAlignment::CalculateB(){
  
  b.ResizeTo(NDOFChamber*NDOFAlign,1);
  b = Bt*E*(R-A*Gamma*At*E*R);
}


void TrackForAlignment::CalculateMatrizes() {
 
  CalculateCoeffMatrices();
  CalculateC();
  CalculateG();
  CalculateGamma();
  CalculateCp();  
  CalculateB();  

  //C.Print("CMatrix");
  //b.Print("bMatrix");
  //A.Print("A");
  //B.Print("B");
  //E.Print("E");
  //R.Print("R");

 
}




