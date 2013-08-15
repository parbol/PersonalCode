/** \class PointForAlignment
*
*  This class encapsulates the information needed from each alignable structure.
*
*  $Date: 2009/10/16 09:41:05 $
*  $Revision: 1.1 $
*  \author P. Martinez Ruiz del Arbol, IFCA (CSIC-UC)  <Pablo.Martinez@cern.ch>
*/


#include <TMatrixD.h>



class PointForInternalAlignment {
  
 public:
  
  PointForInternalAlignment(int, int, int, int);
  ~PointForInternalAlignment();
  void setPointForInternalAlignment(int n, float r, float x, float xp, float z, float m, float phi);
  void setPointForInternalAlignment(int n, float r, float x, float xp, float m, float phi);
  TMatrixD residual();
  TMatrixD errors();
  TMatrixD alignmentMatrix();
  void calculateResiduals();
  
  void calculateErrors();
  
  void calculateAlignmentCoeff();
  
  void calculateAlignmentCoeffMB4();
  
  TMatrixD derivatives();
  
  bool pointIsValid();

 private:

  int nLayer;
  float x, xpredicted, z, phi, r, m;
 
  TMatrixD theDerivatives;
  TMatrixD theErrors;
  TMatrixD theResiduals;
  TMatrixD theResidualsError;
  TMatrixD theAlignmentMatrix;

  int NDOFTrack;
  int NDOFAlign;
  int NDOFChamber;
  int NDOFCoor;

  bool validPoint;
	
};

