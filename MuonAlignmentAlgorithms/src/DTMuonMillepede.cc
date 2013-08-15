#include "Alignment/MuonAlignmentAlgorithms/interface/DTMuonMillepede.h"


DTMuonMillepede::DTMuonMillepede(std::string path, int n_files, float MaxPt, float MinPt, TFile *f_) {
  
  ntuplePath = path;
  numberOfRootFiles = n_files;
  
  f = f_;  
 
  ptMax = MaxPt; 
  ptMin = MinPt; 

  //Interface to Survey information
  myPG = new ReadPGInfo("InternalData.root"); 

  setBranchTree();
 
  initNTuples();

  calculationMillepede();
  
}


DTMuonMillepede::~DTMuonMillepede() {}



void DTMuonMillepede::calculationMillepede() {
  

  //Init Matrizes
  TMatrixD ****C = new TMatrixD ***[5];
  TMatrixD ****b = new TMatrixD ***[5];
  
  for(int whI = -2; whI < 3; ++whI) {
    C[whI+2] = new TMatrixD **[4];
    b[whI+2] = new TMatrixD **[4];
    for(int stI = 1; stI < 5; ++stI) {
      C[whI+2][stI-1] = new TMatrixD * [14];
      b[whI+2][stI-1] = new TMatrixD * [14];
      for(int seI = 1; seI < 15; ++seI) {
        if(seI > 12 && stI != 4) continue;
        if(stI == 4) {
          C[whI+2][stI-1][seI-1] = new TMatrixD(40,40);
          b[whI+2][stI-1][seI-1] = new TMatrixD(40,1);
        } else { 
          C[whI+2][stI-1][seI-1] = new TMatrixD(60,60);
          b[whI+2][stI-1][seI-1] = new TMatrixD(60,1);
        }
      }
    }
  }
  
  
  //Run over the TTree  
  Int_t nentries = (Int_t)tali->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    tali->GetEntry(i);
    //Basic cuts
    if(pt > ptMax || pt < ptMin) continue;
    
    for(int counter = 0; counter < nseg; ++counter) {
      
      TMatrixD A(12,60);
      TMatrixD R(12,1);
      TMatrixD E(12,12);
      TMatrixD B(12, 10);
      if(st[counter] == 4) {
        A.ResizeTo(8,40);
        R.ResizeTo(8,1);
        E.ResizeTo(8,8);
        B.ResizeTo(8,8);
      }
      
      for(int counterHi = 0; counterHi < nhits[counter]; counterHi++) {
        
        int row = 0;
        if(sl[counter][counterHi] == 3) {
          row = 4 + (la[counter][counterHi]-1);
        } else if(sl[counter][counterHi] == 2) {
          row = 8 + (la[counter][counterHi]-1);
        } else {
          row = (la[counter][counterHi]-1);
        }
	
        float x = xc[counter][counterHi];
        float y = yc[counter][counterHi];
        float xp = xcp[counter][counterHi];
        float yp = ycp[counter][counterHi];
        float zp = zc[counter][counterHi];
        float error = ex[counter][counterHi];        
        float dxdz = dxdzSl[counter];
        float dydz = dydzSl[counter];
	
        if(sl[counter][counterHi] == 2) {
          A(row, row*5) = -1.0;
          A(row, row*5+1) = dydz;
          A(row, row*5+2) = y*dydz;
          A(row, row*5+3) = -x*dydz;
          A(row, row*5+4) = -x;
          R(row, row*5+5) = y-yp;
        } else {
          A(row, row*5+0) = -1.0;
          A(row, row*5+1) = dxdz;
          A(row, row*5+2) = y*dxdz;
          A(row, row*5+3) = -x*dxdz;
          A(row, row*5+4) = y;
          R(row, 0) = x-xp;
        }
        E(row, row) = 1.0/error;
        B(row, row*2) = 1.0;
        B(row, row*2+1) = zp;
      }
      TMatrixD Bt = B; Bt.T();
      TMatrixD At = A; At.T();
      TMatrixD gamma = (Bt*E*B); gamma.Invert();
      *(C[wh[counter]+2][st[counter]-1][sr[counter]-1]) += At*E*(B*gamma*Bt*E*A-A);
      *(b[wh[counter]+2][st[counter]-1][sr[counter]-1]) += At*E*(B*gamma*Bt*E*A-A)*R;
    } 
  }   
  
  //Final Calculation and Constraints 
  for(int wheel = -2; wheel < 3; ++wheel) {
    for(int station = 1; station < 5; ++station) {
      for(int sector = 1; sector < 15; ++sector) {
        if(sector > 12 && station != 4) continue;
        TMatrixD Ctr = prepareForLagrange(*C[wheel+2][station-1][sector-1]);
        TMatrixD btr = prepareForLagrange(*b[wheel+2][station-1][sector-1]);
        TMatrixD Cqc = prepareForLagrange(getCqcMatrix(wheel, station, sector));
        TMatrixD bqc = prepareForLagrange(getbqcMatrix(wheel, station, sector));
        TMatrixD Csurvey = prepareForLagrange(getCsurveyMatrix(wheel, station, sector));
        TMatrixD bsurvey = prepareForLagrange(getbsurveyMatrix(wheel, station, sector));
        TMatrixD Clag = getLagMatrix(wheel, station, sector);
        TMatrixD SC = Ctr + Cqc + Csurvey + Clag;
        TMatrixD Sb = btr + bqc + bsurvey;
        
        SC.Invert();
	
        TMatrixD solution = SC*Sb;
        for(int icounter = 0; icounter < SC.GetNrows(); icounter++) {
          for(int jcounter = 0; jcounter < SC.GetNrows(); jcounter++) {
            cov[icounter][jcounter] = SC(icounter, jcounter);
          }
        }
        whC = wheel;
        stC = station;
        srC = sector;
        
        for(int c = 0; c < 60; ++c) {
          for(int s = 0; s < 60; ++s) {
            cov[c][s] = SC(c, s);
          }
        } 

        for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
          for(int counterDeg = 0; counterDeg < 5; ++counterDeg) {
            if(counterLayer > 7 && station == 4) continue;
            if(counterLayer < 4) {
              slC[counterLayer] = 1;
              laC[counterLayer] = counterLayer+1;
            } else if(counterLayer > 3 && counterLayer < 8) {
              slC[counterLayer] = 3;
              laC[counterLayer] = counterLayer-3;
            } else {
              slC[counterLayer] = 2;
              laC[counterLayer] = counterLayer-7;
            }
            if(counterLayer < 8) {
              dx[counterLayer] = solution(counterLayer*5, 0);
              dy[counterLayer] = 0.0;
            } else {
              dx[counterLayer] = 0.0;
              dy[counterLayer] = solution(counterLayer*5, 0); 
            }
            dz[counterLayer] = solution(counterLayer*5+1, 0);
            phix[counterLayer] = solution(counterLayer*5+2, 0);
            phiy[counterLayer] = solution(counterLayer*5+3, 0);
            phiz[counterLayer] = solution(counterLayer*5+4, 0);
          }
        }
        ttreeOutput->Fill();
      }
    }
  }
  f->Write();
}



