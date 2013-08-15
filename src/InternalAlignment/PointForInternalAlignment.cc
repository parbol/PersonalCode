/** \class PointForAlignment
*
*
*  $Date: 2009/10/16 09:41:05 $
*  $Revision: 1.1 $
*  \author P. Martinez Ruiz del Arbol, IFCA (CSIC-UC)  <Pablo.Martinez@cern.ch>
*/


#include "PointForInternalAlignment.h"
#include <TMath.h>

PointForInternalAlignment::PointForInternalAlignment(int track, int align, int chamber, int coor){
  NDOFTrack = track;
  NDOFAlign = align;
  NDOFChamber = chamber;
  NDOFCoor = coor;
}

PointForInternalAlignment::~PointForInternalAlignment(){};



void PointForInternalAlignment::setPointForInternalAlignment(int n, float r, float x, float xp, float z, float m, float phi) {
  
  //The number of the layer
  this->nLayer = n;
  this->r = r; 
  this->x = x;
  this->xpredicted = xp;
  this->phi = phi;
  this->z = z;
  this->m = m;

  theDerivatives.ResizeTo(NDOFCoor, NDOFTrack);
  theDerivatives(0,0) = 1.0;
  theDerivatives(0,1) = r;
  
  calculateErrors();
  calculateResiduals();
  calculateAlignmentCoeff();
  
  if(TMath::Abs(theResiduals(0,0)) < 1.0 && n != 0) {
    validPoint = true;
  } else {
    validPoint = false;
  } 
}

void PointForInternalAlignment::setPointForInternalAlignment(int n, float r, float x, float xp, float m, float phi) {
  
  //The number of the layer
  this->nLayer = n;
  this->r = r; 
  this->x = x;
  this->xpredicted = xp;
  this->phi = phi;
  //this->z = z;
  this->m = m;

  theDerivatives.ResizeTo(NDOFCoor, NDOFTrack);
  theDerivatives(0,0) = 1.0;
  theDerivatives(0,1) = r;
  
  calculateErrors();
  calculateResiduals();
  calculateAlignmentCoeffMB4();
  
  if(TMath::Abs(theResiduals(0,0)) < 1.0 && n != 0) {
    validPoint = true;
  } else {
    validPoint = false;
  } 
}

TMatrixD PointForInternalAlignment::derivatives(){return theDerivatives;}
 
TMatrixD PointForInternalAlignment::residual() {return theResiduals;}
 
TMatrixD PointForInternalAlignment::errors() {return theErrors;}

TMatrixD PointForInternalAlignment::alignmentMatrix() {return theAlignmentMatrix;}


void PointForInternalAlignment::calculateResiduals() {
  theResiduals.ResizeTo(1,1);
  theResiduals(0,0) = x-xpredicted;
}

void PointForInternalAlignment::calculateErrors() {
  theErrors.ResizeTo(NDOFCoor, NDOFCoor);
  if(nLayer != 0) {
    //theErrors(0,0) = 1.0/(0.02*0.02);
    theErrors(0,0) = 1.0;
  } else {
    theErrors(0,0) = 0;
  }
}

void PointForInternalAlignment::calculateAlignmentCoeff() {
  
  double tanphi = TMath::Tan(phi);
  theAlignmentMatrix.ResizeTo(NDOFCoor, NDOFAlign);
  if(nLayer != 0) {
    theAlignmentMatrix(0,0) = 1.0; 
    theAlignmentMatrix(0,1) = -m; 
    theAlignmentMatrix(0,2) = -m*z/100.0;
    theAlignmentMatrix(0,3) = m*x/100.0;
    theAlignmentMatrix(0,4) = -z/100.0; 
  } else {
    theAlignmentMatrix(0,0) = 0; 
    theAlignmentMatrix(0,1) = 0; 
    theAlignmentMatrix(0,2) = 0;
    theAlignmentMatrix(0,3) = 0;
    theAlignmentMatrix(0,4) = 0;
  }
}


void PointForInternalAlignment::calculateAlignmentCoeffMB4() {
  double tanphi = TMath::Tan(phi);
  theAlignmentMatrix.ResizeTo(NDOFCoor, NDOFAlign);
  if(nLayer != 0) {
    theAlignmentMatrix(0,0) = 1.0; 
    theAlignmentMatrix(0,1) = -m; 
    theAlignmentMatrix(0,2) = 0.0;
    theAlignmentMatrix(0,3) = m*x/100.0;
    theAlignmentMatrix(0,4) = 0;
  } else {
    theAlignmentMatrix(0,0) = 0; 
    theAlignmentMatrix(0,1) = 0; 
    theAlignmentMatrix(0,2) = 0;
    theAlignmentMatrix(0,3) = 0;
    theAlignmentMatrix(0,4) = 0;
  }
}


bool PointForInternalAlignment::pointIsValid() {return validPoint;}




