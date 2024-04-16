#pragma once

#include "IStrategy.h"

class TrickyMan : public IStrategy
{
	std::array<Choice,6> GameLogs={Choice::DECEIVE};
	std::array<Choice,2> TrickyLogs={Choice::DECEIVE};
	int move = 1;
public:
	Choice Run() override;

	void SetLogs(const std::array<Choice,3> Result) override;
};
