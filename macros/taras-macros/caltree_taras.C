#include <TSystem.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TEnv.h"
#include "TH1F.h"
#include "TH1.h"
#include "TVector3.h"
#include "TVectorD.h"
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include "TArtPlastic.hh"
#include "TArtCalibPlastic.hh"
#include "THashList.h"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPID.hh"
#include "TArtPPACPara.hh"
#include "TArtPPAC.hh"
#include "TArtFocalPlane.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtEventStore.hh"
#include "TArtEventInfo.hh"
#include "TArtCalibCoin.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibICF.hh"
#include "TArtICPara.hh"

#include "TArtCalibDCTrack.hh"
#include "TArtCalibDCHit.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC0Hit.hh"
#include "TArtCalibFDC0Track.hh"
/*
#include "TArtCalibSimpleFDC0Track.hh"
#include "TArtCalibSimpleFDC1Track.hh"
#include "TArtCalibSimpleFDC2Track.hh"
#include "TArtCalibSimpleBDC1Track.hh"
#include "TArtCalibSimpleBDC2Track.hh"
*/
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Track.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtDCTrack.hh"
#include "TArtCalibBPCHit.hh"
#include "TArtCalibHODPla.hh"
#include "TArtHODPla.hh"
#include "TArtHODPlaPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtCalibNEBULAHPC.hh"
#include "TArtNEBULAFilter.hh"
#include "TArtNEBULAHPC.hh"
#include "TArtNEBULAHPCPara.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"
#include "TArtCATANACsIPara.hh"
#include "TArtCATANACsI.hh"
#include "TArtCATANAParameters.hh"
#include "TArtCalibCATANA.hh"
#include "TArtRecoFragment.hh"
#include "TArtFragment.hh"
#include "TArtCalibNeuLAND.hh"
#include "TArtCalibNeuLANDVETO.hh"
#include "TArtNeuLANDPla.hh"
#include "TArtNeuLANDPlaPara.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include "/data/RIKEN/DayOne/users/taras/Analysis/samurai_function/samurai_fun_len.C"
#include "/data/RIKEN/DayOne/users/taras/Analysis/samurai_function/samurai_fun_rig.C"

#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"

#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtFocalPlane.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"
#include "TArtBeam.hh"

#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "signal.h"
#include "fstream"
#include "iostream"

#include <climits>
#include <map>
#include <ctime>

//Si GSI
#include "TArtCalibSILICONS.hh"
#include "TArtSILICONS.hh"
#include "TArtSILICONSPara.hh"
#include "TArtSILICONSParameters.hh"

using namespace std;

//#define NUMBER_OF_HODO 24
#define NUMBER_OF_HODO 24 // ID=48 when ID=25 and 26 have hits


#define WriteOneEnvFile(file) file->Write(TString(TString(file->GetRcName()).ReplaceAll("/","_")).ReplaceAll(".","_"));
#define WriteAllEnvFiles WriteOneEnvFile(env_par); WriteOneEnvFile(env_geo); WriteOneEnvFile(env_nebt0); WriteOneEnvFile(env_neut0);

// Fuctions used
//void LoadDCTDCDistributionFull(char *FileName, TArtCalibBDC1Track *, TArtCalibBDC2Track *, TArtCalibSimpleFDC1Track *, TArtCalibSimpleFDC2Track *);
void LoadDCTDCDistributionFull(char *FileName, TArtCalibBDC1Track *, TArtCalibBDC2Track *, TArtCalibFDC1Track *, TArtCalibFDC2Track *);
//void LoadDCTDCDistributionFull(char *FileName, TArtCalibSimpleBDC1Track *, TArtCalibSimpleBDC2Track *, TArtCalibSimpleFDC0Track *, TArtCalibSimpleFDC2Track *);
//void LoadDCTDCDistributionFull(char *FileName, TArtCalibSimpleBDC1Track *, TArtCalibSimpleBDC2Track *, TArtCalibFDC0Track *, TArtCalibFDC2Track *);
void SortHodoHit(vector<Int_t> &, vector<Double_t> &);
//Double_t DopplerCorrection(Double_t, Double_t, Double_t);
inline bool exists_test (const std::string&);
inline bool exists_test (const TString&);
void SortDaliHit(Int_t, Int_t,vector <Int_t> *,vector <Double_t> *, vector <Double_t> *, vector <Double_t> *, vector <Double_t> *);
void SortNeutronHit(int, int,vector <Bool_t> *, vector <Int_t> *,vector <Double_t> *,vector <Double_t> *,vector <Double_t> *,vector <Double_t> *, vector <Double_t> *, vector <Double_t> *, vector <Double_t> *);

