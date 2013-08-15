#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TKey.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"


#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"

#include "Alignment/CommonAlignment/interface/Alignable.h"  
#include "Alignment/CommonAlignment/interface/AlignableDet.h"
#include "Alignment/CommonAlignment/interface/AlignableNavigator.h"  
#include "Alignment/CommonAlignment/interface/AlignableObjectId.h"  
#include "Alignment/CommonAlignment/interface/AlignmentParameters.h"
#include "Alignment/CommonAlignment/interface/SurveyResidual.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentParameterStore.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentParameterSelector.h"
#include "Alignment/MuonAlignment/interface/AlignableMuon.h"
#include <DataFormats/GeometrySurface/interface/LocalError.h> 
#include "Alignment/TrackerAlignment/interface/AlignableTracker.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "Alignment/MuonAlignmentAlgorithms/plugins/MuonMillepedeAlgorithm.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentAlgorithmPluginFactory.h"

#include "TMatrixD.h"
#include "TF1.h"

using namespace std;

// Constructor ----------------------------------------------------------------

MuonMillepedeAlgorithm::MuonMillepedeAlgorithm(const edm::ParameterSet& cfg):
  AlignmentAlgorithmBase( cfg )
{
  
  // parse parameters
  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] constructed.";

  ntuplePath = cfg.getParameter<std::string>( "ntuplePath" );
  numberOfRootFiles = cfg.getParameter<int>( "numberOfRootFiles" ); 
  ptMax = cfg.getParameter<double>( "ptMax" );
  ptMin = cfg.getParameter<double>( "ptMin" );
  numberOfSigmasX = cfg.getParameter<double>( "numberOfSigmasX" );
  numberOfSigmasDXDZ = cfg.getParameter<double>( "numberOfSigmasDXDZ" );
  numberOfSigmasY = cfg.getParameter<double>( "numberOfSigmasY" );
  numberOfSigmasDYDZ = cfg.getParameter<double>( "numberOfSigmasDYDZ" );
  nPhihits = cfg.getParameter<double>( "nPhihits" );
  nThetahits = cfg.getParameter<double>( "nThetahits" );
  nTrackerHits = cfg.getParameter<double>( "nTrackerHits" );
  trackerChi2n = cfg.getParameter<double>( "trackerChi2n" );
  refuseTEC = cfg.getParameter<bool>( "refuseTEC" );
  refuseTID = cfg.getParameter<bool>( "refuseTID" );
  workingmode = cfg.getParameter<int>( "workingMode" );

  //Opening root file and setting up the tree
  //-----------------------------------------
  if(workingmode == 0) {
    char nameOfFile[200];
    sprintf(nameOfFile, "%s/MyNtupleResidual.root", ntuplePath.c_str());
    f = new TFile(nameOfFile, "RECREATE");
    setBranchTrees();
  } else {
    f = new TFile("ChamberResultCorrected.root", "RECREATE");
    f->cd();
    setBranchOutputTrees(); 
  }
}


// Call at beginning of job ---------------------------------------------------
void 
MuonMillepedeAlgorithm::initialize( const edm::EventSetup& setup, 
				    AlignableTracker* tracker, AlignableMuon* muon, 
				    AlignmentParameterStore* store )
{
  
  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Initializing...";
  
  // accessor Det->AlignableDet
  if ( !muon )
    theAlignableDetAccessor = new AlignableNavigator(tracker);
  else if ( !tracker )
    theAlignableDetAccessor = new AlignableNavigator(muon);
  else 
    theAlignableDetAccessor = new AlignableNavigator(tracker,muon);
  
  // set alignmentParameterStore
  theAlignmentParameterStore=store;
  
  // get alignables
  theAlignables = theAlignmentParameterStore->alignables();
  
}


// Call at end of job ---------------------------------------------------------

void MuonMillepedeAlgorithm::terminate(void)
{

  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Terminating";
  
  if(workingmode == 1) {
    produceAnalysis();
  } else {
    f->Write();
    f->Close();
  }

}



