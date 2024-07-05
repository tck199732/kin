#include "ana.hh"
#include "argparse/argparse.hpp"

const static std::string dataTreeName = "ridf";

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
		std::cout << args.outputFile << std::endl;
		std::cout << args.treeName << std::endl;
		std::exit(0);
	}

	PhysicsData dataPhy(args.kyotoCalib, args.himeVetoCalib);

	auto chain = new TChain(dataTreeName.c_str(), dataTreeName.c_str());
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

	tr->Branch("runNumber", &samurai.run, "runNumber/i");
	tr->Branch("eventNumber", &samurai.event, "eventNumber/i");
	tr->Branch("lupoTimeStamp", &samurai.lupots, "lupoTimeStamp/l");

	tr->Branch("kyotoMulti", &dataPhy.kyotoArray.multi, "kyotoMulti/i");
	tr->Branch("kyotoBarId", &dataPhy.kyotoArray.bars[0], "kyotoBarId[kyotoMulti]/i");

	tr->Branch("vetoMulti", &dataPhy.himeVeto.multi, "vetoMulti/i");
	tr->Branch("vetoBarId", &dataPhy.himeVeto.bars[0], "vetoBarId[vetoMulti]/i");
	tr->Branch("vetoTof", &dataPhy.himeVeto.tofs[0], "vetoTof[vetoMulti]/D");
	tr->Branch("vetoTot", &dataPhy.himeVeto.charges[0], "vetoTot[vetoMulti]/D");
	tr->Branch("vetoTdiff", &dataPhy.himeVeto.tdiffs[0], "vetoTdiff[vetoMulti]/D");

	tr->Branch("sbtCharge1", &dataPhy.sbtDetectors.sbt1.charge, "sbtCharge1/D");
	tr->Branch("sbtTime1", &dataPhy.sbtDetectors.sbt1.time, "sbtTime1/I");
	tr->Branch("sbtTrigger1", &dataPhy.sbtDetectors.sbt1.trigger, "sbtTrigger1/I");
	tr->Branch("sbtCharge2", &dataPhy.sbtDetectors.sbt2.charge, "sbtCharge2/D");
	tr->Branch("sbtTime2", &dataPhy.sbtDetectors.sbt2.time, "sbtTime2/I");
	tr->Branch("sbtTrigger2", &dataPhy.sbtDetectors.sbt2.trigger, "sbtTrigger2/I");

	tr->Branch("scalerB2f", &dataPhy.scalerB2F.values[0], "scalerB2f[32]/I");
	return;
}

void AddArguments(int argc, char **argv) {
	program.add_argument("-i", "--input-filename")
		.default_value(std::string("unpacked/data1036.root"))
		.help("path to the input file.")
		.required();

	program.add_argument("--kyoto_calib")
		.default_value(std::string("database/kyoto/bar_channels.dat"))
		.help("path to the kyoto barId-channel mapping file.")
		.required();

	program.add_argument("--hime_veto_calib")
		.default_value(std::string("database/veto/channels.json"))
		.help("path to the hime veto pmt-channel mapping file.")
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
