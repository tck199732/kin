
void CheckRIDF() {

	gSystem->Load("libanaroot.so");

	TArtEventStore *estore = new TArtEventStore();
	estore->Open("test.ridf");
	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	int neve = 0;
	ULong64_t lupots;

	while (estore->GetNextEvent() && neve < 10) {
		// while(estore->GetNextEvent()){
		for (int i = 0; i < rawevent->GetNumSeg(); i++) {
			TArtRawSegmentObject *seg = rawevent->GetSegment(i);
			int efn = seg->GetAddress();
			int fpl = seg->GetFP();
			int det = seg->GetDetector();
			int mod = seg->GetModule();

			if (fpl == 13 && det == 60 && efn == 85) { // LUPO
				TArtRawDataObject *d = seg->GetData(0);
				ULong64_t valh = d->GetVal();
				d = seg->GetData(1);
				ULong64_t vall = d->GetVal();
				lupots = (valh << 32) + vall;
				std::cout << "lupots: " << lupots << std::endl;
			}

		} // for(int i=0;i<rawevent->GetNumSeg();i++)

		estore->ClearData();
		neve++;
	} // while(estore->GetNextEvent())
}
