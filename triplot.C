//
// Created by herve on 19/06/19.
//

#include "triplot.h"

triplot::triplot(const char * branchName,std::string plotName,int nBins, double binMin, double binMax){
    m_plotName = plotName;
    m_branchName = std::string(branchName);
    m_nBins      = nBins;
    m_binMin     = binMin;
    m_binMax     = binMax;
    m_nPlots     = 1;
    m_normalize  = true;
    m_isMaxValue = false;
    m_isMinValue = false;
    m_isFilled[0] = true;
    m_isFilled[1] = false;
    m_isFilled[2] = false;
    m_yLabel      = std::string("Events");
    m_xLabel      = std::string("");
    m_title       = std::string("NONE");
}

triplot::~triplot() {

}

void triplot::setVariables(std::vector<mVariable> *myVariable) {
    m_myVariables = myVariable;
    int  mySize = myVariable->size();
    bool found = false;
    m_myVarId = -1;
    for(int i=0;i<mySize && !found;i++){
        if(myVariable->at(i).getName() == m_branchName){
            m_myVarId = i;
            found = true;
        }
    }
    if(!found){
        mVariable newVar;
        newVar.setBranchName(m_branchName);
        m_myVarId = mySize;
        myVariable->push_back(newVar);
    }

}
void triplot::setData(int i,TTree* aTree){
    std::string m_tname, stitle;
    std::stringstream is;
    is << m_plotName << "_d" << i;
    is >> m_tname;

    if(m_title != std::string("NONE")){
        stitle = m_title;
    } else {
        stitle = m_tname;
    }
    m_data[i] = new TH1F(m_tname.c_str(),stitle.c_str(),m_nBins,m_binMin,m_binMax);
    m_myVariables->at(m_myVarId).setBranchAddress(i, aTree);
    m_bdata[i] = aTree->GetBranch(m_branchName.c_str());
    //m_datap[i] = (T *)aTree->fChain->GetBranch(m_branchName.c_str())->GetAddress();
    if(m_nPlots<(i+1)){m_nPlots=i+1;}
}

void triplot::fillData(int i){
    if(!m_myVariables->at(m_myVarId).isVector()){
        m_data[i]->Fill(m_myVariables->at(m_myVarId).getValue(i));
    }
    else {
        for(auto datap : *m_myVariables->at(m_myVarId).getValues(i)){
                m_data[i]->Fill(datap);
        }
    }
}

void triplot::write(TFile * myfile, std::string df1, std::string df2,std::string df3, std::string mdir) {
    myfile->cd();
    TDirectory * dir = myfile->mkdir(m_plotName.c_str());
    dir->cd();
    for(int i=0;i<m_nPlots;i++) {
        m_data[i]->Write();
    }

    m_c1 = new TCanvas((m_plotName+"_can").c_str(),(m_plotName+"_can").c_str(),200,10,700,600);

    if(m_isFilled[0] == true) {
        m_data[0]->SetFillColor(kBlue);
        m_data[0]->SetFillStyle(3003);
    }
    if(m_normalize == true){
        m_data[0]->Scale(1/(double)m_data[0]->Integral());
        if(m_nPlots>1){m_data[1]->Scale(1 / (double) m_data[1]->Integral());}
        if(m_nPlots>2){m_data[2]->Scale(1 / (double) m_data[2]->Integral());}
        if(m_yLabel=="Events"){m_yLabel=std::string("");}
    }
    double mymax = m_data[0]->GetMaximum();
    for(int i=1;i<m_nPlots;i++) {
        if(mymax < m_data[i]->GetMaximum()){
            mymax = m_data[i]->GetMaximum();
        }
    }
    //m_data1->Draw("EF");
    for(int i=1;i<m_nPlots;i++) {
        if(mymax < m_data[i]->GetMaximum()){
            mymax = m_data[i]->GetMaximum();
        }
    }
    if(m_isMaxValue == false) {
        m_data[0]->SetMaximum(1.2*mymax);
    } else {
        m_data[0]->SetMaximum(m_maxValue);
    }
    if(m_isMinValue == true) {
        m_data[0]->SetMinimum(m_minValue);
    }
    m_data[0]->GetXaxis()->SetTitle(m_xLabel.c_str());
    m_data[0]->GetYaxis()->SetTitle(m_yLabel.c_str());
    m_data[0]->Draw("EHIST");

    if (m_nPlots>1) {
        m_data[1]->SetLineColor(kRed);
        if(m_isFilled[1] == true) {
            m_data[1]->SetFillColor(kRed);
            m_data[1]->SetFillStyle(3003);
            m_data[1]->Draw("EHIST SAME");
        }
        else {
            m_data[1]->Draw("ESAME");
        }
    }

    if(m_nPlots>2){
        m_data[2]->SetLineColor(kBlack);
        if(m_isFilled[2] == true) {
            m_data[2]->SetFillColor(kBlack);
            m_data[2]->SetFillStyle(3003);
            m_data[2]->Draw("EHIST SAME");
        }
        else {
            m_data[2]->Draw("ESAME");
        }
    }


    TLegend *legend;
    if(m_nPlots>1) {
        if ((double) m_data[0]->GetMaximumBin() < 0.4 * (double) m_data[0]->GetNbinsX()) {
            legend = new TLegend(0.4, 0.7, 0.6, 0.8);
        } else {
            legend = new TLegend(0.1, 0.7, 0.3, 0.8);
        }
        legend->AddEntry(m_data[0], df1.c_str());
        legend->AddEntry(m_data[1], df2.c_str());
        if (m_nPlots > 2) {
            legend->AddEntry(m_data[2], df3.c_str());
        }
        legend->Draw();
    }
    if(m_isFig==true){drawLogo();}
    m_c1->SaveAs((mdir+"/"+m_plotName+"_can.png").c_str());
    m_c1->Write();
    if(m_nPlots>1){delete legend;}
}

void triplot::setLogo(const char *fileName, const bool isFig) {
    m_figName = std::string(fileName);
    m_isFig   = isFig;
}

void triplot::drawLogo(){
    TImage *img = TImage::Open(m_figName.c_str());

    if (!img) {
        printf("Could not create an image... exit\n");
        return;
    }
    TPad *l = new TPad("l", "l", 0.78, 0.5, 0.95, 0.75);
    l->SetFillStyle(4000);
    l->Draw();
    l->cd();
    img->Draw();
    // have te delete l and img somewhere....
}


