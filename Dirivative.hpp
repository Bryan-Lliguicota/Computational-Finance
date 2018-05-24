/*
 * =====================================================================================
 *
 *       Filename:  Database.cpp
 *
 *    Description: Database class with be used to store indicative data
 *                  Indicitive data - these are paramteres that define the
 *                  characteristics of a dirivative(In our case an option
 *
 *        Version:  1.0
 *        Created:  05/03/2018 20:09:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */
#include<fstream>
#include<iomanip>
#include<math.h>
#include<iostream>
using namespace std;

class Database{

    public:
        //techniqually interest rates change everyday and are really market day
        //we wouldhave to create a YeildCurve class and interpolate it
        double r;
        double q;
        Database() {r=0; q=0;}
        ~Database(){}

};

class TreeNode{
    public:
        double S;
        double V;
        double t;

        TreeNode() {S=0; V=0; t=0;}
        ~TreeNode(){}
};

class Derivative{
    protected:
        Derivative(){T=0;}// the constructor is protected b/c we do not want it
        //to be instatiated directly
    public:
        double T;
        virtual double TerminalPayoff(double S)const {return 0;}
        virtual int ValidationTests(TreeNode& node)const {return 0;}
        virtual~ Derivative(){}
};
double max(double x, double v){
    if(x>v)
        return x;
    return v;
}
