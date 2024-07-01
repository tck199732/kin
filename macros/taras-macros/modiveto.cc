#include "TTree.h"

struct VmeEasiroc {
	int ndata;
	int multi;

	std::array<int, 64> hadc; // high gain
	std::array<int, 64> ladc; // low gain
	std::array<int, 64> ltdc; // leading edge
	std::array<int, 64> ttdc; // trailing edge
							  // std::array<int, 67> scaler;
	void reset() {
		ndata = 0;
		multi = 0;
		hadc.fill(-1);
		ladc.fill(-1);
		ltdc.fill(-1);
		ttdc.fill(-1);
	}
};
VmeEasiroc easiroc;

void modiveto(Int_t nRun = 1020) {

	TArtEventStore *estore = new TArtEventStore();
	if (nRun <= 0)
		estore->Open(0);
	else
		estore->Open(Form("ridf/data%04d.ridf", nRun)); // cosmic, ecl term off
	// else        estore->Open(Form("ridf/data%04d.ridf.gz",nRun));// cosmic, ecl term off

	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile("macros/chkveto/chkveto.root", "RECREATE");
	TH1 *hchval = new TH2D("hchval", "Ch Val leading", 32, -0.5, 31.5, 100, 120000, 200000);
	TH1 *hchval_t = new TH2D("hchval_t", "Ch Val trailing", 32, -0.5, 31.5, 100, 120000, 200000);
	TH1 *hchtwid = new TH2D("hchtfwid", "Ch width(ch)", 32, -0.5, 31.5, 100, 0, 20000);
	// TNtuple *ntp = new TNtuple("ntp","ntp","dev:fp:det:geo:ch:val:edge");
	/*TH1* QvsT[32];
	for(int ich=0;ich<32;++ich){
	  new TH2D("QvsT","QvsT",100,0,30, 100,0,30);
	  }*/
	TH1 *QvsT0 = new TH2D("QvsT0", "QvsT0", 80, -1000, 1000, 100, 0, 4000);
	TH1 *QvsT1 = new TH2D("QvsT1", "QvsT1", 80, -1000, 1000, 100, 0, 4000);
	TH1 *QvsT2 = new TH2D("QvsT2", "QvsT2", 80, -1000, 1000, 100, 0, 4000);
	TH1 *QvsT3 = new TH2D("QvsT3", "QvsT3", 80, -1000, 1000, 100, 0, 4000);
	TH1 *QvsT4 = new TH2D("QvsT4", "QvsT4", 80, -1000, 1000, 100, 0, 4000);
	TH1 *QvsT5 = new TH2D("QvsT5", "QvsT5", 80, -1000, 1000, 100, 0, 4000);
	TH1 *dT1a = new TH1D("dT1", "dT1", 80, -1000, 1000);
	TH1 *dT2a = new TH1D("dT2", "dT2", 80, -1000, 1000);
	TH1 *dT3a = new TH1D("dT3", "dT1", 80, -1000, 1000);
	TH1 *dT4a = new TH1D("dT4", "dT2", 80, -1000, 1000);
	TH1 *dT5a = new TH1D("dT5", "dT1", 80, -1000, 1000);
	TH1 *dT6a = new TH1D("dT6", "dT2", 80, -1000, 1000);
	TH1 *count = new TH1D("count", "coount", 32, -0.5, 31.5);
	TH1 *QvsToF1L = new TH2D("QvsTOF1L", "QvsTOF1L", 200, -22000, -15000, 100, 0, 8000);
	TH1 *QvsToF2L = new TH2D("QvsTOF2L", "QvsTOF2L", 200, -22000, -15000, 100, 0, 8000);
	TH1 *QvsToF3L = new TH2D("QvsTOF3L", "QvsTOF3L", 200, -22000, -15000, 100, 0, 8000);
	TH1 *QvsToF1R = new TH2D("QvsTOF1R", "QvsTOF1R", 200, -22000, -15000, 100, 0, 8000);
	TH1 *QvsToF2R = new TH2D("QvsTOF2R", "QvsTOF2R", 200, -22000, -15000, 100, 0, 8000);
	TH1 *QvsToF3R = new TH2D("QvsTOF3R", "QvsTOF3R", 200, -22000, -15000, 100, 0, 8000);
	TH1 *twidup = new TH2D("twidup", "twidup", 100, 0, 4000, 100, 0, 4000);
	TH1 *twidmid = new TH2D("twidmid", "twidmid", 100, 0, 4000, 100, 0, 4000);
	TH1 *twidbel = new TH2D("twidbel", "twidbel", 100, 0, 4000, 100, 0, 4000);
	int t0[32] = {-10};
	int t1[32] = {-10};

	TTree *tree = new TTree("tofTree", "Tree with TOF values");
	Double_t ToF = -99999;
	Int_t neve = 0;
	uint64_t lupots = 0;
	std::vector<float> TDiff_VETO;
	std::vector<float> X_VETO;
	std::vector<int> VETO_ID;
	std::vector<float> Charge_VETO;
	std::vector<float> ToF_VETO;
	std::vector<int> VETO_mult;

	tree->Branch("neve", &neve);
	tree->Branch("lupots", &lupots);
	tree->Branch("TDiff_VETO", &TDiff_VETO);
	tree->Branch("X_VETO", &X_VETO);
	tree->Branch("ToF_VETO", &ToF_VETO);
	tree->Branch("VETO_ID", &VETO_ID);
	tree->Branch("Charge_VETO", &Charge_VETO);
	tree->Branch("VETO_mult", &VETO_mult);

	tree->Branch("multi_Kyoto", &easiroc.multi);
	tree->Branch("ltdc_Kyoto", &easiroc.ltdc[0]);
	tree->Branch("ttdc_Kyoto", &easiroc.ttdc[0]);

	// while(estore->GetNextEvent() && neve < 1e+8){
	while (estore->GetNextEvent()) {
		// if (neve%100==0) cout<<"\r neve="<<neve<<flush;
		// std::cout<<"neve = "<<neve<<std::endl;
		for (int ich = 0; ich < 32; ++ich) {
			t0[ich] = -10;
			t1[ich] = -10;
		}
		easiroc.reset();
		TDiff_VETO.clear();
		X_VETO.clear();
		VETO_ID.clear();
		Charge_VETO.clear();
		ToF_VETO.clear();
		VETO_mult.clear();
		// std::cout << "neve: " << neve << std::endl;

		// std::cout<<"rawevent->GetNumSeg() = "<<rawevent->GetNumSeg()<<std::endl;
		for (int i = 0; i < rawevent->GetNumSeg(); i++) {
			TArtRawSegmentObject *seg = rawevent->GetSegment(i);
			int device = seg->GetDevice();
			int fp = seg->GetFP();
			int detector = seg->GetDetector();
			int module = seg->GetModule();
			int efn = seg->GetAddress();
			int ndata = seg->GetNumData();

			if (module == 35) {
				// std::cout << "kyoto" << std::endl;
				//  EASIROC
				easiroc.ndata += ndata;
				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();

					// std::cout << ch << " " << geo << std::endl;
					if (ch > 64) {
						std::cerr << "Error!! Channel for EASIROC should be less than 64!!: " << ch << std::endl;
						break;
					}

					/*
					 * @file TArtDecoderVmeEasyroc.cc
					 * geo:0 High gain ADC
					 * geo:1 Low gain ADC
					 * geo:2 Leading edge TDC
					 * geo:3 Trailing edge TDC
					 * geo:4 Scaler
					 */

					switch (geo) {
					case 0:
						easiroc.hadc[ch] = val;
						break;
					case 1:
						easiroc.ladc[ch] = val;
						break;
					case 2:
						easiroc.ltdc[ch] = val;
						break;
					case 3:
						easiroc.ttdc[ch] = val;
						break;

					default:
						break;
					}
				}
			}

			if (fp == 13 && detector == 60 && efn == 85) { // LUPO
				TArtRawDataObject *d = seg->GetData(0);
				ULong64_t valh = d->GetVal();
				d = seg->GetData(1);
				ULong64_t vall = d->GetVal();
				lupots = (valh << 32) + vall;
				// std::cout << "lupots: " << lupots << std::endl;
			}

			if (detector != 50)
				continue;

			// std::cout<<"seg->GetNumData() = "<<seg->GetNumData()<<std::endl;
			for (int j = 0; j < seg->GetNumData(); j++) {
				TArtRawDataObject *d = seg->GetData(j);
				int geo = d->GetGeo();
				int ch = d->GetCh();
				int val = d->GetVal();
				int cat = d->GetCategoryID();
				int det = d->GetDetectorID();
				int id = d->GetDatatypeID();
				int edge = d->GetEdge();
				double dT1 = -99999;
				double dT2 = 99999;
				double dT3 = -99999;
				double dT4 = 99999;
				double dT5 = -99999;
				double dT6 = 99999;
				ToF = -99999;

				//-------------------------------
				if (edge == 0) { // leading
					t0[ch] = val;
					hchval->Fill(ch, val);

					//-------------------------------
				} else if (edge == 1) { // trailing
					t1[ch] = val;
					hchval_t->Fill(ch, val);
				}
			}
		}
		int twidc[32] = {0};
		for (int ich = 0; ich < 32; ++ich) {
			int twid = t1[ich] - t0[ich];
			twidc[ich] = t1[ich] - t0[ich];
			if (twidc[6] > 0 && twidc[1] > 0) {
				twidup->Fill(twidc[6], twidc[1]);
			}
			if (twidc[2] > 0 && twidc[3] > 0) {
				twidmid->Fill(twidc[2], twidc[3]);
			}
			if (twidc[4] > 0 && twidc[7] > 0) {
				twidbel->Fill(twidc[4], twidc[7]);
			}
			// cout<<ich<<" "<<twid<<endl;
			if (twid > 0) {
				hchtwid->Fill(ich, twid);
				// cout<<"a"<<endl;
				count->Fill(ich);
				if (ich == 6) {
					double dT1 = t0[1] - t0[6];
					// cout<<t1[ich+1]<<" "<<t1[ich]<<" "<<dT1<<" "<<twid<<endl;
					QvsT0->Fill(dT1, twid);
					dT1a->Fill(dT1);
					double ave = (t0[1] + t0[6]) / 2;
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF1L->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}
				if (ich == 1) {
					double dT2 = t0[6] - t0[1];
					// cout<<ich<<" "<<dT<<" "<<twid<<endl;
					QvsT1->Fill(dT2, twid);
					dT2a->Fill(dT2);
					double ave = (t0[1] + t0[6]) / 2;
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF1R->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}

				if (t0[1] > -1 && t0[6] > -1) {
					// std::cout<<"Two PMTs are fired for VETO_ID == 1"<<std::endl;
					TDiff_VETO.push_back((t0[1] - t0[6]) - 178);
					X_VETO.push_back((t0[1] - t0[6] - 178) * 525 / 378);
					ToF_VETO.push_back((t0[1] + t0[6]) / 2 - t0[31]);
					VETO_ID.push_back(1);
					Charge_VETO.push_back(sqrt(twidc[1] * twidc[6]));
				}

				if (ich == 2) {
					double dT3 = t0[ich + 1] - t0[ich];
					// cout<<ich<<" "<<dT<<" "<<twid<<endl;
					QvsT2->Fill(dT3, twid);
					dT3a->Fill(dT3);
					double ave = (t0[2] + t0[3]) / 2;
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF2L->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}
				if (ich == 3) {
					double dT4 = t0[ich] - t0[ich - 1];
					// cout<<ich<<" "<<dT<<" "<<twid<<endl;
					QvsT3->Fill(dT4, twid);
					dT4a->Fill(dT4);
					double ave = (t0[2] + t0[3]) / 2 - t0[31];
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF2R->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}

				if (t0[2] > -1 && t0[3] > -1) {
					// std::cout<<"Two PMTs are fired for VETO_ID == 2"<<std::endl;
					TDiff_VETO.push_back((t0[3] - t0[2]) + 69.0);
					X_VETO.push_back((t0[3] - t0[2] + 69.0) * 525 / 378);
					ToF_VETO.push_back((t0[3] + t0[2]) / 2 - t0[31]);
					VETO_ID.push_back(2);
					Charge_VETO.push_back(sqrt(twidc[3] * twidc[2]));
				}

				if (ich == 4) {
					double dT5 = t0[4] - t0[7];
					// cout<<ich<<" "<<dT<<" "<<twid<<endl;
					QvsT4->Fill(dT5, twid);
					dT5a->Fill(dT5);
					double ave = (t0[4] + t0[7]) / 2;
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF3L->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}
				if (ich == 7) {
					double dT6 = t0[7] - t0[4];
					// cout<<ich<<" "<<dT<<" "<<twid<<endl;
					QvsT5->Fill(dT6, twid);
					dT6a->Fill(dT6);
					double ave = (t0[4] + t0[7]) / 2;
					ToF = ave - t0[31];
					// cout<<ave<<" "<<ToF<<" "<<dT1<<" "<<t0[1]<<" "<<t0[6]<<" "<<t0[31]<<endl;
					QvsToF3R->Fill(ToF, twid);
					// std::cout<<"ToF = "<<ToF<<std::endl;
				}

				if (t0[4] > -1 && t0[7] > -1) {
					// std::cout<<"Two PMTs are fired for VETO_ID == 3"<<std::endl;
					TDiff_VETO.push_back(t0[4] - t0[7] + 50);
					X_VETO.push_back((t0[4] - t0[7] + 50) * 525 / 378);
					ToF_VETO.push_back((t0[4] + t0[7]) / 2 - t0[31]);
					VETO_ID.push_back(3);
					Charge_VETO.push_back(sqrt(twidc[4] * twidc[7]));
				}
			}
		}
		if (VETO_ID.size() <= 3) {
			// std::cout<<"VETO_ID.size() = "<<VETO_ID.size()<<std::endl;
			for (int i = 0; i < VETO_ID.size(); i++) {
				// std::cout<<"VETO_ID.at(i) = "<<VETO_ID.at(i)<<std::endl;
			}
			// std::cout<<"                            "<<std::endl;
		}

		VETO_mult.push_back(VETO_ID.size());

		for (auto ich = 0; ich < 64; ++ich) {
			if (easiroc.ltdc[ich] != -1 && easiroc.ttdc[ich] != -1) {
				auto dtdc = easiroc.ltdc[ich] - easiroc.ttdc[ich];
				if (dtdc > 0) {
					easiroc.multi++;
				};
			}
		}

		tree->Fill();
		estore->ClearData();
		neve++;
	}

	std::cout << "Total number of events = " << neve << std::endl;

	fout->Write();
	fout->Close();
}
