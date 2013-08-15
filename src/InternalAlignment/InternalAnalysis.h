#include "TrackForAlignment.cc"
#include <fstream>
#include <TH1F.h>

using namespace std;


//**********************************************************//
//****** Runs alignment with tracks algorithm **************//
//****** including constrains ******************************//
//**********************************************************//
class InternalAnalysis {
 public:
  //**** Constructor
  InternalAnalysis(int, int, int);
  //**** Destructor
  ~InternalAnalysis();
  //**** 
  void begin();
  //**** 
  void end();

  //*** Writes out the solution
  void PrintSolution();

  
 private:
  
  //**** Coordinates of the chamber
  int wheel, station, sector;
  //**** The number of muons in this chamber
  int numberOfMuons; 
  //**** Matrix for Alignment with Tracks
  TMatrixD C, b;
  //**** Matrix for SL survey constrain
  TMatrixD CPH, bPH;
  //**** Matrix for Layer survey constrain
  TMatrixD CPHL, bPHL;
  //**** Matrix for global constrain
  TMatrixD CConstrain, bConstrain;
  //**** Total matrizes
  TMatrixD CTotal, bTotal;
  //**** Solution
  TMatrixD Solution;

  void writeTracks();
  //**** Histograms
  TH1F *hist;
  
  //**** The text file where the tracks are read from
  ifstream textInfo;
  
  //**** Parameters for the alignment algorithm
  int NDOFTrack;
  int NDOFAlign;
  int NDOFChamber;
  int NDOFCoor;

  //**** Adds SL survey constrain (ISR)
  void PHSuperLayer();
  //**** Adds layer survey constrain (QC)
  void PHLayer();
  //**** Adds global constrain
  void AddConstrain();

  //**** Invert matrix and calculate solution
  void Calculate();

  //**** Compares both kind of data
  void MakeComparison();
  
  //**** Compares both kind of data
  void MakeComparisonSurvey();
  
  //**** Reads muon from text file
  bool readMuon();
  
  //**** Reads MB4 muon from text file
  bool readMuonMB4();


};






