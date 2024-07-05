struct KyotoData {
	unsigned int multi;
	std::array<unsigned int, 128> bars;
};

KyotoData kyoto;

void SetBranchAddress(TChain *chain);

void WriteKyotoHits(
	// clang-format off
    const std::string& filename = "analyzed/data1046.root",
    const std::string& filenameOut = "result/kyotoHits/data1046.txt"
	// clang-format on
) {
	auto chain = new TChain("samurai", "samurai");
	SetBranchAddress(chain);
	chain->Add(filename.c_str());

	auto nEntriesTotal = chain->GetEntries();
	std::cout << "nEntriesTotal = " << nEntriesTotal << std::endl;

	auto hMultiBars = new TH1D("hMulti", "", 60, 0, 60);

	for (auto i = 0; i < nEntriesTotal; i++) {
		chain->GetEntry(i);
		for (auto j = 0; j < kyoto.multi; j++) {
			hMultiBars->Fill(kyoto.bars[j]);
		}
	}

	hMultiBars->Scale(1.0 / nEntriesTotal);

	std::ofstream file(filenameOut);
	for (auto ibin = 1; ibin <= hMultiBars->GetNbinsX(); ibin++) {
		file << hMultiBars->GetBinLowEdge(ibin) << " " << hMultiBars->GetBinContent(ibin) << std::endl;
	}
}

long getEntries(const std::string &filename) {
	auto chain = std::make_unique<TChain>("samurai", "samurai");
	SetBranchAddress(chain.get());
	chain->Add(filename.c_str());
	return chain->GetEntries();
}

void SetBranchAddress(TChain *chain) {
	chain->SetBranchAddress("kyotoMulti", &kyoto.multi);
	chain->SetBranchAddress("kyotoBarId", &kyoto.bars[0]);
	return;
}