// Run the algorithm on trajectories and tracks -------------------------------
//void MuonMillepedeAlgorithm::run(const edm::EventSetup& setup, const EventInfo &eventInfo)
void MuonMillepedeAlgorithm::run( const edm::EventSetup& setup, const ConstTrajTrackPairCollection& tracks)
{
	  
  if(workingmode == 1) return;
  //3 3 X
  //const ConstTrajTrackPairCollection &tracks = eventInfo.trajTrackPairs_;
  for( ConstTrajTrackPairCollection::const_iterator it=tracks.begin();
       it!=tracks.end();it++) {
    
    const Trajectory *traj = (*it).first;
    const reco::Track *track = (*it).second;
    
    p    = track->p();
    pt    = track->pt();
    eta   = track->eta();
    phi   = track->phi();
    charge = track->charge();
 

    if(pt < ptMin || pt > ptMax) return;
  

    
    vector<TrajectoryStateOnSurface> tsosvec;
    vector<const TransientTrackingRecHit*> hitvec;
    vector<TrajectoryMeasurement> measurements = traj->measurements();
    
    //In this loop the measurements and hits are extracted and put into two vectors 
    int ch_tracker = 0;
    int ch_muons = 0;
    int ch_trackerrefuse = 0;
    float ch_tracker_chi2 = 0.;
    for (vector<TrajectoryMeasurement>::iterator im=measurements.begin();
	 im!=measurements.end(); im++)
      {
	TrajectoryMeasurement meas = *im;
	const TransientTrackingRecHit* hit = &(*meas.recHit());
	//We are not very strict at this point
	if (hit->isValid())
	  {
	    //***Forward
            TrajectoryStateOnSurface tsos = m_tsoscomb(meas.forwardPredictedState(), meas.backwardPredictedState());
            //TrajectoryStateOnSurface tsos = meas.forwardPredictedState();
	    if (tsos.isValid())
	      {
		tsosvec.push_back(tsos);
                hitvec.push_back(hit);
		//Cuts for the tracker
		if(hit->det()->geographicalId().det() == 1) {
		  ch_tracker++;
		  //If there is a hit in the TEC or TID the track won't be considered
		  if((hit->det()->geographicalId().subdetId() == 6 && refuseTEC) ||
		     (hit->det()->geographicalId().subdetId() == 4 && refuseTID)) ch_trackerrefuse++;
		  float xresid = tsos.localPosition().x() - hit->localPosition().x();
                  float xresiderr2 = tsos.localError().positionError().xx() + hit->localPositionError().xx();
                  ch_tracker_chi2 += xresid * xresid / xresiderr2;
		}
		if(hit->det()->geographicalId().det() == 2 && 
		   hit->det()->geographicalId().subdetId() == 1) ch_muons++;
	      }
	  }
      }
    
    
    nhits = ch_tracker;
    chi2 = ch_tracker_chi2;
    chi2n = ch_tracker_chi2/(ch_tracker-1);
   
 
    if(ch_trackerrefuse != 0 || ch_muons > 59) continue;
    if(ch_tracker < nTrackerHits) continue;    
    if(chi2n > trackerChi2n) continue; 


    
    vector<TrajectoryStateOnSurface>::const_iterator itsos=tsosvec.begin();
    vector<const TransientTrackingRecHit*>::const_iterator ihit=hitvec.begin();
    
    int ch_counter = 0;
    while (itsos != tsosvec.end()) 
      {
	const GeomDet* det=(*ihit)->det();
        if(det->geographicalId().det() == 2) {
	  if(det->geographicalId().subdetId() == 1) {
            DTLayerId mLayer(det->geographicalId().rawId());
            DTChamberId mChamber(mLayer.wheel(), mLayer.station(), mLayer.sector());
            AlignableDet *aliDet = theAlignableDetAccessor->alignableDetFromDetId(mChamber);
	    myTrack1D.wh[ch_counter] = mLayer.wheel();
            myTrack1D.st[ch_counter] = mLayer.station();
            myTrack1D.sr[ch_counter] = mLayer.sector();
            myTrack1D.sl[ch_counter] = mLayer.superlayer();
            myTrack1D.la[ch_counter] = mLayer.layer();
            myTrack1D.xl[ch_counter] = (*ihit)->localPosition().x();
	    myTrack1D.yl[ch_counter] = (*ihit)->localPosition().y();
	    myTrack1D.xlp[ch_counter] = (*itsos).localPosition().x();
	    myTrack1D.ylp[ch_counter] = (*itsos).localPosition().y();
            AlgebraicSymMatrix55 myMat = (*itsos).localError().matrix();
            for(int k = 0; k < 4; k++) {
              for(int s = 0; s < 4; s++) myTrack1D.cov[ch_counter][k][s] = myMat(k+1,s+1);
            } 
            myTrack1D.erx[ch_counter] = (*ihit)->localPositionError().xx();
            myTrack1D.ery[ch_counter] = (*ihit)->localPositionError().yy();
	    GlobalPoint globhit = det->toGlobal((*ihit)->localPosition());
	    GlobalPoint globtsos = det->toGlobal((*itsos).localPosition());
            LocalPoint seghit = aliDet->surface().toLocal(globhit);
            LocalPoint segtsos = aliDet->surface().toLocal(globtsos);
            myTrack1D.xc[ch_counter] = seghit.x();
	    myTrack1D.yc[ch_counter] = seghit.y();
	    myTrack1D.zc[ch_counter] = seghit.z();
	    myTrack1D.xcp[ch_counter] = segtsos.x();
	    myTrack1D.ycp[ch_counter] = segtsos.y();
	    myTrack1D.zcp[ch_counter] = segtsos.z();
            myTrack1D.xg[ch_counter] = globhit.x();
	    myTrack1D.yg[ch_counter] = globhit.y();
	    myTrack1D.zg[ch_counter] = globhit.z();
	    myTrack1D.xgp[ch_counter] = globtsos.x();
	    myTrack1D.ygp[ch_counter] = globtsos.y();
	    myTrack1D.zgp[ch_counter] = globtsos.z();
	    ch_counter++;
	  }
        }
        itsos++;
        ihit++;
      }
    myTrack1D.nhits = ch_counter;
    build4DSegments(); 
    
    ttreeOutput->Fill();
  }
}




