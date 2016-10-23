#pragma once
#include "Constants.h"

class CBasicShape
{
public:
	CBasicShape() {};
	CBasicShape(TYPES const& type);
	~CBasicShape();

	void SetColor(glm::vec3 const& color);
	void SetCoordinates(std::vector<glm::ivec2> const& coordinates);
	void SetRadius(glm::ivec2 const& radius);


	glm::vec3 GetColor();
	std::vector<glm::ivec2> GetCoordinates();
	TYPES GetType();
	glm::ivec2 GetRadius();


private:
	glm::ivec2 m_radius;
	std::vector<glm::ivec2> m_coordinates;
	glm::vec3 m_color;
	TYPES m_type;


};

