#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void loadData(std::vector<double> &x, std::vector<double> &y, std::vector<double> &z, std::string fname);
int main(int argc, char *argv[]) {

	std::vector<double> thresMulti;
	std::vector<double> Counts;
	std::vector<double> Probs;
	loadData(thresMulti, Counts, Probs, "data1046.txt");

	for (auto i = 0; i < thresMulti.size(); i++) {
		thresMulti[i] -= 0.5;
		// std::cout << thresMulti[i] << " " << Counts[i] << " " << Probs[i] << std::endl;
	}

	std::vector<std::vector<double>> rateTable(thresMulti.size(), std::vector<double>(thresMulti.size(), 0));

	for (auto i = 0; i < thresMulti.size(); i++) {
		auto ref_mch = thresMulti[i];
		for (auto j = 0; j < thresMulti.size(); j++) {
			rateTable[i][j] = Probs[j] / Probs[i];
		}
	}
	std::cout << "-------- rate conversion table ---------\n";
	std::cout << "Please input the reference multiplicity and the target multiplicity\n";
	std::cout << "e.g. 5 3\n";
	std::cout << "m = 5 -> m = 3, rate ratio: 2.02942\n";
	std::cout << "----------------------------------------\n";
	int m0, m;
	while (true) {
		try {
			std::cin >> m0 >> m;
			if (m < 0 || m0 < 0) {
				throw std::runtime_error("Negative value is not allowed.");
			}
			if (m0 >= thresMulti.size() || m >= thresMulti.size()) {
				throw std::runtime_error("Multiplicity out of range.");
			}
			if (std::cin.fail()) {
				throw std::runtime_error("Invalid input.");
			}
		} catch (std::runtime_error &e) {
			std::cerr << e.what() << std::endl;
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			continue;
		}
		std::cout << "m = " << m0 << " -> m = " << m << ", rate ratio: " << rateTable[m0][m] << std::endl;
	}
	return 0;
}

void loadData(std::vector<double> &x, std::vector<double> &y, std::vector<double> &z, std::string fname) {
	std::ifstream file(fname);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		double a, b, c;
		if (!(iss >> a >> b >> c)) {
			break;
		}
		x.push_back(a);
		y.push_back(b);
		z.push_back(c);
	}
	return;
}
