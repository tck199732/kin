#include <array>
struct myData {
	unsigned int multi;
	std::array<int, 32> scalers;
};

const std::vector<int> runM1 = {
	1259, 1260, 1261, 1262, 1263, 1264,
};

myData dataTuple;
TChain *getChain(const std::string &name, const std::vector<int> &runIds);
std::string getFileName(int run_number);
void ViewScaler() {

	auto chainM1 = getChain("samurai", runM1);
	chainM1->SetBranchAddress("scaler_b2f", &dataTuple.scalers[0]);
	chainM1->SetBranchAddress("kyoto_multi", &dataTuple.multi);

	int incident, reacted;

	chainM1->GetEntry(0);
	incident = dataTuple.scalers[12];
	reacted = dataTuple.scalers[20];
	// std::cout << "incident: " << incident << " reacted: " << reacted << std::endl;
	chainM1->GetEntry(chainM1->GetEntries() - 1);
	incident = dataTuple.scalers[12] - incident;
	reacted = dataTuple.scalers[20] - reacted;
	int Nmin = 0;
	for (int i = 0; i < chainM1->GetEntries(); i++) {
		chainM1->GetEntry(i);
		if (dataTuple.multi >= 1) {
			Nmin++;
		}
	}

	double weight_sn112 = 111.905;
	double avogadro = 6.02214076e23;
	double areal_density = 3040;							 // g/m^2
	areal_density = areal_density / weight_sn112 * avogadro; // nuclei/m^2
	double reaction_ratio = static_cast<double>(Nmin) / incident;
	// double reaction_ratio = static_cast<double>(reacted) / incident;

	double sigma = reaction_ratio / areal_density * 1e30; // fm^2
	double bmax = std::sqrt(sigma / TMath::Pi());

	std::cout << "Nmin: " << Nmin << std::endl;
	std::cout << "incident: " << incident << " reacted: " << reacted << std::endl;
	std::cout << "reaction_ratio: " << reaction_ratio << std::endl;
	std::cout << "areal_density: " << areal_density << std::endl;
	std::cout << "sigma: " << sigma << std::endl;
	std::cout << "bmax: " << bmax << std::endl;
	std::cout << 1.3 * std::pow(112., 1 / 3.) + 1.3 * std::pow(124, 1. / 3) << std::endl;
}

TChain *getChain(const std::string &name, const std::vector<int> &runIds) {
	auto chain = new TChain(name.c_str(), name.c_str());
	for (const auto &run_number : runIds) {

		std::string file = getFileName(run_number);
		if (gSystem->AccessPathName(file.c_str())) {
			std::cerr << "== File is bad!" << std::endl;
			continue;
		}
		chain->Add(file.c_str());
	}
	return chain;
}

std::string getFileName(int run_number) { return Form("analyzed/data%d.root", run_number); }

// "0": "gated trig",
// "1": "ungated trig",
// "2": "AC up",
// "3": "AC down",
// "4": "AC left",
// "5": "AC right",
// "6": "AC OR",
// "7": "KatanaV left",
// "8": "KatanaV middle",
// "9": "KatanaV right",
// "10": "KatanaV spare",
// "11": "KatanaV OR",
// "12": "SBT",
// "13": "SBT and no AC OR",
// "14": "SBT and no Katana OR",
// "15": "SBT and no Busy",
// "16": "Kyoto mult",
// "17": "1k clk",
// "18": "AC OR from KBox",
// "19": "KatanaV OR from KBox",
// "20": "SBT and Kyoto and no AC OR and no Katana OR",
// "21": "Downscaled Kyoto mult",
// "22": "",
// "23": "",
// "24": "GET Busy",