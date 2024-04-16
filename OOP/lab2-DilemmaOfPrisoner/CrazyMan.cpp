#include "CrazyMan.h"

Choice CrazyMan::Run()
{
	return rand() % 2 == 0 ? Choice::COOPERATE : Choice::DECEIVE;
}

void CrazyMan::SetLogs(const std::array<Choice,3> Result)
{
}