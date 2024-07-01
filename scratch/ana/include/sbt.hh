#ifndef SBT_HH
#define SBT_HH

#include <array>
#include <cmath>

constexpr double SBT1THICKNESS = 25;
constexpr double SBT2THICKNESS = 100;

struct SbtDetector {
	void update(int detId, const std::array<int, 2> &qdc, const std::array<int, 2> &tdc, int trigger);
	void reset();

	// uncalibrated
	double charge;
	int time;
	int trigger;

	int detID;
	double thickness;
};

struct SbtDetectors {
	void update(const std::array<int, 32> &ltdc, const std::array<int, 32> &ttdc, const std::array<int, 2> &trigger);
	void reset();

	SbtDetector sbt1;
	SbtDetector sbt2;
};

#endif