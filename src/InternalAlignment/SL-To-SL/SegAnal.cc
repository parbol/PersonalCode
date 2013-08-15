/******* \class SegAnal *******
 *
 * Description:
 *  
 *  detailed description
 *
 * \author : Stefano Lacaprara - INFN LNL <stefano.lacaprara@pd.infn.it>
 * $date   : 20/11/2006 16:50:57 CET $
 *
 * Modification:
 *
 *********************************/

/* This Class Header */
#include "RecoLocalMuon/DTSegment/test/SegAnal.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace edm;


#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/DTRecHit/interface/DTRangeMapAccessor.h"
#include "RecoLocalMuon/DTSegment/test/DTMeanTimer.h"
#include "RecoLocalMuon/DTSegment/test/DTSegmentResidual.h"


#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"


#include "DataFormats/DTDigi/interface/DTLocalTriggerCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"

/* C++ Headers */
#include <iostream>
#include <cmath>
using namespace std;

/* ====================================================================== */

/* Constructor */ 
SegAnal::SegAnal(const ParameterSet& pset){

  // the name of the 1D rec hits collection
  theRecHits1DLabel = pset.getParameter<edm::InputTag>("recHits1DLabel");

  // the name of the 2D rec hits collection
  theRecHits2DLabel = pset.getParameter<edm::InputTag>("recHits2DLabel");

  // the name of the 4D rec hits collection
  theRecHits4DLabel = pset.getParameter<edm::InputTag>("recHits4DLabel");


  f = new TFile("MyNtuple.root", "RECREATE");
 
  ttreeOutput = new TTree("InternalAlignment", "InternalAlignment"); 
  ttreeOutput->Branch("4DSegmentInfo", &my4DSegment, "wh/I:st/I:se/I:x04D/F:phi4D/F:z04D/F:theta04D/F:x0SL1up/F:phiSL1up/F:x0SL3up/F:phiSL3up/F:x0SL1down/F:phiSL1down/F:x0SL3down/F:phiSL3down/F:hitsPhi[8][3]/F:hitsTheta[4][3]/F");

}




vector<float> SegAnal::parametersSL(vector<LocalPoint> sl1, vector<LocalPoint> sl3) {

 std::vector<float> parameters;
 float zzm = 0, xzm = 0, xm = 0, zm = 0;
 int counter = 0; 
 for(std::vector<LocalPoint>::const_iterator s1 = sl1.begin(); s1 != sl1.end(); ++s1) {
   float x = (*s1).x();
   float z = (*s1).z();
   zzm += z*z;
   xzm += x*z;  
   zm += z;
   xm += x;
   counter++;
 }    

 //Straight line in the chamber system of reference
 float b1 = (counter*xzm - xm*zm)/(counter*zzm-zm*zm);
 float a1 = (xm-b1*zm)/counter;
 
 //The extrapolation to the current SL level is the following:
 float zmeanSL1 = zm/counter;
 
 zzm = 0; xzm = 0; xm = 0; zm = 0;
 counter = 0;
 for(std::vector<LocalPoint>::const_iterator s3 = sl3.begin(); s3 != sl3.end(); ++s3) {
   float x = (*s3).x();
   float z = (*s3).z();
   zzm += z*z;
   xzm += x*z;
   zm += z;
   xm += x;
   counter++;
 } 

 float b3 = (counter*xzm - xm*zm)/(counter*zzm-zm*zm);
 float a3 = (xm-b3*zm)/counter;

 //The extrapolation to the current SL level is the following:
 float zmeanSL3 = zm/counter;
 
 float x3 = a3 + b3 * zmeanSL3;
 float x1 = a1 + b1 * zmeanSL3; 
 

 parameters.push_back(x1);
 parameters.push_back(x3);
 parameters.push_back(b1);
 parameters.push_back(b3);

 return parameters;


}

/* Destructor */ 
SegAnal::~SegAnal() {

  f->Write();
  f->Close();

}

   
void SegAnal::setContainerToZero() {

  my4DSegment.wh = 0;
  my4DSegment.st = 0;
  my4DSegment.se = 0;
  my4DSegment.x04D = 0;
  my4DSegment.phi4D = 0;
  my4DSegment.z04D = 0;
  my4DSegment.theta04D = 0;
  for(int c = 0; c < 8; c++) {
    my4DSegment.hitsPhi[c][0] = 0;
    my4DSegment.hitsPhi[c][1] = 0;
    my4DSegment.hitsPhi[c][2] = 0;
  }
  for(int c = 0; c < 4; c++) {
    my4DSegment.hitsTheta[c][0] = 0;
    my4DSegment.hitsTheta[c][1] = 0;
    my4DSegment.hitsTheta[c][2] = 0;
  }

}

