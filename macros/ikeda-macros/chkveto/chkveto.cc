void chkveto(Int_t nRun = 5) {

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

	int t0[32] = {-1};
	int t1[32] = {-1};

	int neve = 0;
	while (estore->GetNextEvent() && neve < 10000) {
		if (neve % 100 == 0)
			cout << "\r neve=" << neve << flush;

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
			cout << ich << " " << twid << endl;
			if (twid > 0) {
				hchtwid->Fill(ich, twid);
			}
		}

		estore->ClearData();
		neve++;
	}

	fout->Write();
}
