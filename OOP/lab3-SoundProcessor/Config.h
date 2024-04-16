#pragma once

enum class Modes
{
	MUTE,
	MIX,
	REV
};

struct Config
{
	Modes Mode;
	int File;
	int Begin;
	int End;
};