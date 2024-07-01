#include "ana.hh"
#include "argparse/argparse.hpp"

struct PhysicsData {
	PhysicsData(const std::string &kyotoCalib, const std::string &himeVetoCalib) :
		kyotoArray(kyotoCalib),
		himeVeto(himeVetoCalib) {
		reset();
	}
	KyotoArray kyotoArray;
	HimeVeto himeVeto;
	SbtDetectors sbtDetectors;
	ScalerB2F scalerB2F;

	void reset() {
		kyotoArray.reset();
		himeVeto.reset();
		sbtDetectors.reset();
		scalerB2F.reset();
	}
};

struct argParser {
	std::string inputFile;
	std::string kyotoCalib;
	std::string himeVetoCalib;
	std::string outputFile;
	std::string treeName;
	bool debug;
};

argParser args;
argparse::ArgumentParser program("Analysis", "1.0");

void AddArguments(int argc, char **argv);
void DefineTreeBranches(TTree *&tr, PhysicsData &dataPhy);
int main(int argc, char *argv[]) {
	AddArguments(argc, argv);
	if (args.debug) {
		std::cout << args.inputFile << std::endl;
		std::cout << args.kyotoCalib << std::endl;
		std::cout << args.himeVetoCalib << std::endl;
		std::cout << args.outputFile << std::endl;
		std::cout << args.treeName << std::endl;
		std::exit(0);
	}

	PhysicsData dataPhy(args.kyotoCalib, args.himeVetoCalib);

	auto chain = new TChain("ridf", "ridf");
	chain->Add(args.inputFile.c_str());
	SetSpiritBranchAddress(chain);

	auto treePhysics = new TTree(args.treeName.c_str(), args.treeName.c_str());
	DefineTreeBranches(treePhysics, dataPhy);

	auto nentries = chain->GetEntries();
	if (args.debug) {
		std::cout << "nentries: " << nentries << std::endl;
	}
	for (auto iEvt = 0; iEvt < nentries; iEvt++) {
		samurai.reset();
		dataPhy.reset(); // need to reset multi, otherwise it exceeds the limit in array
		chain->GetEntry(iEvt);

		dataPhy.kyotoArray.update(samurai.vme_easiroc.ltdc, samurai.vme_easiroc.ttdc);
		dataPhy.himeVeto.update(samurai.v1290.ltdc, samurai.v1290.ttdc);
		dataPhy.sbtDetectors.update(samurai.mqdc32.qdc, samurai.mtdc32.tdc, samurai.mtdc32.trigger);
		dataPhy.scalerB2F.update(samurai.sis3820.scaler);
		// do stuff for other detectors (BDC, etc...)
		treePhysics->Fill();
	}

	auto fout = new TFile(args.outputFile.c_str(), "RECREATE");
	treePhysics->Write();
	fout->Close();
	return 0;
}

void DefineTreeBranches(TTree *&tr, PhysicsData &dataPhy) {

	tr->Branch("run", &samurai.run, "run/i");
	tr->Branch("event", &samurai.event, "event/i");
	tr->Branch("lupots", &samurai.lupots, "lupots/l");

	tr->Branch("kyoto_multi", &dataPhy.kyotoArray.multi, "kyoto_multi/i");
	tr->Branch("kyoto_bar", &dataPhy.kyotoArray.bars[0], "kyoto_bar[kyoto_multi]/i");

	tr->Branch("hime_veto_multi", &dataPhy.himeVeto.multi, "hime_veto_multi/i");
	tr->Branch("hime_veto_bar", &dataPhy.himeVeto.bars[0], "hime_veto_bar[hime_veto_multi]/i");
	tr->Branch("hime_veto_tof", &dataPhy.himeVeto.tofs[0], "hime_veto_tof[hime_veto_multi]/D");
	tr->Branch("hime_veto_charge", &dataPhy.himeVeto.charges[0], "hime_veto_charge[hime_veto_multi]/D");
	tr->Branch("hime_veto_tdiff", &dataPhy.himeVeto.tdiffs[0], "hime_veto_tdiff[hime_veto_multi]/D");
	tr->Branch("hime_veto_x", &dataPhy.himeVeto.xpos[0], "hime_veto_x[hime_veto_multi]/D");

	tr->Branch("sbt1_charge", &dataPhy.sbtDetectors.sbt1.charge, "sbt1_charge/D");
	tr->Branch("sbt1_time", &dataPhy.sbtDetectors.sbt1.time, "sbt1_time/I");
	tr->Branch("sbt1_trigger", &dataPhy.sbtDetectors.sbt1.trigger, "sbt1_trigger/I");
	tr->Branch("sbt2_charge", &dataPhy.sbtDetectors.sbt2.charge, "sbt2_charge/D");
	tr->Branch("sbt2_time", &dataPhy.sbtDetectors.sbt2.time, "sbt2_time/I");
	tr->Branch("sbt2_trigger", &dataPhy.sbtDetectors.sbt2.trigger, "sbt2_trigger/I");

	tr->Branch("scaler_b2f", &dataPhy.scalerB2F.values[0], "scaler_b2f[32]/I");
	return;
}

void AddArguments(int argc, char **argv) {
	program.add_argument("-i", "--input-filename")
		.default_value(std::string("unpacked/data1036.root"))
		.help("path to the input file.")
		.required();

	program.add_argument("--kyoto_calib")
		.default_value(std::string("database/kyoto/bar_channels.dat"))
		.help("path to the kyoto calibration file.")
		.required();

	program.add_argument("--hime_veto_calib")
		.default_value(std::string("database/veto/calib.dat"))
		.help("path to the hime veto calibration file.")
		.required();

	program.add_argument("-o", "--output-filename")
		.default_value(std::string("output.root"))
		.help("path to the output file.")
		.required();

	program.add_argument("-t", "--treename")
		.default_value(std::string("samurai"))
		.help("name of the tree in the output file.")
		.required();

	program.add_argument("-d", "--debug").help("debug mode").flag();
	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		std::exit(0);
	}

	args.inputFile = program.get<std::string>("--input-filename");
	args.outputFile = program.get<std::string>("--output-filename");
	args.kyotoCalib = program.get<std::string>("--kyoto_calib");
	args.himeVetoCalib = program.get<std::string>("--hime_veto_calib");
	args.treeName = program.get<std::string>("--treename");
	args.debug = program.get<bool>("--debug");

	return;
}