//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::build4DSegments() {
  
  //Set to 0
  int id[20][5];
  for(int s = 0; s < 20; ++s) {
    for(int k = 0; k < 5; ++k) id[s][k] = 0;
  }
  
  
  int nChambers = 0;
  for(int counter = 0; counter < myTrack1D.nhits; ++counter) {
    bool isNew = true;
    for(int counterCham = 0; counterCham < nChambers; counterCham++) {
      if(myTrack1D.wh[counter] == id[counterCham][0] &&
	 myTrack1D.st[counter] == id[counterCham][1] &&
         myTrack1D.sr[counter] == id[counterCham][2]) {
	if(myTrack1D.sl[counter] == 2) { 
	  id[counterCham][4]++;
	} else {
	  id[counterCham][3]++;
	}
	isNew = false;
      } 
    }
    if(isNew) {
      id[nChambers][0] = myTrack1D.wh[counter];
      id[nChambers][1] = myTrack1D.st[counter];
      id[nChambers][2] = myTrack1D.sr[counter];
      if(myTrack1D.sl[counter] == 2) {
        id[nChambers][4]++;
      } else {
        id[nChambers][3]++;
      }
      nChambers++;
    }
  }
  
  
  
  int nsegC = 0;
  for(int counter = 0; counter < nChambers; ++counter) {
    
    if(id[counter][3] > nPhihits && (id[counter][4] > nThetahits || id[counter][1] == 4)) {
      
      TMatrixD phiProjection(2,2);
      TMatrixD thetaProjection(2,2);
      TMatrixD bphiProjection(2,1);
      TMatrixD bthetaProjection(2,1);
      TMatrixD phiProjectionP(2,2);
      TMatrixD thetaProjectionP(2,2);
      TMatrixD bphiProjectionP(2,1);
      TMatrixD bthetaProjectionP(2,1);
      TMatrixD phiProjectionRes(2,2);
      TMatrixD thetaProjectionRes(2,2);
      TMatrixD bphiProjectionRes(2,1);
      TMatrixD bthetaProjectionRes(2,1);
      
      for(int counterH = 0; counterH < myTrack1D.nhits; ++counterH) {
        if(myTrack1D.wh[counterH] == id[counter][0] && myTrack1D.st[counterH] == id[counter][1] &&
           myTrack1D.sr[counterH] == id[counter][2]) {
          cov_xx[nsegC] = myTrack1D.cov[counterH][2][2];
          cov_yy[nsegC] = myTrack1D.cov[counterH][3][3];
          cov_dxdx[nsegC] = myTrack1D.cov[counterH][0][0];
          cov_dydy[nsegC] = myTrack1D.cov[counterH][1][1];
          cov_xdx[nsegC] = myTrack1D.cov[counterH][2][0];
          cov_xdy[nsegC] = myTrack1D.cov[counterH][2][1];
          cov_xy[nsegC] = myTrack1D.cov[counterH][2][3];
          cov_ydx[nsegC] = myTrack1D.cov[counterH][3][0];
          cov_ydy[nsegC] = myTrack1D.cov[counterH][3][1];
          cov_dxdy[nsegC] = myTrack1D.cov[counterH][0][1];
	  if(myTrack1D.sl[counterH] == 2) {
	    thetaProjection(0,0) += 1.0;
            thetaProjection(0,1) += myTrack1D.zc[counterH];
            thetaProjection(1,0) += myTrack1D.zc[counterH];
            thetaProjection(1,1) += myTrack1D.zc[counterH]*myTrack1D.zc[counterH];
            bthetaProjection(0,0) += myTrack1D.yc[counterH];
            bthetaProjection(1,0) += myTrack1D.yc[counterH]*myTrack1D.zc[counterH];
            thetaProjectionP(0,0) += 1.0;
            thetaProjectionP(0,1) += myTrack1D.zcp[counterH];
            thetaProjectionP(1,0) += myTrack1D.zcp[counterH];
            thetaProjectionP(1,1) += myTrack1D.zcp[counterH]*myTrack1D.zcp[counterH];
            bthetaProjectionP(0,0) += myTrack1D.ycp[counterH];
            bthetaProjectionP(1,0) += myTrack1D.ycp[counterH]*myTrack1D.zcp[counterH];
            thetaProjectionRes(0,0) += 1.0/myTrack1D.erx[counterH];
            thetaProjectionRes(0,1) += myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            thetaProjectionRes(1,0) += myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            thetaProjectionRes(1,1) += myTrack1D.zcp[counterH]*myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            bthetaProjectionRes(0,0) += (myTrack1D.yc[counterH]-myTrack1D.ycp[counterH])/myTrack1D.erx[counterH];
            bthetaProjectionRes(1,0) += (myTrack1D.yc[counterH]-myTrack1D.ycp[counterH])*myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
          } else {
	    phiProjection(0,0) += 1.0;
            phiProjection(0,1) += myTrack1D.zc[counterH];
            phiProjection(1,0) += myTrack1D.zc[counterH];
            phiProjection(1,1) += myTrack1D.zc[counterH]*myTrack1D.zc[counterH];
            bphiProjection(0,0) += myTrack1D.xc[counterH];
            bphiProjection(1,0) += myTrack1D.xc[counterH]*myTrack1D.zc[counterH];
            phiProjectionP(0,0) += 1.0;
            phiProjectionP(0,1) += myTrack1D.zcp[counterH];
            phiProjectionP(1,0) += myTrack1D.zcp[counterH];
            phiProjectionP(1,1) += myTrack1D.zcp[counterH]*myTrack1D.zcp[counterH];
            bphiProjectionP(0,0) += myTrack1D.xcp[counterH];
            bphiProjectionP(1,0) += myTrack1D.xcp[counterH]*myTrack1D.zcp[counterH];
            phiProjectionRes(0,0) += 1.0/myTrack1D.erx[counterH];
            phiProjectionRes(0,1) += myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            phiProjectionRes(1,0) += myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            phiProjectionRes(1,1) += myTrack1D.zcp[counterH]*myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
            bphiProjectionRes(0,0) += (myTrack1D.xc[counterH]-myTrack1D.xcp[counterH])/myTrack1D.erx[counterH];
            bphiProjectionRes(1,0) += (myTrack1D.xc[counterH]-myTrack1D.xcp[counterH])*myTrack1D.zcp[counterH]/myTrack1D.erx[counterH];
	    
          }
	}
      }
      
      if (phiProjection(0,0) != 0 && (thetaProjection(0,0) != 0 || id[counter][1] == 4)) {
	
	whS[nsegC] = id[counter][0];
	stS[nsegC] = id[counter][1];
	srS[nsegC] = id[counter][2];
	
	if(thetaProjection(0,0) != 0 && id[counter][1] != 4) { // Already asked (almost)
	  thetaProjection.Invert();
	  thetaProjectionP.Invert();
	  thetaProjectionRes.Invert();
	  TMatrixD solution = thetaProjection*bthetaProjection;
	  TMatrixD solutionP = thetaProjectionP*bthetaProjectionP;
	  TMatrixD solutionRes = thetaProjectionRes*bthetaProjectionRes;
	  ySl[nsegC] = solution(0,0);
	  ySlp[nsegC] = solutionP(0,0);
	  dydzSl[nsegC] = solution(1,0);
	  dydzSlp[nsegC] = solutionP(1,0);
	  resy[nsegC] = solutionRes(0,0);
	  resdydz[nsegC] = solutionRes(1,0);
	  eySl[nsegC] = thetaProjection(0,0);
	  eresy[nsegC] = thetaProjectionRes(0,0);
	  eySlp[nsegC] = thetaProjectionP(0,0);
	  eresdydz[nsegC] = thetaProjectionRes(1,1);
	  eresydydz[nsegC] = thetaProjectionRes(1,0);
	  edydzSl[nsegC] = thetaProjection(1,1);
	  edydzSlp[nsegC] = thetaProjectionP(1,1);
	  eydydzSl[nsegC] = thetaProjection(0,1);
	  eydydzSlp[nsegC] = thetaProjectionP(0,1);
	}
	if(phiProjection(0,0) != 0) { // Already asked 
	  phiProjection.Invert();
	  phiProjectionP.Invert();
	  phiProjectionRes.Invert();
	  TMatrixD solution = phiProjection*bphiProjection;
	  TMatrixD solutionP = phiProjectionP*bphiProjectionP;
	  TMatrixD solutionRes = phiProjectionRes*bphiProjectionRes;
	  xSl[nsegC] = solution(0,0);
	  xSlp[nsegC] = solutionP(0,0);
	  resx[nsegC] = solutionRes(0,0);
	  dxdzSl[nsegC] = solution(1,0);
	  dxdzSlp[nsegC] = solutionP(1,0);
	  resdxdz[nsegC] = solutionRes(1,0);
	  exSl[nsegC] = phiProjection(0,0);
	  exSlp[nsegC] = phiProjectionP(0,0);
	  eresx[nsegC] = phiProjectionRes(0,0);
	  edxdzSl[nsegC] = phiProjection(1,1);
	  edxdzSlp[nsegC] = phiProjectionP(1,1);
	  eresdxdz[nsegC] = phiProjectionRes(1,1);
	  eresxdxdz[nsegC] = phiProjectionRes(0,1);
	  exdxdzSl[nsegC] = phiProjection(0,0);
	  exdxdzSlp[nsegC] = phiProjectionP(0,1);
	}
	nsegC++;
      }
    }
  }
  nseg = nsegC;
}



