/**
 * @brief Check Veto raw data
 * @date 2024-07-09 02:38
 * @details TiTect people are taking cosmic data (run 1372 - 1386), for ch0 to ch15. They are using the samurai daq with
 * similar settings in the experiment. This script excludes data from other detectors : SBT, kyoto, scaler, Lupo module,
 * bdc.
 *
 *
 */
void CheckVetoRawData(
	// clang-format off
    const int& nRun = 1386,
    const std::string& outFileName = "vetoRaw.root"
	// clang-format on
) {

	auto estore = new TArtEventStore();
	TString indata = Form("ridf/data%04d.ridf", nRun);

	// check if the file exists
	if (gSystem->AccessPathName(indata)) {
		cout << "File " << indata << " does not exist." << endl;
		return;
	}
	estore->Open(indata.Data());
	auto rawevent = estore->GetRawEventObject();

	auto fout = new TFile(outFileName.c_str(), "RECREATE");
	auto ntp = new TNtuple("ntp", "ntp", "dev:fp:det:module:addr:geo:ch:val:edge");

	int neve = 0;
	while (estore->GetNextEvent()) {
		for (auto i = 0; i < rawevent->GetNumSeg(); i++) {
			auto seg = rawevent->GetSegment(i);
			auto device = seg->GetDevice();
			auto fp = seg->GetFP();
			auto detector = seg->GetDetector();
			auto module = seg->GetModule();
			auto addr = seg->GetAddress();

			// module 0 is present but never used in my unpacker, Aki said it is not being used.
			if (module == 0) {
				continue;
			}

			// sbt
			if (detector == 2 && module == 33 && addr == 85) {
				continue;
			}
			// sbt
			if (detector == 3 && module == 34 && addr == 85) {
				continue;
			}
			// kyoto
			if (module == 35 && addr == 98) {
				continue;
			}
			// scaler
			if (detector == 61 && module == 8 && addr == 96) {
				continue;
			}
			// bdc
			if (detector == 27 && module == 36 && addr == 59) {
				continue;
			}
			// lupo
			if (fp == 13 && detector == 60 && addr == 85 && module == 42) {
				continue;
			}

			for (auto j = 0; j < seg->GetNumData(); j++) {
				auto d = seg->GetData(j);
				auto geo = static_cast<double>(d->GetGeo());
				auto ch = static_cast<double>(d->GetCh());
				auto val = static_cast<double>(d->GetVal());
				auto cat = static_cast<double>(d->GetCategoryID());
				auto det = static_cast<double>(d->GetDetectorID());
				auto id = static_cast<double>(d->GetDatatypeID());
				auto edge = static_cast<double>(d->GetEdge());
				ntp->Fill(device, fp, detector, module, addr, geo, ch, val, edge);
			}
		}
		estore->ClearData();
		neve++;
	}
	fout->Write();
}
