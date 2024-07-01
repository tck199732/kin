void arrayveto(Int_t nRun = 3) {

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
	/*TH1* QvsT0 = new TH2D("QvsT0","QvsT0",80,-1000,1000, 100,0,20000);
	TH1* QvsT1 = new TH2D("QvsT1","QvsT1",80,-1000,1000, 100,0,20000);
	TH1* QvsT2 = new TH2D("QvsT2","QvsT2",80,-1000,1000, 100,0,20000);
	TH1* QvsT3 = new TH2D("QvsT3","QvsT3",80,-1000,1000, 100,0,20000);
	TH1* QvsT4 = new TH2D("QvsT4","QvsT4",80,-1000,1000, 100,0,20000);
	TH1* QvsT5 = new TH2D("QvsT5","QvsT5",80,-1000,1000, 100,0,20000);
	TH1* dT1a = new TH1D("dT1","dT1",80,-1000,1000);
	TH1* dT2a = new TH1D("dT2","dT2",80,-1000,1000);
	TH1* dT3a = new TH1D("dT3","dT1",80,-1000,1000);
	TH1* dT4a = new TH1D("dT4","dT2",80,-1000,1000);
	TH1* dT5a = new TH1D("dT5","dT1",80,-1000,1000);
	TH1* dT6a = new TH1D("dT6","dT2",80,-1000,1000);*/
	int t0[32] = {-1};
	int t1[32] = {-1};

	TH1 *QvsT[5];
	for (int ich = 0; ich < 6; ++ich) {
		QvsT[ich] = new TH2D(Form("QvsT%d", ich), Form("QvsT%d", ich), 80, -1000, 1000, 100, 0, 4000);
	}
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

		for (int ich = 0; ich < 32; ++ich) {
			int twid = t1[ich] - t0[ich];
			// cout<<ich<<" "<<twid<<endl;
			if (twid > 0) {
				hchtwid->Fill(ich, twid);
				if (ich < 6 & ich % 2 == 0) {
					double dT = t0[ich + 1] - t0[ich];
					QvsT[ich]->Fill(dT, twid);
				}
				if (ich < 6 & ich % 2 == 1) {
					double dT = t0[ich] - t0[ich - 1];
					QvsT[ich]->Fill(dT, twid);
				}
				// cout<<"a"<<endl;
				/*if(ich==0){
				double dT1 = t1[ich+1]-t1[ich];
				//cout<<t1[ich+1]<<" "<<t1[ich]<<" "<<dT1<<" "<<twid<<endl;
				QvsT0->Fill(dT1,twid);
				dT1a->Fill(dT1);
				  }
				  if(ich==1){
				double dT2 = t1[ich-1]-t1[ich];
				//cout<<ich<<" "<<dT<<" "<<twid<<endl;
				QvsT1->Fill(dT2,twid);
				dT2a->Fill(dT2);
				  }


				  if(ich==2){
				double dT3 = t1[ich+1]-t1[ich];
				//cout<<ich<<" "<<dT<<" "<<twid<<endl;
				QvsT2->Fill(dT3,twid);
				dT3a->Fill(dT3);
				  }
				  if(ich==3){
				double dT4 = t1[ich-1]-t1[ich];
				//cout<<ich<<" "<<dT<<" "<<twid<<endl;
				QvsT3->Fill(dT4,twid);
				dT4a->Fill(dT4);
				  }
				  if(ich==4){
				double dT5 = t1[ich]-t1[ich+1];
				//cout<<ich<<" "<<dT<<" "<<twid<<endl;
				QvsT4->Fill(dT5,twid);
				dT5a->Fill(dT5);
				  }
				  if(ich==5){
				double dT6 = t1[ich]-t1[ich-1];
				//cout<<ich<<" "<<dT<<" "<<twid<<endl;
				QvsT5->Fill(dT6,twid);
				dT6a->Fill(dT6);
				}*/
			}
			/*for (int ich=0;ich<32;++ich){
		  if(ich==0){
		  double dT = t1[ich+1]-t1[ich];
			QvsT[ich]->Fill(dT,twid);
			}
		  else if(ich % 2 ==0){
			  double dT = t1[ich]-t1[ich-1];
			  cout<<"b"<<endl;
			QvsT[ich]->Fill(dT,twid);
			}
			else{
		  double dT = t1[ich-1]-t1[ich];
		  QvsT[ich]->Fill(dT,twid);
			}
			}*/
		}

		estore->ClearData();
		neve++;
	}

	fout->Write();
}
