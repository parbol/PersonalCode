#ifndef DTANALYZER_H
#define DTANALYZER_H

/** \class SegAnal
 *
 * Description:
 *  
 *  detailed description
 *
 * \author : Stefano Lacaprara - INFN Padova <stefano.lacaprara@pd.infn.it>
 * $date   : 20/11/2006 16:51:04 CET $
 *
 * Modification:
 *
 */

/* Base Class Headers */
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <vector>
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include <fstream>
#include "TTree.h"


namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

/* Collaborating Class Declarations */
#include "DataFormats/Common/interface/Handle.h"
class TFile;
class TH1D;
class TH2D;
class TProfile;
class DTLayerId;
class DTSuperLayerId;
class DTChamberId;
class DTTTrigBaseSync;

/* C++ Headers */
#include <iosfwd>
#include <bitset>

/* ====================================================================== */

/* Class SegAnal Interface */

class SegAnal : public edm::EDAnalyzer {

  public:

/* Constructor */ 
    SegAnal(const edm::ParameterSet& pset) ;

/* Destructor */ 
    ~SegAnal() ;

/* Operations */ 
    void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  private:

    typedef struct {
      Int_t wh, st, se;
      Float_t x04D, phi4D, z04D, theta04D;
      Float_t x0SL1up, phiSL1up, x0SL3up, phiSL3up;
      Float_t x0SL1down, phiSL1down, x0SL3down, phiSL3down;
      Float_t hitsPhi[8][3];
      Float_t hitsTheta[8][3];
    } DTSegmentAn;

    DTSegmentAn my4DSegment;
 
    TTree *ttreeOutput;
   
    std::ofstream outF;
 
    TFile *f;
    //static std::string theAlgoName;
    edm::InputTag theRecHits4DLabel;
    edm::InputTag theRecHits2DLabel;     
    edm::InputTag theRecHits1DLabel;     

    void setContainerToZero();
    std::vector<float> parametersSL(std::vector<LocalPoint> , std::vector<LocalPoint>);



};
#endif // DTANALYZER_H

