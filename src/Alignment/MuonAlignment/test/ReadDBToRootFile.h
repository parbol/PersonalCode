/** \class ReadDBToRootFile
 *  Read a DB specified in the pythong configuration file into a root tree
 *
 *  $Date: 2010/10/04 15:18:28 $
 *  $Revision: 1.2 $
 *  \author P. Martinez Ruiz del Arbol - IFCA (CSIC-UC) <Pablo.Martinez@cern.ch>
 */

//#ifndef Alignment_MuonAlignment_MuonAlignment_H
//#define Alignment_MuonAlignment_MuonAlignment_H

#ifndef Alignment_MuonAlignment_ReadDBToRootFile_H
#define Alignment_MuonAlignment_ReadDBToRootFile_H


// Base Class Headers
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "Alignment/MuonAlignment/interface/MuonAlignment.h"
#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <string>

namespace edm
{
  class ParameterSet;
  class Event;
  class EventSetup;
}




class ReadDBToRootFile: public edm::EDAnalyzer
{
 public:
  
  typedef struct
  {
    Int_t typeOfObject, subsystem ;
    Long64_t rawId;
    Int_t wheelEndcap, stationStation, sectorRing, superlayerChamber, layerPlane;
    Float_t x, y, z;
    Float_t euler1, euler2, euler3;
  } GEOM;

  GEOM myGeom;


  /// Constructor
  ReadDBToRootFile(const edm::ParameterSet& pset);
  
  /// Destructor
  virtual ~ReadDBToRootFile();
  
  // Operations
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
  
  virtual void beginJob() ;
  
  virtual void endJob();

  void develope(Alignable *); 

  float correctAngle(float);


 private:
  
  MuonAlignment *myMuonAlignment;

  TFile *file;
  TTree *ttree;
  
  std::string nameOfFile;


};
#endif




