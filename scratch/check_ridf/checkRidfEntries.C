void checkRidfEntries(
	// clang-format off
    const std::string& filename = "data1036.root"
	// clang-format on
) {
	std::ofstream ofs("runId.dat", std::ofstream::out | std::ofstream::app);
	TChain *chain = new TChain("ridf", "ridf");
	chain->AddFile(filename.c_str());
	int nEntries = chain->GetEntries();
	auto stem = filename.substr(filename.find_last_of("/") + 1, filename.find_last_of(".") - filename.find_last_of("/"));
	std::cout << "filename: " << stem << ", nEntries: " << nEntries << std::endl;
	if (nEntries > 0) {
		auto runId = std::stoi(stem.substr(4, stem.find_last_of(".") - 4));
		ofs << runId << std::endl;
	}
	delete chain;

    ofs.close();

	return;
}
