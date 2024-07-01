void modiveto(Int_t nRun = 1040) {

	TArtEventStore *estore = new TArtEventStore();
	if (nRun <= 0)
		estore->Open(0);
	else
		estore->Open(Form("ridf/data%04d.ridf", nRun)); // cosmic, ecl term off
	// else        estore->Open(Form("ridf/data%04d.ridf.gz",nRun));// cosmic, ecl term off

	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile("macros/chkveto2.root", "RECREATE");
	TH1 *hchval = new TH2D("hchval", "Ch Val leading", 32, -0.5, 31.5, 100, 120000, 200000);
	TH1 *hchval_t = new TH2D("hchval_t", "Ch Val trailing", 32, -0.5, 31.5, 100, 120000, 200000);
	TH1 *hchtwid = new TH2D("hchtfwid", "Ch width(ch)", 32, -0.5, 31.5, 100, 0, 20000);

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
	int t0[32] = {-1};
	int t1[32] = {-1};

	int neve = 0;
	while (estore->GetNextEvent() && neve < 1e+8) {
		// if (neve%100==0) cout<<"\r neve="<<neve<<flush;

		for (int ich = 0; ich < 32; ++ich) {
			t0[ich] = -1;
			t1[ich] = -1;
		}

		for (int i = 0; i < rawevent->GetNumSeg(); i++) {
			TArtRawSegmentObject *seg = rawevent->GetSegment(i);
			int device = seg->GetDevice();
			int fp = seg->GetFP();
			int detector = seg->GetDetector();
			int module = seg->GetModule();
			if (detector != 50)
				continue;

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
				double ToF = -99999;
				//-------------------------------
				if (edge == 0) { // leading
					t0[ch] = val;

					hchval->Fill(ch, val);

					//-------------------------------
				} else if (edge == 1) { // trailing
					t1[ch] = val;

					hchval_t->Fill(ch, val);
				}
				//-------------------------------

				// ntp->Fill((float)device,(float)fp,(float)detector,(float)geo,(float)ch,(float)val,(float)edge);
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
					QvsT0->Fill(dT1, twid);
					dT1a->Fill(dT1);
					double ave = (t0[1] + t0[6]) / 2;
					double ToF = ave - t0[31];
					QvsToF1L->Fill(ToF, twid);
				}
				if (ich == 1) {
					double dT2 = t0[6] - t0[1];
					QvsT1->Fill(dT2, twid);
					dT2a->Fill(dT2);
					double ave = (t0[1] + t0[6]) / 2;
					double ToF = ave - t0[31];
					QvsToF1R->Fill(ToF, twid);
				}

				if (ich == 2) {
					double dT3 = t0[ich + 1] - t0[ich];
					QvsT2->Fill(dT3, twid);
					dT3a->Fill(dT3);
					double ave = (t0[2] + t0[3]) / 2;
					double ToF = ave - t0[31];
					QvsToF2L->Fill(ToF, twid);
				}
				if (ich == 3) {
					double dT4 = t0[ich] - t0[ich - 1];
					QvsT3->Fill(dT4, twid);
					dT4a->Fill(dT4);
					double ave = (t0[2] + t0[3]) / 2;
					double ToF = ave - t0[31];
					QvsToF2R->Fill(ToF, twid);
				}
				if (ich == 4) {
					double dT5 = t0[4] - t0[7];
					QvsT4->Fill(dT5, twid);
					dT5a->Fill(dT5);
					double ave = (t0[4] + t0[7]) / 2;
					double ToF = ave - t0[31];
					QvsToF3L->Fill(ToF, twid);
				}
				if (ich == 7) {
					double dT6 = t0[7] - t0[4];
					QvsT5->Fill(dT6, twid);
					dT6a->Fill(dT6);
					double ave = (t0[4] + t0[7]) / 2;
					double ToF = ave - t0[31];
					QvsToF3R->Fill(ToF, twid);
				}
			}
		}

		estore->ClearData();
		neve++;
	}

	fout->Write();
}
