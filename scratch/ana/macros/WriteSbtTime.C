
struct SbtData {
	double charge;
	int time;
	int trigger;
	int detID;
};

SbtData sbt1, sbt2;

void SetBranchAddress(TChain *chain);
long getEntries(const std::string &filename);
void WriteSbtTime(
	// clang-format off
    const std::string& filename = "analyzed/data1036.root",
    const std::string& filenameOut = "result/sbtTime/data1036.txt"
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
		ofs << i << " " << sbt1.time << " " << sbt2.time << '\n';
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
	chain->SetBranchAddress("sbt1_charge", &sbt1.charge);
	chain->SetBranchAddress("sbt1_time", &sbt1.time);
	chain->SetBranchAddress("sbt1_trigger", &sbt1.trigger);
	chain->SetBranchAddress("sbt2_charge", &sbt2.charge);
	chain->SetBranchAddress("sbt2_time", &sbt2.time);
	chain->SetBranchAddress("sbt2_trigger", &sbt2.trigger);
	return;
}