//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::initNTuples() {

  tali = new TChain("Residual1D", "Residual1D");
  
  for(int n_file = 0; n_file < numberOfRootFiles; ++n_file) {
    char theNameOfTheFile[250];
    sprintf(theNameOfTheFile, "%sMyNtupleResidual_%d.root", ntuplePath.c_str(), n_file);
    cout << "Opening File " << theNameOfTheFile << endl;
    tali->Add(theNameOfTheFile);
  }
 
  setBranchAddressTrees();

}


//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::fitDistributions(int whC, int stC, int seC, int res, TH1D *h) {

  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Antes del switch 2";
  char fitFunction[160]; sprintf(fitFunction, "[0]*[2]**2/((x-[1])**2+[2]**2)", "%s");
  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Antes del switch 3";
  float nEntry = h->GetEntries();
  float rms = h->GetRMS();
  float mean = h->GetMean();
  
  TF1 tt("fit", fitFunction, mean-1.0*rms, mean+1.0*rms);
  tt.SetParameters(nEntry/20.0, mean, rms/2.0);
  h->Fit(&tt, "", "", mean-1.0*rms, mean+1.0*rms);
  
  switch(res) {
    case 1:
      meanx[whC+2][stC-1][seC-1] = tt.GetParameter(1);
      sigmax[whC+2][stC-1][seC-1] = tt.GetParameter(2);
      break; 
    case 2:
      meandxdz[whC+2][stC-1][seC-1] = tt.GetParameter(1);
      sigmadxdz[whC+2][stC-1][seC-1] = tt.GetParameter(2);
      break; 
    case 3:
      meany[whC+2][stC-1][seC-1] = tt.GetParameter(1);
      sigmay[whC+2][stC-1][seC-1] = tt.GetParameter(2);
      break; 
    case 4:
      meandydz[whC+2][stC-1][seC-1] = tt.GetParameter(1);
      sigmadydz[whC+2][stC-1][seC-1] = tt.GetParameter(2);
      break; 
  }
}


