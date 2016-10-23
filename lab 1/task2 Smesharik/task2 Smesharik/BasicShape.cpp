#include "stdafx.h"
#include "BasicShape.h"


CBasicShape::CBasicShape(TYPES const& type)
	: m_type(type)
{
}


CBasicShape::~CBasicShape()
{
}

void CBasicShape::SetColor(glm::vec3 const & color)
{
	m_color = color;
}

void CBasicShape::SetCoordinates(std::vector<glm::ivec2> const & coordinates)
{
	m_coordinates = coordinates;
}

void CBasicShape::SetRadius(glm::ivec2 const & radius)
{
	m_radius = radius;
}


glm::vec3 CBasicShape::GetColor()
{
	return m_color;
}

std::vector<glm::ivec2> CBasicShape::GetCoordinates()
{
	return m_coordinates;
}

TYPES CBasicShape::GetType()
{
	return m_type;
}

glm::ivec2 CBasicShape::GetRadius()
{
	return m_radius;
}

