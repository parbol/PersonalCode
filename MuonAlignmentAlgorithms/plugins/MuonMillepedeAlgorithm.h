#ifndef Alignment_MuonAlignmentAlgorithms_MuonMillepedeAlgorithm_h
#define Alignment_MuonAlignmentAlgorithms_MuonMillepedeAlgorithm_h


#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentAlgorithmBase.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentIORoot.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"

#include "TH1D.h"
#include "TProfile.h"
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

#include "TMatrixD.h"


class AlignableNavigator;
class TFile;
class TTree;
class AlignableDet;

#define MAX_HIT 60
#define MAX_SEGMENT 5


class MuonMillepedeAlgorithm : public AlignmentAlgorithmBase
{

 public:

  /// Constructor
  MuonMillepedeAlgorithm(const edm::ParameterSet& cfg);

  /// Destructor
  ~MuonMillepedeAlgorithm() {};

  /// Call at beginning of job
  void initialize( const edm::EventSetup& setup, 
                   AlignableTracker* tracker, AlignableMuon* muon, 
                   AlignmentParameterStore* store);

  /// Call at end of job
  void terminate(void);


  /// Run the algorithm on trajectories and tracks: This is for 3XY
  //void run(const edm::EventSetup& setup, const EventInfo &eventInfo);

  //This is for 22X
  void run( const edm::EventSetup& , const ConstTrajTrackPairCollection& );


  // Builds the 4D segments  
  void build4DSegments();

  // 
  void initNTuples();


  //
  void setBranchTrees();
  
  //
  void setBranchOutputTrees();
  
  //
  void setBranchAddressTrees();

  //
  void fitDistributions(int, int, int, int, TH1D *);

  //
  void preSelection();

  //
  void calculation();

  //
  TMatrixD returnBMatrix(float, float, float, float);
 
  //
  TMatrixD returnBMB4Matrix(float, float);

  //
  TMatrixD returnEMatrix(float *, float *, float *);

  //
  TMatrixD returnEMB4Matrix(float *, float *);

  //
  TMatrixD returnRes(float, float, float, float);

  //
  TMatrixD returnResMB4(float, float);

  //
  void produceAnalysis();

  typedef struct {
    int nhits;
    float xl[MAX_HIT]; float yl[MAX_HIT]; float xc[MAX_HIT];
    float yc[MAX_HIT]; float zc[MAX_HIT]; float xg[MAX_HIT];
    float yg[MAX_HIT]; float zg[MAX_HIT]; float erx[MAX_HIT];
    float ery[MAX_HIT]; float xlp[MAX_HIT]; float ylp[MAX_HIT];
    float xcp[MAX_HIT]; float ycp[MAX_HIT]; float zcp[MAX_HIT];
    float xgp[MAX_HIT]; float ygp[MAX_HIT]; float zgp[MAX_HIT];
    int wh[MAX_HIT]; int st[MAX_HIT]; int sr[MAX_HIT];
    int sl[MAX_HIT]; int la[MAX_HIT];
    float cov[MAX_HIT][4][4];
  } Info1D;
  
  
  
  //Variables to be stored in the NTuple-------------------------------------------
  float p, pt, eta, phi, chi2, chi2n, charge;
  int nhits, nseg;
  float xSl[MAX_SEGMENT]; float xSlp[MAX_SEGMENT]; float resx[MAX_SEGMENT];
  float dxdzSl[MAX_SEGMENT]; float dxdzSlp[MAX_SEGMENT]; float resdxdz[MAX_SEGMENT];
  float exSl[MAX_SEGMENT]; float exSlp[MAX_SEGMENT]; float eresx[MAX_SEGMENT];
  float edxdzSl[MAX_SEGMENT]; float edxdzSlp[MAX_SEGMENT]; float eresdxdz[MAX_SEGMENT];
  float exdxdzSl[MAX_SEGMENT]; float exdxdzSlp[MAX_SEGMENT]; float eresxdxdz[MAX_SEGMENT];
  float cov_xx[MAX_SEGMENT]; float cov_yy[MAX_SEGMENT]; 
  float cov_dxdx[MAX_SEGMENT]; float cov_dydy[MAX_SEGMENT]; 
  float cov_xdx[MAX_SEGMENT]; float cov_xdy[MAX_SEGMENT]; 
  float cov_xy[MAX_SEGMENT]; float cov_ydx[MAX_SEGMENT]; 
  float cov_ydy[MAX_SEGMENT]; float cov_dxdy[MAX_SEGMENT]; 
  float ySl[MAX_SEGMENT]; float ySlp[MAX_SEGMENT]; float resy[MAX_SEGMENT];
  float dydzSl[MAX_SEGMENT]; float dydzSlp[MAX_SEGMENT]; float resdydz[MAX_SEGMENT];
  float eySl[MAX_SEGMENT]; float eySlp[MAX_SEGMENT]; float eresy[MAX_SEGMENT];
  float edydzSl[MAX_SEGMENT]; float edydzSlp[MAX_SEGMENT]; float eresdydz[MAX_SEGMENT];
  float eydydzSl[MAX_SEGMENT]; float eydydzSlp[MAX_SEGMENT]; float eresydydz[MAX_SEGMENT];
  int whS[MAX_SEGMENT]; int stS[MAX_SEGMENT]; int srS[MAX_SEGMENT];
  //--------------------------------------------------------------------------------
  
  //Alignment constants-------------------------------------------------------------
  Int_t wh, st, sr;
  float dx, dy, dz, phix, phiy, phiz;
  float cov[6][6];
  //-------------------------------------------------------------------------------- 


  Info1D myTrack1D;
  
  
 private:

  // private data members
  TFile *f;
  TTree *ttreeOutput;
  TChain *tali;


  AlignmentParameterStore* theAlignmentParameterStore;
  std::vector<Alignable*> theAlignables;
  AlignableNavigator* theAlignableDetAccessor;
  
  //Service for histograms
  edm::Service<TFileService> fs;
  
  TrajectoryStateCombiner m_tsoscomb;

  edm::InputTag globalTracks;
  edm::InputTag consTraj;
  std::string ntuplePath;
  float ptMax;
  float ptMin;
  float nTrackerHits;
  float trackerChi2n;
  float nPhihits;
  float nThetahits;
  bool refuseTID, refuseTEC;
  int workingmode;
  int numberOfRootFiles;

  float numberOfSigmasX;
  float numberOfSigmasDXDZ;
  float numberOfSigmasY;
  float numberOfSigmasDYDZ;

  float meanx[5][4][14];
  float sigmax[5][4][14];
  float meandxdz[5][4][14];
  float sigmadxdz[5][4][14];
  float meany[5][4][14];
  float sigmay[5][4][14];
  float meandydz[5][4][14];
  float sigmadydz[5][4][14];


};

#endif
