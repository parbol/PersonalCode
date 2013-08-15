#include "InternalAnalysis.h"
#include <TMath.h>
#include "ReadPGInfo.cc"
#include <TVectorD.h>


//**** The constructor
InternalAnalysis::InternalAnalysis(int wheel, int station, int sector) {
  
  this->wheel = wheel;
  this->station = station;
  this->sector = sector;

  NDOFTrack = 2;
  NDOFAlign = 5;
  NDOFChamber = 8;
  NDOFCoor = 1;
  NDOFAlign = 5;
  
  hist = new TH1F("Intercept", "Intercept sl1 -sl2", 100, -1, 1);
  
  C.ResizeTo(NDOFChamber*NDOFAlign, NDOFChamber*NDOFAlign);
  b.ResizeTo(NDOFChamber*NDOFAlign, 1);
  
  numberOfMuons = 0;
  
  bool hasTracks = false;
  textInfo.open("info.txt");
  if(textInfo.good()) {
    //Starts tracks reading
    hasTracks = true;
    begin();
   
    cout << "Tracks ok" << endl;
  }
  PHSuperLayer();
  cout << "PHSL ok" << endl;
  PHLayer();
  cout << "PHL ok" << endl;
  AddConstrain();
  cout << "constrain" << endl;
  Calculate();
  cout << "Calculation" << endl;
  if(hasTracks) {
    MakeComparison();
    MakeComparisonSurvey();
    writeTracks();
  }
  PrintSolution();
}



void InternalAnalysis::writeTracks() {
  TFile f("subMatrices.root", "RECREATE");
  C.Write("C");
  b.Write("b");
  CPH.Write("CPH");
  CPHL.Write("CPHL");
  bPH.Write("bPH");
  bPHL.Write("bPHL");
  f.Write();
  f.Close();
}

//**** Destructor
InternalAnalysis::~InternalAnalysis(){};  

//**** Executes the alignment with tracks algorithm
void InternalAnalysis::begin() {
  std::cout << "Starting Analysis..." << std::endl; 
  if(station != 4) {
    while(readMuon());
  } else {
    while(readMuonMB4());
  }
  textInfo.close();
}

//**** Reads a muon from a text file
bool InternalAnalysis::readMuon() {
  
  TrackForAlignment myTrack(NDOFTrack, NDOFAlign, NDOFChamber, NDOFCoor);
  for(int countDet = 0; countDet < NDOFChamber; countDet++) {
    int n;
    float r, x, xp, z, phi,m, a1, b1, a2, b2;
    textInfo >> n >> r >> x >> xp >> z >> m >> phi >> a1 >> b1 >> a2 >> b2;
    if(textInfo.eof()) return false;
    PointForInternalAlignment point(NDOFTrack, NDOFAlign, NDOFChamber, NDOFCoor);
    point.setPointForInternalAlignment(n, r, x, xp, z, m, phi);
    myTrack.insert(point);
  }
  myTrack.CalculateMatrizes();
  myTrack.isValid();
  if(myTrack.trackValid) {
    TMatrixD p(C.GetNrows(), C.GetNcols());
    C = C + myTrack.CpMatrix();
    b = b + myTrack.BMatrix();
    numberOfMuons++;
  }
  return true;
}


//**** Reads a muon from a text file
bool InternalAnalysis::readMuonMB4() {
  
  TrackForAlignment myTrack(NDOFTrack, NDOFAlign, NDOFChamber, NDOFCoor);
  for(int countDet = 0; countDet < NDOFChamber; countDet++) {
    int n;
    float r, x, xp, phi,m, a1, b1, a2, b2;
    textInfo >> n >> r >> x >> xp >> m >> phi >> a1 >> b1 >> a2 >> b2;
    if(textInfo.eof()) return false;
    PointForInternalAlignment point(NDOFTrack, NDOFAlign, NDOFChamber, NDOFCoor);
    point.setPointForInternalAlignment(n, r, x, xp, m, phi);
    myTrack.insert(point);
  }
  myTrack.CalculateMatrizes();
  myTrack.isValid();
  if(myTrack.trackValid) {
    TMatrixD p(C.GetNrows(), C.GetNcols());
    C = C + myTrack.CpMatrix();
    b = b + myTrack.BMatrix();
    numberOfMuons++;
  }
  return true;
}





