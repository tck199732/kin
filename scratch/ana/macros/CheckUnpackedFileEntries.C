void CheckUnpackedFileEntries(std::string file1, std::string file2) {

	// auto chain = new TChain("tofTree", "Tree with TOF values");
	// chain->Add(file1.c_str());

	auto chain = new TChain("ridf", "ridf");
	chain->Add(file1.c_str());

	auto chain2 = new TChain("samurai", "samurai");
	chain2->Add(file2.c_str());

	// std::cout << chain->GetEntries() << " " << chain2->GetEntries() << std::endl;

	if (chain->GetEntries() != chain2->GetEntries()) {
		std::cout << "Entries are not equal" << std::endl;
		std::cout << "Entries in taras file: " << chain->GetEntries() << std::endl;
		std::cout << "Entries in my file: " << chain2->GetEntries() << std::endl;
	}
}