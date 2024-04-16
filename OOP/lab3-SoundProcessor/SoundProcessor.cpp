#include "SoundProcessor.h"
#include "Factory.h"
#include "Mix.h"
#include "Reverse.h"
#include "Mute.h"

void SoundProcessor::Convert()
{
	Factory FactoryObj;
	FactoryObj.RegisterCreator("Mix", std::make_shared<Creator<Mix>>());
	FactoryObj.RegisterCreator("Reverse", std::make_shared<Creator<Reverse>>());
	FactoryObj.RegisterCreator("Mute", std::make_shared<Creator<Mute>>());
	Samples.resize(InFile.size());
	int Bits = 0;
	for (int i = 0; i < Header.subchunk2Size / (Header.blockAlign * Header.sampleRate) + 1; i++)
	{
		for (int ii = 0; ii < Samples.size(); ii++)
		{
			InFile[ii].read(reinterpret_cast<char*>(&Samples[ii]), sizeof(Samples[ii]));
		}
		Bits = InFile[0].gcount();
		for (int j = 0; j < Regime.size(); j++)
		{
			switch (Regime[j].Mode)
			{
			case Modes::MUTE:
			{
				std::shared_ptr<IConverter> MuteConverter = FactoryObj.Create("Mute");
				Samples[0] = MuteConverter->Convert(Samples, Regime[j], i);
				break;
			}
			case Modes::MIX:
			{
				std::shared_ptr<IConverter> MixConverter = FactoryObj.Create("Mix");
				MixConverter->SetBits(InFile[Regime[j].File - 1].gcount() / 2);
				Samples[0] = MixConverter->Convert(Samples, Regime[j], i);
				break;
			}
			case Modes::REV:
			{
				std::shared_ptr<IConverter> RevConverter = FactoryObj.Create("Reverse");
				Samples[0] = RevConverter->Convert(Samples, Regime[j], i);
				break;
			}
			}
		}
		OutFile.write(reinterpret_cast<char*>(&Samples[0]), Bits);
	}
}

void SoundProcessor::Preparing(int Val, char** FileNames)
{
	InFile.resize(Val - 4);
	for (int i = 4; i < Val; i++)
	{
		InFile[i - 4].open(FileNames[i], std::ios::binary);
	}
	OutFile.open(FileNames[3], std::ios::binary);
	InFile[0].read(reinterpret_cast<char*>(&Header), sizeof(WAVHEADER));
	std::string Check;
	for (int j = 0; j < 4; j++)
	{
		Check.push_back(Header.subchunk2Id[j]);
	}
	if (Check == "LIST")
	{
		std::array <char,4> a;
		InFile[0].read(reinterpret_cast<char*>(&a), Header.subchunk2Size);
		InFile[0].read(reinterpret_cast<char*>(&Header.subchunk2Id), sizeof(Header.subchunk2Id));
		InFile[0].read(reinterpret_cast<char*>(&Header.subchunk2Size), sizeof(Header.subchunk2Size));
	}
	OutFile.write(reinterpret_cast<char*>(&Header), sizeof(WAVHEADER));
	WAVHEADER Tmp;
	for (int i = 1; i < Val - 4; i++)
	{
		InFile[i].read(reinterpret_cast<char*>(&Tmp), sizeof(WAVHEADER));
		Check.resize(0);
		for (int j = 0; j < 4; j++)
		{
			Check.push_back(Tmp.subchunk2Id[j]);
		}
		if (Check == "LIST")
		{
			std::array <char,4> a;
			InFile[i].read(reinterpret_cast<char*>(&a), Tmp.subchunk2Size);
			InFile[i].read(reinterpret_cast<char*>(&Tmp.subchunk2Id), sizeof(Tmp.subchunk2Id));
			InFile[i].read(reinterpret_cast<char*>(&Tmp.subchunk2Size), sizeof(Tmp.subchunk2Size));
		}
	}
	Config Buf;
	std::ifstream File(FileNames[2]);
	std::string Line = "";
	std::string Exam = "";
	while (getline(File, Line))
	{
		if (Line[Line.size() - 1] == '\n' || Line[Line.size() - 1] == '\r')
		{
			Line.resize(Line.size() - 1);
		}
		Exam = "";
		if (Line[0] == '#')
		{
			continue;
		}
		for (int i = 0; i < 4; i++)
		{
			Exam.push_back(Line[i]);
		}
		if (Exam == "mix ")
		{
			Buf.Mode = Modes::MIX;
			Buf.End = 0;
			int k = 5;
			int Quantity = 0;
			while (Line[k] != ' ')
			{
				Quantity *= 10;
				Quantity += Line[k] - '0';
				k++;
			}
			Buf.File = Quantity;
			k++;
			Quantity = 0;
			while (k < Line.size())
			{
				Quantity *= 10;
				Quantity += Line[k] - '0';
				k++;
			}
			Buf.Begin = Quantity;
			Regime.push_back(Buf);
		}
		else if (Exam == "rev ")
		{
			Buf.Mode = Modes::REV;
			Buf.File = 0;
			int k = 4;
			int FrstQuan = 0;
			while (Line[k] != ' ')
			{
				FrstQuan *= 10;
				FrstQuan += Line[k] - '0';
				k++;
			}
			Buf.Begin = FrstQuan;
			k++;
			int ScndQuan = 0;
			while (k < Line.size())
			{
				ScndQuan *= 10;
				ScndQuan += Line[k] - '0';
				k++;
			}
			Buf.End = ScndQuan;
			Regime.push_back(Buf);
		}
		else
		{
			Exam.push_back(Line[4]);
			if (Exam == "mute ")
			{
				Buf.Mode = Modes::MUTE;
				Buf.File = 0;
				int k = 5;
				int FrstQuan = 0;
				while (Line[k] != ' ')
				{
					FrstQuan *= 10;
					FrstQuan += Line[k] - '0';
					k++;
				}
				Buf.Begin = FrstQuan;
				k++;
				int ScndQuan = 0;
				while (k < Line.size())
				{
					ScndQuan *= 10;
					ScndQuan += Line[k] - '0';
					k++;
				}
				Buf.End = ScndQuan;
				Regime.push_back(Buf);
			}
		}
	}
	File.close();
}