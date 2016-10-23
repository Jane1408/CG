#pragma once
#include "stdafx.h"


static const std::string NAME_OF_INPUT_FILE = "files/input.txt";

enum class TYPES
{
	Line = 0,
	Ellipse,
	FillShape,
};

const std::map < std::string, TYPES> MAP_OF_TYPES =
{
	{"line", TYPES::Line},
	{"ellipse", TYPES::Ellipse },
	{"fill shape", TYPES::FillShape }
};