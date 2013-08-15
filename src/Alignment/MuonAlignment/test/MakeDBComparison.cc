/** \class MakeDBComparison
 *
 *  $Date: 2010/10/04 15:18:28 $
 *  $Revision: 1.2 $
 *  \author P. Martinez Ruiz del Arbol - IFCA (CSIC-UC) <Pablo.Martinez@cern.ch>
 */

#include "MakeDBComparison.h"

// Collaborating Class Header
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "Alignment/CommonAlignment/interface/AlignableSurface.h"

#include "Math/GenVector/Rotation3D.h"

#define pi 3.14159265


#include <stdio.h>

#include <vector>

using namespace std;
using namespace edm;


/// Constructor
MakeDBComparison::MakeDBComparison(const ParameterSet& pset)
{

  nameOfInputFile = pset.getParameter<std::string>("nameOfTheInputFile");
  nameOfOutputFile = pset.getParameter<std::string>("nameOfTheOutputFile");

}

/// Destructor
MakeDBComparison::~MakeDBComparison(){}



void MakeDBComparison::beginJob()
{
  
 
  //Input
  fileInput = new TFile(nameOfInputFile.c_str());
  ttreeInput = (TTree *)fileInput->Get("MuonGeometry");
  
  //Output
  fileOutput = new TFile(nameOfOutputFile.c_str(), "RECREATE");
  ttreeOutput = new TTree("MuonGeometry","MuonGeometry");
  
}



float MakeDBComparison::correctAngle(float angle) {

  float retvalue = angle;
  
  if(angle < -1.0*pi)
  {
    retvalue = angle + 2.0*pi;
  } else if(angle > pi)
  {
    retvalue = angle - 2.0*pi;
  }
  
  return retvalue;

}



