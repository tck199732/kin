void chkraw(Int_t nRun = 5) {

	TArtEventStore *estore = new TArtEventStore();
	if (nRun <= 0)
		estore->Open(0);
	else
		estore->Open(Form("ridf/data%04d.ridf", nRun)); // cosmic, ecl term off
	// else        estore->Open(Form("ridf/data%04d.ridf.gz",nRun));// cosmic, ecl term off

	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile("macros/chkraw/chkraw.root", "RECREATE");
	TNtuple *ntp = new TNtuple("ntp", "ntp", "dev:fp:det:geo:ch:val:edge");

	int neve = 0;
	while (estore->GetNextEvent() && neve < 10000) {
		if (neve % 100 == 0)
			cout << "\r neve=" << neve << flush;

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

				ntp->Fill((float)device, (float)fp, (float)detector, (float)geo, (float)ch, (float)val, (float)edge);
			}
		}

		estore->ClearData();
		neve++;
	}

	fout->Write();
}
