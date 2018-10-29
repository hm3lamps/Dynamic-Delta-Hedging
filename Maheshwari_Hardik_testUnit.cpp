//Dynamic Delta Hedging
//Testing the Functionality of implemented funtions
// precision check of 0.001 in double equals value

#include <stdio.h>
#include "math.h"
#include "Maheshwari_Hardik_StdNormal.hpp"
#include "Maheshwari_Hardik_blackscholes.hpp"
#include "Maheshwari_Hardik_impvol.hpp"

bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

void testStandardNormalFunctionality(){
    //test standard Normal 0, 1 ,-1 , some random case
	  StdNormalCDF N;
	  if(double_equals((round(N(0)*10)/10), 0.5)) {
	  	std::cout << "Passed N(0) should be 0.5" << endl;
	  }
	  else{
	  	std::cout << "Failed N(0) should be 0.5" << endl;
	  }

	  if(double_equals((round(N(100)*10)/10), 1.0)) {
	  	std::cout << "Passed N(100) should be 1.0" << endl;
	  }
	  else{
	  	std::cout << "Failed N(100) should be 1.0" << endl;
	  }
	  if(double_equals((round(N(-100)*10)/10) ,0.0)) {
	  	std::cout << "Passed N(-100) should be 0.0" << endl;
	  }
	  else{
	  	std::cout << "Failed N(-100) should be 0.0" << endl;
	  }
	  if(double_equals((round(N(0.8)*100)/100),0.79)){
	  	std::cout << "Passed N(0.8) should be 0.79" << endl;
	  }
	  else{
	  	std::cout << "Failed N(0.8) should be 0.79" << endl;
	  }
}

void testCallPriceFunctionality(){
    blackscholes bs = blackscholes(100, 0.01, 80, 0.3, 0.136, 0);
    // calculating for the same values from cboe calculator available online
    // gives call value of $20.2 and delta value of 0.998
    if(double_equals((round(bs.callValue() *10)/10),20.2)) {
        std::cout << "Passed Call Price Test" << endl;
     }
      else{
        std::cout << "Failed Call Price Test" << endl;
     }

    if(double_equals((round(bs.deltaCall() *100)/100), 0.98)) {
        std::cout << "Passed Call Delta Value Test" << endl;
    }
    else{
        std::cout << "Failed Call Delta Value Test" << endl;
     }
    // when stock price is way more then strike price, the delta of call option approaches 1
    bs = blackscholes(100000, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.deltaCall(), 1.0)) {
        std::cout << "Passed Call Delta Value Test S>>K should be 1" << endl;
    }
    else{
        std::cout << "Failed Call Delta Value Test S>>K should be 1" << endl;
     }

     // when stock price is way less then strike price, the delta of call option approaches 0
    bs = blackscholes(1, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.deltaCall(), 0.0)) {
        std::cout << "Passed Call Delta Value Test S<<K  should be 0" << endl;
    }
    else{
        std::cout << "Failed Call Delta Value Test S<<K  should be 0" << endl;
     }

    // when stock price goes to 0, price of call option approaches 0
    bs = blackscholes(0, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.callValue(), 0.0)) {
        std::cout << "Passed Call Delta Value Test S==0  should be 0" << endl;
    }
    else{
        std::cout << "Failed Call Delta Value Test S==0  should be 0" << endl;
    }
     // when stock price is way more then strike price, the delta of put option approaches 0
    bs = blackscholes(100000, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.deltaPut(), 0.0)) {
        std::cout << "Passed Put Delta Value Test S>>K should be 0" << endl;
    }
    else{
        std::cout << "Failed Put Delta Value Test S>>K should be 0" << endl;
     }

     // when stock price is way less then strike price, the delta of put option approaches -1
    bs = blackscholes(1, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.deltaPut(), -1.0)) {
        std::cout << "Passed Put Delta Value Test S<<K  should be -1" << endl;
    }
    else{
        std::cout << "Failed Put Delta Value Test S<<K  should be -1" << endl;
     }

    // when stock price goes to 0, price of put option approaches 11
    bs = blackscholes(0, 0.01, 80, 0.3, 0.136, 0);
    if(double_equals(bs.deltaPut(), -1.0)) {
        std::cout << "Passed Put Delta Value Test S==0  should be -1" << endl;
    }
    else{
        std::cout << "Failed Put Delta Value Test S==0  should be -1" << endl;
    }

}

void testCallImpliedVolFuntionality(){
    std::cout << "Stock Price=100, Strike Price=80, volatility=0.3, Time to Maturity=0.136, Riskfree Rate=1%" <<endl;
    std::cout << "Call Value 20.18 and Put Value of 0.077516 "<< endl;
	impvol v = impvol();
	double vol1 = v.findRootBisection(20.1862 ,100, 0.01, 80, 0.136, 0, "C");
	double vol2 = v.findRootBisection(0.077516 ,100, 0.01, 80, 0.136, 0, "P");
	std::cout << "With corresponding value of Call and Put, the two should give nearly same volatility which we used as 0.3" << endl;
    std::cout << "using Call the Bisection Method gives the volatility as:" << vol1 << endl;
	std::cout << "using Put the Bisection Method gives the volatility as:" << vol2 << endl;
}
int testing_func(){

    std::cout <<"TestCases for the Standard Normal Functionality"<< endl;
    testStandardNormalFunctionality();
    std::cout <<"Test cases done for the Standard Normal Functionality"<<endl;
    std::cout << "**********" << endl;

    std::cout <<"TestCases for the Call Price and Delta Functionality"<< endl;
    testCallPriceFunctionality();
    std::cout <<"Test cases done for the Call Price and Delta Functionality"<<endl;
    std::cout << "**********" << endl;

    std::cout <<"TestCases for the Implied Volatility Functionality"<< endl;
    testCallImpliedVolFuntionality();
    std::cout <<"Test cases done for the Implied Volatility Functionality"<<endl;
    std::cout << "**********" << endl;


    return 0;
}