//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::preSelection() {
  


  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Entering preselection";

  //Histograms for the residuals distribution
  TH1D **** hx = new TH1D ***[5];
  TH1D **** hdxdz = new TH1D ***[5];
  TH1D **** hy = new TH1D ***[5];
  TH1D **** hdydz = new TH1D ***[5];

  for(int whC = -2; whC < 3; ++whC) {
    hx[whC+2] = new TH1D **[4]; hdxdz[whC+2] = new TH1D **[4];
    hy[whC+2] = new TH1D **[4]; hdydz[whC+2] = new TH1D **[4];
    for(int stC = 1; stC < 5; ++stC) {
      hx[whC+2][stC-1] = new TH1D * [14]; hdxdz[whC+2][stC-1] = new TH1D * [14];
      hy[whC+2][stC-1] = new TH1D * [14]; hdydz[whC+2][stC-1] = new TH1D * [14];
      for(int seC = 1; seC < 15; ++seC) {
        if(seC>12 && stC != 4) continue;
        char namex[20], namedxdz[20], namey[20], namedydz[20];
        sprintf(namex, "x_%d_%d_%d", whC, stC, seC); sprintf(namedxdz, "dxdz_%d_%d_%d", whC, stC, seC);
        sprintf(namey, "y_%d_%d_%d", whC, stC, seC); sprintf(namedydz, "dydz_%d_%d_%d", whC, stC, seC);
        hx[whC+2][stC-1][seC-1] = new TH1D(namex, namex, 100, -4, 4);
        hdxdz[whC+2][stC-1][seC-1] = new TH1D(namedxdz, namedxdz, 100, -0.03, 0.03);
        if(st != 4) {
          hy[whC+2][stC-1][seC-1] = new TH1D(namey, namey, 100, -4, 4);
          hdydz[whC+2][stC-1][seC-1] = new TH1D(namedydz, namedydz, 100, -0.05, 0.05);
        }
      }
    }
  }
  
  //Run over the TTree  
  Int_t nentries = (Int_t)tali->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    tali->GetEntry(i);
    //Basic cuts
    if(pt > ptMax || pt < ptMin ||
       nhits < nTrackerHits || chi2/(nhits-1) > trackerChi2n) continue;
    for(int counter = 0; counter < nseg; ++counter) {
      float resxC = resx[counter]; float resdxdzC = resdxdz[counter];
      float resyC = resy[counter]; float resdydzC = resdydz[counter];
      int whC = whS[counter]; int stC = stS[counter]; int seC = srS[counter];
      if(fabs(whC) > 3.0 || stC < 1 || stC > 4 || seC < 1 || seC > 14) continue;  
      hx[whC+2][stC-1][seC-1]->Fill(resxC);
      hdxdz[whC+2][stC-1][seC-1]->Fill(resdxdzC);
      if(stC != 4) {
        hy[whC+2][stC-1][seC-1]->Fill(resyC);
        hdydz[whC+2][stC-1][seC-1]->Fill(resdydzC);
      }
    }
  }
 
  //Fit the histograms 
  
  for(int whC = -2; whC < 3; ++whC) {
    for(int stC = 1; stC < 5; ++stC) {
      for(int seC = 1; seC < 15; ++seC) {
        if(seC>12 && stC != 4) continue;
        fitDistributions(whC, stC, seC, 1, hx[whC+2][stC-1][seC-1]);
        fitDistributions(whC, stC, seC, 2, hdxdz[whC+2][stC-1][seC-1]);
        if(whC != 4) {
          fitDistributions(whC, stC, seC, 3, hy[whC+2][stC-1][seC-1]);
          fitDistributions(whC, stC, seC, 4, hdydz[whC+2][stC-1][seC-1]);
        }
      }
    }
  } 
}

