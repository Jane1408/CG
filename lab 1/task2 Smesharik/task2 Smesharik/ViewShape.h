#pragma once

class CBasicShape;
class CViewShape
{
public:
	CViewShape();
	~CViewShape();

	static void DrawShapes(std::vector <std::shared_ptr<CBasicShape>> const& shapes, glm::ivec2 const& pos);

};

