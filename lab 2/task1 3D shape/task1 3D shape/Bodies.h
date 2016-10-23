#pragma once

#include <glm/vec3.hpp>
#include <glm/fwd.hpp>
#include <boost/noncopyable.hpp>

enum class Colors {
	Blue = 0,
	Yellow,
	Green,
	Pink,
	Cyan,
	Purple,
	NumColors,
};

class CRhombicCubicOctahedron
{
public:
	CRhombicCubicOctahedron();
    void Update(float deltaTime);
    void Draw();
	void SetFaceColor(Colors face, const glm::vec4 &color);

private:
	static const size_t COLORS_COUNT = static_cast<size_t>(Colors::NumColors);
	glm::vec4 m_colors[COLORS_COUNT];
};
