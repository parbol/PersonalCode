#include "MakeFit.h"


MakeFit::MakeFit(double *mx, double *my, int mn) {
  
  double mean_x=0.0, mean_y=0.0, mean_xx=0.0, mean_xy=0.0;
  
  x = new double[mn];
  y = new double[mn];
  
  for(int counter = 0; counter < mn; counter++) {
    x[counter] = mx[counter];
    y[counter] = my[counter];
    mean_x += x[counter];
    mean_y += y[counter];
    mean_xx += x[counter]*x[counter];
    mean_xy += x[counter]*y[counter];
  }
  slop = -((mean_x/mn)*(mean_y/mn)-mean_xy/mn)/(mean_xx/mn-(mean_x/mn)*(mean_x/mn));
  inter = (mean_y/mn)-slop*(mean_x/mn);
  
}

MakeFit::~MakeFit() {
  delete [] x;
  delete [] y;
}

double MakeFit::slope() {return slop;}
double MakeFit::intercept() {return inter;}