TMatrixD DTMuonMillepede::getLagMatrix(int wh, int st, int se) {
  
  TMatrixD matrix(60+6,60+6);
  if(st == 4) matrix.ResizeTo(40+5, 40+5); 
  
  for(int counterDeg = 0; counterDeg < 5; ++counterDeg) {
    for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
      if(st == 4) {
        matrix(40+counterDeg, counterLayer*5+counterDeg) = 10000.0;
        matrix(counterLayer*5+counterDeg, 40+counterDeg) = 10000.0;
      } else {
        int realCounter = counterDeg+1;
        if(counterDeg == 1 && counterLayer < 8) {
          realCounter = counterDeg-1;
        }
        matrix(counterLayer*5+counterDeg,40+realCounter) = 10000.0;
        if( (realCounter == 0 && counterLayer > 7) ||
            (realCounter == 1 && counterLayer < 7)) continue;  
        matrix(60+realCounter,counterLayer*5+counterDeg) = 10000.0;
      }
    }
  }
  return matrix;
}


TMatrixD DTMuonMillepede::getCqcMatrix(int wh, int st, int se) {
  
  TMatrixD surv = myPG->giveQCCal(wh, st, se);
  TMatrixD sigmaQC(5, 12);
  
  TMatrixD matrix(60, 60);
  if(st == 4) matrix.ResizeTo(40, 40);
  
  
  if(surv.GetNrows() < 7) {
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
        if(st != 4 && counterLayer > 7) continue;
        if(counterDeg == 0) {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        } else if(counterDeg < 3) {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        } else {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        }
      }
    }
  } else {
    float meanvarSL1 = sqrt(surv(0,1)*surv(0,1)+surv(1,1)*surv(1,1)+surv(2,1)*surv(2,1)+surv(3,1)*surv(3,1))/10000.0; 
    float meanvarSL2 = 0;
    if(surv.GetNrows() > 9) meanvarSL2 = sqrt(surv(8,1)*surv(8,1)+surv(9,1)*surv(9,1)+surv(10,1)*surv(10,1)+surv(11,1)*surv(11,1))/10000.0; 
    float meanvarSL3 = sqrt(surv(4,1)*surv(4,1)+surv(5,1)*surv(5,1)+surv(6,1)*surv(6,1)+surv(7,1)*surv(7,1))/10000.0; 
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
	if(st != 4 && counterLayer > 7) continue;
	float meanerror = 0;
	if(counterLayer < 4) {
	  meanerror = meanvarSL1;
	} else if(counterLayer > 3 && counterLayer < 8){
	  meanerror = meanvarSL2;
	} else {
	  meanerror = meanvarSL3;
	}
	if(counterDeg == 0) {
	  sigmaQC(counterDeg, counterLayer) = sqrt((surv(counterLayer, 1)/10000.0)*(surv(counterLayer, 1)/10000.0)+meanerror*meanerror);
	} else if(counterDeg < 3) {
	  sigmaQC(counterDeg, counterLayer) = 0.05;
	} else {
	  sigmaQC(counterDeg, counterLayer) = 0.05;
	}
      }
    }
    double **Eta = new double *[12];
    for(int counterLayer = 0; counterLayer < 12; counterLayer++) {
      if(counterLayer > 7 && st == 4) continue; 
      Eta[counterLayer] = new double [5];
      for(int counterLayer2 = 0; counterLayer2 < 12; counterLayer2++) {
	if(counterLayer > 7 && st == 4) continue; 
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
  
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; counterLayer++) {
        if(counterLayer > 7 && st == 4) continue; 
        for(int counterLayer2 = 0; counterLayer2 < 12; counterLayer2++) {
          if(counterLayer2 > 7 && st == 4) continue; 
          for(int counterLayer3 = 0; counterLayer3 < 12; counterLayer3++) {
            if(counterLayer3 > 7 && st == 4) continue; 
            matrix(5*counterLayer2+counterDeg, 5*counterLayer3+counterDeg) +=
              Eta[counterLayer][counterLayer2]*Eta[counterLayer][counterLayer3]/(sigmaQC(counterDeg,counterLayer)*sigmaQC(counterDeg,counterLayer));
          }
        }
      }
    }
  }
  return matrix;
}


