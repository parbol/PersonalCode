/** \class ReadDBToRootFile
 *  Analyzer of the StandAlone muon tracks for alignment with tracks
 *
 *  $Date: 2010/10/04 15:18:28 $
 *  $Revision: 1.2 $
 *  \author P. Martinez Ruiz del Arbol - IFCA (CSIC-UC) <Pablo.Martinez@cern.ch>
 */

#include "ReadDBToRootFile.h"

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
#include "Alignment/CommonAlignment/interface/Utilities.h"
#include "Alignment/CommonAlignment/interface/AlignableSurface.h"

#define pi 3.14159265


#include <stdio.h>

#include <vector>

using namespace std;
using namespace edm;


/// Constructor
ReadDBToRootFile::ReadDBToRootFile(const ParameterSet& pset)
{

  nameOfFile = pset.getParameter<std::string>("nameOfTheFile");

}

/// Destructor
ReadDBToRootFile::~ReadDBToRootFile(){}



void ReadDBToRootFile::beginJob()
{
  
 
  file = new TFile(nameOfFile.c_str(),  "RECREATE");
  file->cd(); 
  
  ttree = new TTree("MuonGeometry","MuonGeometry");
  ttree->Branch("info", &myGeom, "objId/I:subsys/I:rawId/L:id1/I:id2/I:id3/I:id4/I:id5/I:x/F:y/F:z/F:phix/F:phiy/F:phiz/F");
  
}



//The angle must be in [-pi, pi] 
float ReadDBToRootFile::correctAngle(float angle)
{

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


void ReadDBToRootFile::develope(Alignable *myAlign)
{

  //static GEOM myGeom; 
  //ttree->Branch("info", &myGeom, "objId/I:subsys/I:rawId/L:id1/I:id2/I:id3/I:id4/I:id5/I:x/F:y/F:z/F:phix/F:phiy/F:phiz/F");
  
  //Get position and rotation for the given alignable
  align::PositionType pos = myAlign->globalPosition();
  myGeom.x = pos.x();
  myGeom.y = pos.y();
  myGeom.z = pos.z();
  
  align::RotationType rottype= myAlign->globalRotation();
  align::rectify(rottype); 
  align::EulerAngles rot =  align::toAngles(rottype);
  myGeom.euler1 = correctAngle(rot[0]);
  myGeom.euler2 = correctAngle(rot[1]);
  myGeom.euler3 = correctAngle(rot[2]);
  
  //Set to default 0 
  myGeom.typeOfObject = 0; myGeom.subsystem = 0; myGeom.wheelEndcap = 0; myGeom.stationStation = 0;
  myGeom.sectorRing = 0; myGeom.superlayerChamber = 0; myGeom.layerPlane = 0;
 
  //Identifiers of the alignable
  myGeom.typeOfObject =  myAlign->alignableObjectId();
  myGeom.rawId =  myAlign->id();
  DetId myDet(myGeom.rawId);
  myGeom.subsystem = myDet.subdetId(); 
  
  if(myGeom.subsystem == 1)
  { //The Barrel
    switch(myGeom.typeOfObject)
    {
      case align::AlignableDTChamber:
        myGeom.wheelEndcap = ((DTChamberId)myDet).wheel();
        myGeom.stationStation = ((DTChamberId)myDet).station();
        myGeom.sectorRing = ((DTChamberId)myDet).sector();
        break;
      case align::AlignableDTSuperLayer:
        myGeom.wheelEndcap = ((DTSuperLayerId)myDet).wheel();
        myGeom.stationStation = ((DTSuperLayerId)myDet).station();
        myGeom.sectorRing = ((DTSuperLayerId)myDet).sector();
        myGeom.superlayerChamber = ((DTSuperLayerId)myDet).superlayer();
        break;
      case 1: //Not corresponding to real ID
        myGeom.wheelEndcap = ((DTLayerId)myDet).wheel();
        myGeom.stationStation = ((DTLayerId)myDet).station();
        myGeom.sectorRing = ((DTLayerId)myDet).sector();
        myGeom.superlayerChamber = ((DTLayerId)myDet).superlayer();
        myGeom.layerPlane = ((DTLayerId)myDet).layer();
        break;
    }
  }
  else
  { //The Endcap
    switch(myGeom.typeOfObject)
    {
      case align::AlignableCSCChamber:
        myGeom.wheelEndcap = ((CSCDetId)myDet).endcap();
        myGeom.stationStation = ((CSCDetId)myDet).station();
        myGeom.sectorRing = ((CSCDetId)myDet).ring();
        myGeom.superlayerChamber = ((CSCDetId)myDet).chamber();
        break;
      case 1: //Not corresponding to real ID
        myGeom.wheelEndcap = ((CSCDetId)myDet).endcap();
        myGeom.stationStation = ((CSCDetId)myDet).station();
        myGeom.sectorRing = ((CSCDetId)myDet).ring();
        myGeom.superlayerChamber = ((CSCDetId)myDet).chamber();
        myGeom.layerPlane = ((CSCDetId)myDet).layer();
        break;
    }
  }  

  ttree->Fill();
  
  //Recursivity: go to the children
  if(myAlign->size() == 0)
  {
    return;
  } 
  else
  {
    std::vector<Alignable *> comp = myAlign->components();
    for(std::vector<Alignable *>::iterator c = comp.begin(); c != comp.end(); ++c)
    {
      develope(*c);
    }
  }
}



void ReadDBToRootFile::endJob(){
 
  AlignableMuon *myMuon = myMuonAlignment->getAlignableMuon();
  develope(myMuon); 
  
  file->Write();
  file->Close();

 
}



void ReadDBToRootFile::analyze(const Event & event, const EventSetup& eventSetup){
  myMuonAlignment = new MuonAlignment(eventSetup);
}


DEFINE_FWK_MODULE(ReadDBToRootFile);
