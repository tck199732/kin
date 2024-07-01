struct KyotoData {
	unsigned int multi;
	std::array<unsigned int, 128> bars;
};

KyotoData kyoto;

void SetBranchAddress(TChain *chain);
long getEntries(const std::string &filename);
std::vector<double> getAxis(TH1D *&h);
std::vector<double> getValue(TH1D *&h, int err = 0);

std::vector<double> getCumSumNormalized(const std::vector<double> &v);
void reverseVector(std::vector<double> &v);
void trimVector(std::vector<double> &x, std::vector<double> &y);

void WriteKyotoRate(
	// clang-format off
    const std::string& filename = "analyzed/data1046.root",
    const std::string& filenameOut = "result/kyotoRate/data1046.txt"
	// clang-format on
) {
	auto chain = new TChain("samurai", "samurai");
	SetBranchAddress(chain);
	chain->Add(filename.c_str());

	auto nEntriesTotal = chain->GetEntries();
	std::cout << "nEntriesTotal = " << nEntriesTotal << std::endl;

	auto hMulti = new TH1D("hMulti", "", 40, 0, 40);

	for (auto i = 0; i < nEntriesTotal; i++) {
		chain->GetEntry(i);
		hMulti->Fill(kyoto.multi);
	}

	auto x = getAxis(hMulti);
	auto y = getValue(hMulti, 0);
	trimVector(x, y);

	// for (auto i = 0; i < x.size(); i++) {
	// 	std::cout << x[i] << " " << y[i] << std::endl;
	// }

	auto cumSum = getCumSumNormalized(y);

	std::ofstream file(filenameOut);
	for (auto i = 0; i < x.size(); i++) {
		file << x[i] << " " << y[i] << " " << cumSum[i] << std::endl;
	}
	file.close();
}

long getEntries(const std::string &filename) {
	auto chain = std::make_unique<TChain>("samurai", "samurai");
	SetBranchAddress(chain.get());
	chain->Add(filename.c_str());
	return chain->GetEntries();
}

void SetBranchAddress(TChain *chain) {
	chain->SetBranchAddress("kyoto_multi", &kyoto.multi);
	chain->SetBranchAddress("kyoto_bar", &kyoto.bars[0]);
	return;
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