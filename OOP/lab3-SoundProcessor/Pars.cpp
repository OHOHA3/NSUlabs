#include "Pars.h"
#include "WAVContainer.h"

bool Pars::CheckConfig(int Val, char** Arguments)
{
	if (Val < 2)
	{
		ErrorArguments();
		return false;
	}
	File.open(Arguments[2]);
	if (!File.is_open())
	{
		std::cout << "Error:Config file doesn't exist" << std::endl;
		return false;
	}
	std::string Line = "";
	std::string Check = "";
	while (getline(File, Line))
	{
		if (Line[Line.size() - 1] == '\n' || Line[Line.size() - 1] == '\r')
		{
			Line.resize(Line.size() - 1);
		}
		Check = "";
		if (Line[0] == '#')
		{
			continue;
		}
		for (int i = 0; i < 4; i++)
		{
			Check.push_back(Line[i]);
		}
		if (Check == "mix ")
		{
			if (Line.size() < 6)
			{
				ErrorConfig();
				return false;
			}
			if (Line[4] != '$' || Line[5] == '0')
			{
				ErrorConfig();
				return false;
			}
			int k = 5;
			int Quantity = 0;
			while (Line[k] != ' ' && k < Line.size())
			{
				if (Line[k] < '0' || Line[k] > '9')
				{
					ErrorConfig();
					return false;
				}
				Quantity *= 10;
				Quantity += Line[k] - '0';
				k++;
			}
			if (Quantity > Val - 4 || Line.size() == k)
			{
				ErrorConfig();
				return false;
			}
			k++;
			if (Line[k] == '0' && k + 1 != Line.size())
			{
				ErrorConfig();
				return false;
			}
			while (k < Line.size())
			{
				if (Line[k] < '0' || Line[k] > '9')
				{
					ErrorConfig();
					return false;
				}
				k++;
			}
		}
		else if (Check == "rev ")
		{
			{
				if (Line.size() < 7)
				{
					ErrorConfig();
					return false;
				}
				int k = 4;
				if (Line[k] == '0' && Line[k + 1] != ' ')
				{
					ErrorConfig();
					return false;
				}
				int FrstQuan = 0;
				while (Line[k] != ' ' && k < Line.size())
				{
					if (Line[k] < '0' || Line[k] > '9')
					{
						ErrorConfig();
						return false;
					}
					FrstQuan *= 10;
					FrstQuan += Line[k] - '0';
					k++;
				}
				if (k == Line.size())
				{
					ErrorConfig();
					return false;
				}
				k++;
				int ScndQuan = 0;
				if (Line[k] == '0' && Line[k + 1] != ' ')
				{
					ErrorConfig();
					return false;
				}
				while (k < Line.size())
				{
					if (Line[k] < '0' || Line[k] > '9')
					{
						ErrorConfig();
						return false;
					}
					ScndQuan *= 10;
					ScndQuan += Line[k] - '0';
					k++;
				}
				if (FrstQuan > ScndQuan)
				{
					ErrorConfig();
					return false;
				}
			}
		}
		else
		{
			Check.push_back(Line[4]);
			if (Check == "mute ")
			{
				if (Line.size() < 8)
				{
					ErrorConfig();
					return false;
				}
				int k = 5;
				if (Line[k] == '0' && Line[k + 1] != ' ')
				{
					ErrorConfig();
					return false;
				}
				int FrstQuan = 0;
				while (Line[k] != ' ' && k < Line.size())
				{
					if (Line[k] < '0' || Line[k] > '9')
					{
						ErrorConfig();
						return false;
					}
					FrstQuan *= 10;
					FrstQuan += Line[k] - '0';
					k++;
				}
				if (k == Line.size())
				{
					ErrorConfig();
					return false;
				}
				k++;
				int ScndQuan = 0;
				if (Line[k] == '0' && Line[k + 1] != ' ')
				{
					ErrorConfig();
					return false;
				}
				while (k < Line.size())
				{
					if (Line[k] < '0' || Line[k] > '9')
					{
						ErrorConfig();
						return false;
					}
					ScndQuan *= 10;
					ScndQuan += Line[k] - '0';
					k++;
				}
				if (FrstQuan > ScndQuan)
				{
					ErrorConfig();
					return false;
				}
			}
			else
			{
				ErrorConfig();
				return false;
			}
		}
	}
	File.close();
	return true;
}

