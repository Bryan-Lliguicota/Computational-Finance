/*
 * =====================================================================================
 *
 *       Filename:  tester.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/24/2018 16:37:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */

#include "BinomialTree.hpp"

int impliedVol_test(){
   freopen("ImpliedVolTest.txt","w",stdout);
    double t0 = 0;
    double q = .02;
    double T = 2;
    double r = 0.05;

    Database db;
    db.r = r;
    db.q = q;

    double S = 100;
    double K = 100;

    double sigma = 0.5;
    Option Eur_put;
    Eur_put.K = K;
    Eur_put.isCall = false;
    Eur_put.isAmerican = false;
    Eur_put.T = T;
    int n = 100;
    double Eur_put_FV = 0.0;
    BinomialTree bin(n);
    bin.FairValue(n,&Eur_put,&db,S,sigma,t0,Eur_put_FV);
    cout<<"Fair Value of this European put is: "<<Eur_put_FV<<endl;

    Option Eur_call;
    Eur_call.K = K;
    Eur_call.isCall = true;
    Eur_call.isAmerican = true;
    Eur_call.T = T;
    int num_iter;
    double i_v;
    bin.ImpliedVolatility(n,&Eur_call,&db,S,t0,28.59,i_v,num_iter);
    cout<<"The implied vol for the call option is: "<<i_v<<endl;

    return 0;
}
int binomial_test(){
    int rc =0;
    freopen("output.txt","w",stdout);
    cout.precision(6);
    double r = 0.05;
    double q = 0.02;
    double T = 1.0;
    double t0 = 0;

    Database db;
    db.r=r;
    db.q=q;

    double S = 100;
    double K = 100;
    double sigma = 0.5;

    Option Eur_put;
    Eur_put.K = K;
    Eur_put.isCall = false;
    Eur_put.isAmerican = false;
    Eur_put.T = T;

       Option Am_put;
       Am_put.K = K;
       Am_put.T = T;
       Am_put.isCall = false;
       Am_put.isAmerican = true;

       Option Eur_call;
       Eur_call.K = K;
       Eur_call.T = T;
       Eur_call.isCall = true;
       Eur_call.isAmerican = false;

       Option Am_call;
       Am_call.K = K;
       Am_call.T = T;
    Am_call.isCall = true;
    Am_call.isAmerican = true;
    double FV_Am_put = 0;
    double FV_Eur_put = 0;
    double FV_Am_call = 0;
    double FV_Eur_call = 0;
    int n = 100;
    BinomialTree binom(n);
    double dS = 0.1;
    int imax = 2000;
    for (int i = 1; i <= imax; ++i) {
        S = i*dS;
        rc = binom.FairValue(n, &Am_put, &db, S, sigma, t0, FV_Am_put);
        rc = binom.FairValue(n, &Eur_put, &db, S, sigma, t0, FV_Eur_put);
        rc = binom.FairValue(n, &Am_call, &db, S, sigma, t0, FV_Am_call);
        rc = binom.FairValue(n, &Eur_call, &db, S, sigma, t0, FV_Eur_call);
        if (n > 100)
            n = 99;
        else
            n = 101;
        cout << std::setw(6) << i << " ";
        cout << std::setw(16) << S << " ";
        cout << std::setw(16) << FV_Am_put << " ";
        cout << std::setw(16) << FV_Eur_put << " ";
        cout << std::setw(16) << FV_Am_call << " ";
        cout << std::setw(16) << FV_Eur_call << " ";
        cout << std::endl;
    }
    return 1;
}

int main(){
    binomial_test();
    impliedVol_test();
}
