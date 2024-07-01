
void ProjectTof(
	// clang-format off
    const int& start_bin = 3,
    const int& end_bin = 34,
    const std::string& fileName = "results/tot_vs_tof.root",
    const std::string& outDir = "tof_data",
    const std::string& histName = "hTotVsTof"
	// clang-format on
) {
	auto tFile = new TFile(fileName.c_str(), "READ");
	auto hTotVsTof = (TH2D *)tFile->Get(histName.c_str());

	for (int ibin = start_bin; ibin <= end_bin; ibin++) {
		std::string outName = Form("%s/%03d.dat", outDir.c_str(), ibin);
		auto hTof = hTotVsTof->ProjectionX(outName.c_str(), ibin, ibin);

		std::ofstream outFile(outName);
		for (int i = 1; i <= hTof->GetNbinsX(); i++) {
			outFile << hTof->GetBinCenter(i) << " " << hTof->GetBinContent(i) << " " << hTof->GetBinError(i)
					<< std::endl;
		}
		outFile.close();
	}
	return;
}