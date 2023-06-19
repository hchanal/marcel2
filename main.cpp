
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <string>
//#include "string.h"

#include "TH1F.h"
#include "triplot.h"
#include "TEventList.h"
#include <cstdlib>
#include "mVariable.h"


#include "tinyxml2.h"

#include <sys/stat.h>
#include <dirent.h>

int exist(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

void getdir (std::string dir, std::vector<std::string> &files, const char *search)
{
    DIR *dp;
    struct dirent *dirp;

    if((dp = opendir(dir.c_str())) == NULL)  {
        std::cerr << "Error(" << errno << ") opening " << dir << std::endl;
        exit(-1);
    }
    while ((dirp = readdir(dp)) != NULL) {
        std::string fname = dirp->d_name;
        if(fname.find(search) != std::string::npos && fname.find(".root") != std::string::npos)
            files.push_back(fname);
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {

    std::string optFileName = std::string(argv[1]);//"marcel.xml";
    std::vector<std::string> dataFileName[3];
    TChain *myChain[3];
    std::string dataFile[3];
    std::string treeName("DefaultReconstruction/nsEsClusCoins");

    std::vector<triplot *> myplotsD;
    std::vector<mVariable> * myVariables;

    std::string m_dir(".");
    std::string m_tcuts("");
    std::string m_pcuts[3];
    bool isPcut[3] = {false, false, false};
    int nPlots = 1;

    std::string m_fig("");
    bool m_figD = false, m_normalize = true;

    std::cout << "<=========================================>" << std::endl;
    std::cout << "== MARCEL2 Plotting tool - Version 1.41  ==" << std::endl;
    std::cout << "<=========================================>" << std::endl;

    if(!exist(optFileName.c_str())){
        std::cerr << "[ERROR] " << optFileName.c_str() << " do not exist !" << std::endl;
        exit(-1);
    }

    tinyxml2::XMLDocument myOptions;
    myOptions.LoadFile(optFileName.c_str());
    tinyxml2::XMLNode *xmlRoot = myOptions.FirstChildElement("marcel");

    for (tinyxml2::XMLElement *child = xmlRoot->FirstChildElement("file");
         child != NULL; child = child->NextSiblingElement()) {
        if (child != NULL && strcmp(child->Name(), "file") == 0) {
            int myType = child->IntAttribute("type");
            std::cout << myType << " :" << child->Attribute("name") << std::endl;
            if (nPlots < myType) { nPlots = myType; }
            dataFileName[myType - 1].push_back(std::string(child->Attribute("name")));
        }
    }

    for (tinyxml2::XMLElement* child = xmlRoot->FirstChildElement("fdir"); child != NULL; child = child->NextSiblingElement())
    {
        if(child != NULL && strcmp(child->Name(), "fdir") == 0) {
            int myType = child->IntAttribute("type");

            std::string m_dir = std::string(child->Attribute("value"));
            std::vector<std::string> files;
            std::string wildcard = "";
            if (child->Attribute("wildcard")!=NULL){
                wildcard = std::string(child->Attribute("wildcard"));
            }
            getdir(m_dir.c_str(), files, wildcard.c_str());
            for (int j = 0; j < files.size(); j++) {
                std::string s = m_dir + "/" + files.at(j);
                std::cout << myType << " :" <<  files.at(j) << std::endl;
                dataFileName[myType - 1].push_back(s.c_str());
            }
            if (nPlots < myType) { nPlots = myType; }
        }
    }


    for (tinyxml2::XMLElement *child = xmlRoot->FirstChildElement("title");
         child != NULL; child = child->NextSiblingElement()) {
        if (child != NULL && strcmp(child->Name(), "title") == 0) {
            int myType = child->IntAttribute("type");
            dataFile[myType - 1] = std::string(child->Attribute("name"));
        }
    }

    if (xmlRoot->FirstChildElement("directory") != NULL) {
        m_dir = std::string(xmlRoot->FirstChildElement("directory")->Attribute("value"));
    }

    if (xmlRoot->FirstChildElement("treeName") != NULL) {
        treeName = std::string(xmlRoot->FirstChildElement("treeName")->Attribute("value"));
    }

    if (xmlRoot->FirstChildElement("figure") != NULL) {
        m_figD = true;
        m_fig = std::string(xmlRoot->FirstChildElement("figure")->Attribute("value"));
    }

    if (xmlRoot->FirstChildElement("cut") != NULL) {
        m_tcuts = std::string(xmlRoot->FirstChildElement("cut")->Attribute("value"));
    }

    for (tinyxml2::XMLElement *child = xmlRoot->FirstChildElement("pCut");
         child != NULL; child = child->NextSiblingElement()) {
        if (child != NULL && strcmp(child->Name(), "pCut") == 0) {
            int myType = child->IntAttribute("type");
            m_pcuts[myType - 1] = std::string(child->Attribute("value"));
            isPcut[myType - 1]  = true;
        }
    }

    if (xmlRoot->FirstChildElement("normalize") != NULL) {
        std::string temp = std::string(xmlRoot->FirstChildElement("normalize")->Attribute("value"));
        if (temp == "True" || temp == "true") {
            m_normalize = true;
        } else {
            m_normalize = false;
        }
    }

    std::cout << m_tcuts << std::endl;
    for (int k = 0; k < nPlots; k++) {
        if(isPcut[k]){
            std::cout << "Cut for " << k << " : " << m_pcuts[k] << std::endl;
        }
    }


    for (tinyxml2::XMLElement *child = xmlRoot->FirstChildElement("plot");
         child != NULL; child = child->NextSiblingElement()) {
        if (child != NULL && strcmp(child->Name(), "plot") == 0) {
            myplotsD.push_back(new triplot(child->Attribute("name"),
                                           child->Attribute("name"), child->IntAttribute("nBins"),
                                           child->DoubleAttribute("vMin"),
                                           child->DoubleAttribute("vMax")));
            if (child->Attribute("hMin") != NULL) { myplotsD.back()->setMinValue(child->DoubleAttribute("hMin")); }
            if (child->Attribute("hMax") != NULL) { myplotsD.back()->setMaxValue(child->DoubleAttribute("hMax")); }
            if (child->Attribute("pName") != NULL) { myplotsD.back()->setNewName(child->Attribute("pName")); }
            if (child->Attribute("filled1") != NULL) {
                std::string temp = std::string(child->Attribute("filled1"));
                if (temp == "True" || temp == "true") {
                    myplotsD.back()->m_isFilled[0] = true;
                } else {
                    myplotsD.back()->m_isFilled[0] = false;
                }
            }
            if (child->Attribute("xLabel") != NULL) {
                myplotsD.back()->setXLabel(std::string(child->Attribute("xLabel")));
            }
            if (child->Attribute("yLabel") != NULL) {
                myplotsD.back()->setYLabel(std::string(child->Attribute("yLabel")));
            }
            if (child->Attribute("filled2") != NULL) {
                std::string temp = std::string(child->Attribute("filled2"));
                if (temp == "True" || temp == "true") {
                    myplotsD.back()->m_isFilled[1] = true;
                } else {
                    myplotsD.back()->m_isFilled[1] = false;
                }
            }
            if (child->Attribute("filled3") != NULL) {
                std::string temp = std::string(child->Attribute("filled3"));
                if (temp == "True" || temp == "true") {
                    myplotsD.back()->m_isFilled[2] = true;
                } else {
                    myplotsD.back()->m_isFilled[2] = false;
                }
            }
            if (child->Attribute("title") != NULL) {
                myplotsD.back()->setTitle(std::string(child->Attribute("title")));
            }
        }

    }

    int t1;
    t1 = system((std::string("mkdir -p ") + m_dir).c_str());

    for (int i = 0; i < 3; ++i) {
        myChain[i] = new TChain(treeName.c_str(), "");
        int nFiles = dataFileName[i].size();
        for (int j = 0; j < nFiles; j++) {
            TFile *file = TFile::Open(dataFileName[i].at(j).c_str());
            if (!file) {
                std::cout << "[ERR] file " << dataFileName[i].at(j).c_str() << " do not exist" << std::endl;
                continue;
            }
            if (file->IsZombie()) {
                std::cout << "[ERR] file " << dataFileName[i].at(j).c_str() << " is crappy" << std::endl;
                continue;
            }
            if (file->TestBit(TFile::kRecovered)) {
                std::cout << "[ERR] file " << dataFileName[i].at(j).c_str() << " has been recovered" << std::endl;
                continue;
            }
            file->Close();

            myChain[i]->AddFile(dataFileName[i].at(j).c_str());
        }
    }


    myVariables = new std::vector<mVariable>;
    for (unsigned int i = 0; i < myplotsD.size(); i++) {
        myplotsD.at(i)->setVariables(myVariables);
        for (int j = 0; j < nPlots; ++j) {
            myplotsD.at(i)->setData(j, myChain[j]);
        }
        myplotsD.at(i)->setLogo(m_fig.c_str(), m_figD);
        myplotsD.at(i)->m_normalize = m_normalize;
    }

    long nentries, nentriesl;

    for (int k = 0; k < nPlots; k++) {
        nentries = myChain[k]->GetEntries();
        if(isPcut[k]) {
            myChain[k]->Draw(">>event1", m_pcuts[k].c_str());
        } else {
            myChain[k]->Draw(">>event1", m_tcuts.c_str());
        }
        TEventList *elist1 = (TEventList *) gDirectory->Get("event1");
        nentriesl = elist1->GetN();
        for (Int_t j = 0; j < nentriesl; j++) {
            Long_t myRealEntry = elist1->GetEntry(j);
            myChain[k]->GetEntry(myRealEntry);
            //if(myCutCommon(m_treeData1) && myCutTime(m_treeData1)){
            for (unsigned int i = 0; i < myplotsD.size(); i++) {
                myplotsD.at(i)->fillData(k);
            }
        }
    }

    auto *top = new TFile((m_dir + "/results.root").c_str(), "recreate");
    for (unsigned int i = 0; i < myplotsD.size(); i++) {
        myplotsD.at(i)->write(top, dataFile[0], dataFile[1], dataFile[2], m_dir);
    }
    top->Close();
    return 0;

}
