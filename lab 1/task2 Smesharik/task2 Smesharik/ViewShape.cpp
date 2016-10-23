#include "stdafx.h"
#include "ComplexShape.h"
#include "BasicShape.h"
#include <GL/glew.h>
#include "Constants.h"
#include <GL/gl.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include "ViewShape.h"

namespace 
{
	void DrawLine(std::vector<glm::ivec2> const& coordinates, glm::ivec2 const& pos)
	{
		glBegin(GL_LINES);
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			glVertex2f(float(coordinates[i].x + pos.x), float(coordinates[i].y + pos.y));
		}
		glEnd();
	}

	void DrawStrokeOfFillShape(std::vector<glm::ivec2> const& coordinates, glm::ivec2 const& pos)
	{
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_STRIP);
		glLineWidth(8);
		for (size_t i = 1; i < coordinates.size(); i++)
		{
			glVertex2f(float(coordinates[i].x + pos.x), float(coordinates[i].y + pos.y));
		}
		glEnd();
	}

	void DrawFillShape(std::vector<glm::ivec2> const& coordinates, glm::ivec2 const& pos)
	{
		glBegin(GL_TRIANGLE_FAN);
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			glVertex2f(float(coordinates[i].x + pos.x), float(coordinates[i].y + pos.y));
		}
		glEnd();
	}

	void DrawCircle(std::vector<glm::ivec2> const& coordinates, glm::ivec2 const& radius, glm::ivec2 const& pos)
	{
		

		const float step = float(2 * M_PI) / 360;
		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(float(coordinates[0].x + pos.x), float(coordinates[0].y + pos.y));

		for (float angle = 0; angle <= 2 * M_PI; angle += step)
		{
			float a = (fabsf(angle - 2 * M_PI) < 0.00001f) ? 0.f : angle;
			const float dx = radius.x * cosf(a);
			const float dy = radius.y * sinf(a);
			glVertex2f(float(dx + (coordinates[0].x) + pos.x), float(dy + (coordinates[0].y) + pos.y));
		}
		glEnd();
	}

	void DrawStrokeOfCircle(std::vector<glm::ivec2> const& coordinates, glm::ivec2 const& radius, glm::ivec2 const& pos)
	{
		const float step = float(2 * M_PI) / 360;
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (float angle = 0; angle <= 2 * M_PI; angle += step)
		{
			float a = (fabsf(angle - 2 * M_PI) < 0.00001f) ? 0.f : angle;
			const float dx = radius.x * cosf(a);
			const float dy = radius.y * sinf(a);
			glVertex2f(float(dx + (coordinates[0].x) + pos.x), float(dy + (coordinates[0].y) + pos.y));
		}
		glEnd();
	}
}
CViewShape::CViewShape()
{
}

CViewShape::~CViewShape()
{
}

void CViewShape::DrawShapes(std::vector <std::shared_ptr<CBasicShape>> const& shapes, glm::ivec2 const& pos)
{
	for (auto &shape : shapes)
	{
		glm::vec3 color = shape->GetColor();
		glColor3f(color.r, color.g, color.b);
		std::vector<glm::ivec2> coordinates = shape->GetCoordinates();

		if (shape->GetType() == TYPES::Line)
		{
			DrawLine(coordinates, pos);
		}
		else if (shape->GetType() == TYPES::FillShape)
		{
			DrawFillShape(coordinates, pos);
			DrawStrokeOfFillShape(coordinates, pos);
		}
		else if (shape->GetType() == TYPES::Ellipse)
		{
			glm::ivec2 radius = shape->GetRadius();
			DrawCircle(coordinates, radius, pos);
			DrawStrokeOfCircle(coordinates, radius, pos);
		}
	}
}
