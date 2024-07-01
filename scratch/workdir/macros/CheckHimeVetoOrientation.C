#include "spirit.h"
#include <random>
#include <vector>

void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);

const double barWidthInMeter = 0.30; // 300 mm

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> unif(0.0, 1.0);

void CheckHimeVetoOrientation(
	// clang-format off
    const std::vector<std::string> &files = {
        "spirit/data1036.root",
        "spirit/data1037.root",
        "spirit/data1038.root",
        "spirit/data1039.root",
        "spirit/data1040.root",
        "spirit/data1041.root",
        "spirit/data1042.root",
        "spirit/data1043.root",
        "spirit/data1044.root",
        "spirit/data1045.root",
        "spirit/data1046.root",
        "spirit/data1047.root",
        "spirit/data1048.root"
    },
	const std::string& filenameOut = "results-kin/check-hime-veto-orientation.root"
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);
	auto barWidth = barWidthInMeter * 1e3;
	auto hVetoHit = new TH2D("hVetoHit", "", 500, -1000, 1000, 250, -500, 500);
	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);
		for (auto vhit = 0; vhit < spirit.hime_veto_multi; vhit++) {
			int iBar = spirit.hime_veto_bar[vhit];
			auto tDiff = spirit.hime_veto_tdiff[vhit];
			double yHit;
			if (iBar == 0) {
				yHit = barWidth / 2. + unif(gen) * barWidth;
			}

			if (iBar == 1) {
				yHit = barWidth / 2. - unif(gen) * barWidth;
			}

			if (iBar == 2) {
				yHit = -barWidth / 2. - unif(gen) * barWidth;
			}
			hVetoHit->Fill(tDiff, yHit);
		}
	}

	auto fOut = new TFile(filenameOut.c_str(), "recreate");
	hVetoHit->Write();
	fOut->Close();
}
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain) {
	for (auto fname : files) {
		chain->AddFile(fname.c_str());
	}
	return;
}
