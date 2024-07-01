#include "kyoto.hh"

KyotoArray::KyotoArray(const std::string &calibFile) {
	reset();
	readGeometry(calibFile);
}

void KyotoArray::readGeometry(const std::string &calibFile) {
	std::ifstream ifs(calibFile.c_str());
	if (!ifs.is_open()) {
		throw std::runtime_error(("Cannot open the file: " + calibFile).c_str());
	}

	int ch, bar;
	while (ifs >> ch >> bar) {
		mBarChannelMap[bar] = ch;
	}
	ifs.close();
}

void KyotoArray::reset() {
	multi = 0;
	bars.fill(-1);
	return;
}

void KyotoArray::update(const std::array<int, 64> &ltdc, const std::array<int, 64> &ttdc) {
	for (auto ich = 0; ich < 64; ich++) {

		// completely skip unused channel
		if (ich == 30 || ich == 31 || ich == 62 || ich == 63) {
			continue;
		}

		if (ltdc[ich] == -1 || ttdc[ich] == -1) {
			continue;
		}

		if (ltdc[ich] - ttdc[ich] > 0) {
			auto barID = -1;
			try {
				barID = mBarChannelMap.at(ich);
			} catch (const std::out_of_range &e) {
				std::cerr << e.what() << std::endl;
				std::cerr << "Skipping this channel." << std::endl;
			}
			if (barID == -1) {
				continue;
			}
			bars[multi] = barID;
			multi++;
		}
	}
	return;
}