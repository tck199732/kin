#ifndef KYOTO_HH
#define KYOTO_HH

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

constexpr int MAX_HITS_KYOTO = 60;

class KyotoArray {
public:
	KyotoArray(const std::string &calibFile);
	~KyotoArray() {}
	void update(const std::array<int, 64> &ltdc, const std::array<int, 64> &ttdc);
	void reset();
	unsigned int multi;
	std::array<unsigned int, MAX_HITS_KYOTO> bars; // barID of each hit
private:
	void readGeometry(const std::string &calibFile);
	std::map<int, int> mBarChannelMap;
};

#endif