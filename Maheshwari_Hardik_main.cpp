//mid term project Design Course
//Dynamic Delta Hedging

#include <iostream>
#include <string>
#include "vector"
#include "math.h"
#include "time.h"
#include <fstream>
#include <string>
#include <sstream>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/date_time.hpp>
#include "boost/algorithm/string.hpp"
#include "Maheshwari_Hardik_blackscholes.hpp"
#include "Maheshwari_Hardik_impvol.hpp"
#include "Maheshwari_Hardik_testUnit.cpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::algorithm;

int convStrIntDate(string str){
    if( contains(str, "/")){
        int start = str.find_first_of("/");
        int endI = str.find_last_of("/");
        string day = str.substr(0, start);
        if(day.length() ==1 ){
            day = std::to_string(0) + day;
        }
        string month = str.substr(start+1, endI-start-1);
        if(month.length() ==1 ){
            month = std::to_string(0) + month;
        }
        string year = str.substr(endI+1, str.length());
        return atoi((year+month+day).c_str());
    }
    else{
        string year = str.substr(0,4);            //getting t0 in int format for efficiency comparison
        string month= str.substr(5,2);
        string day=str.substr(8,2);
        return atoi((year+month+day).c_str());
    }
}

bool checkIfFormat(string t0, string t1, string T){
    if((t0.length() == 10) && (t1.length()==10) && (T.length()== 10)) return true;
    else return false;
}
//int findDistance(int searchParam, vector<int> date, int expiry){
//    std::vector<int>::iterator it1;
//    std::vector<int>::iterator it2;
//    it1 = std::find (date.begin(), date.end(), searchParam);
//    it2 = std::upper_bound(date.begin(), date.end(), expiry);
//    std::cout << expiry << " " << *it2  << " " << *it1 <<endl;
//    std::cout << distance(it1,it2) <<endl;
//    return distance(it1,it2);
//}
int dateDifference( string start_date, string end_date )
{
    date d1(from_undelimited_string(start_date));
    date d2(from_undelimited_string(end_date));
    // counter for weekdays
    int cnt=0;
    for(day_iterator iter = d1; iter!=d2; ++iter)
    {
        // increment counter if it's no saturday and no sunday
        if(    iter->day_of_week() !=  boost::date_time::Saturday
            && iter->day_of_week() !=  boost::date_time::Sunday)
            ++cnt;
    }
    return cnt;
}
int main(){
    std::cout << "Calculations for Part1 of the Assignment" <<endl;
	//part one stock simulation and option price calculation;
    vector <double> simulation;
    vector <double> callprice;
    vector <double> deltaCall;
    vector<double> bVal;
    vector<double> hedgerror;
    vector<double> moneyInvested;
    vector<double> testportfolio;

    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
    generator(boost::mt19937(time(0)), boost::normal_distribution<>()); //  boost to get  random normal distribution variable

    int periods = 100;
	double stock =  100;
	double strike = 105;
	double timetomaturity = 0.4;
	double meanVal = 0.4;
	double volatility = 0.24;
	double riskFree = 0.025;
	double dividend = 0.0;
	double deltaSmallT = timetomaturity/periods;

    simulation.push_back (stock);                  //initial stock price vector in the array
    blackscholes bs = blackscholes(stock, riskFree, strike, volatility, timetomaturity, dividend);
    callprice.push_back(bs.callValue());           // Call Price Initial
    deltaCall.push_back( bs.deltaCall());            // Delta of the Call Value
    bVal.push_back(callprice[0] - deltaCall[0] *stock);				//bvalue for the hedging error equation
    hedgerror.push_back (0);                                 // first value of hedge error is 0

	moneyInvested.push_back  ( deltaCall[0]*simulation[0] );        //amount paid to buy stocks
    testportfolio.push_back ( callprice[0]-moneyInvested[0] );       //cash from shorting call - number of stocks bought to hedge*stock price

 	ofstream hedgetest ("./results/hedgetesting.csv");
    hedgetest<<" Testing the delta hedging implementation"<<endl;
    hedgetest<<"Simulated stock price"<<","<<"Option price"<<","<<"Delta"<<","<<"Hedging error"<<","<<"Stock purchase"<<","<<"Borrowed amount"<<endl;
 	for  (int i=0; i<periods-1; i++)
    {
        double randomVar = generator();
        simulation.push_back( simulation[i]   * (1 + meanVal*deltaSmallT + volatility * sqrt(deltaSmallT) * randomVar));
        blackscholes bs = blackscholes(simulation[i], riskFree, strike, volatility, timetomaturity-(i+1)*deltaSmallT, dividend);
		callprice.push_back( bs.callValue());
        deltaCall.push_back( bs.deltaCall());
        bVal.push_back(deltaCall[i] * simulation[i+1] + bVal[i] * exp (riskFree * deltaSmallT) - deltaCall[i+1] * simulation[i+1]);
        hedgerror.push_back(deltaCall[i]  * simulation[i+1] + bVal[i] * exp( riskFree * deltaSmallT ) - callprice[i+1]);
        moneyInvested.push_back( deltaCall[i+1] * simulation[i+1] );
        testportfolio.push_back(callprice[i+1]-moneyInvested[i+1]);
    }
     for (int i =0; i<periods;i++)
    {
        hedgetest<<simulation[i]<<","<<callprice[i]<<","<<deltaCall[i]<<","<<hedgerror[i]<<","<<moneyInvested[i]<<","<<testportfolio[i]<<endl;
    }
    hedgetest.close();
    std::cout << "Processed Part1 of the Assignment. Excel is in results folder under name hedgetesting.csv"<<endl;
    std::cout << "******************************************************" << endl;
    std::cout << "Starting Part 2nd of the Assignment: " << endl;

    string startdate;
    string enddate;
    string maturity;
    string typeOfOption;
    double strikePrice;

    // make a check for valid date ;
    std::cout <<"Enter the Type of Option Call-C Put-P Capital Case" << endl;
    std::cin >> typeOfOption;

    if( typeOfOption == "C" || typeOfOption == "P"){
          bool check = false;
          int i = 0;
          do{
                std::cout << "Enter the dates in following format yyyy-mm-dd format without space:" << endl;
                std::cout << "Enter the start date:" << endl;
                std::cin >> startdate;
                std::cout << "Enter the end date:" << endl;
                std::cin >> enddate;
                std::cout << "Enter the maturity:" << endl;
                std::cin >> maturity;
                check = checkIfFormat(startdate, enddate, maturity);
                if(check) {
                    i = 2;
                }
                else{
                    std::cout << "Format Entered Wrong! Please enter again" << endl;
                    i = 1;
                }
          }while(i != 2);

        std::cout << "Enter the strike price of the option:" << endl;
        std::cin >> strikePrice;
          //startdate = "2011-07-05";
          //enddate = "2011-07-29";
          //maturity = "2011-09-17";
          //strikePrice = 500;

          int t0 = convStrIntDate(startdate);
          int t1 = convStrIntDate(enddate);
          int T = convStrIntDate(maturity);

          if(t0 >= t1){
                std::cout << "Start Date cannot be greater than the End date:" <<endl;
                return 0;
          }
          else if( t1 >= T) {
                std::cout << "The Expiry date is greater than the hedging period end date. Results will be till Maturity of Option" << endl;
          }
          //read interest rate data
          ifstream interest("./data/interest.csv");
          vector<int> date;
          vector<double> intrate;
          vector<double> sPrice;

          if(interest){
            string data;
            while (getline(interest, data)){
                if(!isalpha(data[0])){
                    int index = data.find_first_of(",");
                    int datecheck  = convStrIntDate(data.substr (0,index));
                    if(datecheck >= t0 && datecheck <= t1){
                        date.push_back(datecheck);
                        intrate.push_back(std::stod(data.substr (index+1,data.length())));
                    }
                }
            }
          }
          interest.close();

          //read security prices data
          ifstream closePrice("./data/sec_GOOG.csv");
           if(closePrice){
            string data;
            while (getline(closePrice, data)){
                if(!isalpha(data[0])){
                    int index = data.find_first_of(",");
                    int datecheck  = convStrIntDate(data.substr (0,index));
                    if(datecheck >= t0 && datecheck <= t1){
                        sPrice.push_back(std::stod(data.substr (index+1,data.length())));
                    }
                }
            }
          }
          closePrice.close();
        //read option related data
        ifstream option("./data/op_GOOG.csv");
        vector<int> datepurchase;
        vector<double> optionPrice;

        double  double4, double5, double6;
        string cell1, cell2, cell3, cell4, cell5, cell6;
        while(getline(option,cell1,','))
        {
            getline(option, cell2,',');
            getline(option, cell3,',');
            getline(option, cell4,',');
            getline(option, cell5,',');
            getline(option, cell6);
            if(!isalpha(cell1[0])){

                int val = convStrIntDate(cell1);
                istringstream todouble3(cell4);
                todouble3>>double4;

                // retrieving filtered option data
                if( val>=t0 &&  val<= t1 &&  convStrIntDate(cell2)== T && cell3 == typeOfOption && double4==strikePrice){
               // std::cout << cell1 << " " << typeOfOption << " " <<double4 << " " << strikePrice <<endl;
                datepurchase.push_back(convStrIntDate(cell1));
                istringstream todouble1(cell5);
                todouble1>>double5;
                istringstream todouble2(cell6);
                todouble2>>double6;
                optionPrice.push_back((double5+double6)/2);
                }
            }
        }
        option.close();
        //std::cout << optionPrice.size();
        if(optionPrice.size() != 0){
            double dt = 0.00396825396; //1/252
            double cent = 0.01; //1/100
            vector<double> impliedVolatility;
            vector<double> deltaOption;

            // calculating implied volatility and using the volatility to calculate delta of the option
            for(int i = 0; i < optionPrice.size(); i++){
                    string startVal = std::to_string(datepurchase[i]);
                    string mat = std::to_string(T);
                    int businessdays = dateDifference(startVal, mat);
                    double timeMat = businessdays * dt;
                    impvol v = impvol();
                   // double vol = v.findRootNewton(optionPrice[i] ,sPrice[i], intrate[i]*cent, strikePrice, timeMat, 0.0, typeOfOption);
                    double vol = v.findRootBisection(optionPrice[i] ,sPrice[i], intrate[i]*cent, strikePrice, timeMat, 0.0, typeOfOption);
                   // std::cout << businessdays << "****" <<endl;
                    impliedVolatility.push_back(vol);
                    bs = blackscholes(sPrice[i], intrate[i]*cent, strikePrice, vol, timeMat, 0);
                    //std::cout <<optionPrice[i] <<" "<< sPrice[i] << " "<<intrate[i] << " "<< timeMat << " " << optionPrice[i] << " delta price: " << bs.callValue() << " " << vol <<endl;
                    if(typeOfOption == "C"){
                        deltaOption.push_back(bs.deltaCall());
                    }
                    else{
                        deltaOption.push_back(bs.deltaPut());
                    }

            }
            vector<double> portfoliobeta;
            vector<double> hedgingerror;
            vector<double> profitLoss;
            portfoliobeta.push_back(optionPrice[0]-deltaOption[0]*sPrice[0]);
            hedgingerror.push_back(0);
            profitLoss.push_back(0);

            for (int i=1; i<optionPrice.size();i++)
            {
                portfoliobeta.push_back(deltaOption[i-1]   *   sPrice[i]   +  portfoliobeta[i-1]  * exp(intrate[i-1] *0.01 *dt)   - deltaOption[i] * sPrice[i]);
                hedgingerror.push_back(deltaOption[i-1]  * sPrice[i]   +   portfoliobeta[i-1] *  exp(intrate[i-1]*0.01 *dt)  - optionPrice[i]);
                profitLoss.push_back(optionPrice[i]-optionPrice[0]);
            }
          // creating my result file
            ofstream result ("./results/finalTable.csv");
            result<<"Results"<<endl;            //creating headers in file
            result<<"Start date ,"<< startdate <<endl;
            result<<"End date ,"<< enddate <<endl;
            result<<"Option Type ,"<< typeOfOption <<endl;
            result<<"Expiry date ,"<< maturity <<endl;
            result<<"Strike price ,"<<strikePrice<<endl;
            result<<endl;
            result<<"\nDate"<<","<<"Stock Price"<<","<<"Option Price"<<","<<"Implied Volatility"<<","<<"Delta"<<","<<"Hedging error"<<","<<"Cumulative hedging error"<<","<<"PNL"<<endl;

            // dumping data in final result csv
            double cumulativehedging=0.0;
            for (int  i=0; i<optionPrice.size();i++)
            {
                cumulativehedging+=hedgingerror[i];
                result<<date[i]<<","<<sPrice[i]<<","<<optionPrice[i]<<","<<impliedVolatility[i]<<","<<deltaOption[i]<<","<<hedgingerror[i]<<","<<cumulativehedging<<","<<profitLoss[i]<<endl;
            }
            result.close();

            std::cout << "Processed Part2 of the Assignment. Excel is in results folder under name finalTable.csv"<<endl;
            std::cout << "*************************END*****************************" << endl;
        }

        else{
            std::cout<< "No Matching Expiry Date / Strike price found in our data" << endl;
        }
   }
    else{
        std::cout <<"We only support the option type Call=C and Put=P" << endl;
    }


//    //code to run the testcases
//    std::cout << "******Testing Start********" << endl;
//    std::cout << "Test Cases for the Functionality Check" << endl;
//    testing_func();
//    std::cout << "******Testing Done********" << endl;

return 0;
}
