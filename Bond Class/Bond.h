/*
 * =====================================================================================
 *
 *       Filename:  Bond_Class.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/08/2018 22:15:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */

#pragma once
#include<vector>
#include<iostream>
#include<math.h>
/*
 * Question 1: The Fair value, duration and yield calculations
 * are call "const" methods. Explain Why
 * ans: This goes back to what was discussed in class 
 * about keeping the integrity of the original data
 * we don not want to change the original data, even if its "bad"
 * all we can do is throw error. Here the original data corresponds to the
 * private member data.
 */
using namespace std;
class Bond {
	private:
		//data
		double Face;		//Face Value
		double T_maturity;	//Time to maturity
		int cpn_freq;		//coupon frequency (semi = 2; quaterly = 4)
		int num_coupons;	//number of coupons
		vector<double> coupons;

	public:
		Bond(double T, double F, double c=0, int freq=2);
		~Bond(){
			coupons.clear();
			num_coupons=0;
		}

		//public methods
		int set_coupons(vector<double>& c);
		int FV_duration(double t0, double y,
				double &B,
				double &Macaulay_duration,
				double &modified_duration) const;
		int yield(double B_target, double tol, int max_iter, 
				double t0, double&y, int& num_iter) const;

		double FairValue(double t0, double y) const;
		double maturity() const {return T_maturity;}
};

		Bond::Bond(double T, double F, double c, int freq){
			Face = F;
			if(Face<0) {Face =0;}
			cpn_freq=freq;
			if(cpn_freq < 1) {cpn_freq=1;}
			const double tol= 1.0e-6;
			num_coupons=int(cpn_freq*T + tol);
			if(num_coupons< 0){num_coupons=0;}
			T_maturity = num_coupons / cpn_freq;
			if ( num_coupons >0) {coupons.resize(num_coupons,c);}
		}

		//variable free coupon bond
		int Bond::set_coupons(vector<double>& c){
			if(c.size() < num_coupons)
				return 1;
            coupons.clear();
			for( auto i =0 ; i <num_coupons ; i++){
				if(c[i] < 0 )
					coupons[i]= 0;
				else
					coupons[i]=c[i];
			}
			return 0;
		}

		double Bond::FairValue(double t0, double y) const{
			double B=0;
			double Macaulay_duration=0;
			double modified_duration=0;
			FV_duration(t0,y,B,Macaulay_duration,modified_duration);
			cout<<"Macaulay_duration: "<<Macaulay_duration<<endl;
			cout<<"modified_duration: "<<modified_duration<<endl;
			cout<<"Bond Price: "<<B<<endl;
			return B;
		}

		int Bond::FV_duration(double t0, double y,
				double &B,
				double &Macaulay_duration,
				double &modified_duration) const{
			
			B=Macaulay_duration=modified_duration=0;
			//validation test
			if( (num_coupons <= 0) || (t0>T_maturity))
				return 1;
			const double tol= 1.0e-6;
			//calculating the fair value of the bond
			y= .01 * y;
			double sum =0;
			double count=1;
			double tmp = (1+ (double)(y/cpn_freq));
			for (auto  i= 0 ; i <coupons.size() ; i++){
				double t_i= ((double)count/(double)cpn_freq);
				count++;
				double delta_t= t_i - (t0+tol);
				if ( delta_t<= 0)
					continue;
				double num= coupons[i]/cpn_freq;
			       	double den= pow(tmp,(cpn_freq*delta_t)); 	
				sum += (num/den);
				if(i==(coupons.size()-1)){
					sum += (Face/den);
				}
			}
			B= sum;
			/////////////////////////////////////////
			//calculating the Macauley Duration
			sum= 0;
			double mult_factor= 1/(double)B;
			count=1;
			for(auto i=0 ; i<coupons.size() ; i++){
				double t_i= ((double)count/(double)cpn_freq);
				count++;
				double delta_t= t_i - (t0+tol);
				if ( delta_t<= 0)
					continue;
				double num= ((double)coupons[i]/(double)cpn_freq);
				double den= pow(tmp, (cpn_freq*delta_t));
				if(i== (coupons.size()-1)){
					double num= (Face + (coupons[i]/cpn_freq)) * delta_t;
					sum += (num /den);
					break;
				}
				sum += (num/den)* delta_t;

			}
			Macaulay_duration= mult_factor*sum;
			///////////////////////////////////
			//modified duration
			modified_duration= (double)Macaulay_duration/tmp;
			return 0;
		}

	int Bond::yield(double B_target, double tol, int max_iter,double t0, double &y, int& num_iter)const{
		y=0;
		num_iter=0;
		//First validation Test
		if( (B_target <= 0.0) || (t0>=T_maturity) || (num_coupons<=0))
			return 1;

		double y_low =0;
		double y_high=100;
		double B_y_low= FairValue(t0,y_low);
		double diff_B_y_low= B_y_low - B_target;
		if(fabs(diff_B_y_low) <= tol){
			y=y_low;
			return 0;
		}
		double B_y_high= FairValue(t0,y_high);
		double diff_B_y_high= B_y_high-B_target;
		if(fabs(diff_B_y_high) <= tol){
			y=y_high;
			return 0;
		}
		//second validation test
		if((diff_B_y_low * diff_B_y_high) > 0){
			y=0;
			return 1;
		}

		//we begin the main Bisection iteration
		double B;
		for(num_iter=1; num_iter<max_iter; ++num_iter){
			y= (y_high + y_low)/2;
			B=FairValue(t0,y);

			double diff_b= B-B_target;
			if(fabs(diff_b) <= tol){
				return 0;
			}
			if( (diff_b* diff_B_y_low) > 0){
				y_low=y;
			}else{
				y_high=y;
			}
			if (fabs(y_high - y_low) <= tol){
				return 0;
			}
		}

		y=0;
		return 1;
	}
