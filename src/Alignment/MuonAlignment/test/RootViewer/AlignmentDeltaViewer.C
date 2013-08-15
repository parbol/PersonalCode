#include <iostream>
#include <fstream>
#include "AlignmentDeltaViewer.h"

ClassImp(DTChamber)   // define this class in ROOT
ClassImp(CSCChamber)  // define this class in ROOT

DTChamber*  DT[5][4][14];
CSCChamber* CSC[8][3][36];
bool        MaskDT[5][4][14];     // used to mask out DTs from 1D histogram plots

int         YBorYE, Wheel, Station;
TString     opt, pdfname;
bool        PNGs = false;  // set to false unless you want a png file for each pad!

int    GetWSS(TString WSSString);
double GetCoordinateFromString(TString Vec, int coor);
double GetDimensionFromString(TString Vec, int coor);
void   ReadDTNominalGeometry();
void   ReadCSCNominalGeometry();
void   ReadDeltaGeom(TString filename);
void   DisplayDTDeltaGeom(int W, TString opt);
void   PlotDTDeltaGeom(int W, TString opt, bool DynamicScale);
void   DisplaySummaryDTDeltaGeom(TString opt);
void   DisplayCSCDeltaGeom(int W, TString opt);
void   DisplaySummaryCSCDeltaGeom(TString opt);

//-------------------------------------------

// Choice = 0 for interactive
// Choice = 1 to produce all barrel plots
// Choice = 2 to produce all barrel summary plots
// Choice = 3 to produce all endcap plots
// Choice = 4 to produce all endcap summary plots

void AlignmentDeltaViewer(TString InputFileName, int Choice) {

  // Initialize all masks to false
  for (int W=0;W<5;W++) {
    for(int sta=0;sta<4;sta++) {
      for(int sec=0;sec<14;sec++) {
        MaskDT[W][sta][sec] = false;
      }
    }
  }

  // Specify which DTs should NOT be included in the 1D histogram plots
  int WH,ST,SE; // wheel, station, sector

  /*
  // Mask out all sectors 13 and 14 (always station 4)
  for (int W=0;W<5;W++) {
    for(int sec=12;sec<14;sec++) {
        MaskDT[W][3][sec] = true;
    }
  }
  */

  /*
  // The following were all moved after PG2009
  WH = -2; ST = 1; SE =  8; MaskDT[WH+2][ST-1][SE-1] = true;
  WH = -2; ST = 2; SE = 10; MaskDT[WH+2][ST-1][SE-1] = true;
  WH = -2; ST = 3; SE = 10; MaskDT[WH+2][ST-1][SE-1] = true;
  WH = -1; ST = 4; SE =  6; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  0; ST = 1; SE =  8; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  0; ST = 2; SE =  8; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  0; ST = 2; SE =  9; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  0; ST = 3; SE =  6; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  0; ST = 4; SE =  3; MaskDT[WH+2][ST-1][SE-1] = true;
  WH =  1; ST = 4; SE = 11; MaskDT[WH+2][ST-1][SE-1] = true; // except this one
  WH =  2; ST = 1; SE =  5; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 3; SE =  6; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 4; SE =  8; MaskDT[WH+2][ST-1][SE-1] = true; 
  */

  // The following were not reconstructed by the barrel alignment
  WH = -2; ST = 4; SE = 11; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  1; ST = 4; SE =  7; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 4; SE =  2; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 4; SE =  4; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 4; SE =  8; MaskDT[WH+2][ST-1][SE-1] = true; 
  WH =  2; ST = 4; SE =  9; MaskDT[WH+2][ST-1][SE-1] = true;  // except this one 
  WH =  2; ST = 4; SE = 11; MaskDT[WH+2][ST-1][SE-1] = true; 

  // Mask out all station 4 (this is for recos of stations 1-2-3 only)
  for (int W=0;W<5;W++) {
    for(int sec=0;sec<14;sec++) {
        MaskDT[W][3][sec] = true;
    }
  }

  ReadDTNominalGeometry();
  ReadCSCNominalGeometry();
  ReadDeltaGeom(InputFileName);
  // open pdf file
  // gStyle->SetPaperSize(20,20);
  pdfname = "PLOTS_";
  if(Choice==1) pdfname+="YB_";
  if(Choice==2) pdfname+="YB_Summary_";
  if(Choice==3) pdfname+="YE_";
  if(Choice==4) pdfname+="YE_Summary_";
  pdfname += InputFileName;
  if(pdfname.Contains(".root")) pdfname.ReplaceAll(".root",".pdf");
  else pdfname+=".pdf";

  TCanvas *dummy1 = new TCanvas("dummy1","dummy1",10,10,900,900);
  dummy1->cd();
  gPad->Range(0,0,1,1);
  TPaveText *tpt = new TPaveText(0.1,0.3,0.9,0.7); 
  TText* txt;
  tpt->SetFillColor(40);
  txt = tpt->AddText(pdfname);
  tpt->Draw();
  dummy1->Print(pdfname+"(","pdf");        // open pdf file
  dummy1->Close();

  if( Choice == 0) {
    // interactively choose wheel or disk
    while(1) {
      cout<<"Enter 1 for YB, 2 for YE (3=exit): "; cin>>YBorYE;
      if(YBorYE==1){
        cout<<"Enter YB wheel number [-2,2] (3 = all wheels, 999=back): "; cin>>Wheel;
        if(Wheel>-3 && Wheel<3){
          cout<<"Enter coordinate to show: "; cin>>opt;
          DisplayDTDeltaGeom(Wheel,opt);
        }
        if(Wheel==3){
          cout<<"Enter coordinate to show: "; cin>>opt;
          DisplaySummaryDTDeltaGeom(opt);
	}
      }
      else if(YBorYE==2){
        cout<<"Enter YE station number [-4,4] (0 = all stations, 999=back): "; cin>>Station;
        if(Station>-5 && Station<5 && Station!=0){
          cout<<"Enter coordinate to show: "; cin>>opt;
          DisplayCSCDeltaGeom(Station,opt);
        }
        if(Station==0){
          cout<<"Enter coordinate to show: "; cin>>opt;
          DisplaySummaryCSCDeltaGeom(opt);
        }
        
      }
      else break;
    }
  }

  if(Choice == 1) {
    // loop over all barrel wheels
    for(Wheel=-2;Wheel<3;Wheel++){
       opt = "localx";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "localy";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "localz";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "localphix";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "localphiy";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "localphiz";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globalx";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globaly";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globalz";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globalphix";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globalphiy";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
       opt = "globalphiz";
       DisplayDTDeltaGeom(Wheel,opt);
       PlotDTDeltaGeom(Wheel,opt,1);
       PlotDTDeltaGeom(Wheel,opt,0);
    }
  }

  if (Choice == 2) {
    // do barrel summary plots
    opt = "localx";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "localy";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "localz";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "localphix";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "localphiy";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "localphiz";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globalx";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globaly";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globalz";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globalphix";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globalphiy";
    DisplaySummaryDTDeltaGeom(opt);
    opt = "globalphiz";
    DisplaySummaryDTDeltaGeom(opt);
  }  

  if (Choice == 3) {
    // loop over all endcap disks
    for(Station=-4;Station<5;Station++){
      if(Station!=0) {
        opt = "localx";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "localy";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "localz";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "localphix";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "localphiy";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "localphiz";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globalx";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globaly";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globalz";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globalphix";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globalphiy";
        DisplayCSCDeltaGeom(Station,opt);
        opt = "globalphiz";
        DisplayCSCDeltaGeom(Station,opt);
      } // if Station!=0
    } // loop
  } // if choice = 3

  if (Choice == 4) {
    // do summary plots
    opt = "localx";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "localy";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "localz";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "localphix";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "localphiy";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "localphiz";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globalx";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globaly";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globalz";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globalphix";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globalphiy";
    DisplaySummaryCSCDeltaGeom(opt);
    opt = "globalphiz";
    DisplaySummaryCSCDeltaGeom(opt);
  } 

  cout<<"Close PDF file"<<endl;
  TCanvas *dummy2 = new TCanvas("dummy2","dummy2",10,10,900,900);
  gPad->Range(0,0,1,1);
  TPaveText *tpt2 = new TPaveText(0.1,0.3,0.9,0.7); 
  TText* txt2;
  tpt2->SetFillColor(40);
  txt2 = tpt2->AddText(pdfname);
  tpt2->Draw();
  dummy2->Print(pdfname+")","pdf");        // close pdf file
  dummy2->Close();

}

