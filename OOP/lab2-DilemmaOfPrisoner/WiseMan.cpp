#include "WiseMan.h"

Choice WiseMan::Run()
{
	if (move < 3)
	{
		WiseLogs[0] = WiseLogs[1];
		WiseLogs[1] = WiseLogs[2];
		if (move == 2)
		{
			move++;
			WiseLogs[2] = Choice::DECEIVE;
			return Choice::DECEIVE;
		}
		else
		{
			move++;
			WiseLogs[2] = Choice::COOPERATE;
			return Choice::COOPERATE;
		}
	}
	else
	{
		int d = 0;
		for (int i = 0; i < 3; i++)
		{
			if (WiseLogs[i] == Choice::DECEIVE)
			{
				d--;
			}
		}
		for (int i = 0; i < 9; i++)
		{
			if (GameLogs[i] == Choice::DECEIVE)
			{
				d++;
			}
		}
		WiseLogs[0] = WiseLogs[1];
		WiseLogs[1] = WiseLogs[2];
		if (d > 2)
		{
			if (rand() % 100 < 10)
			{
				WiseLogs[2] = Choice::COOPERATE;
				return Choice::COOPERATE;
			}
			else
			{
				WiseLogs[2] = Choice::DECEIVE;
				return Choice::DECEIVE;
			}
		}
		else
		{
			WiseLogs[2] = Choice::COOPERATE;
			return Choice::COOPERATE;
		}
	}
}

void WiseMan::SetLogs(const std::array<Choice,3> Result)
{
	for (int i = 0; i < 6; i++)
	{
		GameLogs[i] = GameLogs[i + 3];
	}
	GameLogs[6] = Result[0];
	GameLogs[7] = Result[1];
	GameLogs[8] = Result[2];
}