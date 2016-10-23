#pragma once

class CBasicShape;
class CComplexShape
{
public:
	CComplexShape();
	~CComplexShape();

	void ReadInfo();
	void ReadFromFile(std::string const& inputName);
	bool HitTest(const glm::vec2 &point);
	const std::vector <std::shared_ptr<CBasicShape>> & GetShapes() const;
	void SetPosition(glm::ivec2 const& position);
	const glm::ivec2 GetPosition();
	const glm::ivec2 GetCenter();

private:

	std::vector <std::shared_ptr<CBasicShape>> m_shapes;
	int m_radius;
	glm::ivec2 m_center;
	glm::ivec2 m_position;
};

