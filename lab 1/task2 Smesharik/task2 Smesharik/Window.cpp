#include "stdafx.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>

#include "Constants.h"
#include "ViewShape.h"
#include "Window.h"

namespace
{
	glm::vec2 GetMousePosition(const SDL_MouseMotionEvent &event)
	{
		return{ event.x, event.y };
	}

	glm::vec2 GetMousePosition(const SDL_MouseButtonEvent &event)
	{
		return{ event.x, event.y };
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
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		OnDragBegin(GetMousePosition(event.button));
		break;
	case SDL_MOUSEBUTTONUP:
		OnDragEnd(GetMousePosition(event.button));
		break;
	case SDL_MOUSEMOTION:
		OnDragMotion(GetMousePosition(event.motion));
		break;
	}
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
	(void)deltaSeconds;
}

void CWindow::OnDrawWindow(const glm::ivec2 & size)
{
	SetupView(size);
	CViewShape::DrawShapes(m_complexShape.GetShapes(), m_complexShape.GetPosition());
}

void CWindow::OnDragBegin(const glm::vec2 & pos)
{
	if (m_complexShape.HitTest(pos))
	{
		m_isMove = true;
		glm::ivec2 shapePos = m_complexShape.GetPosition();
		m_offSet = { pos.x - shapePos.x,  pos.y - shapePos.y };
	}
}

void CWindow::OnDragMotion(const glm::vec2 & pos)
{
	if (m_isMove)
	{
		glm::ivec2 newPos = { pos.x - m_offSet.x, pos.y - m_offSet.y };
		m_complexShape.SetPosition(newPos);
	}
}

void CWindow::OnDragEnd(const glm::vec2 & pos)
{
	m_isMove = false;

}

void CWindow::SetupView(const glm::ivec2 & size)
{
	const glm::mat4 matrix = glm::ortho<float>(0, float(size.x), float(size.y), 0);
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
}
