#include "spirit.h"

void PrintMissingSbtLeft(int runId = 1160) {
	auto chain = new TChain("spirit", "spirit");
	chain->Add(Form("spirit/data%04d.root", runId));
	SetBranchAddress(chain);

	auto nEvents = chain->GetEntries();
	std::cout << "Number of entries in the chain: " << chain->GetEntries() << std::endl;

	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		bool is_sbt_left_present = false;
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			auto hitModule = spirit.hime_moduleID[ihit];
			if (hitModule == 1) {
				is_sbt_left_present = true;
			}
		}

		if (!is_sbt_left_present) {
			std::cout << "Sbt left is not present in event " << iEvt << std::endl;
		}
	}
}
