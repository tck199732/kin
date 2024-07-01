#include "spirit.h"
#include <random>
#include <vector>

void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);

void AnaKyotoHimeMulti(
	// clang-format off
    const std::vector<std::string> &files = {
        // threshold = 1 runs in commissioning
        // "spirit/data1145.root",
        // "spirit/data1146.root",
        // 6.27,28 normal data taking
        "spirit/data1160.root",
        "spirit/data1161.root",
        "spirit/data1162.root",
        "spirit/data1163.root",
        "spirit/data1164.root",
        "spirit/data1165.root",
        "spirit/data1166.root",
        "spirit/data1167.root",
        "spirit/data1168.root",
        "spirit/data1169.root",
        "spirit/data1170.root",
        "spirit/data1171.root",
        "spirit/data1172.root",
        "spirit/data1173.root",
        "spirit/data1174.root",
        "spirit/data1175.root",
        "spirit/data1176.root",
        "spirit/data1177.root",
        "spirit/data1178.root",
        "spirit/data1179.root",
        "spirit/data1180.root",
    },
    const std::string& filenameOut = "results-kin/kyoto-hime-multi.root"
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);

	auto hKyotoLeftHimeMulti = new TH2D("hKyotoLeftHimeMulti", "", 40, 0, 40, 40, 0, 40);
	auto hKyotoRightHimeMulti = new TH2D("hKyotoRightHimeMulti", "", 40, 0, 40, 40, 0, 40);
	auto hKyotoTotalHimeMulti = new TH2D("hKyotoTotalHimeMulti", "", 40, 0, 40, 40, 0, 40);

	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);
		hKyotoTotalHimeMulti->Fill(spirit.kyoto_multi, spirit.hime_nHits);

		int left_hit = 0, right_hit = 0;
		for (auto ihit = 0; ihit < spirit.kyoto_multi; ihit++) {
			if (spirit.kyoto_bar[ihit] >= 0 && spirit.kyoto_bar[ihit] < 30) {
				right_hit++;
			} else if (spirit.kyoto_bar[ihit] >= 30 && spirit.kyoto_bar[ihit] < 60) {
				left_hit++;
			}
		}
		hKyotoLeftHimeMulti->Fill(left_hit, spirit.hime_nHits);
		hKyotoRightHimeMulti->Fill(right_hit, spirit.hime_nHits);
	}

	// normalization
	hKyotoLeftHimeMulti->Scale(1.0 / nEvents);
	hKyotoRightHimeMulti->Scale(1.0 / nEvents);
	hKyotoTotalHimeMulti->Scale(1.0 / nEvents);

	auto fileOut = new TFile(filenameOut.c_str(), "RECREATE");
	hKyotoLeftHimeMulti->Write();
	hKyotoRightHimeMulti->Write();
	hKyotoTotalHimeMulti->Write();
	fileOut->Close();
	return;
}

void addFilesToChain(const std::vector<std::string> &files, TChain *&chain) {
	for (auto fname : files) {
		chain->AddFile(fname.c_str());
	}
	return;
}
