#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "TChain.h"
#include "TFile.h"

#include "hime.hh"
#include "merge.hh"
#include "samurai.hh"

#include "argparse/argparse.hpp"

struct argParser {
	std::string himeFilename;
	std::string samuraiFilename;
	std::string mergedFilename;
	std::array<int, 2> window;
	bool debug;
	bool verbose;
};

argParser args;

void CreateBranches(TTree *&mergedTree);
void AddArguments(int argc, char **argv);
argparse::ArgumentParser program("Hime-Samurai Merger", "1.0");
int main(int argc, char *argv[]) {
	AddArguments(argc, argv);
	auto chainSamurai = new TChain(SAMURAI_TREENAME.c_str(), SAMURAI_TREENAME.c_str());
	auto chainHime = new TChain(HIME_TREENAME.c_str());

	chainSamurai->Add(args.samuraiFilename.c_str());
	chainHime->Add(args.himeFilename.c_str());

	SetHimeBranchAddr(chainHime);
	SetSamuraiBranchAddr(chainSamurai);

	auto mergedFile = new TFile(args.mergedFilename.c_str(), "RECREATE");
	auto mergedTree = new TTree(MERGED_TREENAME.c_str(), MERGED_TREENAME.c_str());
	CreateBranches(mergedTree);

	auto entriesHime = chainHime->GetEntries();
	auto entriesSamurai = chainSamurai->GetEntries();

	if (entriesHime != entriesSamurai) {
		if (args.verbose) {
			std::cerr << "entriesHime != entriesSamurai  ";
			std::cerr << entriesHime << " and " << entriesSamurai << " respectively." << '\n';
		}
	}

	if (entriesHime == 0 || entriesSamurai == 0) {
		std::cerr << "No entries found in one of the / both files." << '\n';
		std::cout << "entriesHime = " << entriesHime << '\t';
		std::cout << "entriesSamurai = " << entriesSamurai << '\t';
		std::cout << "mergedEvents = " << 0 << '\n';
		std::exit(1);
	}

	auto absDiff = std::abs(entriesHime - entriesSamurai);
	if (absDiff / (entriesHime + entriesSamurai + 1e-6) > 0.4) {

		if (entriesHime > entriesSamurai) {
			// happens when ridf file is unpacked when that run has not stopped yet.
			std::cerr << "Please check if samurai data being unpacked completely." << '\n';
		}
		std::exit(1);
	}

	std::cout << "entriesHime = " << entriesHime << '\t';
	std::cout << "entriesSamurai = " << entriesSamurai << '\n';

	auto iEvtHime = 0, iEvtSamurai = 0, mergedEvents = 0;
	while (iEvtHime < entriesHime && iEvtSamurai < entriesSamurai) {

		chainHime->GetEntry(iEvtHime);
		chainSamurai->GetEntry(iEvtSamurai);

		auto hime_stamp = hime.fastScaler;
		auto samurai_stamp = samurai.lupots;

		if (args.debug) {
			std::cout << "hime_stamp = " << hime_stamp << '\t';
			std::cout << "samurai_stamp = " << samurai_stamp << '\n';
			iEvtHime++;
			iEvtSamurai++;
			continue;
		}

		if (hime_stamp == 0) {
			if (args.verbose) {
				std::cout << "hime_stamp = 0" << std::endl;
			}
			iEvtHime++;
			continue;
		}

		auto diff = samurai_stamp - hime_stamp;
		if (diff < 0) {
			if (args.verbose) {
				std::cerr << "diff < 0 : not implemented ... exit !\n";
			}
			std::exit(1);
		}

		while (diff > args.window[1] && iEvtHime < entriesHime) {
			if (args.verbose) {
				std::cerr << "diff > window : " << diff;
				std::cerr << " at hime " << iEvtHime << " , samurai " << iEvtSamurai;
				std::cerr << " , skipping event." << '\n';
			}
			iEvtHime++;
			chainHime->GetEntry(iEvtHime);
			hime_stamp = hime.fastScaler;
			diff = samurai_stamp - hime_stamp;
		}

		if (diff < args.window[0]) {
			if (args.verbose) {
				std::cerr << "diff < window : " << diff;
				std::cerr << " at hime " << iEvtHime << " , samurai " << iEvtSamurai;
				std::cerr << " , skipping event." << '\n';
			}
			iEvtHime++;
			iEvtSamurai++;
			continue;
		}

		if (diff >= args.window[0] && diff <= args.window[1]) {
			if (args.verbose) {
				std::cout << "stamp diff = " << diff;
				std::cout << " at hime " << iEvtHime << " , samurai " << iEvtSamurai;
				std::cout << " , merging event." << '\n';
			}
			mergedEvents++;
			mergedTree->Fill();
		}

		iEvtHime++;
		iEvtSamurai++;
	}

	std::cout << "entriesHime = " << entriesHime << '\t';
	std::cout << "entriesSamurai = " << entriesSamurai << '\t';
	std::cout << "mergedEvents = " << mergedEvents << '\n';

	if (!args.debug) {
		mergedFile->Write();
	}
	mergedFile->Close();
	return 0;
}