//**** Prepares the quality Control constrains
void InternalAnalysis::PHLayer() {
  
  //Read the information from the file
  ReadPGInfo myPG("InternalData.root");
  TMatrixD surv = myPG.giveQCCal(wheel, station, sector);
  TMatrixD ResQC(NDOFAlign, NDOFChamber);
  ResQC.Zero();
  TMatrixD sigmaQC(NDOFAlign, NDOFChamber);
  
  if(surv.GetNrows() < 7) {
    for(int counterChamber = 0; counterChamber < NDOFChamber; ++counterChamber) {
      ResQC(0, counterChamber) = 0.0;
    }
    for(int counterAlign = 0; counterAlign < NDOFAlign; counterAlign++) {
      for(int counterChamber = 0; counterChamber < NDOFChamber; ++counterChamber) {
	if(counterAlign == 0) {
	  sigmaQC(counterAlign, counterChamber) = 0.05;
	} else if(counterAlign < 3) {
	  sigmaQC(counterAlign, counterChamber) = 0.05;
	} else {
	  sigmaQC(counterAlign, counterChamber) = 0.05;
	}
      }
    }
  } else {
    for(int counterChamber = 0; counterChamber < NDOFChamber; ++counterChamber) {
      ResQC(0, counterChamber) = -surv(counterChamber, 0)/10000.0;
    }
    for(int counterAlign = 0; counterAlign < NDOFAlign; counterAlign++) {
      for(int counterChamber = 0; counterChamber < NDOFChamber; ++counterChamber) {
	if(counterAlign == 0) {
	  sigmaQC(counterAlign, counterChamber) = surv(counterChamber, 1)/10000.0;
	} else if(counterAlign < 3) {
	  sigmaQC(counterAlign, counterChamber) = 0.05;
	} else {
	  sigmaQC(counterAlign, counterChamber) = 0.05;
	}
      }
    }
  }
  
  double **Eta = new double *[NDOFChamber];
  for(int counterLayer = 0; counterLayer < NDOFChamber; counterLayer++) {
    Eta[counterLayer] = new double [NDOFChamber];
    for(int counterLayer2 = 0; counterLayer2 < NDOFChamber; counterLayer2++) {
      if((counterLayer2 < 4 && counterLayer < 4) || (counterLayer2 > 3 && counterLayer > 3)) {
	if(counterLayer == counterLayer2) {
	  Eta[counterLayer][counterLayer2] = 3.0/(4.0);
	} else {
	  Eta[counterLayer][counterLayer2] = -1.0/(4.0);
	}
      } else {
	Eta[counterLayer][counterLayer2] = 0.0;
      }
    }
  }
  
  CPHL.ResizeTo(NDOFChamber*NDOFAlign, NDOFChamber*NDOFAlign);
  bPHL.ResizeTo(NDOFChamber*NDOFAlign, 1);

  CPHL.Zero();
  bPHL.Zero();
  
  for(int counterAlign = 0; counterAlign < NDOFAlign; counterAlign++) {
    for(int counterChamber = 0; counterChamber < NDOFChamber; counterChamber++) {
      for(int counterChamber2 = 0; counterChamber2 < NDOFChamber; counterChamber2++) {
	for(int counterChamber3 = 0; counterChamber3 < NDOFChamber; counterChamber3++) {
	  CPHL(NDOFAlign*counterChamber2+counterAlign, NDOFAlign*counterChamber3+counterAlign) += Eta[counterChamber][counterChamber2]*Eta[counterChamber][counterChamber3]/(sigmaQC(counterAlign,counterChamber)*sigmaQC(counterAlign,counterChamber));
	}
	////Esto es de verdad un menos: Lo cambio a un +
	bPHL(NDOFAlign*counterChamber2+counterAlign, 0) += Eta[counterChamber][counterChamber2]*ResQC(counterAlign,counterChamber)/(sigmaQC(counterAlign,counterChamber)*sigmaQC(counterAlign,counterChamber));
      }
    }
  }
  delete [] Eta;
  return;
}


