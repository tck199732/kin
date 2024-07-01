
void AnaKyoto(TString indata = "ridf/data0013.ridf", TString outdata = "test.root") {

	gSystem->Load("libanaroot.so");

	TArtEventStore *estore = new TArtEventStore();
	//  estore->Open("/home/daq/data/data0072.ridf");
	estore->Open(indata.Data());
	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile(outdata.Data(), "RECREATE");
	Int_t nedata, nvdata;
	Int_t hadc[64], ladc[64], ltdc[64], ttdc[64]; // EASIROC
	Int_t vltdc[128], vttdc[128];				  // V1190
	Int_t firsthittdc_e, firsthittdc_v;
	Int_t firsthit_e, firsthit_v;
	Int_t maxhittot_e, maxhittot_v;
	Int_t maxhit_e, maxhit_v;

	TTree *evetr = new TTree("mppceve", "mppceve");
	TTree *mppctr = new TTree("mppc", "mppc");
	mppctr->Branch("nedata", &nedata, "nedata/I");
	mppctr->Branch("nvdata", &nvdata, "nvdata/I");
	mppctr->Branch("firsthit_e", &firsthit_e, "firsthit_e/I");
	mppctr->Branch("firsthit_v", &firsthit_v, "firsthit_v/I");
	mppctr->Branch("firsthittdc_e", &firsthittdc_e, "firsthittdc_e/I");
	mppctr->Branch("firsthittdc_v", &firsthittdc_v, "firsthittdc_v/I");
	mppctr->Branch("hadc", hadc, "hadc[64]/I");
	mppctr->Branch("ladc", ladc, "ladc[64]/I");
	mppctr->Branch("ltdc", ltdc, "ltdc[64]/I");
	mppctr->Branch("ttdc", ttdc, "ttdc[64]/I");
	mppctr->Branch("vltdc", vltdc, "vltdc[64]/I");
	mppctr->Branch("vttdc", vttdc, "vttdc[64]/I");

	TH1F *hehit = new TH1F("hehit", "hehit", 128, 0, 128);
	TH1F *hvhit = new TH1F("hvhit", "hvhit", 128, 0, 128);
	TH2F *hevfhit = new TH2F("hevfirsthit", "hevfirsthit", 128, 0, 128, 128, 0, 128);
	TH2F *hevmhit = new TH2F("hevmaxhit", "hevmaxhit", 128, 0, 128, 128, 0, 128);

	int neve = 0;
	int address = 0;
	while (estore->GetNextEvent()) {

		nedata = 0;
		nvdata = 0;

		firsthittdc_e = 0;
		firsthittdc_v = 99999;
		firsthit_e = -1;
		firsthit_v = -1;

		maxhittot_e = -1;
		maxhittot_v = -1;
		maxhit_e = -1;
		maxhit_v = -1;

		for (int i = 0; i < 64; i++) {
			hadc[i] = -1;
			ladc[i] = -1;
			ltdc[i] = -1;
			ttdc[i] = -1;
		}
		for (int i = 0; i < 128; i++) {
			vltdc[i] = -1;
			vttdc[i] = -1;
		}

		for (int i = 0; i < rawevent->GetNumSeg(); i++) {
			TArtRawSegmentObject *seg = rawevent->GetSegment(i);
			address = seg->GetAddress();
			std::cout << "mod:" << seg->GetModule() << " efn:" << seg->GetAddress() << " nd:" << seg->GetNumData()
					  << std::endl;
			if (24 == seg->GetModule()) { // V1190
				// std::cout << "v1190 ndata: " << seg->GetNumData() << std::endl;
				nvdata += seg->GetNumData();
				for (int j = 0; j < seg->GetNumData(); j++) {
					TArtRawDataObject *d = seg->GetData(j);
					int geo = d->GetGeo();
					int ch = d->GetCh();
					// ch = ch>31 ? ch-32 : ch+32; // should be modified based on mapping
					ch = ch > 63 ? ch - 33 : ch; // should be modified based on mapping
					int val = d->GetVal();
					int edge = d->GetEdge();
					// std::cout << "geo:" << geo << " ch:" << ch << " edge:" << edge << " val:" << val << std::endl;

					std::cout << "v ch:" << ch << " val:" << val << " edge:" << edge << std::endl;
					if (0 == edge) {
						if (val < firsthittdc_v) {
							firsthit_v = ch;
							firsthittdc_v = val;
						}
						hvhit->Fill(ch);
						vltdc[ch] = val;
					} else if (1 == edge) {
						vttdc[ch] = val;
					}

				} //	for(int j=0;j<seg->GetNumData();j++)

			} // if(24 == seg->GetModule())
			if (35 == seg->GetModule()) { // EASIROC
				// std::cout << "easir ndata: " << seg->GetNumData() << std::endl;
				nedata += seg->GetNumData();
				for (int j = 0; j < seg->GetNumData(); j++) {
					TArtRawDataObject *d = seg->GetData(j);
					int geo = d->GetGeo();
					int ch = d->GetCh();
					int val = d->GetVal();

					/*
					 * @file TArtDecoderVmeEasyroc.cc
					 * geo:0 High gain ADC
					 * geo:1 Low gain ADC
					 * geo:2 Leading edge TDC
					 * geo:3 Trailing edge TDC
					 * geo:4 Scaler
					 */

					// std::cout << "geo:" << geo << " ch:" << ch << " edge:" << edge << " val:" << val << std::endl;
					std::cout << "geo:" << geo << " ch:" << ch << " val:" << val << std::endl;

					if (ch > 64) {
						std::cout << "Error!! Channel for EASIROC should be less than 64!!: " << ch << std::endl;
					} else if (0 == geo) {
						hadc[ch] = val;
					} else if (1 == geo) {
						ladc[ch] = val;
					} else if (2 == geo) {
						std::cout << "e ch:" << ch << " val:" << val << " edge:" << 0 << std::endl;
						if (val > firsthittdc_e) {
							firsthit_e = ch;
							firsthittdc_e = val;
						}
						hehit->Fill(ch);
						ltdc[ch] = val;
					} else if (3 == geo) {
						std::cout << "e ch:" << ch << " val:" << val << " edge:" << 1 << std::endl;
						ttdc[ch] = val;
					}
				}
			} // if(35 == seg->GetModule()){ // EASIROC

		} // for(int i=0;i<rawevent->GetNumSeg();i++)

		for (int i = 0; i < 64; i++)
			if (ltdc[i] > 0 && ttdc[i] > 0) {
				Int_t etot = ltdc[i] - ttdc[i];
				std::cout << "e ch:" << i << " ltdc:" << ltdc[i] << " tot:" << etot << std::endl;
				if (etot > maxhittot_e) {
					maxhittot_e = etot;
					maxhit_e = i;
				}
			}
		for (int i = 0; i < 64; i++)
			if (vltdc[i] > 0 && vttdc[i] > 0) {
				Int_t vtot = vttdc[i] - vltdc[i];
				std::cout << "v ch:" << i << " ltdc:" << vltdc[i] << " tot:" << vtot << std::endl;
				if (vtot > maxhittot_v) {
					maxhittot_v = vtot;
					maxhit_v = i;
				}
			}

		estore->ClearData();
		neve++;
		mppctr->Fill();
		hevfhit->Fill(firsthit_e, firsthit_v);
		hevmhit->Fill(maxhit_e, maxhit_v);
		std::cout << "firsthit_e:" << firsthit_e << " firsthit_v:" << firsthit_v << " dch:" << firsthit_v - firsthit_e;
		if (firsthit_e > 0)
			std::cout << " adc:" << ladc[firsthit_e];
		std::cout << std::endl;
		std::cout << "maxhit_e:" << maxhit_e << " maxhit_v:" << maxhit_v << " dch:" << maxhit_v - maxhit_e;
		if (maxhit_e > 0)
			std::cout << " adc:" << ladc[maxhit_e];
		std::cout << std::endl;
	}

	mppctr->Fill();
	fout->Write();
	fout->Close();
}
