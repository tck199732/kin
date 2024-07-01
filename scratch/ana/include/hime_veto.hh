#ifndef HIME_VETO_HH
#define HIME_VETO_HH
#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <string>

// static const int HIME_VETO_TL = 1;
// static const int HIME_VETO_TR = 6;
// static const int HIME_VETO_ML = 2;
// static const int HIME_VETO_MR = 3;
// static const int HIME_VETO_BL = 4;
// static const int HIME_VETO_BR = 7;

constexpr int MAX_HITS_HIME = 3;

class HimeVeto {
public:
	HimeVeto(const std::string &calibFile);
	~HimeVeto() {}

	void update(const std::array<int, 32> &ltdc, const std::array<int, 32> &ttdc);
	void reset();

	unsigned int multi;
	std::array<unsigned int, MAX_HITS_HIME> bars; // barID of each hit; 0 : top, 1 : middle, 2 : bottom
	std::array<double, MAX_HITS_HIME> tofs;		  // time of flight of each hit
	std::array<double, MAX_HITS_HIME> charges;	  // geometric mean of charge on both PMTs
	std::array<double, MAX_HITS_HIME> tdiffs;	  // time-diff of each hit
	std::array<double, MAX_HITS_HIME> xpos;		  // x position of each hit
private:
	std::array<double, 3> tdiff_offset;
	std::array<double, 3> tdiff_slope;
};
#endif