void MakeDBComparison::endJob()
{

  AlignableMuon *myMuon = myMuonAlignment->getAlignableMuon();
  AlignableNavigator *myNavy = myMuonAlignment->getAlignableNavigator();

  ttreeInput->SetBranchAddress("info", &myGeom);

  ttreeOutput->Branch("info", &myGeomDiff, "objId/I:subsys/I:rawId/L:id1/I:id2/I:id3/I:id4/I:id5/I:x/F:y/F:z/F:phix/F:phiy/F:phiz/F:dx/F:dy/F:dz/F:dphix/F:dphiy/F:dphiz/F:dxl/F:dyl/F:dzl/F:dphixl/F:dphiyl/F:dphizl/F");

  Int_t nentries = (Int_t)ttreeInput->GetEntries();
  for (Int_t i=0;i<nentries;i++)
  {
    ttreeInput->GetEntry(i);
    //The object has to have a CMSSW id
    if(myGeom.typeOfObject != align::AlignableDTChamber &&
       myGeom.typeOfObject != align::AlignableDTSuperLayer &&
       myGeom.typeOfObject != align::AlignableCSCChamber &&
       myGeom.typeOfObject != 1) continue;
   
    myGeomDiff.typeOfObject = myGeom.typeOfObject;
    myGeomDiff.subsystem = myGeom.subsystem;
    myGeomDiff.rawId = myGeom.rawId;
    myGeomDiff.wheelEndcap = myGeom.wheelEndcap;
    myGeomDiff.stationStation = myGeom.stationStation;
    myGeomDiff.sectorRing = myGeom.sectorRing;
    myGeomDiff.superlayerChamber = myGeom.superlayerChamber;
    myGeomDiff.layerPlane = myGeom.layerPlane;
    //The absolute position is just copied
    myGeomDiff.x = myGeom.x;
    myGeomDiff.y = myGeom.y;
    myGeomDiff.z = myGeom.z;
    myGeomDiff.euler1 = myGeom.euler1;
    myGeomDiff.euler2 = myGeom.euler2;
    myGeomDiff.euler3 = myGeom.euler3;

    
     //Get the alignable and its corresponding surface
     DetId myDet(myGeom.rawId);
     Alignable *myAlign = myNavy->alignableFromDetId( myDet );
     AlignableSurface mySurf = myAlign->surface();
    

     //Everything which comes from the ROOT ends with ROOT
     //Everything which comes from the DB ends with DB

     //Global Point and Euler Angles of the ROOT geometry
     GlobalPoint globROOT(myGeom.x,myGeom.y,myGeom.z);
     align::EulerAngles myEulROOT(3);
     myEulROOT[0] = myGeom.euler1;
     myEulROOT[1] = myGeom.euler2;
     myEulROOT[2] = myGeom.euler3;
     align::RotationType rottypeROOT = align::toMatrix(myEulROOT);
     align::rectify(rottypeROOT);
 
     //Local Point and angles of the ROOT geometry
     LocalPoint locROOT = mySurf.toLocal(globROOT);
     align::EulerAngles myEulLocalAuxROOT = mySurf.toLocal(myEulROOT);
     align::RotationType rottypeLocalROOT = align::toMatrix(myEulLocalAuxROOT);
     align::rectify(rottypeLocalROOT);
     align::EulerAngles myEulLocalROOT = align::toAngles(rottypeLocalROOT);
     myEulLocalROOT[0] = correctAngle(myEulLocalROOT[0]);
     myEulLocalROOT[1] = correctAngle(myEulLocalROOT[1]);
     myEulLocalROOT[2] = correctAngle(myEulLocalROOT[2]);

   
     //Global Point and angles of the DB geometry 
     align::PositionType globDB = myAlign->globalPosition();
     align::RotationType rottypeDB = myAlign->globalRotation();
     align::rectify(rottypeDB);
     align::EulerAngles myEulDB =  align::toAngles(rottypeDB);
     myEulDB[0] = correctAngle(myEulDB[0]);
     myEulDB[1] = correctAngle(myEulDB[1]);
     myEulDB[2] = correctAngle(myEulDB[2]);
     
     //Get the local angles
     align::EulerAngles myEulLocalAuxDB = mySurf.toLocal(myEulDB);
     align::RotationType rottypeLocalDB = align::toMatrix(myEulLocalAuxDB);
     align::rectify(rottypeLocalDB);
     align::EulerAngles myEulLocalDB = align::toAngles(rottypeLocalDB);
     myEulLocalDB[0] = correctAngle(myEulLocalDB[0]);
     myEulLocalDB[1] = correctAngle(myEulLocalDB[1]);
     myEulLocalDB[2] = correctAngle(myEulLocalDB[2]);


     //In order to calculate the difference between the two rotation matrizes I do (ROOT)^-1*(DB) and take the resulting angles
     align::RotationType diffGlobal = rottypeDB.multiplyInverse(rottypeROOT);
     align::RotationType diffLocal = rottypeLocalDB.multiplyInverse(rottypeLocalROOT);
     align::rectify(diffGlobal);
     align::rectify(diffLocal); 
     align::EulerAngles diffEulGlobal = align::toAngles(diffGlobal);
     align::EulerAngles diffEulLocal = align::toAngles(diffLocal);
 

     //Put the global differeances in the Ntuple 
     myGeomDiff.dx = globROOT.x()-globDB.x();
     myGeomDiff.dy = globROOT.y()-globDB.y();
     myGeomDiff.dz = globROOT.z()-globDB.z();
     myGeomDiff.deuler1 = correctAngle(diffEulGlobal[0]);
     myGeomDiff.deuler2 = correctAngle(diffEulGlobal[1]);
     myGeomDiff.deuler3 = correctAngle(diffEulGlobal[2]);
    

     //Put the global differeances in the Ntuple 
     myGeomDiff.dxl = locROOT.x();
     myGeomDiff.dyl = locROOT.y();
     myGeomDiff.dzl = locROOT.z();
     myGeomDiff.deuler1l = correctAngle(diffEulLocal[0]);
     myGeomDiff.deuler2l = correctAngle(diffEulLocal[1]);
     myGeomDiff.deuler3l = correctAngle(diffEulLocal[2]);

     ttreeOutput->Fill();    

   }
    

  
  fileInput->Close();
  fileOutput->Write();
  fileOutput->Close();

 
}




void MakeDBComparison::analyze(const Event & event, const EventSetup& eventSetup){
  myMuonAlignment = new MuonAlignment(eventSetup);
}


DEFINE_FWK_MODULE(MakeDBComparison);