int main(int argc, char *argv[])
{
  Long64_t MaxEventNumber = LLONG_MAX;
  const Int_t DISPLAY_EVERY_EVENT = 500 ;
  std::clock_t start = std::clock();
  std::clock_t clock0 = std::clock();
  Bool_t kMAIN=kFALSE,kNEUT=kFALSE,kCATANA=kFALSE,kFDCs=kFALSE,kHODO=kFALSE,kBEAM=kFALSE, kFDCs_full=kFALSE;
  Bool_t kRunLive = kFALSE, kOverwrite=kFALSE;
  // a (or nothing) or any combinasion mndfhb
  int opt;
  Bool_t kShowHelp = kFALSE;
  std::map<char,bool *> flagMap;

  while ((opt = getopt(argc, argv, "AMNDFHBVGhfn:")) != -1) {
    switch (opt) {
    case 'A':
      kMAIN=kNEUT=kCATANA=kFDCs_full=kHODO=kBEAM=kTRUE;
      break;
    case 'N':
      kNEUT=kTRUE;      
      break;
    case 'M':
      kMAIN=kTRUE;
      break;
    case 'D':
      kCATANA=kTRUE;
      break;
    case 'F':
      kFDCs=kTRUE;
      break;
    case 'H':
      kHODO=kTRUE;
      break;
    case 'B':
      kBEAM=kFDCs_full=kTRUE;
      break;
    case 'V':
      kRunLive=kTRUE;
      break;
    case 'G':
      kBEAM=kFDCs_full=kTRUE;
      break;
    case 'h':
      kShowHelp = kTRUE;
      break;
    case 'f':
      kOverwrite = kTRUE;
      break;
    case 'n':
      MaxEventNumber = atoi(optarg);
      //      MaxEventNumber = 1e3;
      std::clog << "\e[31m" << "  ANALYSIS-Info : You will analyse " << MaxEventNumber << " events" << "\e[0m" << std::endl;
      break;
    default: /* '?' */
      kMAIN=kFALSE,kNEUT=kFALSE,kCATANA=kFALSE,kFDCs=kFALSE,kHODO=kFALSE,kBEAM=kFALSE,kFDCs_full=kFALSE;
    }
  }

  if(kRunLive && MaxEventNumber == LLONG_MAX){
    std::cerr <<  " For the live mode you should put a maximum number, with -n flag" << endl;
    kShowHelp = kTRUE;
  }

  Int_t FileNumber = TString(argv[optind]).Atoi();

  if(!kRunLive && FileNumber==0){
    std::cerr <<  " You should provide either a runnumber or use the -V (for live) flag" << endl;
    kShowHelp = kTRUE;
  }

  if ( kShowHelp || (!kMAIN && !kNEUT && !kCATANA && !kFDCs && !kHODO && !kBEAM && !kFDCs_full)) {
    std::cerr <<  "Usage: " << argv[0] << " run_number " << std::endl;
    std::cerr <<  "Usage: " << argv[0] << " -n nb_of_events run_number " << std::endl ;
    std::cerr <<  "Usage: " << argv[0] << " -h for help"  << std::endl;
    std::cerr <<  "Usage: " << argv[0] << " -V for liVe conversion " << std::endl ;
    std::cerr <<  "       Conversion options: " << endl;
    std::cerr <<  "       -A for ALL " << endl;
    std::cerr <<  "       -B for BEAM (include Bigrips and BDCs)" << endl;
    std::cerr <<  "       -D for CATANA " << endl;
    std::cerr <<  "       -F for FDCs " << endl;
    std::cerr <<  "       -H for HODO " << endl;
    std::cerr <<  "       -M for MAIN " << endl;
    std::cerr <<  "       -N for NEUT(rons) " << endl;
    std::cerr <<  "       -G for FDCs(full tracking) " << endl;
    std::cerr <<  "       Other options: " << endl;
    std::cerr <<  "       -f force overwriting the output files " << endl;
    exit(EXIT_FAILURE);
  }


  flagMap['N']=&kNEUT;            
  flagMap['M']=&kMAIN;      
  flagMap['D']=&kCATANA;      
  flagMap['F']=&kFDCs;      
  flagMap['H']=&kHODO;      
  flagMap['B']=&kBEAM;
  flagMap['G']=&kFDCs_full;

  TEnv *env_par = new  TEnv("/data/RIKEN/DayOne/users/taras/conversion_settings.prm") ;
  cout<<"enf file is read!!"<<endl;

//   TString FileName = Form("%s%04d",env_par->GetValue("runprefix","catana"),FileNumber);
  TString FileName = Form("%s%04d",env_par->GetValue("runprefix","run"),FileNumber);
  //TString FileName = Form("%s%04d",env_par->GetValue("runprefix","nebula"),FileNumber);
  TString pathridf = env_par->GetValue("ridffilepath","");
  TString RidfFilename = pathridf + FileName + (TString)".ridf";
  TString rootfilepath = "";
  TString rootfilesubpath = "";

  if(!kRunLive){
    if(!exists_test(RidfFilename)){
      cerr << "\e[1;33;42m" << "  ANALYSIS-Error : File '" << RidfFilename <<  "' does not exists. Test for " << RidfFilename << ".gz\e[0m\n" << endl;

      RidfFilename += ".gz" ;

      if(!exists_test(RidfFilename)){
	cerr << "\e[1;33;41m" << "  ANALYSIS-Error : File '" << RidfFilename <<  "' does not exists. Abort.\e[0m\n" << endl;
	exit(EXIT_FAILURE);
      }
    }
    rootfilepath = TString(env_par->GetValue("rootfilepath","")) ;
    rootfilesubpath = rootfilepath + "/" + TString(FileName);

    // check and/or create root file directory
    struct stat infopath;
    struct stat infosubpath;
    if(stat( rootfilepath, &infopath ) != 0){
      cerr <<  "\e[1;33;41m" << "Root Directory :'" << rootfilepath <<  "' does not exists. Abort.\e[0m\n" << endl;
    } else if( infopath.st_mode & S_IFDIR ) {
      if(stat( rootfilesubpath, &infosubpath ) != 0){
	cout <<  "\e[33m  ANALYSIS-Info : Create directory '" << rootfilesubpath <<  "'\e[0m" << endl;
	system("mkdir "+rootfilesubpath);
      }
    }
    cout <<  "\e[33m  ANALYSIS-Info : Root files in '" << rootfilesubpath <<  "'\e[0m" << endl;
  }
  // ====================== ANAROOT Parameters =======================

  TArtBigRIPSParameters *BigRIPSPara = TArtBigRIPSParameters::Instance();
  TArtSAMURAIParameters *SamuraiPara = new TArtSAMURAIParameters();
  TArtCATANAParameters *CATANAPara = TArtCATANAParameters::Instance();
  TArtStoreManager *sman = TArtStoreManager::Instance();

  TObjString *xmlfile;

  TIter next(TString(env_par->GetValue("BigRIPSPara","")).Tokenize(" "));
  while(( xmlfile = (TObjString *)next())) {
      BigRIPSPara->LoadParameter((char*)xmlfile->GetName());
  }

  next = TString(env_par->GetValue("SamuraiPara","")).Tokenize(" ");
  while(( xmlfile = (TObjString *)next())) {
      SamuraiPara->LoadParameter((char*)xmlfile->GetName());
  }

  next = TString(env_par->GetValue("CATANAPara","")).Tokenize(" ");
  while(( xmlfile = (TObjString *)next())) {
      CATANAPara->LoadParameter((char*)xmlfile->GetName());
  }


  //Load PID
  TArtCalibPID *Calibbr   = new TArtCalibPID();
  // Load coincidence register
  TArtCalibCoin *CalibCoin = new TArtCalibCoin();
  

// Load Plastic Detectors
//  TArtCalibPlastic *CalibPlastic = new TArtCalibPlastic(); //old version

    TArtCalibPlastic *CalibPlastic = Calibbr->GetCalibPlastic(); //taras modification

  // Load PAC and FocalPlane
  // TArtCalibPPAC *CalibPPAC = new TArtCalibPPAC(); //old version
    TArtCalibPPAC *CalibPPAC = Calibbr->GetCalibPPAC(); //taras modification

//  TArtCalibFocalPlane *CalibFocalPlane = new TArtCalibFocalPlane(); //old version
  TArtCalibFocalPlane *CalibFocalPlane = Calibbr->GetCalibFocalPlane(); //taras modification

  // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID -> 
  // [ RecoTOF , RecoRIPS , RecoBeam] )
  TArtRecoPID *recopid = new TArtRecoPID();

  //BigRIPSPara->PrintListOfPPACPara();
  
  // Load IC
  TArtCalibIC *CalibIC = new TArtCalibIC();
  // Load Drift Chambers
  TArtCalibBDC1Hit   *CalibBDC1Hit   = new TArtCalibBDC1Hit();
  //  TArtCalibSimpleBDC1Track *CalibBDC1Track = new TArtCalibSimpleBDC1Track();
  TArtCalibBDC1Track *CalibBDC1Track = new TArtCalibBDC1Track();

  TArtCalibBDC2Hit   *CalibBDC2Hit   = new TArtCalibBDC2Hit();
  //TArtCalibSimpleBDC2Track *CalibBDC2Track = new TArtCalibSimpleBDC2Track();
  TArtCalibBDC2Track *CalibBDC2Track = new TArtCalibBDC2Track();
  
  //TArtCalibSimpleFDC1Track *CalibSimpleFDC1Track;
  // TArtCalibSimpleFDC2Track *CalibSimpleFDC2Track;
  TArtCalibFDC1Track *CalibFDC1Track;
  TArtCalibFDC2Track *CalibFDC2Track;

  //  TArtCalibFDC0Track *CalibFDC0Track;
  //TArtCalibSimpleFDC0Track *CalibSimpleFDC0Track;
  //  TArtCalibFDC0Hit   *CalibFDC0Hit   = new TArtCalibFDC0Hit();


  //TArtCalibFDC1Track *CalibFDC1Track =  new TArtCalibFDC1Track();
  TArtCalibFDC1Hit   *CalibFDC1Hit   = new TArtCalibFDC1Hit();

  //TArtCalibFDC2Track *CalibFDC2Track = new TArtCalibFDC2Track();
  TArtCalibFDC2Hit   *CalibFDC2Hit   = new TArtCalibFDC2Hit();

  if(kFDCs_full){
    CalibFDC1Track = new TArtCalibFDC1Track();
    CalibFDC2Track = new TArtCalibFDC2Track();
  } else {
    //CalibFDC1Track = new TArtCalibFDC1Track();
    //CalibFDC2Track = new TArtCalibFDC2Track();

    //CalibSimpleFDC0Track = new TArtCalibSimpleFDC0Track();
    //CalibSimpleFDC2Track = new TArtCalibSimpleFDC2Track();
  }

  // Load Hodoscope
  TArtCalibHODPla *CalibHODPla = new TArtCalibHODPla();
  // Load NEBULA
  TArtCalibNEBULA *CalibNEBULA = new TArtCalibNEBULA();
  // Load NEULAND
  TArtCalibNeuLAND *CalibNEULAND = new TArtCalibNeuLAND();
  TArtCalibNeuLANDVETO *CalibNEULANDVETO = new TArtCalibNeuLANDVETO();
  // Load CATANA
  TArtCalibCATANA *CalibCATANA = new TArtCalibCATANA();
  // Load Brho calculation tools
    TArtRecoFragment *RecoFragment = new TArtRecoFragment();
  // Load Event Info



  cout << "  CALTREE-Info  : Load TDC Distribution" << endl;
  if(kFDCs_full) {
    	LoadDCTDCDistributionFull((char*)env_par->GetValue("TDCDistribution",""), CalibBDC1Track, CalibBDC2Track, CalibFDC1Track, CalibFDC2Track);
    //    LoadDCTDCDistributionFull((char*)env_par->GetValue("TDCDistribution",""),CalibBDC1Track,CalibBDC2Track,CalibFDC0Track,CalibFDC2Track);
  
  cout << "Check this line 1" << endl;

} else if(kFDCs || kBEAM) {
	LoadDCTDCDistributionFull((char*)env_par->GetValue("TDCDistribution",""), CalibBDC1Track, CalibBDC2Track, CalibFDC1Track, CalibFDC2Track);

cout << "Check this line 2" << endl;
    //LoadDCTDCDistributionFull((char*)env_par->GetValue("TDCDistribution",""),CalibBDC1Track,CalibBDC2Track,CalibSimpleFDC1Track,CalibSimpleFDC2Track);
    //    LoadDCTDCDistributionFull((char*)env_par->GetValue("TDCDistribution",""),CalibBDC1Track,CalibBDC2Track,CalibSimpleFDC0Track,CalibSimpleFDC2Track);    
  }




  //Data
  TEnv * env_geo= new TEnv(env_par->GetValue("geometrydata","")); //mm
  Double_t BDC1_Width = env_geo->GetValue("BDC1_Width",0.0); //mm
  Double_t FDC1_Width = env_geo->GetValue("FDC1_Width",0.0); //mm
  //  Double_t FDC0_Width = env_geo->GetValue("FDC0_Width",0.0); //mm
  Double_t Dist_BDC1BDC2 = env_geo->GetValue("Dist_BDC1BDC2",0.0); //mm
  Double_t Dist_BDC1Target = env_geo->GetValue("Dist_BDC1Target",0.0); //mm
  Double_t Dist_BDC1FDC1 = env_geo->GetValue("Dist_BDC1FDC1",0.0); //Distance between the middle of BDC1 and the middle of FDC1 mm
  //Double_t Dist_BDC1FDC0 = env_geo->GetValue("Dist_BDC1FDC0",0.0); //Distance between the middle of BDC1 and the middle of FDC0 mm
  Double_t Dist_F7F13 = env_geo->GetValue("Dist_F7F13",0.0); //mm
  cout<<"Dist_F7F13 = "<<Dist_F7F13<<endl;
  Double_t Dist_SBTTarget = env_geo->GetValue("Dist_SBTTarget",0.0); //mm
/*
  Double_t BDC1_Width = 68.;
  Double_t FDC1_Width = 180.;
  Double_t Dist_BDC1BDC2 = 1000.; // mm
  Double_t Dist_BDC1Target = 1887. - BDC1_Width/2.; // mm (Midlle of BDC1 to target entrance) 
  Double_t Dist_BDC1FDC1 = 3477. + (FDC1_Width - BDC1_Width)/2.; //Distance between the middle of BDC1 and the middle of FDC1 mm
  Double_t Dist_F7F13 = 36617.; // mm
  Double_t Dist_SBTTarget = 2267.; // mm
  const Double_t MassNucleont = 1.674e-27; // kg
  const Double_t ElementaryCharge = 1.60217657e-19; // C
  const Double_t SpeedOfLightUSI = 2.99792458e8; // m/s
  const Double_t SpeedOfLightmmns = 299.792458; // mm/ns

*/
  //HODO Time offset
  vector<Double_t> HODOT0(NUMBER_OF_HODO);
  TEnv *env_hodot0 = new TEnv(env_par->GetValue("HODO_TimeOffset",""));

  for (Int_t ID = 1; ID <= NUMBER_OF_HODO; ID++){
    HODOT0[ID-1] = env_hodot0->GetValue(Form("HODO_T0_DET%d",ID),0.0);
    //    cout << "HODO:"<<ID<<"  "<< HODOT0[ID-1] <<"ns"<< endl;
  }

    TEnv *nebula_layer_1 = new TEnv("/data/RIKEN/DayOne/users/taras/dat/NEBULA_dat_test/NEBULA_1_layer_30_03_2021_tmp.dat");
    TEnv *nebula_layer_4 = new TEnv("/data/RIKEN/DayOne/users/taras/dat/NEBULA_dat_test/NEBULA_4_layer_30_03_2021_tmp.dat");
    double y_shift_1[120] = {0};
    double y_shift_2[120] = {0};
    for (Int_t ID = 1; ID <= 120; ID++)
    {
        y_shift_1[ID-1] = 0; //nebula_layer_1->GetValue(Form("NEBULA_ID[%d]",ID),0.0);
        y_shift_2[ID-1] = 0; //nebula_layer_4->GetValue(Form("NEBULA_ID[%d]",ID),0.0);
    }




/*
   // Definition of observables we want to reconstruct, mod by Taras
   std::cout << "Defining bigrips parameters" << std::endl;  
   TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat","D3"); // F3 - F5
   TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat","D5"); // F5 - F7
   TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat","D7"); // F8 - F9
   TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat","D8"); // F9 - F11  
  */
  // Ajout par Julien L, Correction walk et calibration HODO. Un fichier pour chaque coefficient
  
  vector<Double_t> HODOQAVE(NUMBER_OF_HODO);
  vector<Double_t> HODOZCAL0(NUMBER_OF_HODO);
  vector<Double_t> HODOZCAL1(NUMBER_OF_HODO);
  vector<Double_t> HODOZCAL2(NUMBER_OF_HODO);
  vector<Double_t> HODOZA(NUMBER_OF_HODO);
  vector<Double_t> HODOTA(NUMBER_OF_HODO);
  
  
  TEnv *env_hodo_QAve = new TEnv(env_par->GetValue("HODO_QAve_Cal",""));      // Correction Walk
  TEnv *env_hodo_ZCal0 = new TEnv(env_par->GetValue("HODO_Z_Cal0",""));       // Coeff du polinome pour calibration en Z        coeff a (ordre 2)
  TEnv *env_hodo_ZCal1 = new TEnv(env_par->GetValue("HODO_Z_Cal1",""));       // coeff b (ordre 1)
  TEnv *env_hodo_ZCal2 = new TEnv(env_par->GetValue("HODO_Z_Cal2",""));       // coeff c (ordre 0)
  
  TEnv *env_hodo_ZA = new TEnv(env_par->GetValue("HODO_ZA",""));              // Calibration pas bonne, donc alignement manuel du temps et de la charge
  TEnv *env_hodo_TA = new TEnv(env_par->GetValue("HODO_TA",""));              // 
  
  
  for (Int_t ID = 1; ID <= NUMBER_OF_HODO; ID++){
    
    HODOQAVE[ID-1] = env_hodo_QAve->GetValue(Form("HODO_QAve_Cal_DET%d",ID),0.0);
    HODOZCAL0[ID-1] = env_hodo_ZCal0->GetValue(Form("HODO_Z_PAR0_DET%d",ID),0.0);
    HODOZCAL1[ID-1] = env_hodo_ZCal1->GetValue(Form("HODO_Z_PAR1_DET%d",ID),0.0);
    HODOZCAL2[ID-1] = env_hodo_ZCal2->GetValue(Form("HODO_Z_PAR2_DET%d",ID),0.0);
    HODOZA[ID-1] = env_hodo_ZA->GetValue(Form("HODO_ZA_DET%d",ID),0.0);
    HODOTA[ID-1] = env_hodo_TA->GetValue(Form("HODO_TA_DET%d",ID),0.0);
    
  }

  // Fin ajout

  
  vector<Double_t> NEBULAT0(120);
  TEnv *env_nebt0 = new TEnv(env_par->GetValue("NEBULA_TimeOffset",""));

  for (Int_t ID = 1; ID < 121; ID++){
    NEBULAT0[ID-1] = env_nebt0->GetValue(Form("NEBULA_T0_DET%d",ID),0.0);
    //cout << NEBULAT0[ID-1] << endl;
  }

  vector<Double_t> NeuLANDT0(400);
  TEnv *env_neut0 = new TEnv(env_par->GetValue("NeuLAND_TimeOffset",""));
  for (Int_t ID = 1; ID < 401; ID++){
    NeuLANDT0[ID-1] = env_neut0->GetValue(Form("NEULAND_T0_DET%d",ID),0.0);
    //  cout << NeuLANDT0[ID-1] << endl;
  }
  cout << TString(env_nebt0->GetRcName()).ReplaceAll("/","_")<<endl;
  //  exit(0);
//  Double_t hodo_coeffcalib[NUMBER_OF_HODO][2];
//  Double_t TempA,TempB;
//  std::ifstream HodoCalibFile("/home/psp17/exp/exp1705_psp17/anaroot/users/convdev/zrecal_hodo.dat",std::ios::in);
//  if(HodoCalibFile.is_open())
//    {
//      for (Int_t ID = 0; ID < NUMBER_OF_HODO; ID++)
//	{
//	  HodoCalibFile >> TempA >> TempB;
//	  hodo_coeffcalib[ID][0] = TempA;
//	  hodo_coeffcalib[ID][1] = TempB;
//	}
//      HodoCalibFile.close();
//    }
//  else std::cerr << "Error:: The File : /home/psp17/exp/exp1705_psp17/anaroot/users/convdev/zrecal_hodo.dat was not found" << std::endl;


  TArtEventStore *EventStore = new TArtEventStore;

  if (kRunLive)
    {
      EventStore->Open();
      FileName = "RunLive";
      std::clog << std::endl << std::endl <<  "     Converting live stream... " << std::endl;
      std::clog << "     Data will be stored in " << rootfilesubpath << FileName << ".root" << std::endl << std::endl;
    }
  else if(!EventStore->Open(RidfFilename))
    {
      std::cerr << "\e[34m " << "cannot open " << RidfFilename << "\e[37m" << std::endl << "aborting..." << std::endl;
      return 1;
    }

  const TString FileNameBody = FileName;
  FileNameBody(0,FileNameBody.Last('/')+1) = "";
  const TString RootFilenameBody  = rootfilesubpath + (TString)FileName ;
  const TString TmpRootFilenameBody  =  (TString)"/tmp/" + (TString)FileNameBody ;

  system(TString("rm -vf ")+TmpRootFilenameBody+"*.root");

  // MAIN Tree
  const char *TreeName = "CalTree";

  TFile *fout_MAIN = NULL;
  TTree *tree_MAIN = NULL;
  if(kMAIN){
    fout_MAIN = new TFile(TmpRootFilenameBody + (TString)"_MAIN.root" ,"recreate");
    tree_MAIN = new TTree(TreeName,"Analysis Tree MAIN");
    tree_MAIN->AutoSave("SaveSelf");
    tree_MAIN->SetAutoSave(1e5);
  }

  // NEUT Tree

  TFile *fout_NEUT = NULL;
  TTree *tree_NEUT = NULL;
  if(kNEUT){
    fout_NEUT = new TFile(TmpRootFilenameBody + (TString)"_NEUT.root","recreate");
    tree_NEUT = new TTree(TreeName+(TString)"NEUT","Analysis Tree NEUT");
    tree_NEUT->AutoSave("SaveSelf");
    tree_NEUT->SetAutoSave(1e5);
  }

  // CATANA Tree

  TFile *fout_CATANA = NULL;
  TTree *tree_CATANA = NULL;
  if(kCATANA){
    fout_CATANA = new TFile(TmpRootFilenameBody + (TString)"_CATANA.root","recreate");
    tree_CATANA = new TTree(TreeName+(TString)"CATANA","Analysis Tree CATANA");
    //  tree_CATANA->SetFile(fout_CATANA);
    tree_CATANA->AutoSave("SaveSelf");
    tree_CATANA->SetAutoSave(1e5);
  }

  // FDC Tree

  TFile *fout_FDC = NULL;
  TTree *tree_FDC = NULL;
  if(kFDCs||kFDCs_full){
    fout_FDC = new TFile(TmpRootFilenameBody + (TString)"_FDC.root","recreate");
    tree_FDC = new TTree(TreeName+(TString)"FDC","Analysis Tree FDC");
    //  tree_FDC->SetFile(fout_FDC);
    tree_FDC->AutoSave("SaveSelf");
    tree_FDC->SetAutoSave(1e5);
  }

  // HODO Tree

  TFile *fout_HODO = NULL;
  TTree *tree_HODO = NULL;
  if(kHODO){
    fout_HODO = new TFile(TmpRootFilenameBody + (TString)"_HODO.root","recreate");
    tree_HODO = new TTree(TreeName+(TString)"HODO","Analysis Tree HODO");
    //  tree_FDC->SetFile(fout_FDC);
    tree_HODO->AutoSave("SaveSelf");
    tree_HODO->SetAutoSave(1e5);
  }

  // BEAM Tree

  TFile *fout_BEAM = NULL;
  TTree *tree_BEAM = NULL;
  if(kBEAM){
    fout_BEAM = new TFile(TmpRootFilenameBody + (TString)"_BEAM.root","recreate");
    tree_BEAM = new TTree(TreeName+(TString)"BEAM","Analysis Tree BEAM");
    //  tree_BEAM->SetFile(fout_BEAM);
    tree_BEAM->AutoSave("SaveSelf");
    tree_BEAM->SetAutoSave(1e5);
  }

  // Begining of header

  // Variables
  Long64_t EventNumber = 0;
  Int_t RunNumber = -1;
  Int_t CoincidenceTrigger;

  //MaxEventNumber = 200000;
  Double_t F3_Charge,F5_Charge,F7_Charge,F13_Charge;
  Double_t F3_Time,F5_Time,F7_Time,F13_Time;
  ////////////////////////////////////////////////////////
  Double_t F3_LTCal,F3_RTCal;
  Double_t F5_LTRaw,F7_LTRaw,SBT1_LTRaw,SBT2_LTRaw; //
  Double_t F5_RTRaw,F7_RTRaw,SBT1_RTRaw,SBT2_RTRaw; //
  Double_t F5_LQRaw,F7_LQRaw,SBT1_LQRaw,SBT2_LQRaw; //
  Double_t F5_RQRaw,F7_RQRaw,SBT1_RQRaw,SBT2_RQRaw; //
  Double_t F5_LTCal,F7_LTCal,SBT1_LTCal,SBT2_LTCal; //
  Double_t F5_RTCal,F7_RTCal,SBT1_RTCal,SBT2_RTCal; //
  Double_t F5_LTSlw,F7_LTSlw,SBT1_LTSlw,SBT2_LTSlw; //
  Double_t F5_RTSlw,F7_RTSlw,SBT1_RTSlw,SBT2_RTSlw; //
  Double_t F7_TResolution,F13_TResolution; //
  Double_t TOFF3F7,TOFF7F13; //
  Double_t zetBr, aoqBr, zetBrc, aoqBrc;
   	
  ////////////////////////////////////////////////////////

  Double_t F3_TimeDiff,F5_TimeDiff,F7_TimeDiff,F13_TimeDiff;
  Double_t F31A_X,F31A_Y,F31B_X,F31B_Y,F32A_X,F32A_Y,F32B_X,F32B_Y;
  Double_t F51A_X,F51A_Y,F51B_X,F51B_Y,F52A_X,F52A_Y,F52B_X,F52B_Y;
  Double_t F3X,F3Y,F3B,F3A;
  Double_t F5X,F5Y,F5B,F5A;
  Double_t F7X,F7Y,F7B,F7A;
  Double_t F8X,F8Y,F8B,F8A;
  Double_t F12X,F12Y,F12B,F12A;
  Double_t BDC1_X,BDC1_Y,BDC2_X,BDC2_Y;

  Double_t BDC1_drift_length_0, BDC1_drift_length_1, BDC1_drift_length_2, BDC1_drift_length_3, BDC1_drift_length_4, BDC1_drift_length_5, BDC1_drift_length_6, BDC1_drift_length_7;

  Double_t BDC2_drift_length_0, BDC2_drift_length_1, BDC2_drift_length_2, BDC2_drift_length_3, BDC2_drift_length_4, BDC2_drift_length_5, BDC2_drift_length_6, BDC2_drift_length_7;

  Double_t BDC1_ThetaX,BDC1_ThetaY,BDC2_ThetaX,BDC2_ThetaY;
  Double_t Target_X,Target_Y,Target_ThetaX,Target_ThetaY; // Value at the entrance
  Double_t FDC1_X,FDC1_Y,FDC1_ThetaX,FDC1_ThetaY;
  
Double_t FDC1_drift_length_0, FDC1_drift_length_1, FDC1_drift_length_2, FDC1_drift_length_3, FDC1_drift_length_4, FDC1_drift_length_5, FDC1_drift_length_6, FDC1_drift_length_7, FDC1_drift_length_8, FDC1_drift_length_9, FDC1_drift_length_10, FDC1_drift_length_11, FDC1_drift_length_12, FDC1_drift_length_13;  

  //Double_t FDC0_X,FDC0_Y,FDC0_ThetaX,FDC0_ThetaY;
  Double_t FDC2_X,FDC2_Y,FDC2_ThetaX,FDC2_ThetaY;
Double_t FDC2_drift_length_0, FDC2_drift_length_1, FDC2_drift_length_2, FDC2_drift_length_3, FDC2_drift_length_4, FDC2_drift_length_5, FDC2_drift_length_6, FDC2_drift_length_7, FDC2_drift_length_8, FDC2_drift_length_9, FDC2_drift_length_10, FDC2_drift_length_11, FDC2_drift_length_12, FDC2_drift_length_13;  

  Double_t SAMURAI_Brho,SAMURAI_FL,BetaF7F13,ToF_SBTTarget,GammaF7F13;
  //ICB
  Double_t F7IC_E, ICB_AveEnergyLoss, ICB_AveChargeLoss;

  Double_t AllHodo_Charge[NUMBER_OF_HODO];
  Double_t AllHodo_Time[NUMBER_OF_HODO];
  Double_t AllHodo_TimeU[NUMBER_OF_HODO];
  Double_t AllHodo_TimeD[NUMBER_OF_HODO];
  ////////////////////////////////////////////////////////////////////////////
  Double_t AllHodo_ChargeU[NUMBER_OF_HODO], AllHodo_ChargeD[NUMBER_OF_HODO];
  ////////////////////////////////////////////////////////////////////////////
  Double_t AllHodo_Z[NUMBER_OF_HODO];
  Int_t Hodo_ID; // ID of the hodoscope with the highest charge
  vector <Int_t> *Hodo_ID_vec = new vector <Int_t>;



  Int_t HodoMultiplicity;
  Double_t Hodo_Charge; // Highest charge 
  Double_t Hodo_ChargeP; // Highest charge for Hodo P  
  Double_t Hodo_ChargeF; // Highest charge for Hodo F
  Double_t Hodo_Time; // Time of the hodoscope with the highest charge
  Double_t Hodo_TimeP; // Time of the hodoscope with the highest charge
  Double_t Hodo_TimeF; // Time of the hodoscope with the highest charge
  Double_t Hodo_Z,Hodo_AoverZ;

  Bool_t NEBULA_VetoFired_1;
  Bool_t NEBULA_VetoFired_2;
  Int_t NEBULA_Multiplicity;
  vector <Double_t> *NEBULA_X = new vector <Double_t>();
  vector <Double_t> *NEBULA_Y = new vector <Double_t>();
  vector <Double_t> *NEBULA_Z = new vector <Double_t>();
  vector <Double_t> *NEBULA_ChargeUp = new vector <Double_t>();
  vector <Double_t> *NEBULA_ChargeDown = new vector <Double_t>();
  vector <Double_t> *NEBULA_Charge = new vector <Double_t>();
    
    vector <Double_t> *NEBULA_ChargeUp_veto_1 = new vector <Double_t>();
    vector <Double_t> *NEBULA_ChargeDown_veto_1 = new vector <Double_t>();
    vector <Double_t> *NEBULA_Charge_veto_1 = new vector <Double_t>();
    
    vector <Double_t> *NEBULA_ChargeUp_veto_2 = new vector <Double_t>();
    vector <Double_t> *NEBULA_ChargeDown_veto_2 = new vector <Double_t>();
    vector <Double_t> *NEBULA_Charge_veto_2 = new vector <Double_t>();
    
  vector <Int_t> *NEBULA_ID = new vector <Int_t>();
  vector <Double_t> *NEBULA_Time = new vector <Double_t>();
  vector <Double_t> *NEBULA_TUSlw = new vector <Double_t>();
  vector <Double_t> *NEBULA_TDSlw = new vector <Double_t>();

  vector <Double_t> *NEBULA_Time_veto_1 = new vector <Double_t>();
  vector <Double_t> *NEBULA_TUSlw_veto_1 = new vector <Double_t>();
  vector <Double_t> *NEBULA_TDSlw_veto_1 = new vector <Double_t>();

  vector <Double_t> *NEBULA_Time_veto_2 = new vector <Double_t>();
  vector <Double_t> *NEBULA_TUSlw_veto_2 = new vector <Double_t>();
  vector <Double_t> *NEBULA_TDSlw_veto_2 = new vector <Double_t>();


  Bool_t NEULAND_VetoFired;
  Int_t NEULAND_Multiplicity;
  vector <Double_t> *NEULAND_X = new vector <Double_t>();
  vector <Double_t> *NEULAND_Y = new vector <Double_t>();
  vector <Double_t> *NEULAND_Z = new vector <Double_t>();
  vector <Double_t> *NEULAND_ChargeUp = new vector <Double_t>();
  vector <Double_t> *NEULAND_ChargeDown = new vector <Double_t>();
  vector <Double_t> *NEULAND_Charge = new vector <Double_t>();

  vector <Double_t> *NEULAND_ChargeUp_veto_1 = new vector <Double_t>();
  vector <Double_t> *NEULAND_ChargeDown_veto_1 = new vector <Double_t>();
  vector <Double_t> *NEULAND_Charge_veto_1 = new vector <Double_t>();

  vector <Int_t> *NEULAND_ID = new vector <Int_t>();
  vector <Int_t> *NEULAND_ID_veto = new vector <Int_t>();
  vector <Double_t> *NEULAND_Time = new vector <Double_t>();
  vector <Double_t> *NEULAND_TimeUp = new vector <Double_t>();
  vector <Double_t> *NEULAND_TimeDown = new vector <Double_t>();

  vector <Double_t> *NEULAND_Time_veto = new vector <Double_t>();
  vector <Double_t> *NEULAND_TimeUp_veto = new vector <Double_t>();
  vector <Double_t> *NEULAND_TimeDown_veto = new vector <Double_t>();

  vector <Double_t> *Neutron_X = new vector <Double_t>();
  vector <Double_t> *Neutron_Y = new vector <Double_t>();
  vector <Double_t> *Neutron_Z = new vector <Double_t>();
  vector <Double_t> *Neutron_ChargeUp = new vector <Double_t>();
  vector <Double_t> *Neutron_ChargeDown = new vector <Double_t>();
  vector <Double_t> *Neutron_Charge = new vector <Double_t>();
  vector <Int_t> *Neutron_ID = new vector <Int_t>();
  vector <Double_t> *Neutron_Time = new vector <Double_t>();
  vector <Bool_t>   *Neutron_IsNEBULA = new vector <Bool_t>();
  Int_t Neutron_Multiplicity;

  vector <Double_t> *CATANA_Energy = new vector <Double_t>();
  vector <Double_t> *CATANA_CosTheta = new vector<Double_t>();
  vector <Double_t> *CATANA_Time = new vector<Double_t>();
  // vector <Double_t> *CATANA_EnergyDopplerCorrected = new vector<Double_t>();
  vector <Int_t> *CATANA_ID = new vector <Int_t>();
  vector <Double_t> *CATANA_Layer = new vector<Double_t>();
  vector <Double_t> *CATANA_X = new vector<Double_t>();
  vector <Double_t> *CATANA_Y = new vector<Double_t>();
  vector <Double_t> *CATANA_Z = new vector<Double_t>();
  vector <TVector3> *CATANA_Pos = new vector<TVector3>();
  vector <Int_t> *CATANA_Multiplicity = new vector<Int_t>();
   

  //void Get_Branch_SAMURAI(TChain *tree){ // DO NOT SUPPRESS THIS COMMENT

  if(kMAIN){
    tree_MAIN->Branch("RunNumber",&RunNumber);
    tree_MAIN->Branch("EventNumber",&EventNumber);
    //Triggers
    tree_MAIN->Branch("CoincidenceTrigger",&CoincidenceTrigger);
  }

  //Beam
  if(kBEAM){
    tree_BEAM->Branch("RunNumber",&RunNumber);
    tree_BEAM->Branch("EventNumber",&EventNumber);
    tree_BEAM->Branch("F3_Charge",&F3_Charge);
    tree_BEAM->Branch("F3_Time",&F3_Time);
    tree_BEAM->Branch("F3_TimeDiff",&F3_TimeDiff);
    tree_BEAM->Branch("F5_Charge",&F5_Charge);
    tree_BEAM->Branch("F5_Time",&F5_Time);
    tree_BEAM->Branch("F5_TimeDiff",&F5_TimeDiff);
    tree_BEAM->Branch("F7_Charge",&F7_Charge);
    tree_BEAM->Branch("F7_Time",&F7_Time);
    tree_BEAM->Branch("F7_TimeDiff",&F7_TimeDiff);
    tree_BEAM->Branch("F13_Charge",&F13_Charge);
    tree_BEAM->Branch("F13_Time",&F13_Time);
    tree_BEAM->Branch("F13_TimeDiff",&F13_TimeDiff);

    ///////////////////////////////////////////////////////////////////
//    tree_BEAM->Branch("F5_LTRaw",&F5_LTRaw);
//    tree_BEAM->Branch("F7_LTRaw",&F7_LTRaw);
//    tree_BEAM->Branch("SBT1_LTRaw",&SBT1_LTRaw);
//    tree_BEAM->Branch("SBT2_LTRaw",&SBT2_LTRaw);
//    tree_BEAM->Branch("F5_RTRaw",&F5_RTRaw);
//    tree_BEAM->Branch("F7_RTRaw",&F7_RTRaw);
//    tree_BEAM->Branch("SBT1_RTRaw",&SBT1_RTRaw);
//    tree_BEAM->Branch("SBT2_RTRaw",&SBT2_RTRaw);
    tree_BEAM->Branch("F5_LQRaw",&F5_LQRaw);
    tree_BEAM->Branch("F7_LQRaw",&F7_LQRaw);
    tree_BEAM->Branch("SBT1_LQRaw",&SBT1_LQRaw);
    tree_BEAM->Branch("SBT2_LQRaw",&SBT2_LQRaw);
    tree_BEAM->Branch("F5_RQRaw",&F5_RQRaw);
    tree_BEAM->Branch("F7_RQRaw",&F7_RQRaw);
    tree_BEAM->Branch("SBT1_RQRaw",&SBT1_RQRaw);
    tree_BEAM->Branch("SBT2_RQRaw",&SBT2_RQRaw);
    
//    tree_BEAM->Branch("F3_LTCal",&F3_LTCal);
//    tree_BEAM->Branch("F5_LTCal",&F5_LTCal);
//    tree_BEAM->Branch("F7_LTCal",&F7_LTCal);
//    tree_BEAM->Branch("SBT1_LTCal",&SBT1_LTCal);
//    tree_BEAM->Branch("SBT2_LTCal",&SBT2_LTCal);
//    tree_BEAM->Branch("F3_RTCal",&F3_RTCal);
//    tree_BEAM->Branch("F5_RTCal",&F5_RTCal);
//    tree_BEAM->Branch("F7_RTCal",&F7_RTCal);
//    tree_BEAM->Branch("SBT1_RTCal",&SBT1_RTCal);
//    tree_BEAM->Branch("SBT2_RTCal",&SBT2_RTCal);
//
//    tree_BEAM->Branch("F5_LTSlw",&F5_LTSlw);
//    tree_BEAM->Branch("F7_LTSlw",&F7_LTSlw);
      tree_BEAM->Branch("SBT1_LTSlw",&SBT1_LTSlw);
      tree_BEAM->Branch("SBT2_LTSlw",&SBT2_LTSlw);
//    tree_BEAM->Branch("F5_RTSlw",&F5_RTSlw);
//    tree_BEAM->Branch("F7_RTSlw",&F7_RTSlw);
      tree_BEAM->Branch("SBT1_RTSlw",&SBT1_RTSlw);
      tree_BEAM->Branch("SBT2_RTSlw",&SBT2_RTSlw);
    
    tree_BEAM->Branch("F7_TResolution",&F7_TResolution);
    tree_BEAM->Branch("F13_TResolution",&F13_TResolution);
    tree_BEAM->Branch("TOFF3F7",&TOFF3F7);
    tree_BEAM->Branch("TOFF7F13",&TOFF7F13);
    ///////////////////////////////////////////////////////////////////

    tree_BEAM->Branch("BetaF7F13",&BetaF7F13);
    tree_BEAM->Branch("GammaF7F13",&GammaF7F13);

    tree_BEAM->Branch("zetBr",&zetBr);
    tree_BEAM->Branch("aoqBr",&aoqBr);
    tree_BEAM->Branch("zetBrc",&zetBrc);
    tree_BEAM->Branch("aoqBrc",&aoqBrc);

    tree_BEAM->Branch("F51A_X",&F51A_X);
    tree_BEAM->Branch("F51A_Y",&F51A_Y);
    tree_BEAM->Branch("F51B_X",&F51B_X);
    tree_BEAM->Branch("F51B_Y",&F51B_Y);
    tree_BEAM->Branch("F52A_X",&F52A_X);
    tree_BEAM->Branch("F52A_Y",&F52A_Y);
    tree_BEAM->Branch("F52B_X",&F52B_X);
    tree_BEAM->Branch("F52B_Y",&F52B_Y);

    tree_BEAM->Branch("F3X",&F3X);
    tree_BEAM->Branch("F3Y",&F3Y);
    tree_BEAM->Branch("F3A",&F3A);
    tree_BEAM->Branch("F3B",&F3B);

    tree_BEAM->Branch("F5X",&F5X);
    tree_BEAM->Branch("F5Y",&F5Y);
    tree_BEAM->Branch("F5A",&F5A);
    tree_BEAM->Branch("F5B",&F5B);

    tree_BEAM->Branch("F7X",&F7X);
    tree_BEAM->Branch("F7Y",&F7Y);
    tree_BEAM->Branch("F7A",&F7A);
    tree_BEAM->Branch("F7B",&F7B);

    tree_BEAM->Branch("F8X",&F8X);
    tree_BEAM->Branch("F8Y",&F8Y);
    tree_BEAM->Branch("F8A",&F8A);
    tree_BEAM->Branch("F8B",&F8B);

    tree_BEAM->Branch("F12X",&F12X);
    tree_BEAM->Branch("F12Y",&F12Y);
    tree_BEAM->Branch("F12A",&F12A);
    tree_BEAM->Branch("F12B",&F12B);

    tree_BEAM->Branch("F7IC_E",&F7IC_E);

    tree_BEAM->Branch("BDC1_drift_length_0",&BDC1_drift_length_0);
    tree_BEAM->Branch("BDC1_drift_length_1",&BDC1_drift_length_1);
    tree_BEAM->Branch("BDC1_drift_length_2",&BDC1_drift_length_2);
    tree_BEAM->Branch("BDC1_drift_length_3",&BDC1_drift_length_3);
    tree_BEAM->Branch("BDC1_drift_length_4",&BDC1_drift_length_4);
    tree_BEAM->Branch("BDC1_drift_length_5",&BDC1_drift_length_5);
    tree_BEAM->Branch("BDC1_drift_length_6",&BDC1_drift_length_6);
    tree_BEAM->Branch("BDC1_drift_length_7",&BDC1_drift_length_7);

    tree_BEAM->Branch("BDC2_drift_length_0",&BDC2_drift_length_0);
    tree_BEAM->Branch("BDC2_drift_length_1",&BDC2_drift_length_1);
    tree_BEAM->Branch("BDC2_drift_length_2",&BDC2_drift_length_2);
    tree_BEAM->Branch("BDC2_drift_length_3",&BDC2_drift_length_3);
    tree_BEAM->Branch("BDC2_drift_length_4",&BDC2_drift_length_4);
    tree_BEAM->Branch("BDC2_drift_length_5",&BDC2_drift_length_5);
    tree_BEAM->Branch("BDC2_drift_length_6",&BDC2_drift_length_6);
    tree_BEAM->Branch("BDC2_drift_length_7",&BDC2_drift_length_7);



    tree_BEAM->Branch("BDC1_X",&BDC1_X);
    tree_BEAM->Branch("BDC1_Y",&BDC1_Y);
    tree_BEAM->Branch("BDC1_ThetaX",&BDC1_ThetaX);
    tree_BEAM->Branch("BDC1_ThetaY",&BDC1_ThetaY);
    tree_BEAM->Branch("BDC2_X",&BDC2_X);
    tree_BEAM->Branch("BDC2_Y",&BDC2_Y);
    tree_BEAM->Branch("BDC2_ThetaX",&BDC2_ThetaX);
    tree_BEAM->Branch("BDC2_ThetaY",&BDC2_ThetaY);
    tree_BEAM->Branch("Target_X",&Target_X);
    tree_BEAM->Branch("Target_Y",&Target_Y);
    tree_BEAM->Branch("Target_ThetaX",&Target_ThetaX);
    tree_BEAM->Branch("Target_ThetaY",&Target_ThetaY);

    tree_BEAM->Branch("ICB_AveChargeLoss",&ICB_AveChargeLoss); 
    tree_BEAM->Branch("ICB_AveEnergyLoss",&ICB_AveEnergyLoss);
  }

  //SAMURAI
  if(kFDCs||kFDCs_full){
    tree_FDC->Branch("RunNumber",&RunNumber);
    tree_FDC->Branch("EventNumber",&EventNumber);

     tree_FDC->Branch("FDC1_drift_length_0",&FDC1_drift_length_0);
     tree_FDC->Branch("FDC1_drift_length_1",&FDC1_drift_length_1);
     tree_FDC->Branch("FDC1_drift_length_2",&FDC1_drift_length_2);
     tree_FDC->Branch("FDC1_drift_length_3",&FDC1_drift_length_3);
     tree_FDC->Branch("FDC1_drift_length_4",&FDC1_drift_length_4);
     tree_FDC->Branch("FDC1_drift_length_5",&FDC1_drift_length_5);
     tree_FDC->Branch("FDC1_drift_length_6",&FDC1_drift_length_6);
     tree_FDC->Branch("FDC1_drift_length_7",&FDC1_drift_length_7);
     tree_FDC->Branch("FDC1_drift_length_8",&FDC1_drift_length_8);
     tree_FDC->Branch("FDC1_drift_length_9",&FDC1_drift_length_9);
     tree_FDC->Branch("FDC1_drift_length_10",&FDC1_drift_length_10);
     tree_FDC->Branch("FDC1_drift_length_11",&FDC1_drift_length_11);
     tree_FDC->Branch("FDC1_drift_length_12",&FDC1_drift_length_12);
     tree_FDC->Branch("FDC1_drift_length_13",&FDC1_drift_length_13);

     tree_FDC->Branch("FDC1_X",&FDC1_X);
     tree_FDC->Branch("FDC1_Y",&FDC1_Y);
     tree_FDC->Branch("FDC1_ThetaX",&FDC1_ThetaX);
     tree_FDC->Branch("FDC1_ThetaY",&FDC1_ThetaY);
//    tree_FDC->Branch("FDC0_X",&FDC0_X);
//    tree_FDC->Branch("FDC0_Y",&FDC0_Y);
//    tree_FDC->Branch("FDC0_ThetaX",&FDC0_ThetaX);
//    tree_FDC->Branch("FDC0_ThetaY",&FDC0_ThetaY);


     tree_FDC->Branch("FDC2_drift_length_0",&FDC2_drift_length_0);
     tree_FDC->Branch("FDC2_drift_length_1",&FDC2_drift_length_1);
     tree_FDC->Branch("FDC2_drift_length_2",&FDC2_drift_length_2);
     tree_FDC->Branch("FDC2_drift_length_3",&FDC2_drift_length_3);
     tree_FDC->Branch("FDC2_drift_length_4",&FDC2_drift_length_4);
     tree_FDC->Branch("FDC2_drift_length_5",&FDC2_drift_length_5);
     tree_FDC->Branch("FDC2_drift_length_6",&FDC2_drift_length_6);
     tree_FDC->Branch("FDC2_drift_length_7",&FDC2_drift_length_7);
     tree_FDC->Branch("FDC2_drift_length_8",&FDC2_drift_length_8);
     tree_FDC->Branch("FDC2_drift_length_9",&FDC2_drift_length_9);
     tree_FDC->Branch("FDC2_drift_length_10",&FDC2_drift_length_10);
     tree_FDC->Branch("FDC2_drift_length_11",&FDC2_drift_length_11);
     tree_FDC->Branch("FDC2_drift_length_12",&FDC2_drift_length_12);
     tree_FDC->Branch("FDC2_drift_length_13",&FDC2_drift_length_13);


    tree_FDC->Branch("FDC2_X",&FDC2_X);
    tree_FDC->Branch("FDC2_Y",&FDC2_Y);
    tree_FDC->Branch("FDC2_ThetaX",&FDC2_ThetaX);
    tree_FDC->Branch("FDC2_ThetaY",&FDC2_ThetaY);
    tree_FDC->Branch("SAMURAI_Brho",&SAMURAI_Brho);
    tree_FDC->Branch("SAMURAI_Flight_Length",&SAMURAI_FL);
  }

  if(kHODO){
    tree_HODO->Branch("RunNumber",&RunNumber);
    tree_HODO->Branch("EventNumber",&EventNumber);
       for (Int_t i=0;i<NUMBER_OF_HODO;i++)
      {
	tree_HODO->Branch(Form("Hodo%d_Charge",i+1),&AllHodo_Charge[i]);
	tree_HODO->Branch(Form("Hodo%d_ChargeU",i+1),&AllHodo_ChargeU[i]);
	tree_HODO->Branch(Form("Hodo%d_ChargeD",i+1),&AllHodo_ChargeD[i]);
	tree_HODO->Branch(Form("Hodo%d_Time",i+1),&AllHodo_Time[i]);
	tree_HODO->Branch(Form("Hodo%d_TimeU",i+1),&AllHodo_TimeU[i]);
	tree_HODO->Branch(Form("Hodo%d_TimeD",i+1),&AllHodo_TimeD[i]);
	tree_HODO->Branch(Form("Hodo%d_Z",i+1),&AllHodo_Z[i]);
	} 
    tree_HODO->Branch("Hodo_Multiplicity",&HodoMultiplicity);
    tree_HODO->Branch("Hodo_ID",&Hodo_ID);
    tree_HODO->Branch("Hodo_ID_vec",&Hodo_ID_vec);
    tree_HODO->Branch("Hodo_Charge",&Hodo_Charge);  
    tree_HODO->Branch("Hodo_ChargeP",&Hodo_ChargeP); 
    tree_HODO->Branch("Hodo_ChargeF",&Hodo_ChargeF);
    tree_HODO->Branch("Hodo_Time",&Hodo_Time);
    tree_HODO->Branch("Hodo_TimeP",&Hodo_TimeP);
    tree_HODO->Branch("Hodo_TimeF",&Hodo_TimeF);
    tree_HODO->Branch("Hodo_Z",&Hodo_Z);
    tree_HODO->Branch("Hodo_AoverZ",&Hodo_AoverZ);
  }

  //Neutrons
  if(kNEUT){
    tree_NEUT->Branch("RunNumber",&RunNumber);
    tree_NEUT->Branch("EventNumber",&EventNumber);
    tree_NEUT->Branch("Neutron_X",&Neutron_X);
    tree_NEUT->Branch("Neutron_Y",&Neutron_Y);
    tree_NEUT->Branch("Neutron_Z",&Neutron_Z);
    tree_NEUT->Branch("Neutron_Time",&Neutron_Time);
    tree_NEUT->Branch("Neutron_ID",&Neutron_ID);
    tree_NEUT->Branch("Neutron_ChargeUp",&Neutron_ChargeUp);
    tree_NEUT->Branch("Neutron_ChargeDown",&Neutron_ChargeDown);
    tree_NEUT->Branch("Neutron_Charge",&Neutron_Charge);
    tree_NEUT->Branch("Neutron_IsNEBULA",&Neutron_IsNEBULA);
    tree_NEUT->Branch("Neutron_Multiplicity",&Neutron_Multiplicity);

    //NEBULA
    tree_NEUT->Branch("NEBULA_Multiplicity",&NEBULA_Multiplicity);
    tree_NEUT->Branch("NEBULA_VetoFired_1",&NEBULA_VetoFired_1);
    tree_NEUT->Branch("NEBULA_VetoFired_2",&NEBULA_VetoFired_2);
    tree_NEUT->Branch("NEBULA_ID",&NEBULA_ID);
    tree_NEUT->Branch("NEBULA_X",&NEBULA_X);
    tree_NEUT->Branch("NEBULA_Y",&NEBULA_Y);
    tree_NEUT->Branch("NEBULA_Z",&NEBULA_Z);
    tree_NEUT->Branch("NEBULA_ChargeUp",&NEBULA_ChargeUp);
    tree_NEUT->Branch("NEBULA_ChargeDown",&NEBULA_ChargeDown);
    tree_NEUT->Branch("NEBULA_Charge",&NEBULA_Charge);

      tree_NEUT->Branch("NEBULA_ChargeUp_veto_1", &NEBULA_ChargeUp_veto_1);
      tree_NEUT->Branch("NEBULA_ChargeDown_veto_1", &NEBULA_ChargeDown_veto_1);
      tree_NEUT->Branch("NEBULA_Charge_veto_1", &NEBULA_Charge_veto_1);
      
      tree_NEUT->Branch("NEBULA_ChargeUp_veto_2", &NEBULA_ChargeUp_veto_2);
      tree_NEUT->Branch("NEBULA_ChargeDown_veto_2", &NEBULA_ChargeDown_veto_2);
      tree_NEUT->Branch("NEBULA_Charge_veto_2", &NEBULA_Charge_veto_2);

      
    tree_NEUT->Branch("NEBULA_Time",&NEBULA_Time);
    tree_NEUT->Branch("NEBULA_TUSlw",&NEBULA_TUSlw);
    tree_NEUT->Branch("NEBULA_TDSlw",&NEBULA_TDSlw);

    tree_NEUT->Branch("NEBULA_Time_veto_1", &NEBULA_Time_veto_1);
    tree_NEUT->Branch("NEBULA_TUSlw_veto_1", &NEBULA_TUSlw_veto_1);
    tree_NEUT->Branch("NEBULA_TDSlw_veto_1", &NEBULA_TDSlw_veto_1);

    tree_NEUT->Branch("NEBULA_Time_veto_2", &NEBULA_Time_veto_2);
    tree_NEUT->Branch("NEBULA_TUSlw_veto_2", &NEBULA_TUSlw_veto_2);
    tree_NEUT->Branch("NEBULA_TDSlw_veto_2", &NEBULA_TDSlw_veto_2);



    //NEULAND
    tree_NEUT->Branch("NEULAND_Multiplicity",&NEULAND_Multiplicity);
    tree_NEUT->Branch("NEULAND_VetoFired",&NEULAND_VetoFired);
    tree_NEUT->Branch("NEULAND_ID",&NEULAND_ID);
    tree_NEUT->Branch("NEULAND_ID_veto",&NEULAND_ID_veto);
    tree_NEUT->Branch("NEULAND_X",&NEULAND_X);
    tree_NEUT->Branch("NEULAND_Y",&NEULAND_Y);
    tree_NEUT->Branch("NEULAND_Z",&NEULAND_Z);
    tree_NEUT->Branch("NEULAND_ChargeUp",&NEULAND_ChargeUp);
    tree_NEUT->Branch("NEULAND_ChargeDown",&NEULAND_ChargeDown);
    tree_NEUT->Branch("NEULAND_Charge",&NEULAND_Charge);

    tree_NEUT->Branch("NEULAND_ChargeUp_veto_1",&NEULAND_ChargeUp_veto_1);
    tree_NEUT->Branch("NEULAND_ChargeDown_veto_1",&NEULAND_ChargeDown_veto_1);
    tree_NEUT->Branch("NEULAND_Charge_veto_1",&NEULAND_Charge_veto_1);

    tree_NEUT->Branch("NEULAND_Time",&NEULAND_Time);
    tree_NEUT->Branch("NEULAND_TimeUp",&NEULAND_TimeUp);
    tree_NEUT->Branch("NEULAND_TimeDown",&NEULAND_TimeDown);

    tree_NEUT->Branch("NEULAND_Time_veto",&NEULAND_Time_veto);
    tree_NEUT->Branch("NEULAND_TimeUp_veto",&NEULAND_TimeUp_veto);
    tree_NEUT->Branch("NEULAND_TimeDown_veto",&NEULAND_TimeDown_veto);

  }

  //CATANA
  if(kCATANA){
    tree_CATANA->Branch("RunNumber",&RunNumber);
    tree_CATANA->Branch("EventNumber",&EventNumber);
    tree_CATANA->Branch("CATANA_ID",&CATANA_ID);
    tree_CATANA->Branch("CATANA_Energy",&CATANA_Energy);
    tree_CATANA->Branch("CATANA_Time",&CATANA_Time);
    tree_CATANA->Branch("CATANA_CosTheta",&CATANA_CosTheta);
    tree_CATANA->Branch("CATANA_Layer",&CATANA_Layer);
    tree_CATANA->Branch("CATANA_X",&CATANA_X);
    tree_CATANA->Branch("CATANA_Y",&CATANA_Y);
    tree_CATANA->Branch("CATANA_Z",&CATANA_Z);
    tree_CATANA->Branch("CATANA_Pos",&CATANA_Pos);
    tree_CATANA->Branch("CATANA_Multiplicity",&CATANA_Multiplicity);
    //  tree_CATANA->Branch("CATANA_EnergyDopplerCorrected",&CATANA_EnergyDopplerCorrected);
  }

  // End of header

  // Aliases
  if(kMAIN){
    tree_MAIN->SetAlias("Trig_DSB","CoincidenceTrigger & 1");
    tree_MAIN->SetAlias("Trig_BxNEB","CoincidenceTrigger & 2");
    tree_MAIN->SetAlias("Trig_BxNEU","CoincidenceTrigger & 4");
    tree_MAIN->SetAlias("Trig_BxD","CoincidenceTrigger & 8");
    tree_MAIN->SetAlias("Trig_BxNEB_gamma","CoincidenceTrigger & 8");
    tree_MAIN->SetAlias("Trig_BxNEU_gamma","CoincidenceTrigger & 16");
  }
  if(kNEUT){
    tree_NEUT->SetAlias("Neutron_VetoFired","NEBULA_VetoFired_1 || NEBULA_VetoFired_2 || NEULAND_VetoFired");
    tree_NEUT->SetAlias("Neutron_NoVeto","!Neutron_VetoFired");
  }

  while(EventStore->GetNextEvent() && EventNumber < MaxEventNumber)
//   while(EventStore->GetNextEvent() && EventNumber < 300000)
    {

      EventNumber++;
      if (EventNumber%DISPLAY_EVERY_EVENT == 0){
	double duration =  ((std::clock()-start)/ (double) CLOCKS_PER_SEC) ;
	std::clog << "\e[0m  ANALYSIS-Info : " << EventNumber/1000 << "k events treated in " 
		  << duration << "s. Instantaneous rate = " 
		  << DISPLAY_EVERY_EVENT/((std::clock()-clock0)/ (double) CLOCKS_PER_SEC)  << "evt/s \r";
	clock0 = std::clock();
      }

      
      CalibIC->ClearData();
      CalibCoin->ClearData();
      CalibNEBULA->ClearData();
      CalibNEULAND->ClearData();
      CalibNEULANDVETO->ClearData();
      CalibPlastic->ClearData();
      CalibBDC1Hit->ClearData();
      CalibBDC1Track->ClearData();
      CalibBDC2Hit->ClearData();
      CalibBDC2Track->ClearData();
      CalibFDC1Hit->ClearData();
      //CalibFDC0Hit->ClearData();
      CalibFDC2Hit->ClearData();
      CalibHODPla->ClearData();
      CalibCATANA->ClearData();
      RecoFragment->ClearData();
      CalibPPAC->ClearData();

      if(kFDCs)
      {
        CalibFDC1Track->ClearData();
	//CalibFDC0Track->ClearData();
	CalibFDC2Track->ClearData();
	

	//CalibSimpleFDC1Track->ClearData();
	//CalibSimpleFDC0Track->ClearData();
	//CalibSimpleFDC2Track->ClearData();
      }
      else if(kFDCs_full)
      {
	CalibFDC1Track->ClearData();
	//CalibFDC0Track->ClearData();
	CalibFDC2Track->ClearData();
      }
      //      CalibPID->ClearData();
      CalibFocalPlane->ClearData();


      TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

      for(Int_t i=0;i<fEvent->GetNumSeg();i++){
	TArtRawSegmentObject *seg = fEvent->GetSegment(i);
	Int_t device   = seg->GetDevice();
	Int_t detector = seg->GetDetector();
	
	if(device == SAMURAI){
	  switch(detector) {
	    //kw: plastic appears twice, under device SAMURAI 2 and BigRIPS 0, Samurai comes first, so if we read this in BigRIPS we actually overwrite
	  case    PLAT: if(kBEAM) CalibPlastic->LoadData(seg); break;
	  case STOPPLA: if(kBEAM) CalibPlastic->LoadData(seg); break;
	  case      RF: if(kBEAM) CalibPlastic->LoadData(seg); break;
	  case    PLAQ: if(kBEAM) CalibPlastic->LoadData(seg); break;
	  case     COIN: CalibCoin->LoadData(seg); break; // included to see coin. trigger

	  case       BDC: if(kBEAM || kFDCs||kFDCs_full) CalibBDC1Hit->LoadData(seg); break;
	  case      FDC1: if(kFDCs||kFDCs_full) CalibFDC1Hit->LoadData(seg); break;
	    //case      FDC0: if(kFDCs||kFDCs_full) CalibFDC0Hit->LoadData(seg); break;
	  case      FDC2: if(kFDCs||kFDCs_full) CalibFDC2Hit->LoadData(seg); break;
	  case     HODFQ: if(kHODO) CalibHODPla->LoadData(seg);  break;
	  case     HODPQ: if(kHODO) CalibHODPla->LoadData(seg);  break;
	    //	  case    HODPQ2: if(kHODO) CalibHODPla->LoadData(seg); break;
	  case     HODFT: if(kHODO) CalibHODPla->LoadData(seg);  break;
	  case     HODPT: if(kHODO) CalibHODPla->LoadData(seg);  break;
	  case  NEBULA1Q: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA1T: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA2Q: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA2T: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA3Q: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA3T: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA4Q: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEBULA4T: if(kNEUT) CalibNEBULA->LoadData(seg); break;
	  case  NEULANDVETOQ: if(kNEUT) CalibNEULANDVETO->LoadData(seg); break;
	  case  NEULANDVETOT: if(kNEUT) CalibNEULANDVETO->LoadData(seg); break;
	  case  NEULAND: if(kNEUT) CalibNEULAND->LoadData(seg); break;
	  default:  break;
	  }
	} else if(device== BIGRIPS){
	  switch(detector) {
	  case   PPACT: if(kBEAM) CalibPPAC->LoadData(seg); break;
	  case   PPACQ: if(kBEAM) CalibPPAC->LoadData(seg); break;
	  default:  break;
	  }
	} else {
	  switch(detector) {
	    //case     COIN: CalibCoin->LoadData(seg); break;
	  case    CATANAQ: if(kCATANA) CalibCATANA->LoadData(seg); break;
          case    CATANAT: if(kCATANA) CalibCATANA->LoadData(seg); break;
	  default:  break;
	  }
	}

      }

      // Coincidence reg


      TClonesArray *EventInfo = (TClonesArray*)sman->FindDataContainer("EventInfo");

      CoincidenceTrigger = (Int_t)NAN;
      RunNumber = (Int_t)NAN;
      if(EventInfo){
	CoincidenceTrigger = ((TArtEventInfo *)EventInfo->At(0))->GetTriggerBit();
	RunNumber = ((TArtEventInfo *)EventInfo->At(0))->GetRunNumber() ;
      }

      if(kMAIN) {
	tree_MAIN->Fill();
      }

      //Beam
      if(kBEAM){
	//plastics
	CalibPlastic->ReconstructData();
	TArtPlastic *PlasticF3 = CalibPlastic->FindPlastic((char*)"F3pl");
	TArtPlastic *PlasticF5 = CalibPlastic->FindPlastic((char*)"F5pl");
	TArtPlastic *PlasticF7 = CalibPlastic->FindPlastic((char*)"F7pl");
	TArtPlastic *PlasticSBT1 = CalibPlastic->FindPlastic((char*)"F13pl-1");
	TArtPlastic *PlasticSBT2 = CalibPlastic->FindPlastic((char*)"F13pl-2");

	F3_Charge = NAN;
	F3_Time = NAN;
	F5_Charge = NAN;
	F5_Time = NAN;
	F5_TimeDiff = NAN;
	F7_Charge = NAN;
	F7_Time = NAN;
	F13_Charge = NAN;
	F13_Time = NAN;
	BetaF7F13 = NAN;
	GammaF7F13 = NAN;
	aoqBr = NAN;
        aoqBrc = NAN;
	zetBr = NAN;
	zetBrc = NAN;
	////////////////////////////////////////////////////////////

	F5_LTRaw = NAN;
	F7_LTRaw = NAN;
	SBT1_LTRaw = NAN;
	SBT2_LTRaw = NAN;
	F5_RTRaw = NAN;
	F7_RTRaw = NAN;
	SBT1_RTRaw = NAN;
	SBT2_RTRaw = NAN;
	F5_LQRaw = NAN;
	F7_LQRaw = NAN;
	SBT1_LQRaw = NAN;
	SBT2_LQRaw = NAN;
	F5_RQRaw = NAN;
	F7_RQRaw = NAN;
	SBT1_RQRaw = NAN;
	SBT2_RQRaw = NAN;

	F3_LTCal = NAN;
	F5_LTCal = NAN;
	F7_LTCal = NAN;
	SBT1_LTCal = NAN;
	SBT2_LTCal = NAN;
	F3_RTCal = NAN;
	F5_RTCal = NAN;
	F7_RTCal = NAN;
	SBT1_RTCal = NAN;
	SBT2_RTCal = NAN;

	F5_LTSlw = NAN;
	F7_LTSlw = NAN;
	SBT1_LTSlw = NAN;
	SBT2_LTSlw = NAN;
	F5_RTSlw = NAN;
	F7_RTSlw = NAN;
	SBT1_RTSlw = NAN;
	SBT2_RTSlw = NAN;

	F7_TResolution = NAN;
	F13_TResolution = NAN;
	TOFF3F7 = NAN;
	TOFF7F13 = NAN;

	//cout << "time: " << PlasticF7->GetTLRaw() << "\t" << PlasticF7->GetTRRaw() << ", charge: " << PlasticF7->GetQLRaw() << "\t" << PlasticF7->GetQRRaw() << endl;

	if(PlasticF3){
	  F3_Charge = PlasticF3->GetQAveRaw();
	  //F3_Time = PlasticF3->GetTimeSlew() + 25.91;

	  F3_LTCal = PlasticF3->GetTimeL();
	  F3_RTCal = PlasticF3->GetTimeR();
	  F3_Time  = (F3_LTCal + F3_RTCal)/2.;
	}

	if(PlasticF5){
	  F5_Charge = PlasticF5->GetQAveRaw();
	  //F5_Time = PlasticF5->GetTimeSlew();

	  F5_LTRaw = PlasticF5->GetTLRaw();
	  F5_RTRaw = PlasticF5->GetTRRaw();
	  F5_LQRaw = PlasticF5->GetQLRaw();
	  F5_RQRaw = PlasticF5->GetQRRaw();
	  F5_LTCal = PlasticF5->GetTimeL();
	  F5_RTCal = PlasticF5->GetTimeR();
	  F5_LTSlw = PlasticF5->GetTimeLSlew();
	  F5_RTSlw = PlasticF5->GetTimeRSlew();
	  F5_Time = (F5_LTSlw + F5_RTSlw)/2.+298.472-2.08134;
	}
	
	if(PlasticF7){
	  F7_Charge = PlasticF7->GetQAveRaw();
	  //F7_Time = PlasticF7->GetTimeSlew() - 552.1; // Test of offset to have a ToF F7-F13

	  F7_LTRaw = PlasticF7->GetTLRaw();
	  F7_RTRaw = PlasticF7->GetTRRaw();
	  F7_LQRaw = PlasticF7->GetQLRaw();
	  F7_RQRaw = PlasticF7->GetQRRaw();
	  F7_LTCal = PlasticF7->GetTimeL();
	  F7_RTCal = PlasticF7->GetTimeR();
	  F7_LTSlw = PlasticF7->GetTimeLSlew();
	  F7_RTSlw = PlasticF7->GetTimeRSlew();
	  F7_Time = (F7_LTSlw + F7_RTSlw)/2.+298.472-2.08134;
	  if(F5_Time < -9000.)
	    F5_Time = F7_Time;
	}

	if(PlasticSBT1){

	  SBT1_LTRaw = PlasticSBT1->GetTLRaw();
	  SBT1_RTRaw = PlasticSBT1->GetTRRaw();
	  SBT1_LQRaw = PlasticSBT1->GetQLRaw();
	  SBT1_RQRaw = PlasticSBT1->GetQRRaw();
	  SBT1_LTCal = PlasticSBT1->GetTimeL();
	  SBT1_RTCal = PlasticSBT1->GetTimeR();
	  SBT1_LTSlw = PlasticSBT1->GetTimeLSlew();
	  SBT1_RTSlw = PlasticSBT1->GetTimeRSlew();

	}
	if(PlasticSBT2){

	  SBT2_LTRaw = PlasticSBT2->GetTLRaw();
	  SBT2_RTRaw = PlasticSBT2->GetTRRaw();
	  SBT2_LQRaw = PlasticSBT2->GetQLRaw();
	  SBT2_RQRaw = PlasticSBT2->GetQRRaw();
	  SBT2_LTCal = PlasticSBT2->GetTimeL();
	  SBT2_RTCal = PlasticSBT2->GetTimeR();
	  SBT2_LTSlw = PlasticSBT2->GetTimeLSlew();
	  SBT2_RTSlw = PlasticSBT2->GetTimeRSlew();

	}


//cout<<"Check this line"<<endl;

	if(PlasticSBT1 && PlasticSBT2){
	  F13_Charge = ( PlasticSBT1->GetQAveRaw() + PlasticSBT2->GetQAveRaw() ) / 2.;
	  //F13_Time = ( PlFasticSBT1->GetTimeSlew() + PlasticSBT2->GetTimeSlew() ) / 2.;
	  F13_Time = ((SBT1_LTSlw + SBT1_RTSlw)/2. + (SBT2_LTSlw + SBT2_RTSlw)/2.)/2. + 891.63 - 1.55335;
	}
	Double_t TOF_offset=-55.33;//-19.064;//ns
	//BetaF7F13 = Dist_F7F13 / F7_Time / 300.;
	BetaF7F13 = Dist_F7F13 / ((F13_Time - (F7_Time+F5_Time)/2.)+TOF_offset) / 299.792;//in SAMURAI_4n exp, F7 time = (F5_Time + F7_Time)/2.
	GammaF7F13 = 1./sqrt(1. - BetaF7F13 * BetaF7F13);
	ToF_SBTTarget = Dist_SBTTarget / ( BetaF7F13 * 299.792); // ns

	F7_TResolution = F7_Time - F5_Time;
	F13_TResolution = (SBT1_RTSlw + SBT1_LTSlw)/2. - (SBT2_RTSlw + SBT2_LTSlw)/2.;
	//TOFF3F7 = F7_Time - F3_Time;
	//TOFF7F13 = F13_Time - F7_Time;
	TOFF3F7 = (F7_Time + F5_Time)/2. - F3_Time;
	TOFF7F13 = F13_Time - (F7_Time + F5_Time)/2.+TOF_offset;

//Mod by taras
     
      /*  TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat","8.5198"); // F5 - F7
        TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl", 0); //BIGRIPS TOF

        TArtBeam *beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC");
*/
        Calibbr->ClearData();
        Calibbr->ReconstructData();
        
	if(PlasticF5){
	  if (PlasticF5->GetTLRaw() > 0 && PlasticF5->GetTLRaw()<65536 && PlasticF5->GetTRRaw() > 0 && PlasticF5->GetTRRaw()<65536)
	    {
	      F3_TimeDiff = PlasticF3->GetTimeRSlew() - PlasticF3->GetTimeLSlew();
	      //F5_TimeDiff = PlasticF5->GetTimeRSlew() - PlasticF5->GetTimeLSlew();
	      //F7_TimeDiff = PlasticF7->GetTimeRSlew() - PlasticF7->GetTimeLSlew();
	      //F7_TimeDiff = (PlasticF5->GetTLRaw() + PlasticF5->GetTRRaw())/2. - (PlasticF7->GetTLRaw() + PlasticF7->GetTRRaw())/2.; // for plastic resolution check
	      //F13_TimeDiff = (PlasticSBT1->GetTimeRSlew() - PlasticSBT1->GetTimeLSlew() + PlasticSBT2->GetTimeRSlew() - PlasticSBT2->GetTimeLSlew()) / 2.;
	      //F13_TimeDiff = (PlasticSBT1->GetTLRaw() + PlasticSBT1->GetTRRaw())/2. - (PlasticSBT2->GetTLRaw() + PlasticSBT2->GetTRRaw())/2.; // for plastic resolution check
	      F5_TimeDiff = F5_RTSlw - F5_LTSlw;
	      F7_TimeDiff = F7_RTSlw - F7_LTSlw;
	      F13_TimeDiff = (SBT1_RTSlw - SBT1_LTSlw + SBT2_RTSlw - SBT2_LTSlw)/2.;
	    }
	}

// Mod by taras
/*        TArtBeam *beam_br_35 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
        TArtBeam *beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC"); 

        // PID:
        Double_t tof[2];
   	Double_t beta[4];

   	Double_t zet[4];
   	Double_t aoq[4];
   	Double_t zetc[4];  //Corrected PID
	Double_t aoqc[4];  //Corrected PID

   	Double_t delta[4];

   	tree_BEAM->Branch("tof",tof,"tof[2]/D");
	tree_BEAM->Branch("beta",beta,"beta[4]/D");

   	tree_BEAM->Branch("zet",zet,"zet[4]/D");
   	tree_BEAM->Branch("aoq",aoq,"aoq[4]/D");
   	tree_BEAM->Branch("zetc",zetc,"zetc[4]/D");
   	tree_BEAM->Branch("aoqc",aoqc,"aoqc[4]/D");
   	tree_BEAM->Branch("delta",delta,"delta[4]/D");

        Calibbr->ClearData();
        Calibbr->ReconstructData();

      //Reconstructiong the PID
      recopid->ClearData();
      recopid->ReconstructData();

      //The TOF
      tof[0] = tof3to7->GetTOF();
      //cout<<"time of flight[0] = "<<tof[0]<<endl;
      beta[0] = tof3to7->GetBeta();
      beta[1] = tof3to7->GetBeta();
      //cout<<"beta[0] = "<<beta[0]<<endl;

      tof[1] = tof[0];
      //cout<<"time of flight[1] = "<<tof[1]<<endl;
      
      //The PID
      zet[0] = beam_br_35->GetZet();
      aoq[0] = beam_br_35->GetAoQ();

      zet[1] = beam_br_57->GetZet();
      aoq[1] = beam_br_57->GetAoQ();

     //Corrected PID
      aoqc[0] = aoq[0];
      zetc[0] = zet[0];
      aoqc[1] = aoq[1];
      zetc[1] = zet[1];
*/

	Double_t Z_coeff1 = 1;
	Double_t Z_coeff2 = 1;

  	Double_t clight = 299.792458; // mm/nsec
  	Double_t mnucleon = 931.49432; // MeV

    Double_t beam_brho_mean=8.5198; // Mean Brho at D5 taken from logbook
    Double_t beam_brho = NAN;


//AOQ calculations for BigRIPS
    Double_t par[7];
   // par[0]   =     4.13842;
    par[0]   =     0;
    par[1]   =      0.200357;
    par[2]   =       -0.236757;
    par[3]   =     0.106246;
    par[4]   =     -0.0242094;
    par[5]   =     0.00270171;
    par[6]   =     -0.00011664;

    Double_t f5dt;
   
    Double_t bbeta;
    f5dt=F5_TimeDiff;
    Double_t F5pla_X = NAN;
    
	F5pla_X=par[0]+par[1]*f5dt+par[2]*TMath::Power(f5dt,2)+par[3]*TMath::Power(f5dt,3)+par[4]*TMath::Power(f5dt,4)+par[5]*TMath::Power(f5dt,5)+par[6]*TMath::Power(f5dt,6);
        
        //Double_t de_v = TMath::Log(ionpair*beta*beta) - TMath::Log((1-beta*beta)) - beta*beta;
	zetBr = Z_coeff1*BetaF7F13*TMath::Sqrt(F13_Charge)+Z_coeff2;
        
	//Double_t brho = beam_br_57->GetBrho();

	//beam_brho=beam_brho_mean*(1+F5pla_X/3300.0); //what is 3300.0?
    beam_brho=beam_brho_mean*(1+F5pla_X);
       
    aoqBr =  beam_brho_mean*clight/(mnucleon*BetaF7F13*GammaF7F13);
	aoqBrc = aoqBr - F5pla_X;


	//PPAC (F5)
	F51A_X = NAN;
	F51A_Y = NAN;
	F51B_X = NAN;
	F51B_Y = NAN;
	F52A_X = NAN;
	F52A_Y = NAN;
	F52B_X = NAN;
	F52B_Y = NAN;

   	F3X= NAN; F3A= NAN; F3Y= NAN; F3B= NAN;
   	F5X= NAN; F5A= NAN; F5Y= NAN; F5B= NAN;
   	F7X= NAN; F7A= NAN; F7Y= NAN; F7B= NAN;
   	F12X= NAN; F12A= NAN; F12Y= NAN; F12B= NAN;


	CalibFocalPlane->ReconstructData();
	CalibPPAC->ReconstructData();

	TArtPPAC *PPAC_F51A = CalibPPAC->FindPPAC((char*)"F5PPAC-1A");
	TArtPPAC *PPAC_F51B = CalibPPAC->FindPPAC((char*)"F5PPAC-1B");
	TArtPPAC *PPAC_F52A = CalibPPAC->FindPPAC((char*)"F5PPAC-2A");
	TArtPPAC *PPAC_F52B = CalibPPAC->FindPPAC((char*)"F5PPAC-2B");


	if(PPAC_F51A){
	  F51A_X = PPAC_F51A->GetX();
	//cout<<"Filling F51A_X PPAC data"<<endl;
	  F51A_Y = PPAC_F51A->GetY();
	//cout<<"Filling F51A_Y PPAC data"<<endl;
	}
	if(PPAC_F51B){
	  F51B_X = PPAC_F51B->GetX();
	//cout<<"Filling F51B_X PPAC data"<<endl;
	  F51B_Y = PPAC_F51B->GetY();
	//cout<<"Filling F51B_Y PPAC data"<<endl;
	}
	if(PPAC_F52A){
	  F52A_X = PPAC_F52A->GetX();
	  F52A_Y = PPAC_F52A->GetY();
	}
	if(PPAC_F52B){
	  F52B_X = PPAC_F52B->GetX();
	  F52B_Y = PPAC_F52B->GetY();
	}
	
	// PID
	// ==============================

	
	TArtFocalPlane *tfpl;
	TVectorD *vec;
        
	tfpl = CalibFocalPlane->FindFocalPlane(3);
	F3X = F3Y = F3A = F3B = NAN;
	if(tfpl){
	vec=tfpl->GetOptVector(); F3X=(*vec)(0); F3A=(*vec)(1); F3Y=(*vec)(2); F3B=(*vec)(3);
	//cout<<"Filling F3 PPAC data"<<endl;
	}

        tfpl = CalibFocalPlane->FindFocalPlane(5);
	F5X = F5Y = F5A = F5B = NAN;
	if(tfpl){
	vec=tfpl->GetOptVector(); F5X=(*vec)(0); F5A=(*vec)(1); F5Y=(*vec)(2); F5B=(*vec)(3);
	//cout<<"Filling F5 PPAC data"<<endl;
	}

        tfpl = CalibFocalPlane->FindFocalPlane(7);
	F7X = F7Y = F7A = F7B = NAN;
	if(tfpl){
	vec=tfpl->GetOptVector(); F7X=(*vec)(0); F7A=(*vec)(1); F7Y=(*vec)(2); F7B=(*vec)(3);
	//cout<<"Filling F7 PPAC data"<<endl;
	}

        tfpl = CalibFocalPlane->FindFocalPlane(8);
	F8X = F8Y = F8A = F8B = NAN;
	if(tfpl){
	vec=tfpl->GetOptVector(); F8X=(*vec)(0); F8A=(*vec)(1); F8Y=(*vec)(2); F8B=(*vec)(3);
	//cout<<"Filling F8 PPAC data"<<endl;
	}

        tfpl = CalibFocalPlane->FindFocalPlane(12);
	F12X = F12Y = F12A = F12B = NAN;
	if(tfpl){
	vec=tfpl->GetOptVector(); F12X=(*vec)(0); F12A=(*vec)(1); F12Y=(*vec)(2); F12B=(*vec)(3);
	//cout<<"Filling F12 PPAC data"<<endl;
	}

	// ============== ICB ===========

        ICB_AveChargeLoss = -9999.;
        ICB_AveEnergyLoss = -9999.;

        CalibIC->ReconstructData();
        TArtIC *IonChamber = CalibIC->FindIC((char*)"ICB");

        if(IonChamber){
	ICB_AveChargeLoss = IonChamber->GetRawADCAvSum();
	ICB_AveEnergyLoss = IonChamber->GetEnergyAvSum();	
        }


	/*
	CalibIC->ReconstructData();
	TArtIC *BeamIonChamb = CalibIC->FindIC((char*)"F7IC");

	F7IC_E = NAN;
	if(BeamIonChamb) {
	  F7IC_E = BeamIonChamb->GetEnergyAvSum();
	}

        */
	
      }
      
      if(kBEAM || kFDCs||kFDCs_full){
	//BDC1
	BDC1_X = NAN;
	BDC1_Y = NAN;
	BDC1_ThetaX = NAN;
	BDC1_ThetaY = NAN;

 	BDC1_drift_length_0 = NAN;
 	BDC1_drift_length_1 = NAN;
 	BDC1_drift_length_2 = NAN;
 	BDC1_drift_length_3 = NAN;
 	BDC1_drift_length_4 = NAN;
 	BDC1_drift_length_5 = NAN;
 	BDC1_drift_length_6 = NAN;
 	BDC1_drift_length_7 = NAN;

	CalibBDC1Hit->ReconstructData();
	CalibBDC1Track->ReconstructData();
	
	TClonesArray *BDC1Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
	if (BDC1Tracks)
	  {
	    Int_t BDC1NumberOfTracks = BDC1Tracks->GetEntries();
	    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x = 1e6, MinChi2y = 1e6;

	    if (BDC1NumberOfTracks > 0)
	      {
		TArtDCTrack *TrackBDC1;

		for(Int_t i=0; i<BDC1NumberOfTracks; i++)
		  {
		    TrackBDC1 = (TArtDCTrack *)BDC1Tracks->At(i);

		    if(TrackBDC1)
		      {
			BDC1_drift_length_0 = TrackBDC1->GetDriftLength(0);
 			BDC1_drift_length_1 = TrackBDC1->GetDriftLength(1);
		 	BDC1_drift_length_2 = TrackBDC1->GetDriftLength(2);
		 	BDC1_drift_length_3 = TrackBDC1->GetDriftLength(3);
		 	BDC1_drift_length_4 = TrackBDC1->GetDriftLength(4);
		 	BDC1_drift_length_5 = TrackBDC1->GetDriftLength(5);
		 	BDC1_drift_length_6 = TrackBDC1->GetDriftLength(6);
		 	BDC1_drift_length_7 = TrackBDC1->GetDriftLength(7);

			TempXPosition = TrackBDC1->GetPosition(0);
			TempYPosition = TrackBDC1->GetPosition(1);
			TempChi2 = TrackBDC1->GetChi2() / (Double_t)TrackBDC1->GetNDF();

			if(TempChi2 > 0 ){

			  if(TMath::Abs(TempXPosition) < 5000 && TempChi2<MinChi2x){
			    BDC1_X = TempXPosition;
			    BDC1_ThetaX = TMath::ATan(TrackBDC1->GetAngle(0));
			    MinChi2x = TempChi2;
			  }

			  if (TMath::Abs(TempYPosition) < 5000 && TempChi2<MinChi2y){
			    BDC1_Y = TempYPosition;
			    BDC1_ThetaY = TMath::ATan(TrackBDC1->GetAngle(1));
			    MinChi2y = TempChi2;
			  }
			}
		      }
		  }
	      }
	  }
	
	//BDC2
	BDC2_X = NAN;
	BDC2_Y = NAN;

 	BDC2_drift_length_0 = NAN;
 	BDC2_drift_length_1 = NAN;
 	BDC2_drift_length_2 = NAN;
 	BDC2_drift_length_3 = NAN;
 	BDC2_drift_length_4 = NAN;
 	BDC2_drift_length_5 = NAN;
 	BDC2_drift_length_6 = NAN;
 	BDC2_drift_length_7 = NAN;

	BDC2_ThetaX = NAN;
	BDC2_ThetaY = NAN;

	CalibBDC2Hit->ReconstructData();
	CalibBDC2Track->ReconstructData();

	TClonesArray *BDC2Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
	if (BDC2Tracks)
	  {
	    Int_t BDC2NumberOfTracks = BDC2Tracks->GetEntries();
	    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x=1e6, MinChi2y=1e6;

	    if (BDC2NumberOfTracks > 0)
	      {
		TArtDCTrack *TrackBDC2;

		for(Int_t i=0; i<BDC2NumberOfTracks; i++)
		  {
		    TrackBDC2 = (TArtDCTrack *)BDC2Tracks->At(i);

		    if(TrackBDC2)
		      {

			BDC2_drift_length_0 = TrackBDC2->GetDriftLength(0);
 			BDC2_drift_length_1 = TrackBDC2->GetDriftLength(1);
		 	BDC2_drift_length_2 = TrackBDC2->GetDriftLength(2);
		 	BDC2_drift_length_3 = TrackBDC2->GetDriftLength(3);
		 	BDC2_drift_length_4 = TrackBDC2->GetDriftLength(4);
		 	BDC2_drift_length_5 = TrackBDC2->GetDriftLength(5);
		 	BDC2_drift_length_6 = TrackBDC2->GetDriftLength(6);
		 	BDC2_drift_length_7 = TrackBDC2->GetDriftLength(7);

			TempXPosition = TrackBDC2->GetPosition(0);
			TempYPosition = TrackBDC2->GetPosition(1);
			TempChi2 = TrackBDC2->GetChi2() / (Double_t)TrackBDC2->GetNDF();

			if(TempChi2 > 0 ){
			  if(TMath::Abs(TempXPosition) < 5000 && TempChi2<MinChi2x){
			    BDC2_X = TempXPosition;
			    BDC2_ThetaX = TMath::ATan(TrackBDC2->GetAngle(0));
			    MinChi2x = TempChi2;
			  }
			  if (TMath::Abs(TempYPosition) < 5000 && TempChi2<MinChi2y){
			    BDC2_Y = TempYPosition;
			    BDC2_ThetaY = TMath::ATan(TrackBDC2->GetAngle(1));
			    MinChi2y = TempChi2;
			  }
			}
		      }
		  }
	      }
	  }

	//TARGET
	Target_X = NAN;
	Target_Y = NAN;
	Target_ThetaX = NAN;
	Target_ThetaY = NAN;

	Target_X = BDC1_X + Dist_BDC1Target / Dist_BDC1BDC2 * (BDC2_X - BDC1_X);
	Target_Y = BDC1_Y + Dist_BDC1Target / Dist_BDC1BDC2 * (BDC2_Y - BDC1_Y);
	Target_ThetaX = TMath::ATan( (BDC2_X - BDC1_X) / Dist_BDC1BDC2 );
	Target_ThetaY = TMath::ATan( (BDC2_Y - BDC1_Y) / Dist_BDC1BDC2 );

	if(kBEAM){
	  tree_BEAM ->Fill();
	}

        //Reconstruction of IC observables for ID
       

      } // END BEAM ANALYSIS
      
      
      if(kFDCs||kFDCs_full){
	// ============== FDC0 ================
	/*
	FDC0_X = NAN;
	FDC0_Y = NAN;
	FDC0_ThetaX = NAN;
	FDC0_ThetaY = NAN;

	if(kFDCs){
	  CalibFDC0Hit->ReconstructData();
	  //	  CalibSimpleFDC0Track->ReconstructData();
	} else if(kFDCs_full){
	  CalibFDC0Hit->ReconstructData();
	  CalibFDC0Track->ReconstructData();
	}


	TClonesArray *FDC0Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC0Track");
	if (FDC0Tracks)
	  {
	    Int_t FDC0NumberOfTracks = FDC0Tracks->GetEntries();
	    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x=1e6, MinChi2y=1e6;

	    if (FDC0NumberOfTracks > 0)
	      {
		TArtDCTrack *TrackFDC0;

		for(Int_t i=0; i<FDC0NumberOfTracks; i++)
		  {
		    TrackFDC0 = (TArtDCTrack *)FDC0Tracks->At(i);

		    if(TrackFDC0)
		      {
			TempXPosition = TrackFDC0->GetPosition(0);
			TempYPosition = TrackFDC0->GetPosition(1);
			TempChi2 = TrackFDC0->GetChi2() / (Double_t)TrackFDC0->GetNDF();

			if(TempChi2 > 0 ){
			  if(TMath::Abs(TempXPosition) < 5000 && TempChi2<MinChi2x){
			    FDC0_X = TempXPosition;
			    MinChi2x = TempChi2;
			  }
			  if (TMath::Abs(TempYPosition) < 5000 && TempChi2<MinChi2y){
			    FDC0_Y = TempYPosition;
			    MinChi2y = TempChi2;
			  }
			}
		      }
		  }

		if( TMath::Abs(Target_X)<100 && // 100mm for safety (can be used in most of the experiment ! to be ajusted ?
		    TMath::Abs(Target_Y)<100 && // 100mm for safety (can be used in most of the experiment ! to be ajusted ?
		    TMath::Abs(FDC0_X) < 5000 && TMath::Abs(FDC0_Y) < 5000 )
		  {
		    FDC0_ThetaX = TMath::ATan((FDC0_X - Target_X) / (Dist_BDC1FDC0-Dist_BDC1Target));
		    FDC0_ThetaY = TMath::ATan((FDC0_Y - Target_Y) / (Dist_BDC1FDC0-Dist_BDC1Target));
		  }
	      }
	  }

	*/
	// ============== FDC1 ================
 	FDC1_X = NAN;
 	FDC1_Y = NAN;
        FDC1_drift_length_0 = NAN;
	FDC1_drift_length_1 = NAN;
	FDC1_drift_length_2 = NAN;
	FDC1_drift_length_3 = NAN;
	FDC1_drift_length_4 = NAN;
	FDC1_drift_length_5 = NAN;
	FDC1_drift_length_6 = NAN;
	FDC1_drift_length_7 = NAN;
	FDC1_drift_length_8 = NAN;
	FDC1_drift_length_9 = NAN;
	FDC1_drift_length_10 = NAN;
	FDC1_drift_length_11 = NAN;
	FDC1_drift_length_12 = NAN;
	FDC1_drift_length_13 = NAN;
 	FDC1_ThetaX = NAN;
 	FDC1_ThetaY = NAN;

 	CalibFDC1Hit->ReconstructData();        
	if(kFDCs){
	CalibFDC1Track->ReconstructData();
        //cout<<"********* reconstruction is done 1*************"<<endl;
	  // 	CalibSimpleFDC1Track->ReconstructData();
	} else if(kFDCs_full) {
	  CalibFDC1Track->ReconstructData();
        //cout<<"********* reconstruction is done 2*************"<<endl;
	}
	
 	TClonesArray *FDC1Tracks = (TClonesArray *)sman->FindDataContainer((char*)"SAMURAIFDC1Track");

 	TClonesArray *FDC1Hits = (TClonesArray *)sman->FindDataContainer((char*)"SAMURAIFDC1Hit");  //Added by Taras to check FDC1_Y data hit by hit

 	Int_t FDC1NumberOfTracks = -1;
 	Double_t Chi2_FDC1 = -1.;

 	if(FDC1Tracks) {
 	  FDC1NumberOfTracks = FDC1Tracks->GetEntries();
	  //cout<<"FDC1Tracks->GetEntries() = "<<FDC1Tracks->GetEntries()<<endl;
 	  if(FDC1NumberOfTracks > 0) { //Tracks are already sorted by chi2
 	    TArtDCTrack *FDC1Track = (TArtDCTrack *)FDC1Tracks->At(0);

	     TArtDCHit *FDC1Track_1 = (TArtDCHit *)FDC1Hits->At(0); //Added by Taras to check FDC1_Y data hit by hit


 	    if(FDC1Track) {
	     //cout<<"			"<<endl;
	     //cout<<"Number of hits per layer = "<<FDC1Track->GetNumHitLayer()<<endl;

	     FDC1_drift_length_0 = FDC1Track->GetDriftLength(0);
	     FDC1_drift_length_1 = FDC1Track->GetDriftLength(1);
	     FDC1_drift_length_2 = FDC1Track->GetDriftLength(2);
	     FDC1_drift_length_3 = FDC1Track->GetDriftLength(3);
	     FDC1_drift_length_4 = FDC1Track->GetDriftLength(4);
	     FDC1_drift_length_5 = FDC1Track->GetDriftLength(5);
	     FDC1_drift_length_6 = FDC1Track->GetDriftLength(6);
	     FDC1_drift_length_7 = FDC1Track->GetDriftLength(7);
	     FDC1_drift_length_8 = FDC1Track->GetDriftLength(8);
	     FDC1_drift_length_9 = FDC1Track->GetDriftLength(9);
	     FDC1_drift_length_10 = FDC1Track->GetDriftLength(10);
	     FDC1_drift_length_11 = FDC1Track->GetDriftLength(11);
	     FDC1_drift_length_12 = FDC1Track->GetDriftLength(12);
	     FDC1_drift_length_13 = FDC1Track->GetDriftLength(13);

 	      FDC1_X = FDC1Track->GetPosition(0);
 	      FDC1_Y = FDC1Track->GetPosition(1);

//GetWireZPosition


/*
              cout<<"FDC1Track->GetPosition(1) (central value) = "<<FDC1Track->GetPosition(1)<<endl;
	      for(int i=0; i<FDC1Track->GetNumHitLayer(); i++)
	      {
	      	cout<<"GetHitZPosition("<<i<<") (position layer by layer) = "<<FDC1Track->GetHitZPosition(i)<<endl;
	      }
		cout<<"		"<<endl;
	      for(int i=0; i<FDC1Track->GetNumHitLayer(); i++)
	      {
	      	cout<<"GetWireZPosition("<<i<<") (position layer by layer) = "<<FDC1Track_1->GetWireZPosition()<<endl;
	      }
		cout<<"		"<<endl;
	      for(int i=0; i<FDC1Track->GetNumHitLayer(); i++)
	      {
	      	cout<<"GetWireZPosition("<<i<<") (position layer by layer) = "<<FDC1Track_1->GetWirePosition()<<endl;
	      }
		cout<<"		"<<endl;
		cout<<"FDC1Track->GetPosition(0) (central value) = "<<FDC1Track->GetPosition(0)<<endl;
	      for(int i=0; i<FDC1Track->GetNumHitLayer(); i++)
	      {
	      	cout<<"GetHitXPosition("<<i<<") (position layer by layer) = "<<FDC1Track->GetHitXPosition(i)<<";	GetHitID("<<i<<") = "<<FDC1Track->GetHitID(i)<<endl;
	      }
*/


 

 	      Chi2_FDC1 = FDC1Track->GetChi2() / (Double_t)FDC1Track->GetNDF();


 	    }

 	    if( FDC1NumberOfTracks > 0 &&
 		TMath::Abs(Target_X)<100 && // 100mm for safety (can be used in most of the experiment ! to be ajusted ?
 		TMath::Abs(Target_Y)<100 && // 100mm for safety (can be used in most of the experiment ! to be ajusted ?
 		TMath::Abs(FDC1_X) < 5000 && TMath::Abs(FDC1_Y) < 5000 )
 	      {
 		FDC1_ThetaX = TMath::ATan((FDC1_X - Target_X) / (Dist_BDC1FDC1-Dist_BDC1Target));
 		FDC1_ThetaY = TMath::ATan((FDC1_Y - Target_Y) / (Dist_BDC1FDC1-Dist_BDC1Target));
 	      }
 	  }
 	}

	// ============== FDC2 ================


	FDC2_X = NAN;
	FDC2_Y = NAN;

        FDC2_drift_length_0 = NAN;
	FDC2_drift_length_1 = NAN;
	FDC2_drift_length_2 = NAN;
	FDC2_drift_length_3 = NAN;
	FDC2_drift_length_4 = NAN;
	FDC2_drift_length_5 = NAN;
	FDC2_drift_length_6 = NAN;
	FDC2_drift_length_7 = NAN;
	FDC2_drift_length_8 = NAN;
	FDC2_drift_length_9 = NAN;
	FDC2_drift_length_10 = NAN;
	FDC2_drift_length_11 = NAN;
	FDC2_drift_length_12 = NAN;
	FDC2_drift_length_13 = NAN;

	FDC2_ThetaX = NAN;
	FDC2_ThetaY = NAN;

	if(kFDCs){
	  CalibFDC2Hit->ReconstructData();
	  CalibFDC2Track->ReconstructData();
	  //	  CalibSimpleFDC2Track->ReconstructData();
	} else if(kFDCs_full){
	  CalibFDC2Hit->ReconstructData();
	  CalibFDC2Track->ReconstructData();
	}

	TClonesArray *FDC2Tracks = (TClonesArray *)sman->FindDataContainer((char*)"SAMURAIFDC2Track");
	Int_t FDC2NumberOfTracks = -1;
	if(FDC2Tracks) {
	 // cout<<"Get number of FD2 tracks:";
	  FDC2NumberOfTracks = FDC2Tracks->GetEntries();
	 // cout<<FDC2NumberOfTracks<<endl;
	  if(FDC2NumberOfTracks > 0) { //Tracks are already sorted by chi2
	    TArtDCTrack *FDC2Track = (TArtDCTrack *)FDC2Tracks->At(0);
	    if(FDC2Track) {
	//	cout<<"Getting FDC2 data"<<endl;

	      FDC2_drift_length_0 = FDC2Track->GetDriftLength(0);
	      FDC2_drift_length_1 = FDC2Track->GetDriftLength(1);
	      FDC2_drift_length_2 = FDC2Track->GetDriftLength(2);
	      FDC2_drift_length_3 = FDC2Track->GetDriftLength(3);
	      FDC2_drift_length_4 = FDC2Track->GetDriftLength(4);
	      FDC2_drift_length_5 = FDC2Track->GetDriftLength(5);
	      FDC2_drift_length_6 = FDC2Track->GetDriftLength(6);
	      FDC2_drift_length_7 = FDC2Track->GetDriftLength(7);
	      FDC2_drift_length_8 = FDC2Track->GetDriftLength(8);
	      FDC2_drift_length_9 = FDC2Track->GetDriftLength(9);
	      FDC2_drift_length_10 = FDC2Track->GetDriftLength(10);
	      FDC2_drift_length_11 = FDC2Track->GetDriftLength(11);
	      FDC2_drift_length_12 = FDC2Track->GetDriftLength(12);
 	      FDC2_drift_length_13 = FDC2Track->GetDriftLength(13);

	      FDC2_X = FDC2Track->GetPosition(0);
	      FDC2_Y = FDC2Track->GetPosition(1);
	      FDC2_ThetaX = TMath::ATan(FDC2Track->GetAngle(0));
	      FDC2_ThetaY = TMath::ATan(FDC2Track->GetAngle(1));

	    }
	  }
	}


	// ============== Reconstruct Brho in SAMURAI ==============

	SAMURAI_Brho = NAN;
	if(TMath::Abs(FDC1_X) < 5000 && TMath::Abs(FDC1_Y) < 5000  &&
//	if(TMath::Abs(FDC0_X) < 5000 && TMath::Abs(FDC0_Y) < 5000  &&
	   TMath::Abs(FDC2_X) < 5000 && TMath::Abs(FDC2_Y) < 5000)
	  {
	   // 	    RecoFragment->ReconstructData();
	   // 	    TArtFragment *Fragment = RecoFragment->GetFragment(0);
	   // 	    if (Fragment){
	    Double_t x[] = {FDC1_X,FDC1_Y,FDC1_ThetaX,FDC1_ThetaY,FDC2_X,FDC2_ThetaX};
	    SAMURAI_Brho = rigidity(x);
	    SAMURAI_FL   = length(x);
	  //  SAMURAI_Brho = Fragment->GetBrho();
	   // SAMURAI_FL = Fragment->GetPathLengthToHOD(); }
	    //	    cout << SAMURAI_Brho << endl;
	  }

	tree_FDC  ->Fill();
      } // END FDCs ANALYSIS

      if(kHODO) {
	// ============== Hodoscope data ==============
	for (Int_t i=0;i<NUMBER_OF_HODO;i++)
	  {
	    AllHodo_Charge[i] = NAN;
	    AllHodo_ChargeU[i] = NAN;
	    AllHodo_ChargeD[i] = NAN;
	    AllHodo_Time[i] = NAN;
	    AllHodo_TimeU[i] = NAN;
	    AllHodo_TimeD[i] = NAN;
	    AllHodo_Z[i] = NAN;
	  }

        HodoMultiplicity = 0;
	Hodo_ID = -1;
	Hodo_ID_vec->clear();
	Hodo_Time = NAN;
	Hodo_TimeP = NAN;
	Hodo_TimeF = NAN;
	Hodo_Charge = NAN;	
	Hodo_ChargeP = NAN;	
	Hodo_ChargeF = NAN;
	Hodo_Z = NAN;
	Hodo_AoverZ = NAN;

	CalibHODPla->ReconstructData();

	vector<Int_t> HodoHitID;
	vector<Double_t> HodoHitCharge;
	vector<Double_t> HodoHitChargeU;
	vector<Double_t> HodoHitChargeD;
	vector<Double_t> HodoHitTime;
	vector<Double_t> HodoHitTimeU;
	vector<Double_t> HodoHitTimeD;

	if(CalibHODPla) {
	   HodoMultiplicity = CalibHODPla->GetNumHODPla();
	   
	  if(HodoMultiplicity != 0)
	    {
	      
	      for(Int_t i=0; i<HodoMultiplicity; i++)
		{
		  TArtHODPla *HODPla = CalibHODPla->GetHODPla(i);
		  //cout<<"test "<<HODPla->GetTDRaw()<<endl;;
	
		  if(HODPla->GetID()>=1&&HODPla->GetID()<=NUMBER_OF_HODO) //Only consider HODOF24--2017-05-10
		    { 
		      Hodo_ID_vec->push_back(HODPla->GetID());
		      HodoHitID.push_back(HODPla->GetID());
		      HodoHitCharge.push_back(HODPla->GetQAveCal());
		      HodoHitChargeU.push_back(HODPla->GetQURaw());
		      HodoHitChargeD.push_back(HODPla->GetQDRaw());
		      HodoHitTime.push_back(HODPla->GetTime());
		      HodoHitTimeU.push_back(HODPla->GetTimeU());
		      HodoHitTimeD.push_back(HODPla->GetTimeD());
		      //-----------Beyond the demension-------------
		      if(HodoHitCharge.back() > 0 &&
			 HodoHitCharge.back() < 4095 &&
			 HodoHitTime.back() > -1000 &&
			 HodoHitTime.back() < 4095)
			{
			  AllHodo_Charge[HodoHitID.back()-1] = HodoHitCharge.back();// * hodo_coeffcalib[HodoHitID.back()-1][0] + hodo_coeffcalib[HodoHitID.back()-1][1];
			  AllHodo_ChargeU[HodoHitID.back()-1] = HodoHitChargeU.back();// 
			  AllHodo_ChargeD[HodoHitID.back()-1] = HodoHitChargeD.back();// 
			  AllHodo_Time[HodoHitID.back()-1] = HodoHitTime.back();// - F13_Time;
			  AllHodo_TimeU[HodoHitID.back()-1] = HodoHitTimeU.back();
			  AllHodo_TimeD[HodoHitID.back()-1] = HodoHitTimeD.back();
			}

		      else
			{
			  HodoHitID.pop_back();
			  HodoHitCharge.pop_back();
			  HodoHitChargeU.pop_back();
			  HodoHitChargeD.pop_back();
			  HodoHitTime.pop_back(); //
			  HodoHitTimeU.pop_back(); //
			  HodoHitTimeD.pop_back(); //
			}
		    }
		}
	      
	      //////////////////////////////////////////////////////////////////////////////////////////////////////
	      if(HodoMultiplicity>1){
		for(Int_t i=0; i<HodoMultiplicity; i++){
		  TArtHODPla *HODPla = CalibHODPla->GetHODPla(i);
		  if(HODPla->GetID()==25){ // when ID=25 has a hit, check ID=26
		    if(AllHodo_Charge[25]>0 && AllHodo_Charge[25]<4095 && AllHodo_Time[25]>-1000 && AllHodo_Time[25]<4095){ 

		    AllHodo_Charge[47] = AllHodo_Charge[24];
		    AllHodo_ChargeU[47] = AllHodo_ChargeU[24]; 
		    AllHodo_ChargeD[47] = AllHodo_ChargeD[24]; 
		    AllHodo_Time[47] = AllHodo_Time[24]; 

		    //AllHodo_Charge[24] = NAN;
		    //AllHodo_ChargeU[24] = NAN;
		    //AllHodo_ChargeD[24] = NAN;
		    //AllHodo_Time[24] = NAN;
		    AllHodo_Charge[25] = NAN;
		    AllHodo_ChargeU[25] = NAN;
		    AllHodo_ChargeD[25] = NAN;
		    AllHodo_Time[25] = NAN;

		    break;
		    }}}}
	      //////////////////////////////////////////////////////////////////////////////////////////////////////

	      Int_t RealMultiplicity = HodoHitID.size();
	      
	      if(RealMultiplicity != 0) {


//Comment by Taras for checking the Brho scan run

		/*if(RealMultiplicity > 1) {
		  SortHodoHit(HodoHitID,HodoHitCharge);
		}
		Hodo_ID = HodoHitID.at(0);
*/





		/***************** Ajout par Julien L ************************/
        
/*      
		Hodo_Time = AllHodo_Time[Hodo_ID-1] + HODOT0[Hodo_ID-1];
		Hodo_TimeP = AllHodo_TimeU[Hodo_ID-1] + HODOT0[Hodo_ID-1];
		Hodo_TimeF = AllHodo_TimeD[Hodo_ID-1] + HODOT0[Hodo_ID-1];	  
*/


//Calibration of Hodo time is done with Analysis file.

		Hodo_Time = AllHodo_Time[Hodo_ID-1];
		Hodo_TimeP = AllHodo_TimeU[Hodo_ID-1];
		Hodo_TimeF = AllHodo_TimeD[Hodo_ID-1];	  


		Hodo_Charge = AllHodo_Charge[Hodo_ID-1];


//Hodo_Charge = AllHodo_Charge[Hodo_ID-1] - Hodo_Time*HODOQAVE[Hodo_ID-1];   // Commentaire fait pour plot rapport
		Hodo_Z = Hodo_Charge*Hodo_Charge*HODOZCAL0[Hodo_ID-1] + Hodo_Charge*HODOZCAL1[Hodo_ID-1] + HODOZCAL2[Hodo_ID-1]; //Check the coefficients



//		Hodo_Time = Hodo_Time + HODOTA[Hodo_ID-1];
    
		Hodo_Z = Hodo_Z + HODOZA[Hodo_ID-1];

		
		/*********************************************** Fin de l'ajout ************************************************************/
		  
		// Hodo_Charge = AllHodo_Charge[Hodo_ID-1];
		// //		cout << Hodo_ID << " " << Hodo_Charge << " " ;
        
		// //              cout << Hodo_Time << " " << HODOT0[Hodo_ID-1] << endl ;
		// //if(AllHodo_Time[Hodo_ID-1]>0 )
		// //cout << Hodo_ID<<" "<<Hodo_Time << " " << AllHodo_Time[Hodo_ID-1] <<" "<<HODOT0[Hodo_ID-1] << endl ;
		
		// Hodo_Z = Hodo_Charge-(-17.98+0.7443*Hodo_Time-0.005006*TMath::Power(Hodo_Time,2.));
		// Hodo_AoverZ = Hodo_Time-(-0.01205*FDC2_X + 63.41)
		//   //		  -(-3.168 + 0.02068 * FDC0_X);
		//    -(-3.168 + 0.02068 * FDC1_X);

	      }
	    }
	}
	tree_HODO->Fill();
      } // END HODOSCOPE ANALYSIS


      

      if(kNEUT){
          
          //cout<<"             "<<endl;
          //cout<<"Starting NEBULA and NEULAND analysis"<<endl;
          
	//NEBULA
	NEBULA_ID->clear();
	NEBULA_X->clear();
	NEBULA_Y->clear();
	NEBULA_Z->clear();
	NEBULA_ChargeUp->clear();
	NEBULA_ChargeDown->clear();
	NEBULA_Charge->clear();
    
    NEBULA_ChargeUp_veto_1->clear();
    NEBULA_ChargeDown_veto_1->clear();
    NEBULA_Charge_veto_1->clear();
          
    NEBULA_ChargeUp_veto_2->clear();
    NEBULA_ChargeDown_veto_2->clear();
    NEBULA_Charge_veto_2->clear();

    NEBULA_Time->clear();
    NEBULA_TUSlw->clear();
    NEBULA_TDSlw->clear();

    NEBULA_Time_veto_1->clear();
    NEBULA_TUSlw_veto_1->clear();
    NEBULA_TDSlw_veto_1->clear();

    NEBULA_Time_veto_2->clear();
    NEBULA_TUSlw_veto_2->clear();
    NEBULA_TDSlw_veto_2->clear();

	NEBULA_VetoFired_1 = 0;
	NEBULA_VetoFired_2 = 0;

	CalibNEBULA->ReconstructData();

	if(CalibNEBULA) {
	  Int_t NEBULAMultiplicity = CalibNEBULA->GetNumNEBULAPla();

	  for(Int_t i=0; i<NEBULAMultiplicity; i++)
	    {
            //cout<<"NEBULAMultiplicity = "<<NEBULAMultiplicity<<endl;
	      TArtNEBULAPla *NEBULAPla = CalibNEBULA->GetNEBULAPla(i);
	      if(NEBULAPla)
          {
              //cout<<"NEBULAPla condition"<<endl;
		  //MODIF ALDRIC 21/11/2015
		  // if(TMath::Abs(NEBULAPla->GetPos(1)) < 1000 && !TMath::IsNaN(NEBULAPla->GetPos(1)))
		  //   {
		  //MODIF ALDRIC 19/11/2015
              //cout<<"NEBULAPla->GetTURaw() = "<<NEBULAPla->GetTURaw()<<endl;

		NEBULA_TUSlw_veto_1->push_back(NEBULAPla->GetTURaw());
		NEBULA_TDSlw_veto_1->push_back(NEBULAPla->GetTDRaw());
		NEBULA_Time_veto_1->push_back( (NEBULAPla->GetTURaw() + NEBULAPla->GetTDRaw())/2);

		  if(NEBULAPla->GetTURaw() > 0 && NEBULAPla->GetTURaw() < 4095 &&  NEBULAPla->GetTDRaw() > 0 && NEBULAPla->GetTDRaw() < 4095)
		  {
              //cout<<"NEBULA Raw time condition for VETO detectors"<<endl;
		    NEBULA_ID->push_back(NEBULAPla->GetID());
		    NEBULA_ChargeUp->push_back(NEBULAPla->GetQUCal());
		    NEBULA_ChargeDown->push_back(NEBULAPla->GetQDCal());
		    NEBULA_Charge->push_back(TMath::Sqrt(NEBULA_ChargeUp->back()*NEBULA_ChargeDown->back()));
			
		    if (NEBULA_ID->back() >= 121 && NEBULA_ID->back() <= 132) // First Veto wall
		    {
		        NEBULA_ChargeUp_veto_1->push_back(NEBULAPla->GetQUCal());
		        NEBULA_ChargeDown_veto_1->push_back(NEBULAPla->GetQDCal());
		        NEBULA_Charge_veto_1->push_back(TMath::Sqrt(NEBULA_ChargeUp_veto_1->back()*NEBULA_ChargeDown_veto_1->back()));
                
			//if(NEBULAPla->GetQDCal() > 1.)
			//if(NEBULA_ChargeDown_veto_1->back() > 2.)
			if(NEBULA_Charge_veto_1->back() > 0.1)
			    {
				NEBULA_VetoFired_1 = 1; // Don't put veto into tree
			    }
			NEBULA_ID->pop_back();
                	NEBULA_ChargeUp->pop_back();
                	NEBULA_ChargeDown->pop_back();
                	NEBULA_Charge->pop_back();
                
		    }
			
		    else if (NEBULA_ID->back() >= 133 && NEBULA_ID->back() <= 144) // Second Veto wall
		    {
	                NEBULA_ChargeUp_veto_2->push_back(NEBULAPla->GetQUCal());
	                NEBULA_ChargeDown_veto_2->push_back(NEBULAPla->GetQDCal());
	                NEBULA_Charge_veto_2->push_back(TMath::Sqrt(NEBULA_ChargeUp_veto_2->back()*NEBULA_ChargeDown_veto_2->back()));
                
                //if(NEBULAPla->GetQDCal() > 1.)
                //if(NEBULA_ChargeDown_veto_2->back() > 2.)
		if(NEBULA_Charge_veto_2->back() > 0.1)
                {
                    NEBULA_VetoFired_2 = 1;
                }
			  
                NEBULA_ID->pop_back();
                NEBULA_ChargeUp->pop_back();
                NEBULA_ChargeDown->pop_back();
                NEBULA_Charge->pop_back();
		    }

		    else
		      {
			//NEBULA_ChargeUp->push_back(NEBULAPla->GetQURaw());
			//NEBULA_ChargeDown->push_back(NEBULAPla->GetQDRaw());
			//NEBULA_Charge->push_back(TMath::Sqrt(NEBULA_ChargeUp->back()*NEBULA_ChargeDown->back()));
			NEBULA_Time->push_back(NEBULAPla->GetTAveSlw()); // - NEBULAT0.at(NEBULA_ID->back()-1));

			NEBULA_TUSlw->push_back(NEBULAPla->GetTUSlw());
            		NEBULA_TDSlw->push_back(NEBULAPla->GetTDSlw());

			NEBULA_X->push_back(-NEBULAPla->GetPos(0)); //Inverting X coordinate on global scale
			NEBULA_Y->push_back(NEBULAPla->GetPos(1) - y_shift_1[NEBULA_ID->back() - 1] - y_shift_2[NEBULA_ID->back() - 1]);
			NEBULA_Z->push_back(NEBULAPla->GetPos(2));
			//cout<<"Check line 3"<<endl;
		      }
		  }
		}
	    }

	  NEBULA_Multiplicity = NEBULA_X->size();
	}

	//NEULAND
	NEULAND_ID->clear();
	NEULAND_ID_veto->clear();
	NEULAND_X->clear();
	NEULAND_Y->clear();
	NEULAND_Z->clear();
	NEULAND_ChargeUp->clear();
	NEULAND_ChargeDown->clear();
	NEULAND_Charge->clear();

	NEULAND_ChargeUp_veto_1->clear();
	NEULAND_ChargeDown_veto_1->clear();
	NEULAND_Charge_veto_1->clear();

	NEULAND_Time->clear();
	NEULAND_TimeUp->clear();
	NEULAND_TimeDown->clear();

	NEULAND_Time_veto->clear();
	NEULAND_TimeUp_veto->clear();
	NEULAND_TimeDown_veto->clear();

	NEULAND_VetoFired = 0;

	CalibNEULAND->ReconstructData();

	if(CalibNEULAND) {

	  Int_t NEULANDMultiplicity = CalibNEULAND->GetNumNeuLANDPla();
	  if(NEULANDMultiplicity != 0)
	    {
	      for(Int_t i=0; i<NEULANDMultiplicity; i++)
		{
		  TArtNeuLANDPla *NEULANDPla = CalibNEULAND->GetNeuLANDPla(i);
		  if(NEULANDPla)
		    {

		      if(NEULANDPla->GetFired(0) && NEULANDPla->GetFired(1))
			{
                	NEULAND_ID->push_back(NEULANDPla->GetID());
			//NEULAND_ChargeUp->push_back(NEULANDPla->GetQRaw(0));
			//NEULAND_ChargeDown->push_back(NEULANDPla->GetQRaw(1));
	                NEULAND_ChargeUp->push_back(NEULANDPla->GetQCal(0));
        	        NEULAND_ChargeDown->push_back(NEULANDPla->GetQCal(1));
        	        NEULAND_Charge->push_back(TMath::Sqrt(NEULAND_ChargeUp->back()*NEULAND_ChargeDown->back()));
        	        NEULAND_Time->push_back((NEULANDPla->GetTCal(0) + NEULANDPla->GetTCal(1))/2.);// - NeuLANDT0.at(NEULAND_ID->back()-1));

					NEULAND_TimeUp->push_back(NEULANDPla->GetTCal(0));
					NEULAND_TimeDown->push_back(NEULANDPla->GetTCal(1));

        	        NEULAND_X->push_back(NEULANDPla->GetX());
        	        NEULAND_Y->push_back(NEULANDPla->GetY());
        	        NEULAND_Z->push_back(NEULANDPla->GetZ());
			}
		    }
		}
	    }

	  NEULAND_Multiplicity = NEULAND_X->size();
	}

	//NEULAND VETO
	CalibNEULANDVETO->ReconstructData();
	if(CalibNEULANDVETO) {
	  for (Int_t i=0;i<CalibNEULANDVETO->GetNumNeuLANDVETOPla();i++)
	    {
	      TArtNEBULAPla *VetoPlastic = CalibNEULANDVETO->GetNeuLANDVETOPla(i);
	      Double_t TempTimeUp = VetoPlastic->GetTURaw();
	      Double_t TempTimeDown = VetoPlastic->GetTDRaw();

		NEULAND_Time_veto->push_back((TempTimeUp+TempTimeDown)/2);
		NEULAND_TimeUp_veto->push_back(TempTimeUp);
		NEULAND_TimeDown_veto->push_back(TempTimeDown);
		
	      if ( TempTimeUp > 0 && TempTimeUp < 4096 && TempTimeDown > 0 && TempTimeDown < 4096)
		{
			NEULAND_ID_veto->push_back(VetoPlastic->GetID());
			//cout<<"NEULAND_ID_veto = "<<NEULAND_ID_veto->back()<<endl;
			NEULAND_ChargeUp_veto_1->push_back(VetoPlastic->GetQUCal());
        	        NEULAND_ChargeDown_veto_1->push_back(VetoPlastic->GetQDCal());
        	        NEULAND_Charge_veto_1->push_back(TMath::Sqrt(NEULAND_ChargeUp_veto_1->back()*NEULAND_ChargeDown_veto_1->back()));


		  NEULAND_VetoFired = 1;
		  break;
		}
	    }
	}

	// ============== Merge NEBULA & NeuLAND ==============

	Neutron_X->clear();
	Neutron_Y->clear();
	Neutron_Z->clear();
	Neutron_ID->clear();
	Neutron_Time->clear();
	Neutron_ChargeUp->clear();
	Neutron_ChargeDown->clear();
	Neutron_Charge->clear();
	Neutron_IsNEBULA->clear();
	Neutron_Multiplicity = 0;



	for (Int_t i=0;i<NEBULA_Multiplicity;i++)
	  {
	    //cout<<"Check line 3"<<endl;
	    Neutron_X->push_back(NEBULA_X->at(i));
	    Neutron_Y->push_back(NEBULA_Y->at(i));
	    Neutron_Z->push_back(NEBULA_Z->at(i));

	    Double_t DistanceX = Neutron_X->back();
	    Double_t DistanceZ = Neutron_Z->back() - 337.; // Distance from Al Tgt used for calib
	    Double_t Distance = TMath::Sqrt(DistanceX*DistanceX + DistanceZ*DistanceZ);

	    Neutron_ID->push_back(NEBULA_ID->at(i));
	    Neutron_Time->push_back(NEBULA_Time->at(i) - ToF_SBTTarget + Distance / 300.);
	    Neutron_ChargeUp->push_back(NEBULA_ChargeUp->at(i));
	    Neutron_ChargeDown->push_back(NEBULA_ChargeDown->at(i));
	    Neutron_Charge->push_back(NEBULA_Charge->at(i));
	    Neutron_IsNEBULA->push_back(true);
	    Neutron_Multiplicity++;
	  }

	for (Int_t i=0;i<NEULAND_Multiplicity;i++)
	  {
	    Neutron_X->push_back(NEULAND_X->at(i));
	    Neutron_Y->push_back(NEULAND_Y->at(i));
	    Neutron_Z->push_back(NEULAND_Z->at(i));
	    Neutron_ID->push_back(NEULAND_ID->at(i));

	    Double_t FlightLengthX;
	    Double_t FlightLengthY;

	    if (((Neutron_ID->back()-1) / 50)%2 == 0)
	      {
		FlightLengthX = 0;
		FlightLengthY = Neutron_Y->back();
	      }

	    else
	      {
		FlightLengthX = Neutron_X->back();
		FlightLengthY = 0;
	      }

	    Double_t FlightLengthZ = Neutron_Z->back() - 337.; // Distance from Al Tgt used for calib
	    Double_t FlightLength = TMath::Sqrt(FlightLengthX*FlightLengthX + FlightLengthY*FlightLengthY + FlightLengthZ*FlightLengthZ);


	    Neutron_Time->push_back(NEULAND_Time->at(i) - ToF_SBTTarget + FlightLength/300.);
	    Neutron_ChargeUp->push_back(NEULAND_ChargeUp->at(i));
	    Neutron_ChargeDown->push_back(NEULAND_ChargeDown->at(i));
	    Neutron_Charge->push_back(NEULAND_Charge->at(i));
	    Neutron_IsNEBULA->push_back(false);
	    Neutron_Multiplicity++;
	  }
	tree_NEUT ->Fill();
      }

      // ============== CATANA ==============
      if(kCATANA){
	CATANA_ID->clear();
	CATANA_Time->clear();
	CATANA_Energy->clear();
	CATANA_CosTheta->clear();
	CATANA_Layer->clear();
	CATANA_X->clear();
	CATANA_Y->clear();
	CATANA_Z->clear();
	CATANA_Pos->clear();
	CATANA_Multiplicity->clear();
 	//cout<<"Check line 4"<<endl;

	Double_t FragmentBeta = 0.6; // TODO TODO TODO
	CalibCATANA->ReconstructData();

	TClonesArray *CATANACsIHits = (TClonesArray *)sman->FindDataContainer("CATANACsI");
	if(CATANACsIHits) {
	  Int_t CATANA_Mult = 0;
	  Int_t NumberOfCsIHit = CATANACsIHits->GetEntries();
	  Double_t ADC;
	  Double_t Time;
	  for(Int_t i=0; i<NumberOfCsIHit; i++) {
	    TArtCATANACsI *CATANACsI = (TArtCATANACsI *)CATANACsIHits->At(i);
	    ADC = CATANACsI->GetRawADC();
	    Time = CATANACsI->GetTimeOffseted();
	    if(ADC > 0){// &&  ADC < 4095*2 /*&& Time > 0*/) {
	      CATANA_ID->push_back(CATANACsI->GetID());
	      CATANA_Energy->push_back(CATANACsI->GetEnergy());
	      Double_t radius2 = CATANACsI->GetPositionX()*CATANACsI->GetPositionX()+CATANACsI->GetPositionY()*CATANACsI->GetPositionY();
	      Double_t abs = TMath::Sqrt(radius2+CATANACsI->GetPositionZ()*CATANACsI->GetPositionZ());
	      Double_t costheta = CATANACsI->GetPositionZ()/abs ;
	      CATANA_CosTheta->push_back(costheta);
	      CATANA_Time->push_back(Time);
	      CATANA_Layer->push_back((CATANACsI->GetID()-1)/20+1);//CATANACsI->GetLayer());
	      CATANA_X->push_back(CATANACsI->GetPositionX());
	      CATANA_Y->push_back(CATANACsI->GetPositionY());
	      CATANA_Z->push_back(CATANACsI->GetPositionZ());
	      CATANA_Pos->push_back(TVector3(CATANACsI->GetPositionX(),CATANACsI->GetPositionY(),CATANACsI->GetPositionZ()));
	     
	      CATANA_Mult++;
	      if(CATANACsI->GetEnergy() > 0 || Time != (-1) ) {CATANA_Multiplicity->push_back(1);}
	      else {CATANA_Multiplicity->push_back(0);}
	    }
	  }
	  // if (CATANA_Mult > 1)
	  // SortDaliHit(0,CATANA_Mult-1, CATANA_ID, CATANA_Energy, CATANA_EnergyDopplerCorrected, CATANA_Time, CATANA_CosTheta);
	}
	tree_CATANA ->Fill();
      }


    }

  std::clog << std::endl;

  if(kMAIN) { tree_MAIN->BuildIndex("RunNumber","EventNumber"); fout_MAIN->cd(); WriteAllEnvFiles ; fout_MAIN->Write(); fout_MAIN->Close("R"); }
  if(kNEUT) { tree_NEUT->BuildIndex("RunNumber","EventNumber"); fout_NEUT->cd(); WriteAllEnvFiles ; fout_NEUT->Write(); fout_NEUT->Close("R"); }
  if(kCATANA) { tree_CATANA->BuildIndex("RunNumber","EventNumber"); fout_CATANA->cd(); WriteAllEnvFiles ; fout_CATANA->Write(); fout_CATANA->Close("R"); }
  if(kFDCs||kFDCs_full) { tree_FDC ->BuildIndex("RunNumber","EventNumber"); fout_FDC ->cd(); WriteAllEnvFiles ; fout_FDC ->Write();  fout_FDC ->Close("R"); }
  if(kHODO) { tree_HODO->BuildIndex("RunNumber","EventNumber"); fout_HODO->cd(); WriteAllEnvFiles ; fout_HODO->Write(); fout_HODO->Close("R"); }
  if(kBEAM) { tree_BEAM->BuildIndex("RunNumber","EventNumber"); fout_BEAM->cd(); WriteAllEnvFiles ; fout_BEAM->Write(); fout_BEAM->Close("R"); }


  if(kOverwrite){
    system(TString("mv -vfu ")+TmpRootFilenameBody+"*.root "+rootfilesubpath+"/");
  } else {
    system(TString("mv -viu ")+TmpRootFilenameBody+"*.root "+rootfilesubpath+"/");
  }


  EventStore->ClearData();

  delete CalibNEBULA;
  delete CalibNEULAND;
  delete CalibNEULANDVETO;
  delete CalibPlastic;
  delete CalibPPAC;
  delete CalibBDC1Hit;
  delete CalibBDC1Track;
  delete CalibBDC2Hit;
  delete CalibBDC2Track;
  if(kFDCs)
  {
     delete CalibFDC1Track;
     delete CalibFDC2Track;
    //delete CalibSimpleFDC1Track;
    //     delete CalibSimpleFDC2Track;
    // delete CalibSimpleFDC0Track;
  }
  if(kFDCs_full)
  {
    delete CalibFDC1Track;
     delete CalibFDC2Track;
     //     delete CalibFDC0Track;
  }
   delete CalibFDC1Hit;
   //delete CalibFDC0Hit;
  delete CalibFDC2Hit;
  delete CalibHODPla;
  delete CalibCATANA;
  delete CalibCoin;
  //  delete RecoFragment;
  delete NEBULA_X;
  delete NEBULA_Y;
  delete NEBULA_Z;
  delete NEBULA_ID;
  delete NEBULA_Time;
  delete NEBULA_TUSlw;
  delete NEBULA_TDSlw;
  delete NEBULA_ChargeUp;
  delete NEBULA_ChargeDown;
  delete NEULAND_X;
  delete NEULAND_Y;
  delete NEULAND_Z;
  delete NEULAND_ID;
  delete NEULAND_Time;
  delete NEULAND_TimeUp;
  delete NEULAND_TimeDown;
  delete NEULAND_ChargeUp;
  delete NEULAND_ChargeDown;
  delete Neutron_X;
  delete Neutron_Y;
  delete Neutron_Z;
  delete Neutron_ID;
  delete Neutron_Time;
  delete Neutron_ChargeUp;
  delete Neutron_ChargeDown;
  delete CATANA_ID;
  delete CATANA_Time;
  delete CATANA_Energy;
  //  delete CATANA_EnergyDopplerCorrected;
  delete CATANA_CosTheta;
  delete TArtStoreManager::Instance();

  return 0;
}

