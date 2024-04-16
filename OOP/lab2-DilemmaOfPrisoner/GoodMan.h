#pragma once

#include "IStrategy.h"

class GoodMan : public IStrategy
{
public:
	Choice Run() override;

	void SetLogs(const std::array<Choice,3> Result) override;
};