//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::calculation() {

  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Entering calculation";
  
  TMatrixD ****C = new TMatrixD ***[5];
  TMatrixD ****b = new TMatrixD ***[5];
  
  for(int whC = -2; whC < 3; ++whC) {
    C[whC+2] = new TMatrixD **[4];
    b[whC+2] = new TMatrixD **[4];
    for(int stC = 1; stC < 5; ++stC) {
      C[whC+2][stC-1] = new TMatrixD * [14];
      b[whC+2][stC-1] = new TMatrixD * [14];
      for(int seC = 1; seC < 15; ++seC) {
        if(seC > 12 && stC != 4) continue;
        if(stC != 4) {
          C[whC+2][stC-1][seC-1] = new TMatrixD(6,6);
          b[whC+2][stC-1][seC-1] = new TMatrixD(6,1);
        } else {
          C[whC+2][stC-1][seC-1] = new TMatrixD(3,3);
          b[whC+2][stC-1][seC-1] = new TMatrixD(3,1);
        }
      }
    }
  }
  
  //Run over the TTree  
  Int_t nentries = (Int_t)tali->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    tali->GetEntry(i);
    //Basic cuts
    if(pt > ptMax || pt < ptMin ||
        nhits < nTrackerHits || chi2/(nhits-1) > trackerChi2n) continue;

    for(int counter = 0; counter < nseg; ++counter) {
      float x = xSl[counter]; float xp = xSlp[counter];
      float y = ySl[counter]; float yp = ySlp[counter];
      float dxdz = dxdzSl[counter]; float dxdzp = dxdzSlp[counter];
      float dydz = dydzSl[counter]; float dydzp = dydzSlp[counter];
      float ex[3] = {exSl[counter], edxdzSl[counter], exdxdzSl[counter]};
      float ey[3] = {eySl[counter], edydzSl[counter], eydydzSl[counter]};
      float e_ext[10] = {cov_xx[counter], cov_yy[counter], cov_dxdx[counter], cov_dydy[counter],
                         cov_xy[counter], cov_xdx[counter], cov_xdy[counter], cov_ydx[counter],
                         cov_ydy[counter], cov_dxdy[counter]}; 
      float resxC = resx[counter]; float resdxdzC = resdxdz[counter];
      float resyC = resy[counter]; float resdydzC = resdydz[counter];
      int wheel = whS[counter]; int station = stS[counter]; int sector = srS[counter];
      if(fabs(wheel) > 3.0 || station > 5.0 || station < 1 || sector > 15 || sector < 1) continue;
      double mx = meanx[wheel+2][station-1][sector-1]; double sx = sigmax[wheel+2][station-1][sector-1];
      double mdxdz = meandxdz[wheel+2][station-1][sector-1]; double sdxdz = sigmadxdz[wheel+2][station-1][sector-1];
      double my = meany[wheel+2][station-1][sector-1]; double sy = sigmay[wheel+2][station-1][sector-1];
      double mdydz = meandydz[wheel+2][station-1][sector-1]; double sdydz = sigmadydz[wheel+2][station-1][sector-1];
      //Tail selection
      if(station != 4 && 
         ((x-xp) > mx + numberOfSigmasX*sx || (x-xp) < mx - numberOfSigmasX*sx || 
         (dxdz-dxdzp) > mdxdz + numberOfSigmasDXDZ*sdxdz || (dxdz-dxdzp) < mdxdz - numberOfSigmasDXDZ*sdxdz ||
         (y-yp) > my + numberOfSigmasY*sy || (y-yp) < my - numberOfSigmasY*sy ||
         (dydz-dydzp) > mdydz + numberOfSigmasDYDZ*sdydz || (dydz-dydzp) < mdydz - numberOfSigmasDYDZ*sdydz)) continue;

      if(station == 4 && 
         ((x-xp) > mx + numberOfSigmasX*sx || (x-xp) < mx - numberOfSigmasX*sx || 
         (dxdz-dxdzp) > mdxdz + numberOfSigmasDXDZ*sdxdz || (dxdz-dxdzp) < mdxdz - numberOfSigmasDXDZ*sdxdz)) continue;
     
      if(station == 4) { 
        TMatrixD B = returnBMB4Matrix(xp,dxdzp);
        TMatrixD E = returnEMB4Matrix(ex, e_ext);
        if(E(0,0) == 0) continue;  
        TMatrixD R = returnResMB4(resxC, resdxdzC);
        TMatrixD Bt = B;
        Bt.T();
        TMatrixD Cj = Bt*E*B;
        TMatrixD bj = Bt*E*R;
        *(C[wheel+2][station-1][sector-1]) += Cj;
        *(b[wheel+2][station-1][sector-1]) += bj;
      } else {
	TMatrixD B = returnBMatrix(xp,dxdzp,yp,dydzp);
	TMatrixD E = returnEMatrix(ex, ey, e_ext);
        if(E(0,0) == 0) continue;
	TMatrixD R = returnRes(resxC, resdxdzC, resyC, resdydzC);
  	TMatrixD Bt = B;
        Bt.T();
      	TMatrixD Cj = Bt*E*B;
        TMatrixD bj = Bt*E*R;
	*(C[wheel+2][station-1][sector-1]) += Cj;
        *(b[wheel+2][station-1][sector-1]) += bj;
      }
    }
  }
  
  for(int wheel = -2; wheel < 3; ++wheel) {
    for(int station = 1; station < 5; ++station) {
      for(int sector = 1; sector < 15; ++sector) {
	if(sector > 12 && station != 4) continue;
        	
        TMatrixD solution(6,1);
        TMatrixD Cova(6,6);
        solution.Zero();
        Cova.Zero();
        
        if(station == 4) {
          TMatrixD cov = *(C[wheel+2][station-1][sector-1]);
          TMatrixD bs = *(b[wheel+2][station-1][sector-1]);
          cov.Invert();
          TMatrixD sol = cov * bs;
          solution(0,0) = sol(0,0); solution(1,0) = sol(1,0); solution(4,0) = sol(2,0);
          Cova(0,0) = cov(0,0); Cova(1,0) = cov(1,0); Cova(0,1) = cov(0,1);
          Cova(1,1) = cov(1,1); Cova(4,0) = cov(2,0); Cova(4,1) = cov(2,1);
          Cova(4,4) = cov(2,2); Cova(0,4) = cov(0,2); Cova(1,4) = cov(1,2);
        } else {
          TMatrixD cov = *(C[wheel+2][station-1][sector-1]);
          TMatrixD bs = *(b[wheel+2][station-1][sector-1]);
          cov.Invert();
          TMatrixD sol = cov * bs;
          solution = sol;
          Cova = cov;
        }  
       
        wh = wheel; st = station; sr = sector;
        dx = solution(0,0); dz = solution(1,0); dy = solution(2,0);
        phix = solution(3,0); phiy = solution(4,0); phiz = solution(5,0);
        for(int k = 0; k < 6; ++k) {
          for(int s = 0; s < 6; ++s) cov[k][s] = Cova(k,s);
        }
        ttreeOutput->Fill();
      }
    }
  }
  f->Write();
  f->Close();
  edm::LogWarning("Alignment") << "[MuonMillepedeAlgorithm] Leaving calculation";
}


//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnBMatrix(float x, float dxdz, float y, float dydz) {
  
  TMatrixD matrix(4, 6);
  matrix(0,0) = -1; matrix(0,1) = dxdz; matrix(0,2) = 0;
  matrix(0,3) = -y*dxdz; matrix(0,4) = x*dxdz; matrix(0,5) = -y;
  
  matrix(1,0) = 0; matrix(1,1) = 0; matrix(1,2) = 0;
  matrix(1,3) = -dxdz*dydz; matrix(1,4) = 1+dxdz*dxdz; matrix(1,5) = -dydz;
  
  matrix(2,0) = 0; matrix(2,1) = dydz; matrix(2,2) = -1;
  matrix(2,3) = -y*dydz; matrix(2,4) = x*dydz; matrix(2,5) = x;

  matrix(3,0) = 0; matrix(3,1) = 0; matrix(3,2) = 0;
  matrix(3,3) = -1-dydz*dydz; matrix(3,4) =  dxdz*dydz; matrix(3,5) =  dxdz;
  
  return matrix;

} 

//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnBMB4Matrix(float x, float dxdz) {
  
  TMatrixD matrix(2, 3);
  matrix(0,0) = -1; matrix(0,1) = dxdz; matrix(0,2) = x*dxdz;

  matrix(1,0) = 0;  matrix(1,1) = 0; matrix(1,2) = 1+dxdz*dxdz;

  return matrix;

}