//-------------------------------------------

void ReadDTNominalGeometry() {

  int    Wheel,Station,Sector;
  double X,Y,Z,W,H,L,NormX,NormY,NormZ;

  // lines are assumed to have the following format
  // Chamber  Wh:-2 St:1 Se:1  Position  (431.175,39.12,-533.35)  normVect  (-1,-5.0532e-16,6.12303e-17)  bounds W/H/L: 218/36.2/251.1

  ifstream DTNominal; 
  DTNominal.open("NominalGeometryDT.txt");
  for (int ch = 0; ch<250; ch++) {
    TString Chamber, Wh, St, Se, Pos, Position, Dir, Direction, Bou, WHL, Bounds;
    DTNominal >> Chamber >> Wh >> St >> Se >> Pos >> Position >> Dir >> Direction >> Bou >> WHL >> Bounds;

    Wheel   = GetWSS(Wh); 
    Station = GetWSS(St); 
    Sector  = GetWSS(Se);
    X       = GetCoordinateFromString(Position,0);
    Y       = GetCoordinateFromString(Position,1);
    Z       = GetCoordinateFromString(Position,2);
    NormX   = GetCoordinateFromString(Direction,0);
    NormY   = GetCoordinateFromString(Direction,1);
    NormZ   = GetCoordinateFromString(Direction,2);
    W       = GetDimensionFromString(Bounds,0);
    H       = GetDimensionFromString(Bounds,1);
    L       = GetDimensionFromString(Bounds,2);

    DT[Wheel+2][Station-1][Sector-1] 
            = new DTChamber(Wheel,Station,Sector,X,Y,Z,W,H,L,NormX,NormY,NormZ);
  }
  DTNominal.close();
}

//-------------------------------------------

void ReadCSCNominalGeometry() {

  int     Endcap,Station,Ring,Chamber,indx;
  double  X,Y,Z,PhiZ,LengthR,WidthPhi;
  double  pi = 3.141592654;
  TString Header,DataString,myString;

  // lines are assumed to have the following format
  // x y z phix phiy phiz endcap station ring chamber 
  // IMPORTANT: lines MUST have at least one space after the last entry (chamber).
  //            Otherwise chamber will be 0, Chamber-1=-1 is an out of range index: CRASH.
  ifstream CSCNominal; 
  CSCNominal.open("NominalGeometryCSC.txt");
  // read header string (do nothing with it)
  Header.ReadLine(CSCNominal);
  // now read data lines until end of file
  while (1) {
    DataString.ReadLine(CSCNominal);
    if (!CSCNominal.good()) break;
    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    X = myString.Atof();

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Y = myString.Atof();

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Z = myString.Atof();

    // do nothing with PhiX and PhiY
    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string

    // phiZ is rotated 90 degrees w.r.t. CMS phi convention, so ignore it also
    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    // PhiZ = myString.Atof();
    // Calculate PhiZ directly from (X,Y) of each chamber
    PhiZ = atan2(Y,X);

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Endcap = myString.Atoi();

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Station = myString.Atoi();

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Ring = myString.Atoi();

    indx = DataString.First(' ');
    myString = DataString(0,indx);
    DataString.Remove(0,indx+1); // remove value read and space from data string
    Chamber = myString.Atoi();

    if(Endcap==2) Station = -Station;

    int S;
    if(Station<0) S = Station+4;
    else S = Station+3; 
    CSC[S][Ring-1][Chamber-1] 
      = new CSCChamber(Station,Ring,Chamber,X,Y,Z,PhiZ);

    LengthR = 0; WidthPhi = 0; 
    if(abs(Station)>1 && Ring==2) {LengthR = 338.0; WidthPhi = 2.0*pi/36.0;}
    if(abs(Station)==1){
      if(Ring==1){LengthR = 168.0; WidthPhi = 2.0*pi/36.0;}
      if(Ring==2){LengthR = 180.0; WidthPhi = 2.0*pi/36.0;}
      if(Ring==3){LengthR = 190.0; WidthPhi = 2.0*pi/36.0;}
    }
    if(abs(Station)==2 && Ring==1){LengthR = 206.5; WidthPhi = 2.0*pi/18.0;}
    if(abs(Station)==3 && Ring==1){LengthR = 184.5; WidthPhi = 2.0*pi/18.0;}
    if(abs(Station)==4 && Ring==1){LengthR = 166.5; WidthPhi = 2.0*pi/18.0;}

    CSC[S][Ring-1][Chamber-1]->SetDimensions(LengthR,WidthPhi); 

  } 

  CSCNominal.close();
}

//------------------------------

