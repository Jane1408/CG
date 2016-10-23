#include "stdafx.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include "Window.h"

namespace
{
	void DrawCoordinates(const glm::ivec2 & size)
	{
		glBegin(GL_LINES);
		glVertex2f(0.0f, size.y / 2);
		glVertex2f(size.x, size.y / 2);

		glVertex2f(size.x / 2, 0.0f);
		glVertex2f(size.x / 2, size.y);
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(size.x / 2 - 7, 10);
		glVertex2f(size.x / 2, 0.0f);
		glVertex2f(size.x / 2 + 7, 10);
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(size.x - 10, size.y / 2 - 7);
		glVertex2f(size.x, size.y / 2);
		glVertex2f(size.x - 10, size.y / 2 + 7);
		glEnd();

		float centerX = size.x / 2;
		float centerY = size.y / 2;

		float x = 0.0f;
		float y = 0.0f;
		glBegin(GL_LINES);
		for (int i = 1; i < 10; i++)
		{
			x = (centerX / 10) * i;
			y = (centerY / 10) * i;
			
			glVertex2f(centerX + x, size.y / 2 - 5);
			glVertex2f(centerX + x, size.y / 2 + 5);

			glVertex2f(centerX - x, size.y / 2 - 5);
			glVertex2f(centerX - x, size.y / 2 + 5);

			glVertex2f(size.x / 2 - 5, centerY + y);
			glVertex2f(size.x / 2 + 5, centerY + y);

			glVertex2f(size.x / 2 - 5, centerY - y);
			glVertex2f(size.x / 2 + 5, centerY - y);
			
		}
		glEnd();
	}

	void DrawCardioid(const glm::ivec2 & size)
	{
		float range = 2.0f * M_PI;
		glBegin(GL_LINE_STRIP);

		for (float i = 0.0f; i <= range; i += 0.1)
		{
			float x = 2 * (size.x / 8) *  std::cos(i) - (size.x / 8) *  std::cos(2 * i) + size.x / 2;
			float y = 2 * (size.y / 8) * std::sin(i) - (size.y / 8) * std::sin(2 * i) + size.y / 2;
			glVertex2f(x, y);
		}
		glEnd();
	}	
}
CWindow::CWindow()
{
}


CWindow::~CWindow()
{
}

void CWindow::OnWindowEvent(const SDL_Event &event)
{

}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
	(void)deltaSeconds;
}

void CWindow::OnDrawWindow(const glm::ivec2 & size)
{
	SetupView(size);
	glColor3f(1, 1, 1);
	DrawCoordinates(size);
	glColor3f(1, 0, 0);
	DrawCardioid(size);
}

void CWindow::SetupView(const glm::ivec2 & size)
{
	const glm::mat4 matrix = glm::ortho<float>(0, float(size.x), float(size.y), 0);
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
}
