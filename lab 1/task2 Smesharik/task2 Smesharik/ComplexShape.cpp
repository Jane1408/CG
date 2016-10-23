#include "stdafx.h"
#include "Constants.h"
#include "BasicShape.h"
#include "ComplexShape.h"


CComplexShape::CComplexShape()
{
	ReadInfo();
}


CComplexShape::~CComplexShape()
{
}

void CComplexShape::ReadInfo()
{
	std::ifstream input(NAME_OF_INPUT_FILE);
	std::string line;

	int x = 0;
	int y = 0;
	input >> x >> y;
	m_center = { x, y };


	input >> m_radius;

	while (getline(input, line))
	{
		ReadFromFile(line);
	}
}

void CComplexShape::ReadFromFile(std::string const & inputName)
{
	if (inputName == "")
		return;
	std::ifstream input(inputName);
	std::string typeOfShape;
	getline(input, typeOfShape);
	TYPES type = MAP_OF_TYPES.find(typeOfShape)->second;
	CBasicShape shape(type);

	glm::vec3 color;
	input >> color.r;
	input >> color.g;
	input >> color.b;

	if (shape.GetType() == TYPES::Ellipse)
	{
		glm::ivec2 radius;
		input >> radius.x >> radius.y;
		shape.SetRadius(radius);
	}
	std::vector<glm::ivec2> coordinates;

	glm::ivec2 pairOfCoord;

	while (input >> pairOfCoord.x >> pairOfCoord.y)
	{
		coordinates.push_back(pairOfCoord);
	}
	shape.SetColor(color);
	shape.SetCoordinates(coordinates);

	m_shapes.push_back(std::make_shared<CBasicShape>(shape));
}

bool CComplexShape::HitTest(const glm::vec2 & point)
{
	double distance = std::sqrt(std::pow(m_center.x + m_position.x - point.x, 2) + std::pow(m_center.y + m_position.y - point.y, 2));
	return distance <= m_radius;
}

const std::vector<std::shared_ptr<CBasicShape>>& CComplexShape::GetShapes() const
{
	return m_shapes;
}

void CComplexShape::SetPosition(glm::ivec2 const & position)
{
	m_position = position;
}

const glm::ivec2 CComplexShape::GetPosition()
{
	return m_position;
}

const glm::ivec2 CComplexShape::GetCenter()
{
	return m_center;
}

