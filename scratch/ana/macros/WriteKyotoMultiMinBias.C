struct Kyoto {
	unsigned int multi;
};

std::vector<double> getAxis(TH1D *&h);
std::vector<double> getValue(TH1D *&h, int err = 0);
std::vector<double> getCumSumNormalized(const std::vector<double> &v);
void reverseVector(std::vector<double> &v);
void trimVector(std::vector<double> &x, std::vector<double> &y);

TChain *getChain(const std::string &name, const std::vector<int> &runIds);
void setChainAddress(TChain *chain, Kyoto &kyoto);
std::string getFileName(int run_number);
void writeKyoto(const std::string &fname, TH1D *hMulti);

const std::vector<int> runM1 = {
	1259, 1260, 1261, 1262, 1263, 1264,
};
const std::vector<int> runM2 = {
	1265,
	1266,
};
const std::vector<int> runM3 = {
	1267,
	1268,
	1269,
};

void WriteKyotoMultiMinBias() {

	Kyoto kyoto;
	auto chainM1 = getChain("samurai", runM1);
	auto chainM2 = getChain("samurai", runM2);
	auto chainM3 = getChain("samurai", runM3);
	setChainAddress(chainM1, kyoto);
	setChainAddress(chainM2, kyoto);
	setChainAddress(chainM3, kyoto);

	auto hMultiM1 = new TH1D("hMultiM1", "Kyoto Multi M1", 40, 0, 40);
	auto hMultiM2 = new TH1D("hMultiM2", "Kyoto Multi M2", 40, 0, 40);
	auto hMultiM3 = new TH1D("hMultiM3", "Kyoto Multi M3", 40, 0, 40);

	std::cout << "chainM1->GetEntries(): " << chainM1->GetEntries() << std::endl;
	for (int i = 0; i < chainM1->GetEntries(); i++) {
		chainM1->GetEntry(i);
		hMultiM1->Fill(kyoto.multi);
	}
	std::cout << "chainM2->GetEntries(): " << chainM2->GetEntries() << std::endl;

	for (int i = 0; i < chainM2->GetEntries(); i++) {
		chainM2->GetEntry(i);
		hMultiM2->Fill(kyoto.multi);
	}

	std::cout << "chainM3->GetEntries(): " << chainM3->GetEntries() << std::endl;
	for (int i = 0; i < chainM3->GetEntries(); i++) {
		chainM3->GetEntry(i);
		hMultiM3->Fill(kyoto.multi);
	}

	writeKyoto("result/kyotoMinBias/minBiasM1.txt", hMultiM1);
	writeKyoto("result/kyotoMinBias/minBiasM2.txt", hMultiM2);
	writeKyoto("result/kyotoMinBias/minBiasM3.txt", hMultiM3);
	return;
}

TChain *getChain(const std::string &name, const std::vector<int> &runIds) {
	auto chain = new TChain(name.c_str(), name.c_str());
	for (const auto &run_number : runIds) {

		std::string file = getFileName(run_number);
		// std::cout << "file: " << file << std::endl;
		if (gSystem->AccessPathName(file.c_str())) {
			std::cerr << "== File is bad!" << std::endl;
			continue;
		}
		chain->Add(file.c_str());
	}
	return chain;
}

std::string getFileName(int run_number) { return Form("analyzed/data%d.root", run_number); }

void setChainAddress(TChain *chain, Kyoto &kyoto) {
	chain->SetBranchAddress("kyoto_multi", &kyoto.multi);
	return;
}

void writeKyoto(const std::string &fname, TH1D *hMulti) {
	auto x = getAxis(hMulti);
	auto y = getValue(hMulti, 0);
	trimVector(x, y);
	auto cumSum = getCumSumNormalized(y);

	std::ofstream file(fname.c_str());
	for (auto i = 0; i < x.size(); i++) {
		file << x[i] << " " << y[i] << " " << cumSum[i] << std::endl;
	}
	file.close();
}

std::vector<double> getAxis(TH1D *&h) {
	std::vector<double> axisValues;
	for (auto i = 1; i <= h->GetNbinsX(); i++) {
		axisValues.push_back(h->GetBinCenter(i));
	}
	return axisValues;
}

std::vector<double> getValue(TH1D *&h, int err = 0) {
	std::vector<double> values;
	for (auto i = 1; i <= h->GetNbinsX(); i++) {
		values.push_back(err ? h->GetBinError(i) : h->GetBinContent(i));
	}
	return values;
}

std::vector<double> getCumSumNormalized(const std::vector<double> &v) {
	std::vector<double> cumSum;
	double sum = 0;
	for (auto &i : v) {
		cumSum.push_back(sum);
		sum += i;
	}

	for (auto i = 0; i < cumSum.size(); i++) {
		cumSum[i] = sum - cumSum[i];
		cumSum[i] /= sum;
	}

	return cumSum;
}

void reverseVector(std::vector<double> &v) {
	std::reverse(v.begin(), v.end());
	return;
}

void trimVector(std::vector<double> &x, std::vector<double> &y) {
	auto n = x.size();
	auto m = y.size();
	for (auto i = n - 1; i >= 0; i--) {
		if (y[i] == 0.0) {
			x.erase(x.begin() + i);
			y.erase(y.begin() + i);
		} else {
			break;
		}
	}
	return;
}