void ReadDeltaGeom(TString filename) {
  // Open root file
  TFile*   file      = new TFile(filename);
  TTree*   tree      = (TTree*)file->Get("MuonGeometry");
  TBranch* branch    = tree->GetBranch("info");
  TLeaf*   L_objId   = branch->GetLeaf("objId");
  TLeaf*   L_id1     = branch->GetLeaf("id1");
  TLeaf*   L_id2     = branch->GetLeaf("id2");
  TLeaf*   L_id3     = branch->GetLeaf("id3");
  TLeaf*   L_id4     = branch->GetLeaf("id4");
  TLeaf*   L_id5     = branch->GetLeaf("id5");
  TLeaf*   L_x       = branch->GetLeaf("x");
  TLeaf*   L_y       = branch->GetLeaf("y");
  TLeaf*   L_z       = branch->GetLeaf("z");
  TLeaf*   L_phix    = branch->GetLeaf("phix");
  TLeaf*   L_phiy    = branch->GetLeaf("phiy");
  TLeaf*   L_phiz    = branch->GetLeaf("phiz");
  TLeaf*   L_dx      = branch->GetLeaf("dx");
  TLeaf*   L_dy      = branch->GetLeaf("dy");
  TLeaf*   L_dz      = branch->GetLeaf("dz");
  TLeaf*   L_dphix   = branch->GetLeaf("dphix");
  TLeaf*   L_dphiy   = branch->GetLeaf("dphiy");
  TLeaf*   L_dphiz   = branch->GetLeaf("dphiz");
  TLeaf*   L_dxl     = branch->GetLeaf("dxl");
  TLeaf*   L_dyl     = branch->GetLeaf("dyl");
  TLeaf*   L_dzl     = branch->GetLeaf("dzl");
  TLeaf*   L_dphixl  = branch->GetLeaf("dphixl");
  TLeaf*   L_dphiyl  = branch->GetLeaf("dphiyl");
  TLeaf*   L_dphizl  = branch->GetLeaf("dphizl");

  int nb  = 0;                    // dummy, number of bytes
  int Nev = tree->GetEntries();   // number of entries in Tree
  for(int i=0;i<Nev;i++) {
    nb = tree->GetEntry(i);
    if(L_objId->GetValue() == 103){  // DT chamber
      // wheel, station and sector
      int W   = int(L_id1->GetValue());  
      int sta = int(L_id2->GetValue());
      int sec = int(L_id3->GetValue());
      // differences in global coordinates 
      double xg = L_dx->GetValue()*10.0;        // convert from cm to mm
      double yg = L_dy->GetValue()*10.0;
      double zg = L_dz->GetValue()*10.0;
      double ag = L_dphix->GetValue()*1000.0;  // convert from rad to mrad
      double bg = L_dphiy->GetValue()*1000.0;
      double gg = L_dphiz->GetValue()*1000.0;
      // differences in local coordinates
      double xl = L_dxl->GetValue()*10.0;
      double yl = L_dyl->GetValue()*10.0;
      double zl = L_dzl->GetValue()*10.0;
      double al = L_dphixl->GetValue()*1000.0;
      double bl = L_dphiyl->GetValue()*1000.0;
      double gl = L_dphizl->GetValue()*1000.0;

      // Correct angular differences of +pi and -pi
      if(ag>3000.0) ag -= 3141.592654;  if(ag<-3000.0) ag += 3141.592654;      
      if(bg>3000.0) bg -= 3141.592654;  if(bg<-3000.0) bg += 3141.592654;      
      if(gg>3000.0) gg -= 3141.592654;  if(gg<-3000.0) gg += 3141.592654;      
      if(al>3000.0) al -= 3141.592654;  if(al<-3000.0) al += 3141.592654;      
      if(bl>3000.0) bl -= 3141.592654;  if(bl<-3000.0) bl += 3141.592654;      
      if(gl>3000.0) gl -= 3141.592654;  if(gl<-3000.0) gl += 3141.592654;      

      DT[W+2][sta-1][sec-1]->SetDeltaGlobal(xg,yg,zg,ag,bg,gg);
      DT[W+2][sta-1][sec-1]->SetDeltaLocal(xl,yl,zl,al,bl,gl);

    }
    if(L_objId->GetValue() == 109){  // CSC chamber
      // endcap, station, ring and chamber
      int ec  = int(L_id1->GetValue());
      int sta = int(L_id2->GetValue());
      int rin = int(L_id3->GetValue());
      int cha = int(L_id4->GetValue());
      // differences in global coordinates
      double xg = L_dx->GetValue()*10.0;
      double yg = L_dy->GetValue()*10.0;
      double zg = L_dz->GetValue()*10.0;
      double ag = L_dphix->GetValue()*1000.0;
      double bg = L_dphiy->GetValue()*1000.0;
      double gg = L_dphiz->GetValue()*1000.0;
      // differences in local coordinates
      double xl = L_dxl->GetValue()*10.0;
      double yl = L_dyl->GetValue()*10.0;
      double zl = L_dzl->GetValue()*10.0;
      double al = L_dphixl->GetValue()*1000.0;
      double bl = L_dphiyl->GetValue()*1000.0;
      double gl = L_dphizl->GetValue()*1000.0;

      // Correct angular differences of +pi and -pi
      if(ag>3000.0) ag -= 3141.592654;  if(ag<-3000.0) ag += 3141.592654;      
      if(bg>3000.0) bg -= 3141.592654;  if(bg<-3000.0) bg += 3141.592654;      
      if(gg>3000.0) gg -= 3141.592654;  if(gg<-3000.0) gg += 3141.592654;      
      if(al>3000.0) al -= 3141.592654;  if(al<-3000.0) al += 3141.592654;      
      if(bl>3000.0) bl -= 3141.592654;  if(bl<-3000.0) bl += 3141.592654;      
      if(gl>3000.0) gl -= 3141.592654;  if(gl<-3000.0) gl += 3141.592654;      

      if(ec==2) sta = -sta;
      int S;
      if(sta<0) S = sta+4;
      else S = sta+3; 
      // ME11 stations contain ring 4 in data NOT included in geometry. EXCLUDE or crash.
      if(rin!=4) {
	//cout<<"S,rin,cha: "<<S<<" "<<rin<<" "<<cha<<endl;
	CSC[S][rin-1][cha-1]->SetDeltaGlobal(xg,yg,zg,ag,bg,gg);
        CSC[S][rin-1][cha-1]->SetDeltaLocal(xl,yl,zl,al,bl,gl);
      }
    }

  } // loop over entries

}

//------------------------------

