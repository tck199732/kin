#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"

std::map<std::string, std::string> MODULE_MAP;
void LoadModuleMap(const std::string &filename);
void CheckDataModule(
	// clang-format off
    const std::string& indata = "ridf/data1179.ridf", 
    const std::string& outdata = "modules.dat",
    const std::string& mapfile = "database/module_map.dat"
	// clang-format on
) {

	gSystem->Load("libanaroot.so");
	LoadModuleMap(mapfile);
	auto estore = new TArtEventStore();
	estore->Open(indata.c_str());
	auto rawevent = estore->GetRawEventObject();

	std::map<std::string, bool> mod_counter;

	for (auto &kv : MODULE_MAP) {
		mod_counter[kv.first] = false;
	}

	while (estore->GetNextEvent()) {
		for (auto i = 0; i < rawevent->GetNumSeg(); i++) {
			auto seg = rawevent->GetSegment(i);
			auto mod = seg->GetModule();
			auto address = seg->GetAddress();
			auto ndata = seg->GetNumData();
			// std::cout << "mod:" << mod << " efn:" << address << " nd:" << ndata << std::endl;
			auto mod_str = std::to_string(mod);
			if (mod_counter.count(mod_str) == 0) {
				std::cerr << "Unknown module: " << mod_str << std::endl;
			}
			mod_counter[mod_str] = true;
		}
		estore->ClearData();
	}

	std::ofstream ofs(outdata.c_str());
	for (auto &kv : mod_counter) {
		if (kv.second) {
			ofs << kv.first << '\n';
		}
	}
	ofs.close();
}

void LoadModuleMap(const std::string &filename) {
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cerr << "Cannot open file: " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(ifs, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		std::string module, name;
		std::istringstream iss(line);
		iss >> module >> name;
		MODULE_MAP[module] = name;
	}
}
