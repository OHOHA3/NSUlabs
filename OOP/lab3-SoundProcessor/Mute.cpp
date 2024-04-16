#include "Mute.h"

std::array<unsigned short, 44100> Mute::Convert(std::vector<std::array<unsigned short, 44100>> Samples, Config Regime, int Time)
{
	if (Time >= Regime.Begin && Time < Regime.End)
	{
		for (int k = 0; k < 44100; k++)
		{
			Samples[0][k] = 0;
		}
	}
	return Samples[0];
}

void Mute::SetBits(int FileGcnt)
{
}