//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnEMatrix(float *ex, float *ey, float *e_ex) {
  
  TMatrixD CovS(4,4);
  CovS.Zero();
  TMatrixD CovE(4,4);
  CovE.Zero();
  
  CovS(0,0) = ex[0];
  CovS(1,0) = ex[2];
  CovS(0,1) = ex[2];
  CovS(1,1) = ex[1];
  CovS(2,2) = ey[0];
  CovS(3,2) = ey[2];
  CovS(2,3) = ey[2];
  CovS(2,2) = ey[1];
  
  CovE(0,0) = e_ex[0];
  CovE(1,1) = e_ex[2];
  CovE(2,2) = e_ex[1];
  CovE(3,3) = e_ex[3];
  CovE(0,1) = e_ex[5];
  CovE(1,0) = e_ex[5];
  CovE(0,2) = e_ex[4];
  CovE(2,0) = e_ex[4];
  CovE(0,3) = e_ex[6];
  CovE(3,0) = e_ex[6];
  CovE(1,2) = e_ex[7];
  CovE(2,1) = e_ex[7];
  CovE(1,3) = e_ex[9];
  CovE(3,1) = e_ex[9];
  CovE(2,3) = e_ex[8];
  CovE(3,2) = e_ex[8];
 
  TMatrixD Cov = CovS+CovE;
 
  double det; Cov.Invert(&det);
  if(det == 0) Cov.Zero();
  return Cov;
}

//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnEMB4Matrix(float *ex, float *e_x) {

  TMatrixD CovS(2,2);
  CovS.Zero();
  TMatrixD CovE(2,2);
  CovE.Zero();

  CovS(0,0) = ex[0];
  CovS(1,0) = ex[2];
  CovS(0,1) = ex[2];
  CovS(1,1) = ex[1];

  CovE(0,0) = e_x[0];
  CovE(1,1) = e_x[2];
  CovE(0,1) = e_x[5];
  CovE(1,0) = e_x[5];


  TMatrixD Cov = CovS + CovE;
  double det; Cov.Invert(&det);
  if(det == 0) Cov.Zero();
  return Cov;
}

//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnRes(float resx, float resdxdz, float resy, float resdydz) {

  TMatrixD res(4,1);
  res(0,0) = resx; res(1,0) = resdxdz;
  res(2,0) = resy; res(3,0) = resdydz;

  return res;   

}

//--------------------------------------------------------------------------------------------------------------
TMatrixD MuonMillepedeAlgorithm::returnResMB4(float resx, float resdxdz) {

  TMatrixD res(2,1);
  res(0,0) = resx; res(1,0) = resdxdz;
  return res;

}


//--------------------------------------------------------------------------------------------------------------
void MuonMillepedeAlgorithm::produceAnalysis() {

  initNTuples();
 
  preSelection();
  
  calculation();

}



void MuonMillepedeAlgorithm::setBranchTrees() {

  ttreeOutput = new TTree("Residual1D", "Residual1D");
  ttreeOutput->Branch("p", &p, "p/F");
  ttreeOutput->Branch("pt", &pt, "pt/F");
  ttreeOutput->Branch("eta", &eta, "eta/F");
  ttreeOutput->Branch("phi", &phi, "phi/F");
  ttreeOutput->Branch("chi2", &chi2, "chi2/F");
  ttreeOutput->Branch("chi2n", &chi2n, "chi2n/F");
  ttreeOutput->Branch("charge", &charge, "charge/F");
  ttreeOutput->Branch("nhits", &nhits, "nhits/I");
  ttreeOutput->Branch("nseg", &nseg, "nseg/I");
  ttreeOutput->Branch("xSl", xSl, "xSl[nseg]/F");
  ttreeOutput->Branch("xSlp", xSlp, "xSlp[nseg]/F");
  ttreeOutput->Branch("resx", resx, "resx[nseg]/F");
  ttreeOutput->Branch("dxdzSl", dxdzSl, "dxdzSl[nseg]/F");
  ttreeOutput->Branch("dxdzSlp", dxdzSlp, "dxdzSlp[nseg]/F");
  ttreeOutput->Branch("resdxdz", resdxdz, "resdxdz[nseg]/F");
  ttreeOutput->Branch("exSl", exSl, "exSl[nseg]/F");
  ttreeOutput->Branch("exSlp", exSlp, "exSlp[nseg]/F");
  ttreeOutput->Branch("eresx", eresx, "eresx[nseg]/F");
  ttreeOutput->Branch("edxdzSl", edxdzSl, "edxdzSl[nseg]/F");
  ttreeOutput->Branch("edxdzSlp", edxdzSlp, "edxdzSlp[nseg]/F");
  ttreeOutput->Branch("eresdxdz", eresdxdz, "eresdxdz[nseg]/F");
  ttreeOutput->Branch("exdxdzSl", exdxdzSl, "exdxdzSl[nseg]/F");
  ttreeOutput->Branch("exdxdzSlp", exdxdzSlp, "exdxdzSlp[nseg]/F");
  ttreeOutput->Branch("eresxdxdz", eresdxdz, "eresxdxdz[nseg]/F");
  ttreeOutput->Branch("cov_xx", cov_xx, "cov_xx[nseg]/F");
  ttreeOutput->Branch("cov_yy", cov_yy, "cov_yy[nseg]/F");
  ttreeOutput->Branch("cov_dxdx", cov_dxdx, "cov_dxdx[nseg]/F");
  ttreeOutput->Branch("cov_dydy", cov_dydy, "cov_dydy[nseg]/F");
  ttreeOutput->Branch("cov_xdx", cov_xdx, "cov_xdx[nseg]/F");
  ttreeOutput->Branch("cov_xdy", cov_xdy, "cov_xdy[nseg]/F");
  ttreeOutput->Branch("cov_xy", cov_xy, "cov_xy[nseg]/F");
  ttreeOutput->Branch("cov_ydx", cov_ydx, "cov_ydx[nseg]/F");
  ttreeOutput->Branch("cov_ydy", cov_ydy, "cov_ydy[nseg]/F");
  ttreeOutput->Branch("cov_dxdy", cov_dxdy, "cov_dxdy[nseg]/F");
  ttreeOutput->Branch("ySl", ySl, "ySl[nseg]/F");
  ttreeOutput->Branch("ySlp", ySlp, "ySlp[nseg]/F");
  ttreeOutput->Branch("resy", resy, "resy[nseg]/F");
  ttreeOutput->Branch("dydzSl", dydzSl, "dydzSl[nseg]/F");
  ttreeOutput->Branch("dydzSlp", dydzSlp, "dydzSlp[nseg]/F");
  ttreeOutput->Branch("resdydz", resdydz, "resdydz[nseg]/F");
  ttreeOutput->Branch("eySl", eySl, "eySl[nseg]/F");
  ttreeOutput->Branch("eySlp", eySlp, "eySlp[nseg]/F");
  ttreeOutput->Branch("eresy", eresy, "eresy[nseg]/F");
  ttreeOutput->Branch("edydzSl", edydzSl, "edydzSl[nseg]/F");
  ttreeOutput->Branch("edydzSlp", edydzSlp, "edydzSlp[nseg]/F");
  ttreeOutput->Branch("eresdydz", eresdydz, "eresdydz[nseg]/F");
  ttreeOutput->Branch("eydydzSl", eydydzSl, "eydydzSl[nseg]/F");
  ttreeOutput->Branch("eydydzSlp", eydydzSlp, "eydydzSlp[nseg]/F");
  ttreeOutput->Branch("eresydydz", eresdydz, "eresydydz[nseg]/F");
  ttreeOutput->Branch("whS", whS, "whS[nseg]/I");
  ttreeOutput->Branch("stS", stS, "stS[nseg]/I");
  ttreeOutput->Branch("srS", srS, "srS[nseg]/I");
 

}