TMatrixD DTMuonMillepede::getbqcMatrix(int wh, int st, int se) {

  TMatrixD surv = myPG->giveQCCal(wh, st, se);
  TMatrixD ResQC(5, 12);
  TMatrixD sigmaQC(5, 12);
  TMatrixD matrix(60, 1);
  if(st == 4) matrix.ResizeTo(40, 1);
  
  if(surv.GetNrows() < 7) {
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
        if(st != 4 && counterLayer > 7) continue;
        if(counterDeg == 0) {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        } else if(counterDeg < 3) {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        } else {
          sigmaQC(counterDeg, counterLayer) = 0.05;
        }
      }
    }
  } else {
    for(int counterChamber = 0; counterChamber < 12; ++counterChamber) {
      ResQC(0, counterChamber) = -surv(counterChamber, 0)/10000.0;
    }
    float meanvarSL1 = sqrt(surv(0,1)*surv(0,1)+surv(1,1)*surv(1,1)+surv(2,1)*surv(2,1)+surv(3,1)*surv(3,1))/10000.0;
    float meanvarSL2 = 0;
    if(surv.GetNrows() > 9) {
      meanvarSL2 = sqrt(surv(8,1)*surv(8,1)+surv(9,1)*surv(9,1)+surv(10,1)*surv(10,1)+surv(11,1)*surv(11,1))/10000.0;
    }  
    float meanvarSL3 = sqrt(surv(4,1)*surv(4,1)+surv(5,1)*surv(5,1)+surv(6,1)*surv(6,1)+surv(7,1)*surv(7,1))/10000.0;
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; ++counterLayer) {
	if(st != 4 && counterLayer > 7) continue;
	float meanerror = 0;
	if(counterLayer < 4) {
	  meanerror = meanvarSL1;
	} else if(counterLayer > 3 && counterLayer < 8){
	  meanerror = meanvarSL2;
	} else {
	  meanerror = meanvarSL3;
	}
	if(counterDeg == 0) {
	  sigmaQC(counterDeg, counterLayer) = sqrt((surv(counterLayer, 1)/10000.0)*(surv(counterLayer, 1)/10000.0)+meanerror*meanerror);
	} else if(counterDeg < 3) {
	  sigmaQC(counterDeg, counterLayer) = 0.05;
	} else {
	  sigmaQC(counterDeg, counterLayer) = 0.05;
	}
      }
    }
    double **Eta = new double *[12];
    for(int counterLayer = 0; counterLayer < 12; counterLayer++) {
      if(counterLayer > 7 && st == 4) continue;
      Eta[counterLayer] = new double [5];
      for(int counterLayer2 = 0; counterLayer2 < 12; counterLayer2++) {
	if(counterLayer > 7 && st == 4) continue;
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
  
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer = 0; counterLayer < 12; counterLayer++) {
	if(counterLayer > 7 && st == 4) continue; 
	for(int counterLayer2 = 0; counterLayer2 < 12; counterLayer2++) {
	  if(counterLayer2 > 7 && st == 4) continue;
	  float mean = 0;
	  if(counterDeg != 0) {
	    if(counterLayer < 4) {
	      mean = (ResQC(counterDeg, 0)+ResQC(counterDeg, 1)+ResQC(counterDeg, 2)+ResQC(counterDeg, 3))/4.0;
	    } else if(counterLayer > 3 && counterLayer < 8) {
	      mean = (ResQC(counterDeg, 4)+ResQC(counterDeg, 5)+ResQC(counterDeg, 6)+ResQC(counterDeg, 7))/4.0;
	    } else {
	      mean = (ResQC(counterDeg, 8)+ResQC(counterDeg, 9)+ResQC(counterDeg, 10)+ResQC(counterDeg, 11))/4.0;
	    }
	  }
	  matrix(5*counterLayer2+counterDeg, 0) += Eta[counterLayer][counterLayer2]*(ResQC(counterDeg,counterLayer)-mean)/(sigmaQC(counterDeg,counterLayer)*sigmaQC(counterDeg,counterLayer));
	}
      }
    }
  }
  return matrix;
}


