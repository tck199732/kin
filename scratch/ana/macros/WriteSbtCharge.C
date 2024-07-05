
struct SbtData {
	double charge;
	int time;
	int trigger;
	int detID;
};

SbtData sbt1, sbt2;

void SetBranchAddress(TChain *chain);
long getEntries(const std::string &filename);
void WriteSbtCharge(
	// clang-format off
    const std::string& filename = "analyzed/data1036.root",
    const std::string& filenameOut = "result/sbtCharge/data1036.txt"
	// clang-format on
) {
	auto chain = new TChain("samurai", "samurai");
	SetBranchAddress(chain);
	chain->Add(filename.c_str());

	auto nEntriesTotal = chain->GetEntries();
	std::cout << "nEntriesTotal = " << nEntriesTotal << std::endl;

	std::ofstream ofs(filenameOut.c_str());

	for (auto i = 0; i < nEntriesTotal; i++) {
		chain->GetEntry(i);
		ofs << i << " " << sbt1.charge << " " << sbt2.charge << '\n';
	}
	ofs.close();
}

long getEntries(const std::string &filename) {
	auto chain = std::make_unique<TChain>("samurai", "samurai");
	SetBranchAddress(chain.get());
	chain->Add(filename.c_str());
	return chain->GetEntries();
}

void SetBranchAddress(TChain *chain) {
	chain->SetBranchAddress("sbtCharge1", &sbt1.charge);
	chain->SetBranchAddress("sbtTime1", &sbt1.time);
	chain->SetBranchAddress("sbtTrigger1", &sbt1.trigger);
	chain->SetBranchAddress("sbtCharge2", &sbt2.charge);
	chain->SetBranchAddress("sbtTime2", &sbt2.time);
	chain->SetBranchAddress("sbtTrigger2", &sbt2.trigger);
	return;
}