void CreateBranches(TTree *&mergedTree) {
	mergedTree->Branch("hime_nHits", &hime.nHits, "hime_nHits/i");
	mergedTree->Branch("hime_tofRaw", &hime.tofRaw[0], "hime_tofRaw[hime_nHits]/D");
	mergedTree->Branch("hime_tDiff", &hime.tDiff[0], "hime_tDiff[hime_nHits]/D");
	mergedTree->Branch("hime_tSum", &hime.tSum[0], "hime_tSum[hime_nHits]/D");
	mergedTree->Branch("hime_tot0", &hime.tot0[0], "hime_tot0[hime_nHits]/D");
	mergedTree->Branch("hime_tot1", &hime.tot1[0], "hime_tot1[hime_nHits]/D");
	mergedTree->Branch("hime_moduleID", &hime.moduleID[0], "hime_moduleID[hime_nHits]/i");

	mergedTree->Branch("hime_slowScaler", &hime.slowScaler, "hime_slowScaler/l");
	mergedTree->Branch("hime_fastScaler", &hime.fastScaler, "hime_fastScaler/l");
	mergedTree->Branch("hime_eventNumber", &hime.eventNumber, "hime_eventNumber/l");

	mergedTree->Branch("run", &samurai.run, "run/i");
	mergedTree->Branch("event", &samurai.event, "event/i");
	mergedTree->Branch("lupots", &samurai.lupots, "lupots/l");
	mergedTree->Branch("kyoto_multi", &samurai.kyoto_multi, "kyoto_multi/i");
	mergedTree->Branch("kyoto_bar", &samurai.kyoto_bar[0], "kyoto_bar[kyoto_multi]/i");
	mergedTree->Branch("hime_veto_multi", &samurai.hime_veto_multi, "hime_veto_multi/i");
	mergedTree->Branch("hime_veto_bar", &samurai.hime_veto_bar[0], "hime_veto_bar[hime_veto_multi]/i");
	mergedTree->Branch("hime_veto_tof", &samurai.hime_veto_tof[0], "hime_veto_tof[hime_veto_multi]/D");
	mergedTree->Branch("hime_veto_charge", &samurai.hime_veto_charge[0], "hime_veto_charge[hime_veto_multi]/D");
	mergedTree->Branch("hime_veto_tdiff", &samurai.hime_veto_tdiff[0], "hime_veto_tdiff[hime_veto_multi]/D");
	mergedTree->Branch("hime_veto_x", &samurai.hime_veto_x[0], "hime_veto_x[hime_veto_multi]/D");

	return;
}

void AddArguments(int argc, char **argv) {
	program.add_argument("-i", "--hime-filename")
		.default_value(std::string("hime/data1046.root"))
		.help("path to the hime file.")
		.required();

	program.add_argument("-s", "--samurai-filename")
		.default_value(std::string("samurai/data1046.root"))

		.help("path to the samurai file.")
		.required();

	program.add_argument("-o", "--output-filename")
		.default_value(std::string("merged.root"))
		.help("path to the output file.")
		.required();

	program.add_argument("-w", "--timestamp-window")
		.default_value(std::vector<int>{33, 36})
		.nargs(2)
		.scan<'i', unsigned int>()
		.help("register as the same event if `window[0] <= diff <= window[1]")
		.required();

	program.add_argument("-d", "--debug").help("debug mode").flag();
	program.add_argument("--verbose").help("verbose mode").flag();

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		exit(0);
	}

	args.himeFilename = program.get<std::string>("--hime-filename");
	args.samuraiFilename = program.get<std::string>("--samurai-filename");
	args.mergedFilename = program.get<std::string>("--output-filename");
	args.debug = program.get<bool>("--debug");
	args.verbose = program.get<bool>("--verbose");

	auto window = program.get<std::vector<int>>("--timestamp-window");
	args.window = {window[0], window[1]};
	return;
}