TMatrixD DTMuonMillepede::getCsurveyMatrix(int wh, int st, int se) {

  int size = 60;
  if(st == 4) size = 40;

  TMatrixD matrix(size+6, size+6);
  //Careful with the sign
  float error[] = {0.05, 0.05, 0.05, 0.005, 0.005, 0.005};
  for(int counterLayer = 0; counterLayer < size/5; counterLayer++) {
    float k = 1;
    if(counterLayer < 4) k = -1;
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      for(int counterLayer2 = 0; counterLayer2 < size/5; counterLayer2++) {
        float k2 = 1;
        if(counterLayer2 < 4) k2 = -1;
        matrix(5*counterLayer+counterDeg, 5*counterLayer2+counterDeg) =  k2*k/(16.0*error[counterDeg]*error[counterDeg]);
      }
    }
  }

  return matrix;

}



TMatrixD DTMuonMillepede::getbsurveyMatrix(int wh, int st, int se) {
 
  TMatrixD survey = myPG->giveSurvey(wh, st, se); 
  int size = 60;
  if(st == 4) size = 40;

  TMatrixD matrix(size+6, 1);
  //Careful with the sign
  float error[] = {0.05, 0.05, 0.05, 0.005, 0.005, 0.005};
  for(int counterLayer = 0; counterLayer < size/5; counterLayer++) {
    float k = 1;
    if(counterLayer < 4) k = -1;
    for(int counterDeg = 0; counterDeg < 5; counterDeg++) {
      int counterDegAux = counterDeg+1;
      if(counterLayer < 8 && counterDeg == 1) counterDegAux = 0;  
      matrix(5*counterLayer+counterDeg, 0) =  k*survey(counterDegAux, 0)/(16.0*error[counterDeg]*error[counterDeg]);
    }
  }

  return matrix;

}


TMatrixD DTMuonMillepede::prepareForLagrange(TMatrixD m) {

  TMatrixD updatedMatrix = m;
  updatedMatrix.ResizeTo(m.GetNrows()+6, m.GetNcols()+6);
  return updatedMatrix;

}


void DTMuonMillepede::setBranchTree() {

  ttreeOutput = new TTree("DTLocalMillepede", "DTLocalMillepede");

  ttreeOutput->Branch("wh", &whC, "wh/I");
  ttreeOutput->Branch("st", &stC, "st/I");
  ttreeOutput->Branch("sr", &srC, "sr/I");
  ttreeOutput->Branch("cov", cov, "cov[60][60]/F");
  ttreeOutput->Branch("sl", slC, "sl[12]/I");
  ttreeOutput->Branch("la", laC, "la[12]/I");
  ttreeOutput->Branch("dx", dx, "dx[12]/I");
  ttreeOutput->Branch("dy", dy, "dy[12]/I");
  ttreeOutput->Branch("dz", dz, "dz[12]/I");
  ttreeOutput->Branch("phix", phix, "phix[12]/I");
  ttreeOutput->Branch("phiy", phiy, "phiy[12]/I");
  ttreeOutput->Branch("phiz", phiz, "phiz[12]/I");

}



 