void DisplayDTDeltaGeom(int W,TString opt){

  // Important: set option for all DTs in wheel
  for(int sta=0;sta<4;sta++) {
    for(int sec=0;sec<14;sec++) {
      if (!(sec>11 && sta<3)) DT[W+2][sta][sec]->SetOption(opt);
    }
  }
 
  TString whnum = "YB"; whnum+=W;
  TCanvas *YB = new TCanvas("YB",whnum,10,10,900,900);
  YB->cd();
  double padx1=-1000.0, pady1=-1000.0, padx2=1000.0, pady2=1000.0;
  gPad->Range(padx1,pady1,padx2,pady2);
  gPad->SetFillColor(10); gPad->SetBorderMode(0);

  // Display wheel and coordinate being shown
  TPaveText *pt = new TPaveText(-200,-50,200,50); 
  TText* txt;
  pt->SetFillColor(30);
  txt = pt->AddText(whnum);
  if     (opt=="localx")      txt = pt->AddText("local #DeltaX");
  else if(opt=="localy")      txt = pt->AddText("local #DeltaY");
  else if(opt=="localz")      txt = pt->AddText("local #DeltaZ");
  else if(opt=="localphix")   txt = pt->AddText("local #Delta#phix");
  else if(opt=="localphiy")   txt = pt->AddText("local #Delta#phiy");
  else if(opt=="localphiz")   txt = pt->AddText("local #Delta#phiz");
  else if(opt=="globalx")     txt = pt->AddText("global #DeltaX");
  else if(opt=="globaly")     txt = pt->AddText("global #DeltaY");
  else if(opt=="globalz")     txt = pt->AddText("global #DeltaZ");
  else if(opt=="globalphix")  txt = pt->AddText("global #Delta#phix");
  else if(opt=="globalphiy")  txt = pt->AddText("global #Delta#phiy");
  else if(opt=="globalphiz")  txt = pt->AddText("global #Delta#phiz");
  else txt = pt->AddText("Wrong opt");
  pt->Draw();

  double Min = 9999.9, Max=-9999.9;
  /*
  // Find Min and Max Delta in Wheel (necessary for DT color definition)
  for(int sta=0;sta<4;sta++) {
    for(int sec=0;sec<14;sec++) {
      if (!(sec>11 && sta<3)) {
        double Delta = DT[W+2][sta][sec]->GetDelta();
        if(Delta>Max) Max=Delta;
        if(Delta<Min) Min=Delta;
      }
    }
  }
  */
  // Set Min and Max by hand to have all plots on the same color scale
  if(opt.Contains("phi")){
    Min = -6.5; Max = 6.5;
  } else {
    Min = -15.0; Max = 15.0;
    if(opt=="globalz" || opt=="localy") { Min = -25; Max = 25; }
  }

  // define a color palette
  const int NRGBs     = 5;
  const int NColors   = 255;
  double stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  double red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  double green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  double blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NColors);
  gStyle->SetNumberContours(NColors);

  // Display DTs
  int    ncolors = gStyle->GetNumberOfColors();
  int    ndivz   = ncolors;
  int    theColor,color;
  double scale   = ndivz/(Max - Min);
  for(int sta=0;sta<4;sta++) {
    for(int sec=0;sec<14;sec++) {
      if (!(sec>11 && sta<3)) {
        // obtain DT color from Delta, Min and Max
        double Delta   = DT[W+2][sta][sec]->GetDelta();
        color          = int(0.01+(Delta-Min)*scale);
        theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
        int DTColor    = gStyle->GetColorPalette(theColor);
        if(Delta>Max) DTColor=16;  // grey for overflow
        if(Delta<Min) DTColor=1;   // black for underflow 
        DT[W+2][sta][sec]->Draw(DTColor);
      }
    }
  }
  
  // save Pad without palette
  if(PNGs){
    TString padName = whnum+"_"+opt+".png";
    YB->SaveAs(padName);
  }

  // Display the color palette
  char s[70]; int n;
  TText *t;
  double height = (pady2-pady1-50)/double(ncolors);
  double step   = (Max-Min)/double(ncolors);
  for(int indx=0;indx<=ncolors;indx++) {
    TBox *box = new TBox(padx2-180,pady1+20+height*indx,padx2-140,pady1+20+height*(indx+1));
    color          = int(0.01+(step*double(indx))*scale);
    theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
    int BoxColor   = gStyle->GetColorPalette(theColor);
    box->SetFillColor(BoxColor);
    box->Draw();
  }
  // now loop over integers from Min to Max and find right placement
  for(int i = int(Min); i<=int(Max); i++) {
    if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
    else n = sprintf(s,"%d mm",i);
    t = new TText(padx2-130,pady1+20+height*ncolors*(i-Min)/(Max-Min),s);
    t->SetTextSize(0.015);
    t->Draw();
  }
  YB->Update();
  YB->Print(pdfname,"pdf"); // print canvas to pdf file
  cout<<"Printed "<<whnum<<" "<<opt<<endl;

  // if PNGs, save palette separately as png
  if(PNGs){
    TCanvas *PaletteYB = new TCanvas("pal","pal",10,10,200,900);
    PaletteYB->cd();
    double padx1=0.0, pady1=-1000.0, padx2=100.0, pady2=1000.0;
    gPad->Range(padx1,pady1,padx2,pady2);
    gPad->SetFillColor(10); gPad->SetBorderMode(0);
    // Display the color palette
    char s[70]; int n;
    TText *t;
    double height = (pady2-pady1-50)/double(ncolors);
    double step   = (Max-Min)/double(ncolors);
    for(int indx=0;indx<=ncolors;indx++) {
      TBox *box = new TBox(2,pady1+20+height*indx,48,pady1+20+height*(indx+1));
      color          = int(0.01+(step*double(indx))*scale);
      theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
      int BoxColor   = gStyle->GetColorPalette(theColor);
      box->SetFillColor(BoxColor);
      box->Draw();
    }
    // now loop over integers from Min to Max and find right placement
    for(int i = int(Min); i<=int(Max); i++) {
      if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
      else n = sprintf(s,"%d mm",i);
      t = new TText(50,pady1+20+height*ncolors*(i-Min)/(Max-Min),s);
      t->SetTextSize(0.1);
      t->Draw();
    }
    TString palName = "PaletteYB_"+opt+".png";
    PaletteYB->SaveAs(palName);
  }

}

//------------------------------------------

