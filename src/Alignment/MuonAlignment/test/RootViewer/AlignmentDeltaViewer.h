#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TString.h>
#include <TText.h>
#include <TPaveText.h>
#include <TH2F.h>
#include <TColor.h>
#include <TGraph.h>
#include <TPaletteAxis.h>
#include <TRandom2.h>
#include <TBox.h>
#include <TCrown.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TPaveText.h>
#include <TText.h>
#include <TString.h>
#include <TMath.h>

class DTChamber : public TPolyLine {
private:
  int     fWheel,fStation,fSector;
  double  fX,fY,fZ,fW,fH,fL,fNormX,fNormY,fNormZ;
  double  fDX,fDY,fDZ,fDphix,fDphiy,fDphiz;
  double  fDXL,fDYL,fDZL,fDphixL,fDphiyL,fDphizL;
  TString fOption;

public:
  DTChamber(){};
  ~DTChamber(){};
  DTChamber(int Wheel,int Station,int Sector, 
            double X,double Y,double Z,
            double W,double H,double L,
            double NormX,double NormY,double NormZ){

    fWheel   = Wheel;
    fStation = Station;
    fSector  = Sector;
    fX       = X;
    fY       = Y;
    fZ       = Z;
    fW       = W;
    fH       = H;
    fL       = L;
    fNormX   = NormX;
    fNormY   = NormY;
    fNormZ   = NormZ;
  };

  void SetOption(TString opt){fOption=opt;};

  void SetDeltaGlobal(double DX, double DY, double DZ, double Dphix, double Dphiy, double Dphiz){
    fDX=DX; fDY=DY; fDZ=DZ;
    fDphix=Dphix; fDphiy=Dphiy; fDphiz=Dphiz;
  };

  void SetDeltaLocal(double DXL,double DYL, double DZL, double DphixL, double DphiyL, double DphizL){
    fDXL=DXL; fDYL=DYL; fDZL=DZL;
    fDphixL=DphixL; fDphiyL=DphiyL; fDphizL=DphizL;
  };

  double GetDelta(){
    if (fOption=="localx")      return fDXL;
    if (fOption=="localy")      return fDYL;
    if (fOption=="localz")      return fDZL;
    if (fOption=="localphix")   return fDphixL;
    if (fOption=="localphiy")   return fDphiyL;
    if (fOption=="localphiz")   return fDphizL;
    if (fOption=="globalx")     return fDX;
    if (fOption=="globaly")     return fDY;
    if (fOption=="globalz")     return fDZ;
    if (fOption=="globalphix")  return fDphix;
    if (fOption=="globalphiy")  return fDphiy;
    if (fOption=="globalphiz")  return fDphiz;
    cout<<"Wrong option or GetDelta called before calling SetOption.  Must be one of:"<<endl;
    cout<<"localx, localy, localz, localphix, localphiy, localphiz"<<endl; 
    cout<<"globalx, globaly, globalz, globalphix, globalphiy, globalphiz"<<endl;
    return -9999.0; 
  };

  // override inherited TPolyLine method
  void ExecuteEvent(Int_t event, Int_t px, Int_t py){
    if (!gPad) return;
    if (!gPad->IsEditable()) return;
    if(event == 11) {
      TText *txt;
      double padx1,pady1,padx2,pady2;
      gPad->GetRange(padx1,pady1,padx2,pady2);
      TPaveText *tpt = new TPaveText(padx1+10,pady2-110,padx1+310,pady2-10);
      tpt->SetFillColor(38);
      TString text = "W"; text+=fWheel;
      text+="Sta"; text+=fStation;
      text+="Sec"; text+=fSector;
      txt = tpt->AddText(text);

      text = "";
      if      (fOption=="localx")      text+= fDXL;
      else if (fOption=="localy")      text+= fDYL;
      else if (fOption=="localz")      text+= fDZL;
      else if (fOption=="localphix")   text+= fDphixL;
      else if (fOption=="localphiy")   text+= fDphiyL;
      else if (fOption=="localphiz")   text+= fDphizL;
      else if (fOption=="globalx")     text+= fDX;
      else if (fOption=="globaly")     text+= fDY;
      else if (fOption=="globalz")     text+= fDZ;
      else if (fOption=="globalphix")  text+= fDphix;
      else if (fOption=="globalphiy")  text+= fDphiy;
      else if (fOption=="globalphiz")  text+= fDphiz;
      else                             text = "ERROR";
      txt = tpt->AddText(text);
      tpt->Draw();
    }
    return;
  };

 void Draw(int color){
    double x[5], y[5];
    // calculate DT corners based on DT dimensions
    // assume DT is centered at (0,0) and has NO rotation
    x[0] = -fW/2;    y[0] = -fH/2;
    x[1] =  fW/2;    y[1] = y[0];
    x[2] =  x[1];    y[2] = fH/2;
    x[3] =  x[0];    y[3] = y[2];
    x[4] =  x[0];    y[4] = y[0];  // same as x[0],y[0] to close polygon
    // now rotate the corners according to the DT rotation 
    // given by the Norm vector: fNormY=cos(); fNormX=sin()
    for(int i=0;i<5;i++){
      double tempX,tempY;
      tempX = x[i]; tempY = y[i];
      x[i] =  tempX*fNormY + tempY*fNormX;
      y[i] = -tempX*fNormX + tempY*fNormY;
    }
    // now translate chamber center to its nominal position 
    for(int i=0;i<5;i++){
      x[i] =  x[i]+fX;
      y[i] =  y[i]+fY;
    }

    // now set DTChamber PolyLine points using inherited method
    for(int i=0;i<5;i++){
      this->SetPoint(i,x[i],y[i]);
    }
    this->SetFillColor(color);
    TPolyLine::Draw("f");
  };
 ClassDef(DTChamber,1)  // Add this class to ROOT
};

