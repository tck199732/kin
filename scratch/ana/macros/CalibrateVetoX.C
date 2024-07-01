struct Veto {
	unsigned int multi;
	std::array<unsigned int, 128> bars;
	std::array<double, 128> tdiffs;
	std::array<double, 128> charges;
	std::array<double, 128> xpos;
};

Veto veto;
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);
void SetBranchAddress(TChain *chain);
void CalibrateVetoX(
	// clang-format off
    const std::vector<std::string> &files = {
        "macros/calibrate-veto-runs-ikeda-map/data1036.root",
        "macros/calibrate-veto-runs-ikeda-map/data1037.root",
        "macros/calibrate-veto-runs-ikeda-map/data1038.root",
        "macros/calibrate-veto-runs-ikeda-map/data1039.root",
        "macros/calibrate-veto-runs-ikeda-map/data1040.root",
        "macros/calibrate-veto-runs-ikeda-map/data1041.root",
        "macros/calibrate-veto-runs-ikeda-map/data1042.root",
        "macros/calibrate-veto-runs-ikeda-map/data1043.root",
        "macros/calibrate-veto-runs-ikeda-map/data1044.root",
        "macros/calibrate-veto-runs-ikeda-map/data1045.root",
        "macros/calibrate-veto-runs-ikeda-map/data1046.root",
        "macros/calibrate-veto-runs-ikeda-map/data1047.root",
        "macros/calibrate-veto-runs-ikeda-map/data1048.root"
    },
	const std::string& rfilenameOut = "result/veto-tdiff-uncalibrated.root",
    const std::string& tfilenameOut = "result/veto-tdiff-uncalibrated.txt",
    const int & bins = 250,

    const std::vector<std::array<double, 2>>& fittingRanges = {
        // {-80, 500}, 
        // {-284, 276}, 
        // {-196, 364},
        {-500, 76}, 
        {-284, 276}, 
        {-364, 204},
    } // clang-format on
) {
	auto chain = new TChain("samurai", "samurai");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);
	TH1D *hVetotDiff[3];
	TF1 *hFitDiff[3];

	for (int i = 0; i < 3; i++) {
		hVetotDiff[i] = new TH1D(Form("hVetotDiffBar%d", i), "", bins, -1000, 1000);
	}
	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);
		for (auto vhit = 0; vhit < veto.multi; vhit++) {
			int iBar = veto.bars[vhit];
			auto tDiff = veto.tdiffs[vhit];
			hVetotDiff[iBar]->Fill(tDiff);
		}
	}

	for (int i = 0; i < 3; i++) {
		hVetotDiff[i]->Scale(1.0 / nEvents);
	}

	for (int i = 0; i < 3; i++) {
		hFitDiff[i] = new TF1(Form("hFitDiffBar%d", i), "pol1", fittingRanges[i][0], fittingRanges[i][1]);
		hVetotDiff[i]->Fit(hFitDiff[i], "R");
	}

	std::array<double, 3> offsets;
	for (int i = 0; i < 3; i++) {
		auto y0 = hFitDiff[i]->Eval(fittingRanges[i][0]);
		auto y1 = hFitDiff[i]->Eval(fittingRanges[i][1]);
		auto ym = 0.5 * (y0 + y1);
		// std::cout << y0 << " " << y1 << " " << ym << " " << hFitDiff[i]->GetParameter(0) << " "
		//   << hFitDiff[i]->GetParameter(1) << std::endl;
		std::cout << ym << " " << hFitDiff[i]->GetParameter(0) << " " << hFitDiff[i]->GetParameter(1) << " "
				  << (ym - hFitDiff[i]->GetParameter(0)) / hFitDiff[i]->GetParameter(1) << std::endl;
		offsets[i] = (ym - hFitDiff[i]->GetParameter(0)) / hFitDiff[i]->GetParameter(1);
	}

	auto fOut = new TFile(rfilenameOut.c_str(), "recreate");
	for (int i = 0; i < 3; i++) {
		hVetotDiff[i]->Write();
	}
	fOut->Close();

	std::ofstream ofs(tfilenameOut.c_str());
	ofs << "bar offset" << std::endl;
	for (int i = 0; i < 3; i++) {
		ofs << i << " " << offsets[i] << std::endl;
	}
}
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain) {
	for (auto fname : files) {
		chain->AddFile(fname.c_str());
	}
	return;
}
void SetBranchAddress(TChain *chain) {
	chain->SetBranchAddress("hime_veto_multi", &veto.multi);
	chain->SetBranchAddress("hime_veto_bar", &veto.bars[0]);
	chain->SetBranchAddress("hime_veto_tdiff", &veto.tdiffs[0]);
	return;
}