void DisplaySummaryDTDeltaGeom(TString opt){

  TCanvas *YBS = new TCanvas("YBsummary","YBsummary",10,10,1000,900);
  // make a special pad for the color palette and another one for the plots
  YBS->cd();
  YBS->SetFillColor(10); YBS->SetBorderMode(0);
  TPad *palettepad = new TPad("pp","pp",0.92,0.02,0.98,0.98);
  palettepad->SetFillColor(10); palettepad->SetBorderMode(0); palettepad->Draw();
  TPad *summarypad = new TPad("ybsp","ybsp",0.02,0.02,0.91,0.98);
  summarypad->SetFillColor(10); summarypad->SetBorderMode(0); summarypad->Draw();
  summarypad->Divide(3,3);

  // Important: set option for all DTs in all wheels
  for(int W=-2;W<3;W++){
    for(int sta=0;sta<4;sta++) {
      for(int sec=0;sec<14;sec++) {
        if (!(sec>11 && sta<3)) DT[W+2][sta][sec]->SetOption(opt);
      }
    }

    // get pad number and plot one wheel per pad
    // order pads as: YB2       YB1         1 2 3
    //                     YB0              4 5 6
    //                YB-1      YB-2        7 8 9

    int pad = 11-2*(W+3);

    summarypad->cd(pad); 
    gPad->SetFillColor(10); gPad->SetBorderMode(0); gPad->Draw();
    TString whnum = "YB"; whnum+=W;

    double padx1=-800.0, pady1=-800.0, padx2=800.0, pady2=800.0;
    gPad->Range(padx1,pady1,padx2,pady2);

    // Display wheel and coordinate being shown
    TPaveText *pt = new TPaveText(-200,-75,200,75); 
    TText* txt;
    pt->SetFillColor(30);
    txt = pt->AddText(whnum);
    if     (opt=="localx")      txt = pt->AddText("local #DeltaX");
    else if(opt=="localy")      txt = pt->AddText("local #DeltaY");
    else if(opt=="localz")      txt = pt->AddText("local #DeltaZ");
    else if(opt=="localphix")   txt = pt->AddText("local #Delta#phix");
    else if(opt=="localphiy")   txt = pt->AddText("local #Delta#phiy");
    else if(opt=="localphiz")   txt = pt->AddText("local #Delta#phiz");
    else if(opt=="globalx")     txt = pt->AddText("global #DeltaX");
    else if(opt=="globaly")     txt = pt->AddText("global #DeltaY");
    else if(opt=="globalz")     txt = pt->AddText("global #DeltaZ");
    else if(opt=="globalphix")  txt = pt->AddText("global #Delta#phix");
    else if(opt=="globalphiy")  txt = pt->AddText("global #Delta#phiy");
    else if(opt=="globalphiz")  txt = pt->AddText("global #Delta#phiz");
    else txt = pt->AddText("Wrong opt");
    pt->Draw();

    double Min = 9999.9, Max=-9999.9;
    /*
    // Find Min and Max Delta in Wheel (necessary for DT color definition)
    for(int sta=0;sta<4;sta++) {
      for(int sec=0;sec<14;sec++) {
        if (!(sec>11 && sta<3)) {
          double Delta = DT[W+2][sta][sec]->GetDelta();
          if(Delta>Max) Max=Delta;
          if(Delta<Min) Min=Delta;
        }
      }
    }
    */
    // Set Min and Max by hand to have all plots on the same color scale
    if(opt.Contains("phi")){
      Min = -6.5; Max = 6.5;
    } else {
      Min = -15.0; Max = 15.0;
      if(opt=="globalz" || opt=="localy") { Min = -25; Max = 25; }
    }

    // define a color palette
    const int NRGBs     = 5;
    const int NColors   = 255;
    double stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    double red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    double green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    double blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NColors);
    gStyle->SetNumberContours(NColors);

    // Display DTs
    int    ncolors = gStyle->GetNumberOfColors();
    int    ndivz   = ncolors;
    int    theColor,color;
    double scale   = ndivz/(Max - Min);
    for(int sta=0;sta<4;sta++) {
      for(int sec=0;sec<14;sec++) {
        if (!(sec>11 && sta<3)) {
          // obtain DT color from Delta, Min and Max
          double Delta   = DT[W+2][sta][sec]->GetDelta();
          color          = int(0.01+(Delta-Min)*scale);
          theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
          int DTColor    = gStyle->GetColorPalette(theColor);
          if(Delta>Max) DTColor=16;  // grey for overflow
          if(Delta<Min) DTColor=1;   // black for underflow 
          DT[W+2][sta][sec]->Draw(DTColor);
        }
      }
    }

    // Display the color palette only once in palette pad
    if(pad==1) {
      palettepad->cd();
      double px1=0.0, py1=0.0, px2=100.0, py2=100.0;
      gPad->Range(px1,py1,px2,py2);
      char s[70]; int n;
      TText *t;
      double height = (py2-py1-2)/double(ncolors);
      double step   = (Max-Min)/double(ncolors);
      for(int indx=0;indx<=ncolors;indx++) {
        TBox *box = new TBox(px1+2,py1+1+height*indx,px2-55,py1+1+height*(indx+1));
        color          = int(0.01+(step*double(indx))*scale);
        theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
        int BoxColor   = gStyle->GetColorPalette(theColor);
        box->SetFillColor(BoxColor);
        box->Draw();
      }
      // now loop over integers from Min to Max and find right placement
      for(int i = int(Min); i<=int(Max); i++) {
        if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
        else n = sprintf(s,"%d mm",i);
        t = new TText(px2-53,py1+1+height*ncolors*(i-Min)/(Max-Min),s);
        t->SetTextSize(0.13);
        t->Draw();
      }
    }  // if pad = 1
  } // loop over wheels

  YBS->Update();
  YBS->Print(pdfname,"pdf"); // print canvas to pdf file
  cout<<"Printed barrel summary for "<<opt<<endl;

}

//------------------------------

void PlotDTDeltaGeom(int W,TString opt, bool DynamicScale){

  // Important: set option for all DTs in wheel
  for(int sta=0;sta<4;sta++) {
    for(int sec=0;sec<14;sec++) {
      if (!(sec>11 && sta<3)) DT[W+2][sta][sec]->SetOption(opt);
    }
  }

  TString whnum = "YB"; whnum+=W;
  TCanvas *YB = new TCanvas("YB",whnum,10,10,900,900);
  YB->cd();
  TString Yaxis;
  if     (opt=="localx")      Yaxis = "local #DeltaX (mm)";
  else if(opt=="localy")      Yaxis = "local #DeltaY (mm)";
  else if(opt=="localz")      Yaxis = "local #DeltaZ (mm)";
  else if(opt=="localphix")   Yaxis = "local #Delta#phix (mrad)";
  else if(opt=="localphiy")   Yaxis = "local #Delta#phiy (mrad)";
  else if(opt=="localphiz")   Yaxis = "local #Delta#phiz (mrad)";
  else if(opt=="globalx")     Yaxis = "global #DeltaX (mm)";
  else if(opt=="globaly")     Yaxis = "global #DeltaY (mm)";
  else if(opt=="globalz")     Yaxis = "global #DeltaZ (mm)";
  else if(opt=="globalphix")  Yaxis = "global #Delta#phix (mrad)";
  else if(opt=="globalphiy")  Yaxis = "global #Delta#phiy (mrad)";
  else if(opt=="globalphiz")  Yaxis = "global #Delta#phiz (mrad)";
  else Yaxis = "Wrong opt";

  double Min, Max;
  if(DynamicScale) {
    Min = 9999.9; Max=-9999.9;
    // Find Min and Max Delta in Wheel so set scale dynamically if DynamicScale=true
    for(int sta=0;sta<4;sta++) {
      for(int sec=0;sec<14;sec++) {
        if (!(sec>11 && sta<3)) {
          double Delta = DT[W+2][sta][sec]->GetDelta();
          if(!MaskDT[W+2][sta][sec]) {
            if(Delta>Max) Max=Delta;
            if(Delta<Min) Min=Delta;
	  }
        }
      }
    }
    Max = Max + 0.15*(Max-Min);
    Min = Min - 0.15*(Max-Min);
  } else {
      // Set Min and Max by hand to have all plots on the same  scale
      if(opt.Contains("phix")||opt.Contains("phiy")||opt.Contains("phiz")){
        Min = -4.0; Max = 4.0;
      } else {
        Min = -10.0; Max = 10.0;
      }
  }

  // Define array of 4 TGraphs, one for each DT station 
  TGraph *TGsta[4];

  TGsta[3] = new TGraph();
  TGsta[3]->SetMarkerStyle(21);
  TGsta[3]->SetMarkerColor(1);
  TGsta[3]->SetMarkerSize(2);
  TGsta[3]->SetLineColor(1);
  TGsta[3]->SetLineWidth(3);

  TGsta[0] = new TGraph();
  TGsta[0]->SetMarkerStyle(22);
  TGsta[0]->SetMarkerColor(2);
  TGsta[0]->SetMarkerSize(2);
  TGsta[0]->SetLineColor(2);
  TGsta[0]->SetLineWidth(3);

  TGsta[1] = new TGraph();
  TGsta[1]->SetMarkerStyle(23);
  TGsta[1]->SetMarkerColor(3);
  TGsta[1]->SetMarkerSize(2);
  TGsta[1]->SetLineColor(3);
  TGsta[1]->SetLineWidth(3);

  TGsta[2] = new TGraph();
  TGsta[2]->SetMarkerStyle(24);
  TGsta[2]->SetMarkerColor(4);
  TGsta[2]->SetMarkerSize(2);
  TGsta[2]->SetLineColor(4);
  TGsta[2]->SetLineWidth(3);

  int NP;
  cout<<"Format is: Wheel Station Sector Difference_"<<opt<<endl;
  for(int sta=0;sta<4;sta++) {
    for(int sec=0;sec<14;sec++) {
      if (!(sec>11 && sta<3)) {
        double Delta   = DT[W+2][sta][sec]->GetDelta();
        NP = TGsta[sta]->GetN();
        if(!MaskDT[W+2][sta][sec]) {
          TGsta[sta]->SetPoint(NP,sec+1,Delta);
  	  // add print-outs for Zoltan
          cout<<W<<" "<<sta+1<<" "<<sec+1<<" "<<Delta<<endl;
	}
      }
    }
  }

  // Display the TGraphs, starting with station 4.
  // The other station TGraphs will be plotted with the same title and axis as sta4 (which has 14 sectors)

  // protect against completely masked out station4!
  if(TGsta[3]->GetN()>0) {
    TGsta[3]->SetMaximum(Max);
    TGsta[3]->SetMinimum(Min);
    TGsta[3]->SetTitle(whnum);
    TGsta[3]->Draw("ALP");
    TGsta[3]->GetXaxis()->SetLimits(0,15.5);
    TGsta[3]->GetXaxis()->SetTitle("Sector");
    TGsta[3]->GetYaxis()->SetTitle(Yaxis);
  } else {
    TGsta[2]->SetMaximum(Max);
    TGsta[2]->SetMinimum(Min);
    TGsta[2]->SetTitle(whnum);
    TGsta[2]->Draw("ALP");
    TGsta[2]->GetXaxis()->SetLimits(0,15.5);
    TGsta[2]->GetXaxis()->SetTitle("Sector");
    TGsta[2]->GetYaxis()->SetTitle(Yaxis);
  }
  TText *t1,*t2,*t3,*t4;
  t1 = new TText(14,Max-0.04*(Max-Min),"st1");
  t1->SetTextSize(0.04);
  t1->SetTextColor(2);
  t1->Draw();
  t2 = new TText(14,Max-0.08*(Max-Min),"st2");
  t2->SetTextSize(0.04);
  t2->SetTextColor(3);
  t2->Draw();
  t3 = new TText(14,Max-0.12*(Max-Min),"st3");
  t3->SetTextSize(0.04);
  t3->SetTextColor(4);
  t3->Draw();
  t4 = new TText(14,Max-0.16*(Max-Min),"st4");
  t4->SetTextSize(0.04);
  t4->SetTextColor(1);
  if(TGsta[3]->GetN()>0) t4->Draw();

  TGsta[0]->Draw("LP");
  TGsta[1]->Draw("LP");
  if(TGsta[3]->GetN()>0) TGsta[2]->Draw("LP");
  
  YB->Update();
  YB->Print(pdfname,"pdf"); // print canvas to pdf file
  cout<<"Printed "<<whnum<<" "<<opt<<endl;

  YB->Close();   // avoid memory leak warnings

}

