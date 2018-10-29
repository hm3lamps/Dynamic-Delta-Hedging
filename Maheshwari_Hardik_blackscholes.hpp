//Dynamic Delta Hedging
//Black Scholes Implementation Class
// calculates call value, delta call, first derivative of the call option
// calculates put value, delta put, first derivative of the put option


#ifndef blackscholes_hpp
#define blackscholes_hpp

#include <stdio.h>
#include <iostream>
#include "math.h"
#include "Maheshwari_Hardik_StdNormal.hpp"
using namespace std;

class blackscholes{
	private:
		double stock;
		double riskFree;
		double strike;
		double volatility;
		double timetomaturity;
		double dividend;

	public:
		blackscholes();
		blackscholes(double s, double r, double k, double v, double t, double d);

		double callValue();
		double deltaCall();

		double putValue();
		double deltaPut();

		double firstDer();

		~blackscholes();
};

//black scholes constructor
blackscholes :: blackscholes(){
}

//black scholes parametrized constructor
blackscholes :: blackscholes(double s, double r, double k, double v, double t, double d){
		stock = s;
		riskFree = r;
		strike = k;
		volatility = v;
		timetomaturity = t;
		dividend = d;
}

// black scholes call option value calculation
double blackscholes :: callValue()
{
    double d1=((log(stock/strike)+(riskFree-dividend+0.5*pow(volatility,2))*timetomaturity))/(volatility*sqrt(timetomaturity));
    double d2= d1-volatility*sqrt(timetomaturity);
    StdNormalCDF N;
    double call=stock*exp(-dividend*timetomaturity)*N(d1)-strike*exp(-riskFree*timetomaturity)*N(d2);
    return call;
}



// black scholes greek calculation delta Value
double blackscholes :: deltaCall()
{
 StdNormalCDF N;
 double d1 = ((log(stock/strike)+(riskFree-dividend+0.5*pow(volatility,2))*timetomaturity))/(volatility*sqrt(timetomaturity));
 double deltacall = exp(-dividend*timetomaturity)*N(d1);
 return deltacall;
}


double blackscholes :: putValue()
{
    double d1 = ((log(stock/strike)+(riskFree-dividend+0.5*pow(volatility,2))*timetomaturity))/(volatility*sqrt(timetomaturity));
    double d2 = d1-volatility*sqrt(timetomaturity);
    StdNormalCDF N;
    double put = strike*exp(-riskFree*timetomaturity)*N(-d2) - stock*exp(-dividend*timetomaturity)*N(-d1);
    return put;
}

// black scholes greek calculation delta Value
double blackscholes :: deltaPut()
{
	StdNormalCDF N;
	double d1 = ((log(stock/strike)+(riskFree-dividend+0.5*pow(volatility,2))*timetomaturity))/(volatility*sqrt(timetomaturity));
	double deltaput = exp(-dividend*timetomaturity)*(N(d1) -1);
	return deltaput;
}

// black scholes first derivative (used for Newton Raphson Method)
double blackscholes :: firstDer()
{
	StdNormalCDF N;
    double d1 = ((log(stock/strike)+(riskFree-dividend+0.5*pow(volatility,2))*timetomaturity))/(volatility*sqrt(timetomaturity));
    double d2 = d1-volatility*sqrt(timetomaturity);
    double normaFuncDer = 0.39894244888 * exp(-0.5 * pow(d1,2));
    double value =  stock * sqrt(timetomaturity) * normaFuncDer * exp(-dividend * timetomaturity);
    return value;
}

// black scholes destructor
blackscholes ::  ~blackscholes(){
}
#endif

