#pragma once

#include "IStrategy.h"

class BadMan : public IStrategy
{
public:
	Choice Run() override;

	void SetLogs(const std::array<Choice,3> Result) override;
};