#include "Simulator.C"
#include <string>
#include <stdlib.h>

//To compile: g++ -o CallSimulator mt2_bisect.cpp CallSimulator.C `root-config --cflags --libs`


int main(int argc, char *argv[]) {

  if (argc != 5) {
      std::cout << "Usage: ./CallSimulator outputFileName.root topmass mediatormass numberOfEvents" << std::endl;
      return 1;
  }

  std::string outputname(argv[1]);
  float topmass = atof(argv[2]);
  float mediatormass = atof(argv[3]);
  int nevents = atoi(argv[4]);


  Simulator *a = new Simulator(nevents, topmass, mediatormass, outputname);

  delete a;

  return 0;


}

