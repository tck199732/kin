#ifndef SCALER_HH
#define SCALER_HH

#include <array>

/**
 * Just include for completeness
 */

struct ScalerB2F {
    std::array<int, 32> values;
    void update(const std::array<int,32>& values);
    void reset() { values.fill(0); }
};

// const std::vector<std::string> mapRunMode = {
// 	"Commissioning", "PhysicsJune",
// 	// "PhysicsOctober",
// };
// const std::array<std::string, 32> mapCommissioning = {
// 	"gated trig",
// 	"ungated trig",
// 	"AC up",
// 	"AC down",
// 	"AC left",
// 	"AC right",
// 	"AC OR",
// 	"KatanaV left",
// 	"KatanaV middle",
// 	"KatanaV right",
// 	"KatanaV spare",
// 	"KatanaV OR",
// 	"SBT",
// 	"SBT and no AC OR",
// 	"SBT and no Katana OR",
// 	"SBT and no Busy",
// 	"Kyoto mult",
// 	"1k clk",
// 	"AC OR from KBox",
// 	"KatanaV OR from KBox",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"GET Busy",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// };

// const std::array<std::string, 32> mapPhysicsJune = {
// 	"gated trig",
// 	"ungated trig",
// 	"AC up",
// 	"AC down",
// 	"AC left",
// 	"AC right",
// 	"AC OR",
// 	"KatanaV left",
// 	"KatanaV middle",
// 	"KatanaV right",
// 	"KatanaV spare",
// 	"KatanaV OR",
// 	"SBT",
// 	"SBT and no AC OR",
// 	"SBT and no Katana OR",
// 	"SBT and no Busy",
// 	"Kyoto mult",
// 	"1k clk",
// 	"AC OR from KBox",
// 	"KatanaV OR from KBox",
// 	"SBT and Kyoto and no AC OR and no Katana OR",
// 	"Downscaled Kyoto mult",
// 	"",
// 	"",
// 	"GET Busy",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// 	"",
// };

#endif