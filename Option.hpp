/*
 * =====================================================================================
 *
 *       Filename:  Option.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/24/2018 16:32:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bryan  Lliguicota (), LliguicotaBryan@gmail.com
 *   Organization:  CUNY Queens College
 *
 * =====================================================================================
 */

#include "Dirivative.hpp"

class Option : public Derivative{
    public:
        double K;
        bool isCall;
        bool isAmerican;

        Option(){K=0; isCall = false, isAmerican= false;}
        virtual ~Option(){}

        virtual double TerminalPayoff(double S) const;
        virtual int ValidationTests(TreeNode& node) const;
};

double Option::TerminalPayoff(double S) const{
    if(isCall)
        return max(S-K,0);
    return max(K-S,0);
}

int Option::ValidationTests(TreeNode& node) const{
    if(isAmerican){
        if(isCall){
            node.V = max(max(node.S-K,0),node.V);
            return 0;
        }
        node.V = max(max(K-node.S,0),node.V);
        return 0;
    }
    return 0;
}
