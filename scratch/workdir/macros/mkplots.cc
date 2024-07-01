#include <iostream>
#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <vector>

using namespace std;

void mkplots(){

  TFile *file = new TFile("macros/hime/mkplots.root","recreate");
  TString rootdir("/home/s063/HIME/file_merger/results");
  TChain* chain = new TChain("mergedTree","tree");

  for (int irun=1036;irun<1049;++irun){
  //for (int irun=1148;irun<1149;++irun){
    chain->Add(Form("%s/merged_run_%d.root",rootdir.Data(),irun));
    //cout<<chain->GetEntries()<<endl;
  }
  
  //---------------------------------------
  unsigned int   hime_nHits;
  vector<float> *hime_tofRaw = 0;
  vector<float> *hime_tDiff = 0;
  vector<float> *hime_tSum = 0;
  vector<float> *hime_tot0 = 0;
  vector<float> *hime_tot1 = 0;
  vector<unsigned int> *hime_moduleID = 0;

  ULong64_t hime_slowScaler;
  ULong64_t hime_fastScaler;
  int hime_EventNumber;

  unsigned int run, event;
  ULong64_t lupots;
  unsigned int kyoto_multi;
  vector<unsigned int> kyoto_bar;
  unsigned int hime_veto_multi;
  vector<unsigned int> *hime_veto_id = 0;
  vector<float> *hime_veto_tof = 0;
  vector<float> *hime_veto_charge = 0;
  vector<float> *hime_veto_tdiff = 0;
  vector<float> *hime_veto_x = 0;

  //chain->SetBranchAddress("nHits_HIME_merged", &hime_nHits);
  chain->SetBranchAddress("tofRaw_HIME_merged", &hime_tofRaw);
  chain->SetBranchAddress("tDiff_HIME_merged", &hime_tDiff);
  chain->SetBranchAddress("tSum_HIME_merged", &hime_tSum);
  chain->SetBranchAddress("tot0_HIME_merged", &hime_tot0);
  chain->SetBranchAddress("tot1_HIME_merged", &hime_tot1);
  chain->SetBranchAddress("moduleID_HIME_merged", &hime_moduleID);

//  chain->SetBranchAddress("hime_slowScaler", &hime_slowScaler);
//  chain->SetBranchAddress("hime_fastScaler", &hime_fastScaler);
//  chain->SetBranchAddress("hime_EventNumber", &hime_EventNumber);

//  chain->SetBranchAddress("run", &run);
//  chain->SetBranchAddress("event", &event);
//  chain->SetBranchAddress("lupots", &lupots);
//  chain->SetBranchAddress("kyoto_multi", &kyoto_multi);
//  chain->SetBranchAddress("kyoto_bar", &kyoto_bar);
//  chain->SetBranchAddress("multi", &hime_veto_multi);
  chain->SetBranchAddress("VETO_ID_merged", &hime_veto_id);
  chain->SetBranchAddress("ToF_VETO_merged", &hime_veto_tof);
  chain->SetBranchAddress("Charge_VETO_merged", &hime_veto_charge);
  chain->SetBranchAddress("TDiff_VETO_merged", &hime_veto_tdiff);
  chain->SetBranchAddress("X_VETO_merged", &hime_veto_x);

  //---------------------------------------
  TH1* hhimeid = new TH1D("hhimeid","HIME ID",72,0.5,72.5);
  TH1* hhimeid_v = new TH1D("hhimeid_v","HIME ID Vetoed",72,0.5,72.5);
  TH1* hhimeidtof = new TH2D("hhimeidtof","HIME ID TOF",72,0.5,72.5,200,-300,100);
  TH1* hhimetoftotave = new TH2D("hhimetoftotave","HIME TOF ToTAve",200,-300,100,200,0,50);

  TH1* hhimetoftotave_layer1 = new TH2D("hhimetoftotave_layer1","HIME TOF ToTAve layer1 (ID<24)",200,-300,100,200,0,50);
  TH1* hhimetoftotave_layer1_v = new TH2D("hhimetoftotave_layer1_v","HIME TOF ToTAve layer1 (ID<24) Vetoed",200,-300,100,200,0,50);

  TH1* hhimeidtotave_layer1_v = new TH2D("hhimeidtotave_layer1_v","HIME ID ToTAve layer1 (ID<24) Vetoed",24,0.5,24.5,200,0,50);
  TH1* hhimeiddt_layer1_v = new TH2D("hhimeiddt_layer1_v","HIME ID tDiff layer1 (ID<24) Vetoed",24,0.5,24.5,200,-40,40);

  
  TH1* hhimetot1tot2_v = new TH2D("hhimetot1tot2_v","HIME ToTAve layer1 (ID<24) ToTAve layer2 Vetoed",200,-10,50,200,-10,50);

  TH1* hhimetoftotave_layer2 = new TH2D("hhimetoftotave_layer2","HIME TOF ToTAve layer2 (24<ID<48)",200,-300,100,200,0,50);
  TH1* hhimetoftotave_layer2_v = new TH2D("hhimetoftotave_layer2_v","HIME TOF ToTAve layer2 (24<ID<48) Vetoed(by HIME layer1)",200,-300,100,200,0,50);
  TH1* hhimeidtotave_layer2_v = new TH2D("hhimeidtotave_layer2_v","HIME ID ToTAve layer2 (24<ID<48) Vetoed(by HIME layer1)",24,24.5,48.5,200,0,50);
  TH1* hhimeiddt_layer2_v = new TH2D("hhimeiddt_layer2_v","HIME ID tDiff layer2 (24<ID<48) Vetoed(by HIME layer1)",24,24.5,48.5,200,-40,40);

  TH1* hhimetoftotave_layer3 = new TH2D("hhimetoftotave_layer3","HIME TOF ToTAve layer3 (48<ID<72)",200,-300,100,200,0,50);
  TH1* hhimetoftotave_layer3_v = new TH2D("hhimetoftotave_layer3_v","HIME TOF ToTAve layer3 (48<ID<72) Vetoed(by HIME layer2)",200,-300,100,200,0,50);

  
  TH1* hvetoid = new TH1D("hvetoid","VETO ID",3,0.5,3.5);
  TH1* hvetoidtof = new TH2D("hvetoidtof","VETO ID TOF",3,0.5,3.5,200,-600,0);
  TH1* hvetotoftotave = new TH2D("hvetotoftotave","VETO TOF ToTAve",200,-600,0,200,0,20000);

  TH1* hvetodttot1 = new TH2D("hvetodttot1","VETO dT ToT ID=1",100,-30,30, 200,0,20000);
  TH1* hvetodttot2 = new TH2D("hvetodttot2","VETO dT ToT ID=2",100,-30,30, 200,0,20000);
  TH1* hvetodttot3 = new TH2D("hvetodttot3","VETO dT ToT ID=3",100,-30,30, 200,0,20000);

  Long64_t nentries = chain->GetEntries();
  
  for (Long64_t ieve=0;ieve<nentries;ieve++){
    //for (Long64_t ieve=0;ieve<1000;ieve++){

    hime_tofRaw->clear();
    hime_tDiff->clear();
    hime_tSum->clear();
    hime_tot0->clear();
    hime_tot1->clear();
    hime_moduleID->clear();

    hime_veto_id->clear();
    hime_veto_tof->clear();
    hime_veto_charge->clear();
    hime_veto_tdiff->clear();
    hime_veto_x->clear();

    chain->GetEntry(ieve);

    bool VETOHIT = false;
    bool HIMEHIT1 = false;
    bool HIMEHIT2 = false;
    
    //----------------------------
    // HIME VETO
    Int_t nhits_v = hime_veto_id->size();
    for(int i=0;i<nhits_v;i++){
      int ID = (*hime_veto_id)[i];

      double tof = (*hime_veto_tof)[i];
      double tot = (*hime_veto_charge)[i];
      double dt =  (*hime_veto_tdiff)[i];
      
      hvetoid->Fill(ID);
      hvetoidtof->Fill(ID, tof);
      hvetotoftotave->Fill(tof,tot);

      if (ID==1) hvetodttot1->Fill(dt,tot);
      if (ID==2) hvetodttot2->Fill(dt,tot);
      if (ID==3) hvetodttot3->Fill(dt,tot);

      if (tot>0) VETOHIT = true;
      
//      cout<<ID<<" "
//	  <<tof<<" "
//	  <<tot<<" "
//	  <<endl;

    }
    //----------------------------
    // HIME
    Int_t nhits = hime_moduleID->size();
    double hime_tot[3]={-1,-1,-1};
    for(int i=0;i<nhits;i++){

      int ID = (*hime_moduleID)[i];
      double tof = (*hime_tofRaw)[i];

      hhimeidtof->Fill(ID, tof);
      if (ID==1) continue;

      double tot0 = (*hime_tot0)[i];
      double tot1 = (*hime_tot1)[i];
      double totave = sqrt(tot0*tot1);
      double tdiff = (*hime_tDiff)[i];
      hhimeid->Fill(ID);
      if (!VETOHIT) hhimeid_v->Fill(ID);
      hhimetoftotave->Fill(tof,totave);

      if (ID<24){// 1st layer
	HIMEHIT1 = true;
	hhimetoftotave_layer1->Fill(tof,totave);

	if (!VETOHIT){
	  hhimetoftotave_layer1_v->Fill(tof,totave);
	  hhimeidtotave_layer1_v->Fill(ID,totave);
	  if (totave>20)
	    hhimeiddt_layer1_v->Fill(ID,tdiff);
	}
	hime_tot[0]= totave;
      }else if (ID<48){// 2nd layer
	HIMEHIT2 = true;
	hime_tot[1]= totave;
      }else{// 3rd layer
	hime_tot[2]= totave;
      }

//      cout<<ID<<" "
//	  <<tof<<" "
//	  <<endl;
    }

    for(int i=0;i<nhits;i++){

      int ID = (*hime_moduleID)[i];
      double tof = (*hime_tofRaw)[i];

      hhimeidtof->Fill(ID, tof);
      if (ID==1) continue;

      double tot0 = (*hime_tot0)[i];
      double tot1 = (*hime_tot1)[i];
      double totave = sqrt(tot0*tot1);
      double tdiff = (*hime_tDiff)[i];
      hhimeid->Fill(ID);
      if (!VETOHIT) {
	hhimeid_v->Fill(ID);
	hhimetot1tot2_v->Fill(hime_tot[0],hime_tot[1]);
      }
      hhimetoftotave->Fill(tof,totave);

      if (ID>24 && ID<48){// 2nd layers
	hhimetoftotave_layer2->Fill(tof,totave);
	if (!HIMEHIT1) {
	  hhimetoftotave_layer2_v->Fill(tof,totave);
	  hhimeidtotave_layer2_v->Fill(ID, totave);
	  if (totave<15)
	    hhimeiddt_layer2_v->Fill(ID, tdiff);
	}
      }

      if (ID>48){// 3rd layers
	hhimetoftotave_layer3->Fill(tof,totave);
	if (!HIMEHIT2) {
	  hhimetoftotave_layer3_v->Fill(tof,totave);
	}
      }

//      cout<<ID<<" "
//	  <<tof<<" "
//	  <<endl;
    }

    
  }
  
  return;
}
