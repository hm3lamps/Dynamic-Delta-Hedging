//Dynamic Delta Hedging
//Implied Volatility Implementation Class
// Calculates the implied volatility using Bisection Method
// Alternate Newton method is commented but can be used as an alternate for option (Not gauranteed to give the convergence)

#ifndef impvol_hpp
#define impvol_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include "math.h"
#include "Maheshwari_Hardik_StdNormal.hpp"
#include "Maheshwari_Hardik_blackscholes.hpp"

using namespace std;

class impvol{
	private:
		double volatility;

	public:
		impvol();
		//double findRootNewton(double c, double s, double r, double k, double t, double q, string opt);
		double findRootBisection(double v, double s, double r, double k, double t, double q, string opt);
		double funcDiff(double v, double s, double r, double k, double t, double q, string opt, double guess);
		~impvol();

};

impvol :: impvol(){
	volatility = 0.0;
}
//double impvol :: findRootNewton(double c, double s, double r, double k, double t, double q, string opt){
//	double tolerance = .0001;
//	double initialGuess = 0.4;
//	volatility = 0.0;
//	blackscholes bs = blackscholes(s, r, k, initialGuess, t, q);
//	//std::cout << "imp vol" <<endl;
//	double optval = 0.0;
//	if(opt == "C") {
//		optval = bs.callValue();
//	//	std::cout << optval << " "<<r <<endl;
//	}
//	else{
//		optval = bs.putValue();
//	//	std::cout << optval << " "<<r <<endl;
//	}
//	double diff = c- optval;
//	double firstDer = bs.firstDer();
//	double ratio = diff/firstDer;
//	volatility = initialGuess - ratio;
////	std::cout << "first der " << firstDer << " "<< ratio << " diff " << volatility << endl;
// 	while( (abs(volatility-initialGuess) >tolerance) && (abs(diff)> tolerance)){
//		initialGuess = volatility;
//		blackscholes bs = blackscholes(s, r, k, initialGuess, t, q);
//		if(opt == "C") {
//			optval = bs.callValue();
//		}
//		else{
//			optval = bs.putValue();
//		}
//    	diff = c-optval;
//    	firstDer = bs.firstDer();
//		ratio = diff/firstDer;
//		volatility = initialGuess - ratio;
//	}
//	return volatility;
//}

impvol :: ~impvol(){
}
double impvol :: funcDiff(double v, double s, double r, double k, double t, double q, string opt, double guess){
	blackscholes bs = blackscholes(s, r, k, guess, t, q);
	double optval = 0.0;
	if(opt == "C") {
		optval = bs.callValue();
	}
	else{
		optval = bs.putValue();
	}
	return v-optval;
}
double impvol :: findRootBisection(double v, double s, double r, double k, double t, double q, string opt){
	double epsilon = 0.0001;
	double lowerLimit = 0.0001;
	double upperLimit = 1.0;
	double avgVal = (upperLimit+lowerLimit)/2.0;

	if (funcDiff(v,s,r,k,t,q, opt, lowerLimit)<0.0){
         avgVal = lowerLimit/2.0;
      }
   // std::cout <<"hehy" << funcDiff(v,s,r,k,t,q,opt, upperLimit)<< endl;
    while (funcDiff(v,s,r,k,t,q,opt, upperLimit)>=0.0){
         lowerLimit = 1.0;
         upperLimit = 2.0*upperLimit;
    }
    
    int i = 0;
   while ((upperLimit-lowerLimit)>epsilon && i <=100){
   		 i++;
         if ((funcDiff(v,s, r,k,t,q, opt, upperLimit) *funcDiff(v,s, r,k,t,q, opt, avgVal))>0.0){
            upperLimit = avgVal;
         }
         else if ((funcDiff(v,s, r,k,t,q,opt, lowerLimit)* funcDiff(v,s, r,k,t,q,opt, avgVal))>0.0){
            lowerLimit = avgVal;
         }
         avgVal=(upperLimit+lowerLimit)/2.0;
      }

	return avgVal;
}



#endif
