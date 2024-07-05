void checkEntries(std::string file1, std::string file2) {

	auto chain = new TChain("hime", "hime");
	chain->Add(file1.c_str());

	auto chain2 = new TChain("tree", "tree");
	chain2->Add(file2.c_str());

	// std::cout << chain->GetEntries() << " " << chain2->GetEntries() << std::endl;

	if (chain->GetEntries() != chain2->GetEntries()) {
		std::cout << "Entries are not equal" << std::endl;
		std::cout << "Entries in taras file: " << chain->GetEntries() << std::endl;
		std::cout << "Entries in my file: " << chain2->GetEntries() << std::endl;
	}
}