//------------------------------

void DisplayCSCDeltaGeom(int Station,TString opt){

  int S;
  if(Station<0) S = Station+4;
  else S = Station+3; 

  // Important: set option for all CSCs in wheel
  for(int Ring=0;Ring<3;Ring++) {
    for(int Chamber=0;Chamber<36;Chamber++) {
      if (! ( (abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1) ) ) 
	CSC[S][Ring][Chamber]->SetOption(opt);
    }
  }

  TString stnum = "ME"; stnum+=Station;
  TCanvas *ME = new TCanvas("YE",stnum,10,10,900,900);
  ME->cd();
  double padx1=-1000.0, pady1=-1000.0, padx2=1000.0, pady2=1000.0;
  gPad->Range(padx1,pady1,padx2,pady2);
  gPad->SetFillColor(10); gPad->SetBorderMode(0);

  // Display ME station and coordinate being shown
  TPaveText *pt = new TPaveText(padx1+0.5*(padx2-padx1)-200,pady1+30,padx1+0.5*(padx2-padx1)+200,pady1+190); 
  TText* txt;
  pt->SetFillColor(30);
  txt = pt->AddText(stnum);
  if     (opt=="localx")      txt = pt->AddText("local #DeltaX");
  else if(opt=="localy")      txt = pt->AddText("local #DeltaY");
  else if(opt=="localz")      txt = pt->AddText("local #DeltaZ");
  else if(opt=="localphix")   txt = pt->AddText("local #Delta#phix");
  else if(opt=="localphiy")   txt = pt->AddText("local #Delta#phiy");
  else if(opt=="localphiz")   txt = pt->AddText("local #Delta#phiz");
  else if(opt=="globalx")     txt = pt->AddText("global #DeltaX");
  else if(opt=="globaly")     txt = pt->AddText("global #DeltaY");
  else if(opt=="globalz")     txt = pt->AddText("global #DeltaZ");
  else if(opt=="globalphix")  txt = pt->AddText("global #Delta#phix");
  else if(opt=="globalphiy")  txt = pt->AddText("global #Delta#phiy");
  else if(opt=="globalphiz")  txt = pt->AddText("global #Delta#phiz");
  else txt = pt->AddText("Wrong opt");
  pt->Draw();

  double Min = 9999.9, Max=-9999.9;
  // Find Min and Max Delta in ME station (necessary for CSC color definition)
  /*
  for(int Ring=0;Ring<3;Ring++) {
    for(int Chamber=0;Chamber<36;Chamber++) {
      if (! ( (abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1) ) ) {
        double Delta = CSC[S][Ring][Chamber]->GetDelta();
        if(Delta>Max) Max=Delta;
        if(Delta<Min) Min=Delta;
      }
    }
  }
  */

  // Set Min and Max by hand to have all plots on the same color scale
  if(opt.Contains("phi")){
    Min = -6.5; Max = 6.5;
  } else {
    Min = -15; Max = 15;
    if(opt.Contains("z")) { Min = -25; Max = 25; }
  }

  // define a color palette
  const int NRGBs     = 5;
  const int NColors   = 255;
  double stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  double red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  double green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  double blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NColors);
  gStyle->SetNumberContours(NColors);

  // Display CSCs
  int    ncolors = gStyle->GetNumberOfColors();
  int    ndivz   = ncolors;
  int    theColor,color;
  double scale   = ndivz/(Max - Min);
  for(int Ring=0;Ring<3;Ring++) {
    for(int Chamber=0;Chamber<36;Chamber++) {
      if (! ( (abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1) ) ) {
        // obtain CSC color from Delta, Min and Max
        double Delta   = CSC[S][Ring][Chamber]->GetDelta();
        color          = int(0.01+(Delta-Min)*scale);
        theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
        int CSCColor   = gStyle->GetColorPalette(theColor);
        if(Delta>Max) CSCColor=16;  // grey for overflow
        if(Delta<Min) CSCColor=1;   // black for underflow 

        // highlight DCOPS CSCs: wider, red line for measured CSCs
        int Ch = Chamber+1; // actual chamber number is array index plus one

        // inner rings of ME2,3,4 have DCOPS in CSCs 2,4,8,10,14,16
        if(abs(Station)>1 && Ring==0) {
	  if(Ch==2 || Ch==4 || Ch==8 || Ch==10 || Ch==14 || Ch==16) {
            CSC[S][Ring][Chamber]->SetLineWidth(2);
            CSC[S][Ring][Chamber]->SetLineColor(2);
	  }
	}
        // outer rings of ME2,3 have DCOPS in CSCs 3,8,15,20,27,32
        if(abs(Station)>1 && abs(Station)<4 && Ring==1) {
	  if(Ch==3 || Ch==8 || Ch==15 || Ch==20 || Ch==27 || Ch==32) {
            CSC[S][Ring][Chamber]->SetLineWidth(2);
            CSC[S][Ring][Chamber]->SetLineColor(2);
	  }
	}
        // ME12 have DCOPS in CSCs 2,8,14,20,26,32 
        if(abs(Station)==1 && Ring==1) {
	  if(Ch==2 || Ch==8 || Ch==14 || Ch==20 || Ch==26 || Ch==32) {
            CSC[S][Ring][Chamber]->SetLineWidth(2);
            CSC[S][Ring][Chamber]->SetLineColor(2);
	  }
	}
        // ME13 have DCOPS in CSCs 3,9,14,20,27,33 
        if(abs(Station)==1 && Ring==2) {
	  if(Ch==3 || Ch==9 || Ch==14 || Ch==20 || Ch==27 || Ch==33) {
            CSC[S][Ring][Chamber]->SetLineWidth(2);
            CSC[S][Ring][Chamber]->SetLineColor(2);
	  }
	}

        CSC[S][Ring][Chamber]->Draw(CSCColor);

      }  // if
    }    // loop over Chambers
  }      // loop over rings

  // save Pad without palette
  if(PNGs){
    TString padName = stnum+"_"+opt+".png";
    ME->SaveAs(padName);
  }

  // Display the color palette
  char s[70]; int n;
  TText *t;
  double height = (pady2-pady1-50)/double(ncolors);
  double step   = (Max-Min)/double(ncolors);
  for(int indx=0;indx<=ncolors;indx++) {
    TBox *box = new TBox(padx2-180,pady1+20+height*indx,padx2-140,pady1+20+height*(indx+1));
    color          = int(0.01+(step*double(indx))*scale);
    theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
    int BoxColor   = gStyle->GetColorPalette(theColor);
    box->SetFillColor(BoxColor);
    box->Draw();
  }
  // now loop over integers from Min to Max and find right placement
  for(int i = int(Min); i<=int(Max); i++) {
    if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
    else n = sprintf(s,"%d mm",i);
    t = new TText(padx2-130,pady1+20+height*ncolors*(i-Min)/(Max-Min),s);
    t->SetTextSize(0.015);
    t->Draw();
  }

  ME->Update();
  ME->Print(pdfname,"pdf"); // print canvas to pdf file
  cout<<"Printed "<<stnum<<" "<<opt<<endl;

  // if PNGs, save palette separately as png
  if(PNGs){
    TCanvas *PaletteME = new TCanvas("pal","pal",10,10,200,900);
    PaletteME->cd();
    double padx1=0.0, pady1=-1000.0, padx2=100.0, pady2=1000.0;
    gPad->Range(padx1,pady1,padx2,pady2);
    gPad->SetFillColor(10); gPad->SetBorderMode(0);
    // Display the color palette
    char s[70]; int n;
    TText *t;
    double height = (pady2-pady1-50)/double(ncolors);
    double step   = (Max-Min)/double(ncolors);
    for(int indx=0;indx<=ncolors;indx++) {
      TBox *box = new TBox(2,pady1+20+height*indx,48,pady1+20+height*(indx+1));
      color          = int(0.01+(step*double(indx))*scale);
      theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
      int BoxColor   = gStyle->GetColorPalette(theColor);
      box->SetFillColor(BoxColor);
      box->Draw();
    }
    // now loop over integers from Min to Max and find right placement
    for(int i = int(Min); i<=int(Max); i++) {
      if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
      else n = sprintf(s,"%d mm",i);
      t = new TText(50,pady1+20+height*ncolors*(i-Min)/(Max-Min),s);
      t->SetTextSize(0.1);
      t->Draw();
    }
    TString palName = "PaletteME_"+opt+".png";
    PaletteME->SaveAs(palName);
  }

}

