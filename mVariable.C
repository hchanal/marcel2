//
// Created by herve on 11/03/2021.
//

#include "mVariable.h"

mVariable::mVariable() {
    for(int i=0;i<3;i++){
        m_assigned[i] = false;
    }
}

void mVariable::setBranchAddress(int i, TTree *aTree) {
    if (!m_assigned[i]) {
        m_assigned[i] = true;
        m_dtype = unknow;
        m_vector = false;
        std::string myType = aTree->GetLeaf(m_varName.c_str())->GetTypeName();
        if (myType == "Int_t" || myType == "int") {
            m_dtype = aint;
            m_datap_i[i] = new int;
            aTree->SetBranchAddress(m_varName.c_str(), m_datap_i[i]);
        }
        if (myType == "Double_t" || myType == "double") {
            m_dtype = adouble;
            m_datap_d[i] = new double;
            aTree->SetBranchAddress(m_varName.c_str(), m_datap_d[i]);
        }
        if (myType == "vector<Double_t>" || myType == "vector<double>") {
            m_vector = true;
            m_dtype = avectord;
            m_datap_vd[i] = 0;
            aTree->SetBranchAddress(m_varName.c_str(), &(m_datap_vd[i]));
        }
        if (myType == "Float_t" || myType == "float") {
            m_dtype = afloat;
            m_datap_f[i] = new float;
            aTree->SetBranchAddress(m_varName.c_str(), m_datap_f[i]);
        }
        if (m_dtype == unknow) {
            std::cout << "[ERR] Type " << myType << " is not handled yet by Marcel" << std::endl;
        }
    }
}

double mVariable::getValue(int i) {
    double res = 0.;
    switch(m_dtype) {
        case aint    :
            res = *m_datap_i[i];
            break;
        case adouble :
            res = *m_datap_d[i];
            break;
        case afloat  :
            res = *m_datap_f[i];
            break;
    }
    return res;
}

double mVariable::getValue(int i, int j) {
    double res = 0.;
    if(m_vector){
        switch(m_dtype) {
            case avectord    :
                res = m_datap_vd[i]->at(j);
        }
    }
    return res;
}

std::vector<double> * mVariable::getValues(int i) {
    return  m_datap_vd[i];
}