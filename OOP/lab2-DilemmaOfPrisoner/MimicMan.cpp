#include "MimicMan.h"

Choice MimicMan::Run()
{
	if (move <=2)
	{
		move++;
		MimicLogs[0] = Choice::COOPERATE;
		MimicLogs[1] = Choice::COOPERATE;
		return Choice::COOPERATE;
	}
	else
	{
		int d = 0;
		if (MimicLogs[0] == Choice::DECEIVE)
		{
			d--;
		}
		if (MimicLogs[1] == Choice::DECEIVE)
		{
			d--;
		}
		MimicLogs[0] = MimicLogs[1];
		for (int i = 0; i < 6; i++)
		{
			if (GameLogs[i] == Choice::DECEIVE)
			{
				d++;
			}
		}
		if (d > 1)
		{
			MimicLogs[1] = Choice::DECEIVE;
			return Choice::DECEIVE;
		}
		else
		{
			MimicLogs[1] = Choice::COOPERATE;
			return Choice::COOPERATE;
		}
	}
}

void MimicMan::SetLogs(const std::array<Choice,3> Result)
{
	GameLogs[0] = GameLogs[3];
	GameLogs[1] = GameLogs[4];
	GameLogs[2] = GameLogs[5];
	GameLogs[3] = Result[0];
	GameLogs[4] = Result[1];
	GameLogs[5] = Result[2];
}