void MuonMillepedeAlgorithm::setBranchAddressTrees() {

  tali->SetBranchAddress("p", &p);
  tali->SetBranchAddress("pt", &pt);
  tali->SetBranchAddress("eta", &eta);
  tali->SetBranchAddress("phi", &phi);
  tali->SetBranchAddress("chi2", &chi2);
  tali->SetBranchAddress("chi2n", &chi2n);
  tali->SetBranchAddress("charge", &charge);
  tali->SetBranchAddress("nhits", &nhits);
  tali->SetBranchAddress("nseg", &nseg);
  tali->SetBranchAddress("xSl", xSl);
  tali->SetBranchAddress("xSlp", xSlp);
  tali->SetBranchAddress("resx", resx);
  tali->SetBranchAddress("dxdzSl", dxdzSl);
  tali->SetBranchAddress("dxdzSlp", dxdzSlp);
  tali->SetBranchAddress("resdxdz", resdxdz);
  tali->SetBranchAddress("exSl", exSl);
  tali->SetBranchAddress("exSlp", exSlp);
  tali->SetBranchAddress("eresx", eresx);
  tali->SetBranchAddress("edxdzSl", edxdzSl);
  tali->SetBranchAddress("edxdzSlp", edxdzSlp);
  tali->SetBranchAddress("eresdxdz", eresdxdz);
  tali->SetBranchAddress("exdxdzSl", exdxdzSl);
  tali->SetBranchAddress("exdxdzSlp", exdxdzSlp);
  tali->SetBranchAddress("eresxdxdz", eresdxdz);
  tali->SetBranchAddress("cov_xx", cov_xx);
  tali->SetBranchAddress("cov_yy", cov_yy);
  tali->SetBranchAddress("cov_dxdx", cov_dxdx);
  tali->SetBranchAddress("cov_dydy", cov_dydy);
  tali->SetBranchAddress("cov_xdx", cov_xdx);
  tali->SetBranchAddress("cov_xdy", cov_xdy);
  tali->SetBranchAddress("cov_xy", cov_xy);
  tali->SetBranchAddress("cov_ydx", cov_ydx);
  tali->SetBranchAddress("cov_ydy", cov_ydy);
  tali->SetBranchAddress("cov_dxdy", cov_dxdy);
  tali->SetBranchAddress("ySl", ySl);
  tali->SetBranchAddress("ySlp", ySlp);
  tali->SetBranchAddress("resy", resy);
  tali->SetBranchAddress("dxdzSl", dxdzSl);
  tali->SetBranchAddress("dydzSlp", dydzSlp);
  tali->SetBranchAddress("resdydz", resdydz);
  tali->SetBranchAddress("eySl", eySl);
  tali->SetBranchAddress("eySlp", eySlp);
  tali->SetBranchAddress("eresy", eresy);
  tali->SetBranchAddress("edydzSl", edydzSl);
  tali->SetBranchAddress("edydzSlp", edydzSlp);
  tali->SetBranchAddress("eresdydz", eresdydz);
  tali->SetBranchAddress("eydydzSl", eydydzSl);
  tali->SetBranchAddress("eydydzSlp", eydydzSlp);
  tali->SetBranchAddress("eresydydz", eresdydz);
  tali->SetBranchAddress("whS", whS);
  tali->SetBranchAddress("stS", stS);
  tali->SetBranchAddress("srS", srS);

}

void MuonMillepedeAlgorithm::setBranchOutputTrees() {

   ttreeOutput = new TTree("ChamberAlignmentResult", "ChamberAlignmentResult");

   ttreeOutput->Branch("wh", &wh, "wh/I"); 
   ttreeOutput->Branch("st", &st, "st/I"); 
   ttreeOutput->Branch("sr", &sr, "sr/I"); 
   ttreeOutput->Branch("dx", &dx, "dx/I"); 
   ttreeOutput->Branch("dy", &dy, "dy/I"); 
   ttreeOutput->Branch("dz", &dz, "dz/I"); 
   ttreeOutput->Branch("phix", &phix, "phix/I"); 
   ttreeOutput->Branch("phiy", &phiy, "phiy/I"); 
   ttreeOutput->Branch("phiz", &phiz, "phiz/I"); 
   ttreeOutput->Branch("cov", &cov, "cov[6][6]/I"); 

}

DEFINE_EDM_PLUGIN( AlignmentAlgorithmPluginFactory, MuonMillepedeAlgorithm, "MuonMillepedeAlgorithm" );

