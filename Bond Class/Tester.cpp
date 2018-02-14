/*
 * =====================================================================================
 *
 *       Filename:  Bond_Class_Tester.cpp
 *
 *    Description:  Testing the Bond Class
 *
 *        Version:  1.0
 *        Created:  02/09/2018 09:41:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */

#include "23433931_Bryan_Lliguicota_CS365_HW2.h"
#include<iostream>
#include<math.h>
#include<iomanip>
using namespace std;
void check_HW1();
void check_lec2_data();
int main(){
	/*  
	cout<<"Testing the Bond class: "<<endl;
	cout<<"From work examples in lecture 2 section 2.8"<<endl;
	Bond b1(2,100,0,2);
	vector<double> coupons={4.1,4.2,4.3,4.3};
	b1.set_coupons(coupons);
	cout<<"let t0= 0.0 and the yeild be 6%"<<endl;
	cout<<"Bond Price: "<<b1.FairValue(0,6)<<endl;
        cout<<"let t0= 0.1 and the yeild be 6%"<<endl;
        cout<<"Bond Price: "<<b1.FairValue(0.1,6)<<endl;	
        cout<<"let t0= 0.55 and the yeild be 6%"<<endl;
	cout<<"Bond Price: "<<b1.FairValue(0.55,6)<<endl;
	
	cout<<"testing the yeild method"<<endl;
	Bond b2(2,100,4,2);
	double b2_yeild;
	int num_iter;
	double tol= 1.0e-4;
	cout<<b2.yield(100,tol, 100,0,b2_yeild,num_iter)<<endl;
	cout<<"Yield: "<<b2_yeild<<endl;


	Bond b4 (5,100,28,7);
	double b4_y;
	cout<<"Bond Price: "<<b4.FairValue(.55,6)<<endl;
	cout<<b4.yield(189.998, tol,100,.55,b4_y,num_iter)<<endl;
	cout<<b4_y<<endl;
	*/
	//check_HW1();
	check_lec2_data();
}
void check_lec2_data(){
	double F=100;
	vector<double>coupons= {4.1,4.2,4.3,4.4};
	cout<<coupons[-1]<<endl;
	double y=6;
	double T=2;
	vector<double> time= {0.0,0.1,0.55};
	double FV= 0;
	double freq= 2;
	Bond B(T,F,4,freq);
	B.set_coupons(coupons);
	cout<<"testing:"<<endl;
	for(auto t0 : time){
		cout<<setw(18)<<"t0: "<<t0<<endl;
		FV=B.FairValue(t0,y);
	}

}
void check_HW1(){
	//1.3 
	const double T=2;
	const double F=100;
	const int freq= 2;
	double c=4;
	double t0= 0;
	double y=0;
	double FV=0;

	Bond B(T,F,c,freq);
	cout<<"Flat Coupon:"<<endl;
	for(y=0 ; y<=8.0001; y+=2){
		FV=B.FairValue(t0,y);
		cout<<"Yeild: "<<setw(16)<<y<<" Bond Price: "<<setw(16)<<FV<<endl;
	}

	cout<<"Variable Coupons:"<<endl;
	vector <double> Coupons= {1,3,5,7,9};
	B.set_coupons(Coupons);
	for(y=1; y<=9.0001; y+=2){
		FV=B.FairValue(t0,y);
		cout<<"Yeild: "<<setw(16)<<y<<" Bond Price: "<<setw(16)<<FV<<endl;
	}

	
}
