//
// Created by herve on 19/06/19.
//

#ifndef MARCEL1_TRIPLOT_H
#define MARCEL1_TRIPLOT_H

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <sstream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <TImage.h>
#include <TBranch.h>
#include <TTree.h>
#include <TChain.h>
#include <TLeaf.h>
#include "mVariable.h"

class triplot
{
public :
    TH1F    * m_data[3];
    TCanvas * m_c1;
    TBranch * m_bdata[3];

    std::vector<mVariable> * m_myVariables;
    int                    m_myVarId;

    // Possible type
    enum     d_dtype {unknow, aint, adouble, afloat, avectord};
    d_dtype  m_dtype;
    int    * m_datap_i[3];
    int      m_datav_i[3];

    double * m_datap_d[3];
    double   m_datav_d[3];

    float  * m_datap_f[3];
    float    m_datav_f[3];

    std::vector<double> *  m_datap_vd[3];
    std::vector<double>    m_datav_vd[3];

    int  m_nPlots;

    std::string m_plotName;
    //std::string m_bName;
    std::string m_branchName;
    std::string m_figName;
    std::string  m_yLabel;
    std::string  m_xLabel;
    int m_nBins;
    double m_binMin, m_binMax;
    bool m_isFig, m_normalize, m_isMaxValue, m_isMinValue;
    double m_minValue, m_maxValue;
    bool   m_isFilled[3];

    std::string m_title;


    triplot(const char * branchName,std::string plotName,int nBins, double binMin, double binMax);
    ~triplot();


    void fillData(int i);
    void setData(int i,TTree* aTree);
    void setVariables(std::vector<mVariable> * myVariable);
    void write(TFile * myfile, std::string df1, std::string df2, std::string df3, std::string mdir);
    void setLogo(const char * fileName, const bool isFig);
    void drawLogo();
    void setMaxValue(double v){m_isMaxValue = true;m_maxValue=v;}
    void setMinValue(double v){m_isMinValue = true;m_minValue=v;}
    void setNewName(std::string pltName){m_plotName=pltName;}
    void setYLabel(std::string yLabel){m_yLabel=yLabel;}
    void setXLabel(std::string xLabel){m_xLabel=xLabel;}
    void setTitle(std::string title){m_title=title;}
};

#endif //MARCEL1_TRIPLOT_H
