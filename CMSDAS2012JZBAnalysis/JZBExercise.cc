#include "GeneralTools.cc"
#include "Exercise1.cc"
#include "Exercise2.cc"
#include "Exercise3.cc"
#include "Exercise4.cc"
#include "Exercise5.cc"
#include "Exercise6.cc"


//------------------------Methods-------------------------
void runJZBAnalysis(int);
//--------------------------------------------------------



//------------------Method implementation-----------------

void runJZBAnalysis(int exercise) {

  showMessage("Starting Jet-Z Balance Analysis ...");

  setTDRStyle();

  loadSamples();

  switch(exercise) {
     case 1:
      exercise1();
      break;
    case 2:
      exercise2();
      break;
    case 3:
      exercise3();
      break;
    case 4:
      exercise4();
      break;
    case 5:
      exercise5();
      break;
    case 6:
      exercise6();
      break;
    default:
      exercise1();
      exercise2();
      exercise3();
      exercise4();
      break;
  }

  end();

}



