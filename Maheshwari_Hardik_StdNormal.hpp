//Dynamic Delta Hedging
//Standard Normal CDF Implementation Class
// Calculates the CDF of normal distribution using Approximation

#ifndef StdNormal_hpp
#define StdNormal_hpp
#include <stdio.h>
#include "math.h"

#define pi 3.14159265358979323846264338327950288419716939937510


class StdNormalCDF{
private:
    double const a1 = 0.31938153;
	double const a2 = -0.356563782;
	double const a3 = 1.781477937;
	double const a4 = -1.821255978;
	double const a5 = 1.330274429;
    
    public :
    StdNormalCDF(){}
    double operator() (double) const;
    ~StdNormalCDF(){}
};


double StdNormalCDF :: operator()(double x)const{
    double l = fabs(x);
    double z = 1/(1+0.2316419*l);
    double r = a1*z+a2*pow(z,2)+a3*pow(z,3)+a4*pow(z,4)+a5*pow(z,5);
    double n = 1-1/(sqrt(2*pi))*exp(-pow(l,2)/2)*r;
    if(x >= 0)
        return n;
    else
        return (1-n);
}
#endif /* StdNormal_hpp */