//**** Prepares the ISR survey information
void InternalAnalysis::PHSuperLayer() {

  //Read the information from the file
  ReadPGInfo myPG("InternalData.root");

  TMatrixD surv = myPG.giveSurvey(wheel, station, sector);
  //if(surv.GetNrows() != 6) return false;
  TMatrixD alpha(NDOFAlign, 1);
  //I'm using the following system of coordinates: x is the usual, y is global Z, and z is vertical to the chamber
  //In survey measurements y is changed by z
  for(int count = 0; count < surv.GetNrows(); ++count) {
    //In the root file mm and microrads
    if(count < 2) alpha(count, 0) = -surv(count, 0)/10.0;
    if(count == 1) alpha(count, 0) = 0.0;
    switch(count) {
    case 3:
      alpha(2, 0) = -surv(count, 0)/10000.0;
      break;
    case 4:
      alpha(4, 0) = -surv(count, 0)/10000.0;
      break;
    case 5:
      alpha(3, 0) = -surv(count, 0)/10000.0;
      break;
    default:
      break;
    }
  }
  
  float error[] = {0.005, 0.1, 0.005, 0.005, 0.005};
  CPH.ResizeTo(NDOFChamber*NDOFAlign, NDOFChamber*NDOFAlign);
  bPH.ResizeTo(NDOFChamber*NDOFAlign, 1);
  for(int counterChamber = 0; counterChamber < NDOFChamber; counterChamber++) {
    float k = 1;
    if(counterChamber < 4) k = -1;
    for(int counterAlign = 0; counterAlign < NDOFAlign; counterAlign++) {
      for(int counterChamber2 = 0; counterChamber2 < NDOFChamber; counterChamber2++) {
	float k2 = 1;
	if(counterChamber2 < 4) k2 = -1;
	CPH(NDOFAlign*counterChamber+counterAlign, NDOFAlign*counterChamber2+counterAlign) =  k2*k/(16.0*error[counterAlign]*error[counterAlign]);
      }
      bPH(NDOFAlign*counterChamber+counterAlign, 0) = k*alpha(counterAlign,0)/(4.0*error[counterAlign]*error[counterAlign]);
    }
  }
  return;
}


//**** Inserts the constrains
void InternalAnalysis::AddConstrain() {
  
  int old_N = NDOFChamber*NDOFAlign;

  CTotal.ResizeTo(old_N,old_N);
  bTotal.ResizeTo(old_N, 1);

  for(int counterj = 0; counterj < C.GetNrows(); ++counterj) {
    for(int counteri = 0; counteri < C.GetNcols(); ++counteri) {
      C(counterj, counteri) /= (0.0200*0.0200);
    }
    b(counterj, 0) /= (0.0200*0.0200);
  }
  
  CTotal = C + CPH + CPHL;
  bTotal = b + bPH + bPHL;

  CTotal.ResizeTo(old_N+NDOFAlign, old_N+NDOFAlign);
  bTotal.ResizeTo(old_N+NDOFAlign, 1);
  
  CConstrain.ResizeTo(old_N+NDOFAlign,old_N+NDOFAlign);
  bConstrain.ResizeTo(old_N+NDOFAlign, 1);
  
  for(int k = 0; k < NDOFAlign; ++k) {
    for(int s = 0; s < NDOFChamber; ++s) {
      CConstrain(old_N+k,s*NDOFAlign+k) = 10000.0;
      CConstrain(s*NDOFAlign+k,old_N+k) = 10000.0;
    }
    bConstrain(old_N+k,0) = 0.0;
  }
  
  CTotal = CTotal + CConstrain;
  bTotal = bTotal + bConstrain;
}


//**** Ends
void InternalAnalysis::end() {}

//**** Calculates the solution
void InternalAnalysis::Calculate() {
  Solution.ResizeTo(NDOFAlign*NDOFChamber+NDOFAlign, 1);
  CTotal.Invert();
  Solution = CTotal*bTotal;
}



