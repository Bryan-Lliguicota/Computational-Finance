/*
 * =====================================================================================
 *
 *       Filename:  BinomialTree.hpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/24/2018 16:35:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */

#include "Option.hpp"

class BinomialTree{
    private:
        int n_tree;
        TreeNode** tree_nodes;

        void clear();
        int Allocate(int n);
    public:
        BinomialTree(int n);
        ~BinomialTree();

        int FairValue(int n, const Derivative* p_derivate, const Database* p_db,
                double S, double sigma, double t0, double &FV);

        int ImpliedVolatility(int n, const Derivative* p_derivate, const Database* p_db,
               double S, double t0, double target, double& implied_vol, int& num_iter);
};

int BinomialTree::ImpliedVolatility(int n, const Derivative* p_derivate, const Database* p_db,
               double S, double t0, double target, double& implied_vol, int& num_iter){

    //We set our own limits internally b/c implied volatility is a very expensive
    //function and we don't want users to set unecessary small tolerances
    const double tol = 1.0e-6;
    const int max_iter = 100;
    implied_vol = 0;
    num_iter = 1;
    //perform some validation tests
    double sigma_low = .01; //(==1%)
    double FV_low = 0.0;
    FairValue(n,p_derivate,p_db,S, sigma_low,t0,FV_low);
    double diff_FV_low = FV_low - target;
    if(abs(diff_FV_low) <= tol){
        implied_vol = sigma_low;
        return 0;
    }

    double sigma_high = 2.0; //(==200%)
    double FV_high = 0.0;
    FairValue(n,p_derivate,p_db,S,sigma_high,t0,FV_high);
    double diff_FV_high = FV_high - target;
    if(abs(diff_FV_high) <= tol){
        implied_vol = sigma_high;
        return 0;
    }

    //Test to see if the taget lies b/w FV_low and FV_high
    if(diff_FV_low*diff_FV_high > 0){
        implied_vol=0;
        return 1;
    }

    double sigma = 0.0; double FV = 0.0;
    double diff_FV = 0.00;
    //main bisection loop
    for(num_iter = 1; num_iter<max_iter ; num_iter++){
        sigma = 0.5 * (sigma_low + sigma_high);
        FairValue(n,p_derivate,p_db,S,sigma,t0,FV);
        diff_FV = FV - target;

        if(abs(diff_FV) <= tol){
            target = sigma;
            return 0;
        }

        if(diff_FV_low * diff_FV > 0){
            sigma_low = sigma;
        }else{
            sigma_high = sigma;
        }

        if(abs(sigma_low - sigma_high) <= tol){
            implied_vol = sigma;
            return 0;
        }
    }

    implied_vol = 0;
    return 0;
}

int BinomialTree::Allocate(int n){
    if( n<= n_tree) return 0;

    //deallocate old tree
    if(n_tree != 0) clear();

    n_tree = n;

    tree_nodes = new TreeNode* [n+1];
    for(int i =0 ; i<=n ; i++){
        tree_nodes[i] = new TreeNode[n+1];
        for(int j=0 ; j<=n ;j++){
            tree_nodes[i][j].S=0;
            tree_nodes[i][j].V=0;
            tree_nodes[i][j].t=0;
        }
    }
    //allocate memory
   return 0;
}
BinomialTree::BinomialTree(int n){
    n_tree = 0;
    tree_nodes = NULL;
    Allocate(n);
}

BinomialTree::~BinomialTree(){
    clear();
}

void BinomialTree::clear(){
    for(int i = 0 ; i<=n_tree ; i++)
        delete[]  tree_nodes[i];
    delete []tree_nodes;
}

int BinomialTree::FairValue(int n, const Derivative* p_derivate, const Database* p_db,
                double S, double sigma, double t0, double &FV){

    if( n<1 || S<=0 || p_derivate== NULL|| p_db == NULL|| p_derivate->T<=0||sigma<=0)
        return 1;
    //calculate essential parameters
    double dt = (p_derivate->T - t0)/double(n);
    double df = exp(-p_db->r *dt);
    double growth = exp((p_db->r - p_db->q)*dt);
    double u = exp(sigma*sqrt(dt));
    double d = 1.0/u;

    double p_prob = (growth -d)/(u-d);
    double q_prob = 1.0 - p_prob;

    if( p_prob < 0.0 || q_prob > 1.0)
        return 1;
    Allocate(n);

    TreeNode* node_tmp = tree_nodes[0];
    node_tmp[0].S = S;
    node_tmp[0].t=t0;
    node_tmp[0].V =0;
    int i;
    for(i=1 ; i<=n ;i++){
        double t =t0+ i*dt;
        TreeNode* prev = tree_nodes[i-1];
        node_tmp = tree_nodes[i];
        node_tmp[0].S = prev[0].S*d;
        node_tmp[0].t= t;
        for(int j=1; j<=i; j++){
            node_tmp[j].S = prev[j-1].S*u;
            node_tmp[j].t = t;
        }
    }

    i=n;
    node_tmp = tree_nodes[i];
    for( int j=0 ; j<=n ; j++){
        node_tmp[j].V= p_derivate->TerminalPayoff(node_tmp[j].S);
    }

    for(i = n-1 ; i>=0; --i){
        node_tmp = tree_nodes[i];
        TreeNode* node_next = tree_nodes[i+1];
        for(int j=0 ; j<=i ; j++){
            node_tmp[j].V = df*(p_prob*node_next[j+1].V + q_prob*node_next[j].V);
            p_derivate->ValidationTests(node_tmp[j]);
        }
    }

    //derive the fair value
    node_tmp = tree_nodes[0];
    FV = node_tmp[0].V;

    return 0;

}