void Pars::PrintHelp()
{
	std::cout << "sound_processor -c config.txt output.wav input1.wav input2.wav..." << std::endl;
	std::cout << "from second input is optional" << std::endl;
}

void Pars::ErrorArguments()
{
	std::cout << "Invalid Arguments. Print sound_processor -h for getting help" << std::endl;
}

void Pars::ErrorConfig()
{
	std::cout << "Invalid config" << std::endl;
}

void Pars::ErrorMusic(int Number)
{
	std::cout << "Music file number " << Number << " has incorrect format" << std::endl;
}

bool Pars::CheckArguments(int Val, char** Arguments)
{
	std::string Arg;
	if (Val == 1)
	{
		ErrorArguments();
		return false;
	}
	for (int i = 0; Arguments[1][i]; i++)
	{
		Arg.push_back(Arguments[1][i]);
	}
	if (Arg == "-h")
	{
		if (Val > 2)
		{
			ErrorArguments();
			return false;
		}
		else
		{
			PrintHelp();
			return false;
		}
	}
	else if (Arg != "-c")
	{
		ErrorArguments();
		return false;
	}
	if (Val < 4)
	{
		return false;
	}
	return true;
}

bool Pars::CheckMusic(int Val, char** Arguments)
{
	WAVHEADER Header;
	for (int i = 4; i < Val; i++)
	{
		std::ifstream File(Arguments[i], std::ios::binary);
		if (!File.is_open())
		{
			std::cout << "Music file number " << i - 3 << " doesn't exist";
			return false;
		}
		File.read(reinterpret_cast<char*>(&Header), sizeof(WAVHEADER));
		std::string Check;
		for (int j = 0; j < 4; j++)
		{
			Check.push_back(Header.subchunk2Id[j]);
		}
		if (Check == "LIST")
		{
			std::array <char,4> a;
			File.read(reinterpret_cast<char*>(&a), Header.subchunk2Size);
			File.read(reinterpret_cast<char*>(&Header.subchunk2Id), sizeof(Header.subchunk2Id));
			File.read(reinterpret_cast<char*>(&Header.subchunk2Size), sizeof(Header.subchunk2Size));
		}
		for (int j = 0; j < 4; j++)
		{
			Check[j] = Header.chunkId[j];
		}
		if (Check != "RIFF")
		{
			ErrorMusic(i - 3);
			return false;
		}
		for (int j = 0; j < 4; j++)
		{
			Check[j] = Header.format[j];
		}
		if (Check != "WAVE")
		{
			ErrorMusic(i - 3);
			return false;
		}
		for (int j = 0; j < 4; j++)
		{
			Check[j] = Header.subchunk1Id[j];
		}
		if (Check != "fmt ")
		{
			ErrorMusic(i - 3);
			return false;
			return false;
		}
		if (Header.audioFormat != 1)
		{
			ErrorMusic(i - 3);
			return false;
		}
		if (Header.numChannels != 1)
		{
			ErrorMusic(i - 3);
			return false;
		}
		if (Header.sampleRate != 44100)
		{
			ErrorMusic(i - 3);
			return false;
		}
		if (Header.bitsPerSample != 16)
		{
			ErrorMusic(i - 3);
			return false;
		}
		for (int j = 0; j < 4; j++)
		{
			Check[j] = Header.subchunk2Id[j];
		}
		if (Check != "data")
		{
			ErrorMusic(i - 3);
			return false;
		}
		File.close();
	}
	return true;
}