//-------------------------------------------

void DisplaySummaryCSCDeltaGeom(TString opt){

  TCanvas *MES = new TCanvas("MEsummary","MEsummary",10,10,1300,600);
  // make a special pad for the color palette and another one for the plots
  MES->cd();
  MES->SetFillColor(10); MES->SetBorderMode(0);
  TPad *palettepad = new TPad("pp","pp",0.92,0.02,0.98,0.98);
  palettepad->SetFillColor(10); palettepad->SetBorderMode(0); palettepad->Draw();
  TPad *summarypad = new TPad("mesp","mesp",0.02,0.02,0.91,0.98);
  summarypad->SetFillColor(10); summarypad->SetBorderMode(0); summarypad->Draw();
  summarypad->Divide(4,2);

  // Important: set option for all CSCs in all stations
  for(int Station = -4; Station<5; Station++) {
    if(Station!=0) {
      int S;
      if(Station<0) S = Station+4;
      else S = Station+3; 
      for(int Ring=0;Ring<3;Ring++) {
        for(int Chamber=0;Chamber<36;Chamber++) {
          if (! ((abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1)) )
	  CSC[S][Ring][Chamber]->SetOption(opt);
        }
      }

      // get pad number and plot one pad per station
      // order pads as ME 1,2,3,4 on top and -1,-2,-3,-4 on bottom
      int pad = S+1;
      if(pad>4) pad-=4;  // 5,6,7,8 go to 1,2,3,4
      else pad = 9-pad;  // 4,3,2,1 go to 5,6,7,8

      summarypad->cd(pad); 
      gPad->SetFillColor(10); gPad->SetBorderMode(0); gPad->Draw();
      TString stnum = "ME"; stnum+=Station;
      double padx1=-900.0, pady1=-900.0, padx2=900.0, pady2=900.0;
      gPad->Range(padx1,pady1,padx2,pady2);

      // Display ME station and coordinate being shown
      TPaveText *pt = new TPaveText(padx1+0.5*(padx2-padx1)-200,pady1+5,padx1+0.5*(padx2-padx1)+200,pady1+175); 
      TText* txt;
      pt->SetFillColor(30);
      txt = pt->AddText(stnum);
      if     (opt=="localx")      txt = pt->AddText("local #DeltaX");
      else if(opt=="localy")      txt = pt->AddText("local #DeltaY");
      else if(opt=="localz")      txt = pt->AddText("local #DeltaZ");
      else if(opt=="localphix")   txt = pt->AddText("local #Delta#phix");
      else if(opt=="localphiy")   txt = pt->AddText("local #Delta#phiy");
      else if(opt=="localphiz")   txt = pt->AddText("local #Delta#phiz");
      else if(opt=="globalx")     txt = pt->AddText("global #DeltaX");
      else if(opt=="globaly")     txt = pt->AddText("global #DeltaY");
      else if(opt=="globalz")     txt = pt->AddText("global #DeltaZ");
      else if(opt=="globalphix")  txt = pt->AddText("global #Delta#phix");
      else if(opt=="globalphiy")  txt = pt->AddText("global #Delta#phiy");
      else if(opt=="globalphiz")  txt = pt->AddText("global #Delta#phiz");
      else txt = pt->AddText("Wrong opt");
      pt->Draw();

      double Min = 9999.9, Max=-9999.9;
      // Find Min and Max Delta in ME station (necessary for CSC color definition)
      /*
      for(int Ring=0;Ring<3;Ring++) {
        for(int Chamber=0;Chamber<36;Chamber++) {
          if (! ( (abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1) ) ) {
            double Delta = CSC[S][Ring][Chamber]->GetDelta();
            if(Delta>Max) Max=Delta;
            if(Delta<Min) Min=Delta;
          }
        }
      }
      */

      // Set Min and Max by hand to have all plots on the same color scale
      if(opt.Contains("phi")){
        Min = -6.5; Max = 6.5;
      } else {
        Min = -15; Max = 15;
        if(opt.Contains("z")) { Min = -25; Max = 25; }
      }

      // define a color palette
      const int NRGBs     = 5;
      const int NColors   = 255;
      double stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
      double red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
      double green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
      double blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NColors);
      gStyle->SetNumberContours(NColors);

      // Display CSCs
      int    ncolors = gStyle->GetNumberOfColors();
      int    ndivz   = ncolors;
      int    theColor,color;
      double scale   = ndivz/(Max - Min);
      for(int Ring=0;Ring<3;Ring++) {
        for(int Chamber=0;Chamber<36;Chamber++) {
          if (! ( (abs(Station)>1&&Ring>1) || (abs(Station)>1&&Ring==0&&Chamber>17) || (abs(Station)==4&&Ring>1) ) ) {
            // obtain CSC color from Delta, Min and Max
            double Delta   = CSC[S][Ring][Chamber]->GetDelta();
            color          = int(0.01+(Delta-Min)*scale);
            theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
            int CSCColor   = gStyle->GetColorPalette(theColor);
            if(Delta>Max) CSCColor=16;  // grey for overflow
            if(Delta<Min) CSCColor=1;   // black for underflow 

            // highlight DCOPS CSCs: wider, red line for measured CSCs
            int Ch = Chamber+1; // actual chamber number is array index plus one

            // inner rings of ME2,3,4 have DCOPS in CSCs 2,4,8,10,14,16
            if(abs(Station)>1 && Ring==0) {
	      if(Ch==2 || Ch==4 || Ch==8 || Ch==10 || Ch==14 || Ch==16) {
                CSC[S][Ring][Chamber]->SetLineWidth(2);
                CSC[S][Ring][Chamber]->SetLineColor(2);
	      }
	    }
            // outer rings of ME2,3 have DCOPS in CSCs 3,8,15,20,27,32
            if(abs(Station)>1 && abs(Station)<4 && Ring==1) {
	      if(Ch==3 || Ch==8 || Ch==15 || Ch==20 || Ch==27 || Ch==32) {
                CSC[S][Ring][Chamber]->SetLineWidth(2);
                CSC[S][Ring][Chamber]->SetLineColor(2);
	      }
	    }
            // ME12 have DCOPS in CSCs 2,8,14,20,26,32 
            if(abs(Station)==1 && Ring==1) {
	      if(Ch==2 || Ch==8 || Ch==14 || Ch==20 || Ch==26 || Ch==32) {
                CSC[S][Ring][Chamber]->SetLineWidth(2);
                CSC[S][Ring][Chamber]->SetLineColor(2);
	      }
   	    }
            // ME13 have DCOPS in CSCs 3,9,14,20,27,33 
            if(abs(Station)==1 && Ring==2) {
	      if(Ch==3 || Ch==9 || Ch==14 || Ch==20 || Ch==27 || Ch==33) {
                CSC[S][Ring][Chamber]->SetLineWidth(2);
                CSC[S][Ring][Chamber]->SetLineColor(2);
	      }
	    }

            CSC[S][Ring][Chamber]->Draw(CSCColor);

          }  // if
        }    // loop over Chambers
      }      // loop over rings

      

      // Display the color palette only once in palette pad
      if(pad==1) {
        palettepad->cd();
        double px1=0.0, py1=0.0, px2=100.0, py2=100.0;
        gPad->Range(px1,py1,px2,py2);
        char s[70]; int n;
        TText *t;
        double height = (py2-py1-2)/double(ncolors);
        double step   = (Max-Min)/double(ncolors);
        for(int indx=0;indx<=ncolors;indx++) {
          TBox *box = new TBox(px1+2,py1+1+height*indx,px2-55,py1+1+height*(indx+1));
          color          = int(0.01+(step*double(indx))*scale);
          theColor       = int((color+0.99)*double(ncolors)/double(ndivz));
          int BoxColor   = gStyle->GetColorPalette(theColor);
          box->SetFillColor(BoxColor);
          box->Draw();
	}
        // now loop over integers from Min to Max and find right placement
        for(int i = int(Min); i<=int(Max); i++) {
          if(opt.Contains("phi")) n = sprintf(s,"%d mrad",i);
          else n = sprintf(s,"%d mm",i);
          t = new TText(px2-53,py1+1+height*ncolors*(i-Min)/(Max-Min),s);
          t->SetTextSize(0.13);
          t->Draw();
        }
      } // if pad = 1

    } // if station!=0
  } // loop over stations


  MES->Update();
  MES->Print(pdfname,"pdf"); // print canvas to pdf file
  cout<<"Printed endcap summary for "<<opt<<endl;

  // The next two lines were added to make pdf plots for Himali's thesis
  // TString HimaliPDFname = "ME_"+opt+".pdf";
  // MES->SaveAs(HimaliPDFname);

}