void InternalAnalysis::MakeComparison() {

  cout << "Estoy con la comparacion" << endl;
  
  TMatrixD QCSurvey(NDOFChamber*NDOFAlign,1);
  //Read the information from the file
  ReadPGInfo myPG("InternalData.root");
  TMatrixD surv = myPG.giveSurvey(wheel, station, sector);
  TMatrixD alpha(NDOFAlign, 1);
  for(int count = 0; count < surv.GetNrows(); ++count) {
    //In the root file mm and microrads
    if(count < 2) alpha(count, 0) = -surv(count, 0)/10.0;
    if(count == 1) alpha(count, 0) = 0.0;
    switch(count) {
    case 3:
      alpha(2, 0) = -surv(count, 0)/10000.0;
      break;
    case 4:
      alpha(4, 0) = -surv(count, 0)/10000.0;
      break;
    case 5:
      alpha(3, 0) = -surv(count, 0)/10000.0;
      break;
    default:
      break;
    }
  }
  TMatrixD qc = myPG.giveQCCal(wheel, station, sector);
  for(int counterAlign = 0; counterAlign < NDOFAlign; ++counterAlign) {
    for(int counter = 4; counter < NDOFChamber; counter++) {
      QCSurvey(counter*NDOFAlign+counterAlign, 0) = alpha(counterAlign, 0);
    }
  }
  for(int counter = 0; counter < NDOFChamber; counter++) {
    QCSurvey(counter*NDOFAlign, 0) -= qc(counter,0)/10000.0;
  }

  TMatrixD EigenVector(C.GetNrows(), C.GetNrows());
  TVectorD EigenValue;
  EigenVector = C.EigenVectors((TVectorD &) EigenValue);
  
  ofstream eigenvec("eigenvectors.txt");
  ofstream eigenval("eigenvalues.txt");
  for(int counteri = 0; counteri < C.GetNrows(); counteri++) {
    for(int counterj = 0; counterj < C.GetNcols(); counterj++) {
      eigenvec << EigenVector(counteri, counterj) << " ";
    }
    eigenvec << endl;
    eigenval << EigenValue[counteri] << endl;
  }
  eigenvec.close();
  eigenval.close();



  TMatrixD D(C.GetNrows(), C.GetNcols());
  D.Zero();

  
  TMatrixD A = EigenVector;
  TMatrixD At = A;
  At.T();
  int number = 0;
  for(Int_t counter = 0; counter < C.GetNrows(); counter++) {
    if(TMath::Abs(EigenValue[counter]) < 1e6) {
      D[counter][counter] = 0;
      ++number;
    } else {
      D[counter][counter] = 1.0/EigenValue[counter];
    }
  }
  
  TMatrixD delta(C.GetNrows(), 1);
  delta = A*D*At*b;
  
  TMatrixD comparison = QCSurvey;
  for(int counter = C.GetNrows()-number; counter < C.GetNrows(); ++counter) {
    double scalar = 0;
    for(int counterChamber = 0; counterChamber < C.GetNrows(); counterChamber++) {
      scalar += EigenVector(counterChamber, counter)*QCSurvey(counterChamber, 0);
    }
    for(int counterChamber = 0; counterChamber < C.GetNrows(); counterChamber++) {
      comparison(counterChamber, 0) -= scalar*EigenVector(counterChamber, counter);
    }
  }
  
  ofstream comp("comparison.txt");
  for(int counterChamber = 0; counterChamber < C.GetNrows(); counterChamber++) {
    comp << delta(counterChamber, 0) << " " << comparison(counterChamber, 0) << endl;
  }
  comp.close();
}



//**** Make Comparison Survey
void InternalAnalysis::MakeComparisonSurvey() {
  ifstream text("info.txt");
  if(station == 4) {
    while(!text.eof()) {
      for(int countDet = 0; countDet < NDOFChamber; countDet++) {
	int n;
	float r, x, xp, phi,m,a1, b1, a2, b2;
	text >> n >> r >> x >> xp >> m >> phi >> a1 >> b1 >> a2 >> b2;
 	hist->Fill(a2-a1);
	if(text.eof()) break;
      }
    }
  } else {
    while(!text.eof()) {
      for(int countDet = 0; countDet < NDOFChamber; countDet++) {
	int n;
	float r, x, xp, z, phi, m, a1, b1, a2, b2;
	text >> n >> r >> x >> xp >> m >> phi >> z >> a1 >> b1 >> a2 >> b2;
        hist->Fill(a2-a1);
	if(text.eof()) break;
      }
    }
  }
  text.close();
  
  ReadPGInfo myPG("InternalData.root");
  TMatrixD surv = myPG.giveSurvey(wheel, station, sector);
  ofstream surveycomp("SurveyComparison.txt");
  surveycomp << surv(0,0) << " " << hist->GetMean() << endl;
  surveycomp.close();
  
  TFile f("histograms.root", "RECREATE");
  hist->Write();
  f.Write();
  f.Close();
  
}


//**** Print Solution
void InternalAnalysis::PrintSolution() {
  ofstream solution("solution.txt");
  for(int counterChamber = 0; counterChamber < NDOFChamber; ++counterChamber) {
    solution << counterChamber << " ";
    for(int counterAlign = 0; counterAlign < NDOFAlign; ++counterAlign) {
      solution << 10000.0 * Solution(counterChamber*NDOFAlign+counterAlign, 0) << " ";
    }
    solution << endl;
  }
  solution.close();


  ofstream cm("cmatrix.txt");
  ofstream bm("bmatrix.txt");
  for(int i = 0; i < C.GetNrows(); ++i) {
    for(int j = 0; j < C.GetNcols(); ++j) {
      cm << C(i,j) << " ";
    }
    cm << endl;
    bm << b(i,0) << endl;
  }
  cm.close();
  bm.close();

}
