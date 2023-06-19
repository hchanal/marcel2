//
// Created by herve on 11/03/2021.
//

#ifndef MARCEL2_MVARIABLE_H
#define MARCEL2_MVARIABLE_H

#include <vector>
#include <string>
#include "TTree.h"
#include "TLeaf.h"
#include <iostream>

class mVariable{
private :
    // Possible type
    enum     d_dtype {unknow, aint, adouble, afloat, avectord};
    d_dtype  m_dtype;
    bool     m_vector;
    int *    m_datap_i[3];
    double * m_datap_d[3];
    float  * m_datap_f[3];
    bool     m_assigned[3];
    std::vector<double> *  m_datap_vd[3];
    std::string m_varName;
public:
    mVariable();
    ~mVariable(){;}
    void   setBranchName(std::string bName){m_varName = bName;}
    void   setBranchAddress(int i,TTree* aTree);
    double getValue(int i);
    double getValue(int i, int j);
    std::vector<double> * getValues(int i);
    bool   isVector(){return m_vector;}
    long  size(int i){return m_datap_vd[i]->size();}
    std::string getName(){return m_varName;}
};

#endif //MARCEL2_MVARIABLE_H
