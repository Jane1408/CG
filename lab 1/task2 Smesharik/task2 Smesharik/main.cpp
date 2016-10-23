// main.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include "Window.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <SDL2/SDL.h>

// ��������� ������������ main �� SDL_main,
// �.�. ���������� ���������� c SUBSYSTEM:CONSOLE
#ifdef _WIN32
#undef main
#endif


int main()
{
	CWindow window;
	window.ShowFixedSize({ 450, 600 });
	window.DoGameLoop();

	return 0;
}