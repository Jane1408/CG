#include "stdafx.h"
#include "Bodies.h"
#include <stdint.h>

namespace
{
typedef glm::vec3 Vertex;

const glm::vec4 BLACK = {0.f, 0.f, 0.f, 1.f};

const Vertex RHOMBIC_CUBIC_OCTAHEDRON_VERTICIES[] = {
	{ -0.5, -1.2,  0.5 },
	{ 0.5, -1.2, 0.5 },
	{ 0.5, -1.2, -0.5 },
	{ -0.5, -1.2, -0.5 },

	{-0.5, -0.5, 1.2 },
	{  0.5, -0.5, 1.2 },
	{ 1.2, -0.5, 0.5 },
	{ 1.2, -0.5, -0.5 },
	{ 0.5, -0.5, -1.2 },
	{ -0.5, -0.5, -1.2 },
	{ -1.2, -0.5, -0.5 },
	{ -1.2, -0.5, 0.5 },

	{ -0.5, 0.5, 1.2 },
	{ 0.5, 0.5, 1.2 },
	{ 1.2, 0.5, 0.5 },
	{ 1.2, 0.5, -0.5 },
	{ 0.5, 0.5, -1.2 },
	{ -0.5, 0.5, -1.2 },
	{ -1.2, 0.5, -0.5 },
	{ -1.2, 0.5, 0.5 },

	{ -0.5, 1.2,  0.5 },
	{ 0.5, 1.2, 0.5 },
	{ 0.5, 1.2, -0.5 },
	{ -0.5, 1.2, -0.5 },
};

// Привыкаем использовать 16-битный unsigned short,
// чтобы экономить память на фигурах с тысячами вершин.
const uint16_t RHOMBIC_CUBIC_OCTAHEDRON_EDGES[] = {
	3, 0, 1, 2, 3,
	10, 11, 0,
	4, 5, 1,
	6, 7, 2,
	8, 9, 3,
	10, 18, 19, 11,
	4,12, 13,5,
	6, 14, 15,7,
	8, 16, 17, 9,
	10, 18, 23,
	20, 19, 12, 20,
	21, 13, 14, 21,
	22, 15, 16, 22,
	23, 17, 18
};

struct STriangleFace
{
	uint16_t vertexIndex1;
	uint16_t vertexIndex2;
	uint16_t vertexIndex3;
	uint16_t colorIndex;
};

const STriangleFace RHOMBIC_CUBIC_OCTAHEDRON_FACES[] = {
	
	{0, 3, 2, static_cast<uint16_t>(Colors::Pink) },
	{0, 2, 1, static_cast<uint16_t>(Colors::Pink) },

	{4, 11, 0, static_cast<uint16_t>(Colors::Cyan) },
	{5, 4, 0, static_cast<uint16_t>(Colors::Green) },
	{5, 0, 1, static_cast<uint16_t>(Colors::Green) },
	{6, 5, 1, static_cast<uint16_t>(Colors::Purple) },
	{7, 6, 1, static_cast<uint16_t>(Colors::Pink) },
	{7, 1, 2, static_cast<uint16_t>(Colors::Pink) },
	{8, 7, 2, static_cast<uint16_t>(Colors::Purple) },
	{9, 8, 2, static_cast<uint16_t>(Colors::Cyan) },
	{9, 2, 3, static_cast<uint16_t>(Colors::Cyan) },
	{10, 9, 3, static_cast<uint16_t>(Colors::Blue) },
	{11, 10, 3, static_cast<uint16_t>(Colors::Purple) },
	{11, 3, 0, static_cast<uint16_t>(Colors::Purple) },

	{12, 4, 5, static_cast<uint16_t>(Colors::Purple) },
	{13, 12, 5, static_cast<uint16_t>(Colors::Purple) },
	{13, 5, 6, static_cast<uint16_t>(Colors::Pink) },
	{14, 13, 6, static_cast<uint16_t>(Colors::Pink) },
	{14, 6, 7, static_cast<uint16_t>(Colors::Cyan) },
	{15, 14, 7, static_cast<uint16_t>(Colors::Cyan) },
	{15, 7, 8, static_cast<uint16_t>(Colors::Blue) },
	{16, 15, 8, static_cast<uint16_t>(Colors::Blue) },
	{16, 8, 9, static_cast<uint16_t>(Colors::Green) },
	{17, 16, 9, static_cast<uint16_t>(Colors::Green) },
	{17, 9, 10, static_cast<uint16_t>(Colors::Pink) },
	{18, 17, 10, static_cast<uint16_t>(Colors::Pink) },
	{18, 10, 11, static_cast<uint16_t>(Colors::Cyan) },
	{19, 18, 11, static_cast<uint16_t>(Colors::Cyan) },
	{19, 11, 4, static_cast<uint16_t>(Colors::Blue) },
	{12, 19, 4, static_cast<uint16_t>(Colors::Blue) },

	{19, 12, 20, static_cast<uint16_t>(Colors::Cyan) },
	{12, 13, 20, static_cast<uint16_t>(Colors::Green) },
	{13, 21, 20, static_cast<uint16_t>(Colors::Green) },
	{13, 14, 21, static_cast<uint16_t>(Colors::Purple) },
	{14, 15, 21, static_cast<uint16_t>(Colors::Pink) },
	{15, 22, 21, static_cast<uint16_t>(Colors::Pink) },
	{15, 16, 22, static_cast<uint16_t>(Colors::Purple) },
	{16, 17, 22, static_cast<uint16_t>(Colors::Cyan) },
	{17, 23, 22, static_cast<uint16_t>(Colors::Cyan) },
	{17, 18, 23, static_cast<uint16_t>(Colors::Blue) },
	{18, 19, 23, static_cast<uint16_t>(Colors::Purple) },
	{19, 20, 23, static_cast<uint16_t>(Colors::Purple) },

	{20, 21, 22, static_cast<uint16_t>(Colors::Pink) },
	{20, 22, 23, static_cast<uint16_t>(Colors::Pink) },
};

/// @param phase - Фаза анимации на отрезке [0..1]
glm::mat4 GetRotateZTransfrom(float phase)
{
    // угол вращения вокруг оси Z лежит в отрезке [0...2*pi].
    const float angle = float(2 * M_PI) * phase;

    return glm::rotate(glm::mat4(), angle, {0, 0, 1});
}

/// @param phase - Фаза анимации на отрезке [0..1]
glm::mat4 GetScalingPulseTransform(float phase)
{
    // число пульсаций размера - произвольная константа.
    const int pulseCount = 4;
    float scale = fabsf(cosf(float(pulseCount * M_PI) * phase));

    return glm::scale(glm::mat4(), {scale, scale, scale});
}

/// @param phase - Фаза анимации на отрезке [0..1]
glm::mat4 GetBounceTransform(float phase)
{
    // начальная скорость и число отскоков - произвольные константы.
    const int bounceCount = 4;
    const float startSpeed = 15.f;
    // "время" пробегает bounceCount раз по отрезку [0...1/bounceCount].
    const float time = fmodf(phase, 1.f / float(bounceCount));
    // ускорение подбирается так, чтобы на 0.25с скорость стала
    // противоположна начальной.
    const float acceleration = - (startSpeed * 2.f * float(bounceCount));
    // расстояние - результат интегрирования функции скорости:
    //  speed = startSpeed + acceleration * time;
    float offset = time * (startSpeed + 0.5f * acceleration * time);

    // для отскоков с нечётным номером меняем знак.
    const int bounceNo = int(phase * bounceCount);
    if (bounceNo % 2)
    {
        offset = -offset;
    }
    return glm::translate(glm::mat4(), {offset, 0.f, 0.f});
}

void DrawTriangles(glm::vec4 colors[], bool needNormal)
{
	for (const STriangleFace &face : RHOMBIC_CUBIC_OCTAHEDRON_FACES)
	{
		const Vertex &v1 = RHOMBIC_CUBIC_OCTAHEDRON_VERTICIES[face.vertexIndex1];
		const Vertex &v2 = RHOMBIC_CUBIC_OCTAHEDRON_VERTICIES[face.vertexIndex2];
		const Vertex &v3 = RHOMBIC_CUBIC_OCTAHEDRON_VERTICIES[face.vertexIndex3];

		glColor4fv(glm::value_ptr(colors[face.colorIndex]));

		if (needNormal)
		{
			glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
			glNormal3fv(glm::value_ptr(normal));
		}
		glVertex3fv(glm::value_ptr(v1));
		glVertex3fv(glm::value_ptr(v2));
		glVertex3fv(glm::value_ptr(v3));
	}
}
}

CRhombicCubicOctahedron::CRhombicCubicOctahedron()
{
	for (glm::vec4 &color : m_colors)
	{
		color.x = 1;
		color.y = 1;
		color.z = 1;
		color.w = 0.5;
	}
}

void CRhombicCubicOctahedron::Update(float deltaTime)
{
    (void)deltaTime;
}

void CRhombicCubicOctahedron::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_LINE_STRIP);
	for (uint16_t i : RHOMBIC_CUBIC_OCTAHEDRON_EDGES)
	{
		Vertex v1 = RHOMBIC_CUBIC_OCTAHEDRON_VERTICIES[i];
		glColor4f(BLACK.x, BLACK.y, BLACK.z, BLACK.w);
		glVertex3fv(glm::value_ptr(v1));
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBegin(GL_TRIANGLES);
	DrawTriangles(m_colors, false);
	glEnd();

	glCullFace(GL_BACK);

	glBegin(GL_TRIANGLES);
	DrawTriangles(m_colors, true);
	glEnd();
}

void CRhombicCubicOctahedron::SetFaceColor(Colors colorFace, const glm::vec4 &color)
{
	const size_t index = static_cast<size_t>(colorFace);
	assert(index < COLORS_COUNT);
	m_colors[index] = color;
}