/*
// External Function
void LoadDCTDCDistributionFull(char *FileName, TArtCalibBDC1Track *CalibBDC1Track, TArtCalibBDC2Track *CalibBDC2Track, TArtCalibFDC1Track *CalibFDC1Track, TArtCalibFDC2Track *CalibFDC2Track) {
//void LoadDCTDCDistributionFull(char *FileName, TArtCalibSimpleBDC1Track *CalibBDC1Track, TArtCalibSimpleBDC2Track *CalibBDC2Track, TArtCalibSimpleFDC0Track *CalibFDC0Track, TArtCalibSimpleFDC2Track *CalibFDC2Track) {

  TFile *RootFile = new TFile(FileName,"READ");
  if(RootFile) {
    gROOT->cd();
    TH1F *Hist1D_X = NULL;
    TH1F *Hist1D_Y = NULL;
    TH2F *Hist2D = NULL;
    // Int_t BDCNumberOfLayers = 8;
    // Int_t FDCNumberOfLayers = 14;
    // Int_t FDC0NumberOfLayers=8;
    Int_t BDCNumberOfLayers = 4;
    Int_t FDCNumberOfLayers = 7;
    Int_t FDC0NumberOfLayers=4;
 
    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      
      Hist2D = (TH2F*) RootFile->Get(Form("bdc1_ftdc_corr_%d",i));
      
      if(Hist2D) {
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {
	  CalibBDC1Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibBDC1Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
      // 	  cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("BDC1TDCDistLayer%d",i) << "\e[37m" << endl;
      }
      else {
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("bdc1_ftdc_corr_%d",i) << ". Abort. \e[0m" << endl; exit(1);
      }
    }
    
    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      
      Hist2D = (TH2F*) RootFile->Get(Form("bdc2_ftdc_corr_%d",i));
      
      if(Hist2D) {
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {
	  CalibBDC2Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibBDC2Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
	//   cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("BDC2TDCDistLayer%d",i) << "\e[37m" << endl;
      }
      else {
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("bdc2_ftdc_corr_%d",i) << ". Abort. \e[0m" << endl; exit(1);
      }
    }

    for(Int_t i=0; i<FDC0NumberOfLayers; i++) { //FDC0 8layers
      //Hist1D = (TH1I*) RootFile->Get(Form("hfdc0tdc%d",i));
      
      Hist2D = (TH2F*) RootFile->Get(Form("fdc0_ftdc_corr_%d",i));
      
      if(Hist2D) {
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {
	  CalibFDC0Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibFDC0Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	//else
	// cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC0TDCDistLayer%d",i) << "\e[37m" << endl;
      }
      else {
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("fdc0_ftdc_corr_%d",i) << ". Abort. \e[0m" << endl; exit(1);
      }
    }
    
    // for(Int_t i=0; i<FDCNumberOfLayers; i++) {
    //   Hist1D = (TH1I*) RootFile->Get(Form("hfdc1tdc%d",i));
    //   if(Hist1D) {
    // 	CalibFDC1Track->SetTDCDistribution(Hist1D,i);
    // 	delete Hist1D;
    // 	Hist1D = NULL;
    // 	delete Hist2D;
    // 	Hist2D = NULL;
    //   }
    //   else
    // 	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC1TDCDistLayer%d",i) << "\e[37m" << endl;
    // }

    for(Int_t i=0; i<FDCNumberOfLayers; i++) {
      //Hist1D = (TH1I*) RootFile->Get(Form("hfdc2tdc%d",i));
      
      Hist2D = (TH2F*) RootFile->Get(Form("fdc2_ftdc_corr_%d",i));
      
      if(Hist2D) {
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {
	  CalibFDC2Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibFDC2Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
	//   cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC2TDCDistLayer%d",i) << "\e[0m" << endl;
      }
      else {
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("fdc2_ftdc_corr_%d",i) << ". Abort. \e[0m" << endl; exit(1);
      }
    }
  }
  else {
    cout << "\e[35m " << "Could not find the following file : " << FileName << ". Abort. \e[0m" << endl; exit(1);
  }
  delete RootFile;
}
*/
//==========================================================================
void LoadDCTDCDistributionFull(char *FileName, TArtCalibBDC1Track *CalibBDC1Track, TArtCalibBDC2Track *CalibBDC2Track, TArtCalibFDC1Track *CalibFDC1Track, TArtCalibFDC2Track *CalibFDC2Track) {
//void LoadDCTDCDistributionFull(char *FileName, TArtCalibSimpleBDC1Track *CalibBDC1Track, TArtCalibSimpleBDC2Track *CalibBDC2Track, TArtCalibFDC0Track *CalibFDC0Track, TArtCalibFDC2Track *CalibFDC2Track) {
  
  TFile *RootFile = new TFile(FileName,"READ");
  if(RootFile) {
    gROOT->cd();
    TH1F *Hist1D_X = NULL;
    TH1F *Hist1D_Y = NULL;
    TH2F *Hist2D = NULL;
    // Int_t BDCNumberOfLayers = 8;
    // Int_t FDCNumberOfLayers = 14;
    // Int_t FDC0NumberOfLayers=8;
    Int_t BDCNumberOfLayers = 4;
    Int_t FDCNumberOfLayers = 7;
    Int_t FDC1NumberOfLayers=4; // ??
 
    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      
      Hist2D = (TH2F*) RootFile->Get(Form("bdc1_ftdc_corr_%d",i));
      
      if(Hist2D) {

       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {

	cout << Hist2D->GetName() << endl;
	
	  CalibBDC1Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibBDC1Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
	// 	  cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("BDC1TDCDistLayer%d",i) << "\e[0m" << endl;
      }
      else
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("bdc1_ftdc_corr_%d",i) << "\e[0m" << endl;
    }
    
    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      
      Hist2D = (TH2F*) RootFile->Get(Form("bdc2_ftdc_corr_%d",i));
      
      if(Hist2D) {

	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {

	  cout << Hist2D->GetName() << endl;
	
	  CalibBDC2Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibBDC2Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
	//   cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("BDC2TDCDistLayer%d",i) << "\e[0m" << endl;
      }
      else
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("bdc2_ftdc_corr_%d",i) << "\e[0m" << endl;
    }

//    for(Int_t i=0; i<FDC0NumberOfLayers; i++) { //FDC0 8layers
//      //Hist1D = (TH1I*) RootFile->Get(Form("hfdc0tdc%d",i));
//      
//      Hist2D = (TH2F*) RootFile->Get(Form("fdc0_ftdc_corr_%d",i));
//      
//      if(Hist2D) {
//       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
//	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
//	
//	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
//    
//	if(Hist1D_X && Hist1D_Y) {
//	  CalibFDC0Track->SetTDCDistribution(Hist1D_X,2*i);
//	  CalibFDC0Track->SetTDCDistribution(Hist1D_Y,2*i+1);
//	  
//	  delete Hist1D_X;
//	  Hist1D_X = NULL;
//	  delete Hist1D_Y;
//	  Hist1D_Y = NULL;
//	  delete Hist2D;
//	  Hist2D = NULL;
//	}
//	//else
//	// cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC0TDCDistLayer%d",i) << "\e[0m" << endl;
//      }
//      else
//	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("fdc0_ftdc_corr_%d",i) << "\e[0m" << endl;
//    }
    
    for(Int_t i=0; i<FDCNumberOfLayers; i++) {
      //Hist1D = (TH1I*) RootFile->Get(Form("hfdc0tdc%d",i));
      
      Hist2D = (TH2F*) RootFile->Get(Form("fdc1_ftdc_corr_%d",i));
      
      if(Hist2D) {
	
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());
	
	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    

   
	//   Hist1D = (TH1I*) RootFile->Get(Form("hfdc1tdc%d",i));
	if(Hist1D_X && Hist1D_Y) {

	  cout << Hist2D->GetName() << endl;
	  
	  CalibFDC1Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibFDC1Track->SetTDCDistribution(Hist1D_Y,2*i+1);

	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	else
	  cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC1TDCDistLayer%d",i) << "\e[0m" << endl;
      }
      else
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("fdc0_ftdc_corr_%d",i) << "\e[0m" << endl;
    }

    for(Int_t i=0; i<FDCNumberOfLayers; i++) {
      //Hist1D = (TH1I*) RootFile->Get(Form("hfdc2tdc%d",i));
      
      Hist2D = (TH2F*) RootFile->Get(Form("fdc2_ftdc_corr_%d",i));
      
      if(Hist2D) {
       	Hist1D_X = (TH1F*)(Hist2D->ProjectionX());
	Hist1D_Y = (TH1F*)(Hist2D->ProjectionY());


	//Hist1D = (TH1I*) RootFile->Get(Form("hbdc1tdc%d",i));
    
	if(Hist1D_X && Hist1D_Y) {

	  cout << Hist2D->GetName() << endl;
	  
	  CalibFDC2Track->SetTDCDistribution(Hist1D_X,2*i);
	  CalibFDC2Track->SetTDCDistribution(Hist1D_Y,2*i+1);
	  
	  delete Hist1D_X;
	  Hist1D_X = NULL;
	  delete Hist1D_Y;
	  Hist1D_Y = NULL;
	  delete Hist2D;
	  Hist2D = NULL;
	}
	// else
	//   cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("FDC2TDCDistLayer%d",i) << "\e[0m" << endl;
      }
      else{
	cout << "\e[35m " << "Warning LoadDCTDCDistribution :: Could not find the following histogram " << Form("fdc2_ftdc_corr_%d",i) << ". Abort. \e[0m" << endl; exit(1);
      }
    }
  }
  else {
    cout << "\e[35m " << "Could not find the following file : " << FileName << ". Abort. \e[0m" << endl; exit(1);
  }
  delete RootFile;
}

// ======================================================================================

void SortHodoHit(vector<Int_t> &HodoHitID, vector<Double_t> &HodoHitEnergy) {

  Int_t NumberOfHits = HodoHitID.size();

//  for(int i=0; i<NumberOfHits; i++) {
//    cout <<  HodoHitID[i]<< "] " << HodoHitEnergy[i]  << " " ;
//  }
//  cout << endl;

  for(int i=0; i<NumberOfHits; i++)
    {
      for(int j=0; j<NumberOfHits-1; j++)
	{
	  if(HodoHitID[i]<=NUMBER_OF_HODO && HodoHitID[j]<=NUMBER_OF_HODO){
	    if(HodoHitEnergy[j]<HodoHitEnergy[j+1])
	      {
		Double_t tempE = HodoHitEnergy[j+1];
		HodoHitEnergy[j+1] = HodoHitEnergy[j];
		HodoHitEnergy[j] = tempE;
		
		Double_t tempI = HodoHitID[j+1];
		HodoHitID[j+1] = HodoHitID[j];
		HodoHitID[j] = tempI;
	      
	      }
	  }
	}
    }

//  for(int i=0; i<NumberOfHits; i++) {
//    cout <<  HodoHitID[i]<< ") " << HodoHitEnergy[i]  << " " ;
//  }
//  cout << endl << endl;
}

//---------------------------------------------------

// Double_t DopplerCorrection(Double_t GammaDopplerEnergy, Double_t Beta, Double_t CosTheta) {
//   Double_t Gamma = 1 / TMath::Sqrt(1 - Beta * Beta);
//   Double_t DopplerCorrected = GammaDopplerEnergy * Gamma * (1 - Beta * CosTheta);
//   return DopplerCorrected;
// }

//---------------------------------------------------

inline bool exists_test (const std::string& name) {
  return ( access( name.c_str(), F_OK ) != -1 );
}
inline bool exists_test (const TString& name) {
  return ( access( name.Data(), F_OK ) != -1 );
}


// void SortDaliHit(Int_t left, Int_t right,vector <Int_t> *CATANA_ID,vector <Double_t> *CATANA_Energy, vector <Double_t> *CATANA_EnergyDopplerCorrected, vector <Double_t> *CATANA_Time, vector <Double_t> *CATANA_CosTheta)
// {
//   Int_t TempID;
//   Double_t TempEnergy;
//   Double_t TempEnergyDopplerCorrected;
//   Double_t TempTime;
//   Double_t TempCosTheta;

//   int i = left, j = right;
//   double pivot = CATANA_EnergyDopplerCorrected->at((left + right) / 2);

//   /* partition */
//    while (i <= j) {
//     while (CATANA_EnergyDopplerCorrected->at(i) > pivot)
//       i++;
//     while (CATANA_EnergyDopplerCorrected->at(j) < pivot)
//       j--;
//     if (i <= j) {
//       TempID = CATANA_ID->at(j);
//       TempEnergy = CATANA_Energy->at(j);
//       TempEnergyDopplerCorrected = CATANA_EnergyDopplerCorrected->at(j);
//       TempTime = CATANA_Time->at(j);
//       TempCosTheta = CATANA_CosTheta->at(j);

//       CATANA_ID->at(j) = CATANA_ID->at(i);
//       CATANA_Energy->at(j) = CATANA_Energy->at(i);
//       CATANA_EnergyDopplerCorrected->at(j) = CATANA_EnergyDopplerCorrected->at(i);
//       CATANA_Time->at(j) = CATANA_Time->at(i);
//       CATANA_CosTheta->at(j) = CATANA_CosTheta->at(i);

//       CATANA_ID->at(i) = TempID;
//       CATANA_Energy->at(i) = TempEnergy;
//       CATANA_EnergyDopplerCorrected->at(i) = TempEnergyDopplerCorrected;
//       CATANA_Time->at(i) = TempTime;
//       CATANA_CosTheta->at(i) = TempCosTheta;

//       i++;
//       j--;
//     }
//   }; 
//   /* recursion */
//   if (left < j)
//     SortDaliHit(left, j, CATANA_ID, CATANA_Energy, CATANA_EnergyDopplerCorrected, CATANA_Time, CATANA_CosTheta);
//   if (i < right)
//     SortDaliHit(i, right, CATANA_ID, CATANA_Energy, CATANA_EnergyDopplerCorrected, CATANA_Time, CATANA_CosTheta);
// }

//--------------------------------------------------------

  void SortNeutronHit(int left, int right,vector <Bool_t> *Neutron_IsNEBULA, vector <Int_t> *Neutron_ID,vector <Double_t> *Neutron_X,vector <Double_t> *Neutron_Y,vector <Double_t> *Neutron_Z,vector <Double_t> *Neutron_ChargeUp, vector <Double_t> *Neutron_ChargeDown, vector <Double_t> *Neutron_Charge, vector <Double_t> *Neutron_Time){
  int i = left, j = right;
  double pivot = Neutron_Time->at((left + right) / 2);

  /* partition */
  while (i <= j) {
    while (Neutron_Time->at(i) < pivot)
      i++;
    while (Neutron_Time->at(j) > pivot)
      j--;
    if (i <= j) {

      //MODIF ALDRIC 19/11/2015 : ajout de Neutron_IsNEBULA !!

      Int_t TempID;
      Double_t TempChargeUp;
      Double_t TempChargeDown;
      Double_t TempCharge;
      Double_t TempTime;
      Double_t TempXPosition;
      Double_t TempYPosition;
      Double_t TempZPosition;
      Bool_t TempIsNEBULA;

      TempID = Neutron_ID->at(i);
      TempChargeUp = Neutron_ChargeUp->at(i);
      TempChargeDown = Neutron_ChargeDown->at(i);
      TempCharge = Neutron_Charge->at(i);
      TempTime = Neutron_Time->at(i);
      TempXPosition = Neutron_X->at(i);
      TempYPosition = Neutron_Y->at(i);
      TempZPosition = Neutron_Z->at(i);
      TempIsNEBULA = Neutron_IsNEBULA->at(i);

      Neutron_ID->at(i) =  Neutron_ID->at(j);
      Neutron_ChargeUp->at(i) = Neutron_ChargeUp->at(j);
      Neutron_ChargeDown->at(i) = Neutron_ChargeDown->at(j);
      Neutron_Charge->at(i) = Neutron_Charge->at(j);
      Neutron_Time->at(i) = Neutron_Time->at(j);
      Neutron_X->at(i) = Neutron_X->at(j);
      Neutron_Y->at(i) = Neutron_Y->at(j);
      Neutron_Z->at(i) = Neutron_Z->at(j);
      Neutron_IsNEBULA->at(i) = Neutron_IsNEBULA->at(j);


      Neutron_ID->at(j) = TempID;
      Neutron_ChargeUp->at(j) = TempChargeUp;
      Neutron_ChargeDown->at(j) = TempChargeDown;
      Neutron_Charge->at(j) = TempCharge;
      Neutron_Time->at(j) = TempTime;
      Neutron_X->at(j) = TempXPosition;
      Neutron_Y->at(j) = TempYPosition;
      Neutron_Z->at(j) = TempZPosition;
      Neutron_IsNEBULA->at(j) = TempIsNEBULA;

      i++;
      j--;
    }
  };

  /* recursion */
  if (left < j)
    SortNeutronHit(left, j,Neutron_IsNEBULA,Neutron_ID,Neutron_X,Neutron_Y,Neutron_Z,Neutron_ChargeUp, Neutron_ChargeDown, Neutron_Charge, Neutron_Time);
  if (i < right)
    SortNeutronHit(i, right,Neutron_IsNEBULA,Neutron_ID,Neutron_X,Neutron_Y,Neutron_Z,Neutron_ChargeUp, Neutron_ChargeDown, Neutron_Charge, Neutron_Time);

}

