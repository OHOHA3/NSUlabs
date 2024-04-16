#include "Reverse.h"

std::array<unsigned short, 44100> Reverse::Convert(std::vector<std::array<unsigned short, 44100>> Samples, Config Regime, int Time)
{
	if (Time >= Regime.Begin && Time < Regime.End)
	{
		unsigned short Swap = 0;
		for (int k = 0; k < 44100 / 2; k++)
		{
			Swap = Samples[0][k];
			Samples[0][k] = Samples[0][44100 - 1 - k];
			Samples[0][44100 / 2 - 1 - k] = Swap;
		}
	}
	return Samples[0];
}

void Reverse::SetBits(int FileGcnt)
{
}