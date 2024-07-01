void chkveto(Int_t nRun = 3) {

	int ch_1L = 6;
	int ch_1R = 1;
	int ch_2L = 2;
	int ch_2R = 3;
	int ch_3L = 4;
	int ch_3R = 7;

	TArtEventStore *estore = new TArtEventStore();
	if (nRun <= 0)
		estore->Open(0);
	else
		estore->Open(Form("ridf/data%04d.ridf", nRun)); // cosmic, ecl term off
	// else        estore->Open(Form("ridf/data%04d.ridf.gz",nRun));// cosmic, ecl term off

	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile("macros/chkveto/chkveto.root", "RECREATE");
	// TH1* hchval = new TH2D("hchval","Ch Val leading",32,-0.5,31.5, 100,120000,200000);
	TH1 *hchval = new TH2D("hchval", "Ch Val leading", 32, -0.5, 31.5, 100, 0, 200000);
	TH1 *hchval_z = new TH2D("hchval_z", "Ch Val leading zoom", 32, -0.5, 31.5, 200, 130000, 140000);
	TH1 *hchval_t = new TH2D("hchval_t", "Ch Val trailing", 32, -0.5, 31.5, 100, 120000, 200000);
	TH1 *hchtwid = new TH2D("hchtfwid", "Ch width(ch)", 32, -0.5, 31.5, 100, 0, 20000);

	TH1 *ht0t1 = new TH2D("ht0t1", "T(ch0) T(ch1)", 100, 100000, 140000, 100, 130000, 140000);
	TH1 *ht0t3 = new TH2D("ht0t3", "T(ch0) T(ch3)", 100, 100000, 140000, 100, 130000, 140000);

	TH1 *htwidlr1 = new TH2D("htwidlr1", "Twid(L) Twid(R) VETO1", 100, 0, 10000, 100, 0, 10000);
	TH1 *htwidlr2 = new TH2D("htwidlr2", "Twid(L) Twid(R) VETO2", 100, 0, 10000, 100, 0, 10000);
	TH1 *htwidlr3 = new TH2D("htwidlr3", "Twid(L) Twid(R) VETO3", 100, 0, 10000, 100, 0, 10000);

	TH1 *hdttwid0_1 = new TH2D("hdttwid0_1", "dT(0-1) Twid(ch0)", 100, -1000, 1000, 100, 0, 10000);
	TH1 *hdttwid1_1 = new TH2D("hdttwid1_1", "dT(0-1) Twid(ch1)", 100, -1000, 1000, 100, 0, 10000);
	TH1 *hdttwid2_2 = new TH2D("hdttwid0_2", "dT(2-3) Twid(ch2)", 100, -1000, 1000, 100, 0, 10000);
	TH1 *hdttwid3_2 = new TH2D("hdttwid1_2", "dT(2-3) Twid(ch3)", 100, -1000, 1000, 100, 0, 10000);
	TH1 *hdttwid4_3 = new TH2D("hdttwid0_3", "dT(4-5) Twid(ch4)", 100, -1000, 1000, 100, 0, 10000);
	TH1 *hdttwid5_3 = new TH2D("hdttwid1_3", "dT(4-5) Twid(ch5)", 100, -1000, 1000, 100, 0, 10000);

	TH1 *htoftwl_1 = new TH2D("htoftwl_1", "TOF Twid(1L)", 100, -25000, -15000, 100, 0, 20000);
	TH1 *htoftwr_1 = new TH2D("htoftwr_1", "TOF Twid(1R)", 100, -25000, -15000, 100, 0, 20000);
	TH1 *htoftwl_2 = new TH2D("htoftwl_2", "TOF Twid(2L)", 100, -25000, -15000, 100, 0, 20000);
	TH1 *htoftwr_2 = new TH2D("htoftwr_2", "TOF Twid(2R)", 100, -25000, -15000, 100, 0, 20000);
	TH1 *htoftwl_3 = new TH2D("htoftwl_3", "TOF Twid(3L)", 100, -25000, -15000, 100, 0, 20000);
	TH1 *htoftwr_3 = new TH2D("htoftwr_3", "TOF Twid(3R)", 100, -25000, -15000, 100, 0, 20000);

	//  TH1* htoftwl_1 = new TH2D("htoftwl_1","TOF Twid(1L)",100,-45000,-35000, 100,0,20000);
	//  TH1* htoftwr_1 = new TH2D("htoftwr_1","TOF Twid(1R)",100,-45000,-35000, 100,0,20000);
	//  TH1* htoftwl_2 = new TH2D("htoftwl_2","TOF Twid(2L)",100,-45000,-35000, 100,0,20000);
	//  TH1* htoftwr_2 = new TH2D("htoftwr_2","TOF Twid(2R)",100,-45000,-35000, 100,0,20000);
	//  TH1* htoftwl_3 = new TH2D("htoftwl_3","TOF Twid(3L)",100,-45000,-35000, 100,0,20000);
	//  TH1* htoftwr_3 = new TH2D("htoftwr_3","TOF Twid(3R)",100,-45000,-35000, 100,0,20000);

	// TNtuple *ntp = new TNtuple("ntp","ntp","dev:fp:det:geo:ch:val:edge");

	int t_l[32] = {-1};
	int t_t[32] = {-1};

	int neve = 0;
	// while(estore->GetNextEvent() && neve < 10000){
	while (estore->GetNextEvent()) {
		if (neve % 100 == 0)
			cout << "\r neve=" << neve << flush;

		for (int ich = 0; ich < 32; ++ich) {
			t_l[ich] = -1;
			t_t[ich] = -1;
		}

		for (int i = 0; i < rawevent->GetNumSeg(); i++) {
			TArtRawSegmentObject *seg = rawevent->GetSegment(i);
			int device = seg->GetDevice();
			int fp = seg->GetFP();
			int detector = seg->GetDetector();
			int module = seg->GetModule();

			if (detector != 50)
				continue;

			//      cout<<detector<<" "
			//	  <<module<<" "
			//	  <<endl;

			for (int j = 0; j < seg->GetNumData(); j++) {
				TArtRawDataObject *d = seg->GetData(j);
				int geo = d->GetGeo();
				int ch = d->GetCh();
				int val = d->GetVal();
				int cat = d->GetCategoryID();
				int det = d->GetDetectorID();
				int id = d->GetDatatypeID();
				int edge = d->GetEdge();

				//-------------------------------
				if (edge == 0) { // leading
					t_l[ch] = val;

					hchval->Fill(ch, val);
					hchval_z->Fill(ch, val);

					//-------------------------------
				} else if (edge == 1) { // trailing
					t_t[ch] = val;

					hchval_t->Fill(ch, val);
				}
				//-------------------------------

				// ntp->Fill((float)device,(float)fp,(float)detector,(float)geo,(float)ch,(float)val,(float)edge);
			}
		}

		for (int ich = 0; ich < 32; ++ich) {
			int twid = t_t[ich] - t_l[ich];
			// cout<<ich<<" "<<twid<<endl;
			if (twid > 0) {
				hchtwid->Fill(ich, twid);
			}
		}

		ht0t1->Fill(t_l[ch_1L], t_l[ch_1R]);

		if (t_l[ch_1L] > 0 && t_l[ch_1R] > 0) {
			double tof = 0.5 * (t_l[ch_1L] + t_l[ch_1R]) - t_l[31];
			double tw_L = t_t[ch_1L] - t_l[ch_1L];
			double tw_R = t_t[ch_1R] - t_l[ch_1R];

			hdttwid0_1->Fill(t_l[ch_1L] - t_l[ch_1R], tw_L);
			hdttwid1_1->Fill(t_l[ch_1L] - t_l[ch_1R], tw_R);

			htoftwl_1->Fill(tof, tw_L);
			htoftwr_1->Fill(tof, tw_R);

			htwidlr1->Fill(tw_L, tw_R);
		}

		if (t_l[ch_2L] > 0 && t_l[ch_2R] > 0) {
			double tof = 0.5 * (t_l[ch_2L] + t_l[ch_2R]) - t_l[31];
			double tw_L = t_t[ch_2L] - t_l[ch_2L];
			double tw_R = t_t[ch_2R] - t_l[ch_2R];

			hdttwid2_2->Fill(t_l[ch_2L] - t_l[ch_2R], tw_L);
			hdttwid3_2->Fill(t_l[ch_2L] - t_l[ch_2R], tw_R);

			htoftwl_2->Fill(tof, tw_L);
			htoftwr_2->Fill(tof, tw_R);

			htwidlr2->Fill(tw_L, tw_R);
		}

		if (t_l[ch_3L] > 0 && t_l[ch_3R] > 0) {
			double tof = 0.5 * (t_l[ch_3L] + t_l[ch_3R]) - t_l[31];
			double tw_L = t_t[ch_3L] - t_l[ch_3L];
			double tw_R = t_t[ch_3R] - t_l[ch_3R];

			hdttwid4_3->Fill(t_l[ch_3L] - t_l[ch_3R], tw_L);
			hdttwid5_3->Fill(t_l[ch_3L] - t_l[ch_3R], tw_R);

			htoftwl_3->Fill(tof, tw_L);
			htoftwr_3->Fill(tof, tw_R);

			htwidlr3->Fill(tw_L, tw_R);
		}

		estore->ClearData();
		neve++;
	}

	fout->Write();
}
