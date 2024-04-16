#pragma once

#include <iostream>
#include <array>
#include <vector>
#include "Config.h"

class IConverter
{
public:
	virtual ~IConverter() = default;

	virtual std::array<unsigned short, 44100> Convert(std::vector<std::array<unsigned short, 44100>> Samples, Config Regime, int Time) = 0;
	virtual void SetBits(int FileGcnt) = 0;
};