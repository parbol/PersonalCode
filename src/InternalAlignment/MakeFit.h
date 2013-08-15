

class MakeFit {

 public:
  MakeFit(double *, double *, int);
  ~MakeFit();
  double slope();
  double intercept();

 private:
  double *x;
  double *y;
  double inter;
  double slop;
  int n;

};



  