//----------------------------------------------------------

class CSCChamber : public TCrown {
private:
  int     fStation,fRing,fChamber;
  double  fX,fY,fZ,fPhiZ,fLengthR,fWidthPhi;
  double  fDX,fDY,fDZ,fDphix,fDphiy,fDphiz;
  double  fDXL,fDYL,fDZL,fDphixL,fDphiyL,fDphizL;
  TString fOption;

public:
  CSCChamber(){};
  ~CSCChamber(){};
  CSCChamber(int Station,int Ring,int Chamber, 
 	     double X,double Y,double Z, double PhiZ){

    fStation  = Station;
    fRing     = Ring;
    fChamber  = Chamber;
    fX        = X;
    fY        = Y;
    fZ        = Z;
    fPhiZ     = PhiZ;
  };

  void SetOption(TString opt){fOption=opt;};
 
  void SetDimensions(double LengthR, double WidthPhi){
    fLengthR  = LengthR;
    fWidthPhi = WidthPhi;
  };

  void SetDeltaGlobal(double DX, double DY, double DZ, double Dphix, double Dphiy, double Dphiz){
    fDX=DX; fDY=DY; fDZ=DZ;
    fDphix=Dphix; fDphiy=Dphiy; fDphiz=Dphiz;
  };

  void SetDeltaLocal(double DXL,double DYL, double DZL, double DphixL, double DphiyL, double DphizL){
    fDXL=DXL; fDYL=DYL; fDZL=DZL;
    fDphixL=DphixL; fDphiyL=DphiyL; fDphizL=DphizL;
  };

  double GetDelta(){
    if (fOption=="localx")      return fDXL;
    if (fOption=="localy")      return fDYL;
    if (fOption=="localz")      return fDZL;
    if (fOption=="localphix")   return fDphixL;
    if (fOption=="localphiy")   return fDphiyL;
    if (fOption=="localphiz")   return fDphizL;
    if (fOption=="globalx")     return fDX;
    if (fOption=="globaly")     return fDY;
    if (fOption=="globalz")     return fDZ;
    if (fOption=="globalphix")  return fDphix;
    if (fOption=="globalphiy")  return fDphiy;
    if (fOption=="globalphiz")  return fDphiz;
    cout<<"Wrong option or GetDelta called before calling SetOption.  Must be one of:"<<endl;
    cout<<"localx, localy, localz, localphix, localphiy, localphiz"<<endl; 
    cout<<"globalx, globaly, globalz, globalphix, globalphiy, globalphiz"<<endl;
    return -9999.0; 
  };

  // override inherited method
  void ExecuteEvent(Int_t event, Int_t px, Int_t py){
    if (!gPad) return;
    if (!gPad->IsEditable()) return;
    if(event == 11) {
      TText *txt;
      double padx1,pady1,padx2,pady2;
      gPad->GetRange(padx1,pady1,padx2,pady2);
      TPaveText *tpt = new TPaveText(padx1+10,pady2-110,padx1+310,pady2-10);
      tpt->SetFillColor(38);
      TString text = "ME"; text+=fStation;
      text+="/"; text+=fRing;
      text+="/"; text+=fChamber;
      txt = tpt->AddText(text);

      text = "";
      if      (fOption=="localx")      text+= fDXL;
      else if (fOption=="localy")      text+= fDYL;
      else if (fOption=="localz")      text+= fDZL;
      else if (fOption=="localphix")   text+= fDphixL;
      else if (fOption=="localphiy")   text+= fDphiyL;
      else if (fOption=="localphiz")   text+= fDphizL;
      else if (fOption=="globalx")     text+= fDX;
      else if (fOption=="globaly")     text+= fDY;
      else if (fOption=="globalz")     text+= fDZ;
      else if (fOption=="globalphix")  text+= fDphix;
      else if (fOption=="globalphiy")  text+= fDphiy;
      else if (fOption=="globalphiz")  text+= fDphiz;
      else                             text = "ERROR";
      txt = tpt->AddText(text);
      tpt->Draw();
    }
    return;
  };

 void Draw(int color){
   double r,rMin,rMax,phiMin,phiMax,pi;
   // calculate CSC parameters based on CSC position and dimensions
   pi     = 3.141592654;   
   r      = sqrt(fX*fX+fY*fY);
   rMin   = r-fLengthR/2.0;
   rMax   = r+fLengthR/2.0;
   phiMin = fPhiZ - fWidthPhi/2.0; phiMin = phiMin*180.0/pi; phiMin+=1.0;  // leave space between CSCs  
   phiMax = fPhiZ + fWidthPhi/2.0; phiMax = phiMax*180.0/pi; phiMax-=1.0;  // for visualization
   
   // set CSCChamber TCrown parameters using inherited method
   this->SetX1(0); this ->SetY1(0);  // center is always at (0,0)
   this->SetR1(rMax);
   this->SetR2(rMin);
   this->SetPhimin(phiMin);
   this->SetPhimax(phiMax);
   this->SetFillColor(color);
   TCrown::Draw("f");
  };

 ClassDef(CSCChamber,1)  // Add this class to ROOT
};
