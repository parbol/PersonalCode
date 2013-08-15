/** \class MakeDBComparison
 *  Calculate differences between two geometries, one of them provided as a ROOT file (generated with ReadDBToRootFile)
 *  and the other provided as a geometry in the python configuration file
 *
 *  $Date: 2010/10/04 15:18:28 $
 *  $Revision: 1.2 $
 *  \author P. Martinez Ruiz del Arbol - IFCA (CSIC-UC) <Pablo.Martinez@cern.ch>
 */

#ifndef Alignment_MuonAlignment_MakeDBComparison_H
#define Alignment_MuonAlignment_MakeDBComparison_H


// Base Class Headers
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "Alignment/MuonAlignment/interface/MuonAlignment.h"
#include "TFile.h"
#include "TTree.h"

#include "Alignment/CommonAlignment/interface/Utilities.h"

#include <vector>
#include <string>

namespace edm
{
  class ParameterSet;
  class Event;
  class EventSetup;
}




class MakeDBComparison: public edm::EDAnalyzer
{
 public:
  
  /// Constructor
  MakeDBComparison(const edm::ParameterSet& pset);
  
  /// Destructor
  virtual ~MakeDBComparison();
  
  // Operations
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
  
  virtual void beginJob() ;
  
  virtual void endJob() ;

  typedef struct
  {
    Int_t typeOfObject, subsystem ;
    Long64_t rawId;
    Int_t wheelEndcap, stationStation, sectorRing, superlayerChamber, layerPlane;
    Float_t x, y, z;
    Float_t euler1, euler2, euler3;
  } GEOM;

  typedef struct
  {
    Int_t typeOfObject, subsystem ;
    Long64_t rawId;
    Int_t wheelEndcap, stationStation, sectorRing, superlayerChamber, layerPlane;
    Float_t x, y, z;
    Float_t euler1, euler2, euler3;
    Float_t dx, dy, dz;
    Float_t deuler1, deuler2, deuler3;
    Float_t dxl, dyl, dzl;
    Float_t deuler1l, deuler2l, deuler3l;
  } GEOMDIFF;


  GEOM myGeom;
  GEOMDIFF myGeomDiff;

  float correctAngle(float);
  void myRectify(align::RotationType &);  
  align::EulerAngles myToAngles(align::RotationType ); 

 private:
  
  MuonAlignment *myMuonAlignment;
  
  TFile *fileInput;
  TTree *ttreeInput;
  TFile *fileOutput;
  TTree *ttreeOutput;
  
  std::string nameOfInputFile;
  std::string nameOfOutputFile;

};


#endif