//-------------------------------------------

int GetWSS(TString WSSString) {
  WSSString.Remove(0, 3);
  return WSSString.Atoi();
}

//-------------------------------------------

double GetCoordinateFromString(TString Vec, int coor) {
  TString Dummy = Vec;
  Dummy.ReplaceAll("(", ""); Dummy.ReplaceAll(")", "");
  int Size = Dummy.Sizeof(); 
  int Coma = Dummy.First(",");
  if (coor==0) {
    Dummy.Remove(Coma,Size);
    return Dummy.Atof();
  }
  Dummy.Remove(0,Coma+1);
  Size = Dummy.Sizeof(); 
  Coma = Dummy.First(",");
  if (coor==1) {
    Dummy.Remove(Coma,Size);
    return Dummy.Atof();
  }
  Dummy.Remove(0,Coma+1);
  if (coor==2) return Dummy.Atof();
  cout << "Wrong choice of coordinates in GetCoordinateFromString" << endl;
  return 0.0;
}

//-------------------------------------------

double GetDimensionFromString(TString Vec, int coor) {
  TString Dummy = Vec;
  int Size  = Dummy.Sizeof(); 
  int Slash = Dummy.First("/");
  if (coor==0) {
    Dummy.Remove(Slash,Size);
    return Dummy.Atof();
  }
  Dummy.Remove(0,Slash+1);
  Size  = Dummy.Sizeof(); 
  Slash = Dummy.First("/");
  if (coor==1) {
    Dummy.Remove(Slash,Size);
    return Dummy.Atof();
  }
  Dummy.Remove(0,Slash+1);
  if (coor==2) return Dummy.Atof();
  cout << "Wrong choice of coordinates in GetDimensionFromString" << endl;
  return 0.0;
}