void SegAnal::analyze(const Event & event,
                                   const EventSetup& eventSetup){
  ESHandle<DTGeometry> dtGeom;
  eventSetup.get<MuonGeometryRecord>().get(dtGeom);

  // Get the 4D rechit collection from the event -------------------
  edm::Handle<DTRecSegment4DCollection> segs;
  event.getByLabel(theRecHits4DLabel, segs);

  // Get the 2D rechit collection from the event -------------------
  edm::Handle<DTRecSegment2DCollection> segs2d;
  event.getByLabel(theRecHits2DLabel, segs2d);

  // Get the 1D rechits from the event --------------
  Handle<DTRecHitCollection> dtRecHits; 
  event.getByLabel(theRecHits1DLabel, dtRecHits);


  for (DTRecSegment4DCollection::const_iterator seg=segs->begin() ;
         seg!=segs->end() ; ++seg ) {
    DTChamberId myChamberId = (*seg).chamberId();
    const DTChamber *myChamber = dtGeom->chamber(myChamberId); 
    const DTChamberRecSegment2D* phiSeg= (*seg).phiSegment();
    const DTSLRecSegment2D* zSeg= (*seg).zSegment();
    
    setContainerToZero();
    bool valid = false;
    my4DSegment.wh = myChamberId.wheel();
    my4DSegment.st = myChamberId.station();
    my4DSegment.se = myChamberId.sector();

    //Phi Projection
    vector<DTRecHit1D> phiHits;
    if (phiSeg) {
      phiHits = phiSeg->specificRecHits();
      if (phiHits.size() == 8) {
        valid = true; 
        my4DSegment.x04D = phiSeg->parameters()[1];
        my4DSegment.phi4D = phiSeg->parameters()[0];

        vector<LocalPoint> sl1;
        vector<LocalPoint> sl3;
        for(vector<DTRecHit1D>::const_iterator mHit = phiHits.begin(); mHit != phiHits.end();
            ++mHit) {
          DTWireId myWireId = (*mHit).wireId();
          DTLayerId myLayerId(myWireId.wheel(), myWireId.station(), myWireId.sector(), myWireId.superlayer(), myWireId.layer());
          const DTLayer *myLayer = dtGeom->layer(myLayerId); 

          LocalPoint pos = myChamber->toLocal(myLayer->toGlobal((*mHit).localPosition()));
          float the_x = pos.x();
          float the_y = pos.y();
          float the_z = pos.z();
          
          int layer = 0;
          if(myLayerId.superlayer() == 1) {
            sl1.push_back(pos);
            layer = myLayerId.layer() - 1;
          }else if (myLayerId.superlayer() == 3) {
            sl3.push_back(pos);
            layer = myLayerId.layer() + 3;
          }
          my4DSegment.hitsPhi[layer][0] = the_x;
          my4DSegment.hitsPhi[layer][1] = the_y;
          my4DSegment.hitsPhi[layer][2] = the_z;

        }
        //vector<float> SLparwrt1 = parametersSL1(sl1local, sl3local, mySLayer1, mySLayer3);
        vector<float> SLpar = parametersSL(sl1, sl3);    
        my4DSegment.x0SL1up = SLpar[0];
        my4DSegment.x0SL3up = SLpar[1];
        my4DSegment.phiSL1up = SLpar[2];
        my4DSegment.phiSL3up = SLpar[3];
        vector<float> SLpar2 = parametersSL(sl3, sl1);    
        my4DSegment.x0SL1down = SLpar2[0];
        my4DSegment.x0SL3down = SLpar2[1];
        my4DSegment.phiSL1down = SLpar2[2];
        my4DSegment.phiSL3down = SLpar2[3];
      }
    }
    //Z Projection
    vector<DTRecHit1D> zHits;
    if (zSeg) {
      zHits = zSeg->specificRecHits();
      if (zHits.size() == 4) {
        
        my4DSegment.z04D = zSeg->parameters()[1];
        my4DSegment.theta04D = zSeg->parameters()[0];

        for(vector<DTRecHit1D>::const_iterator mHit = phiHits.begin(); mHit != phiHits.end();
            ++mHit) {
          DTWireId myWireId = (*mHit).wireId();
          DTLayerId myLayerId(myWireId.wheel(), myWireId.station(), myWireId.sector(), myWireId.superlayer(), myWireId.layer());
          const DTLayer *myLayer = dtGeom->layer(myLayerId);

          LocalPoint pos = myChamber->toLocal(myLayer->toGlobal((*mHit).localPosition()));
          float the_x = pos.x();
          float the_y = pos.y();
          float the_z = pos.z();
          int layer = myLayerId.layer();
          my4DSegment.hitsPhi[layer-1][0] = the_x;
          my4DSegment.hitsPhi[layer-1][1] = the_y;
          my4DSegment.hitsPhi[layer-1][2] = the_z;
        }
      }
    }
    if(valid == true) ttreeOutput->Fill();
  } 
}
 
        



DEFINE_FWK_MODULE(SegAnal);
