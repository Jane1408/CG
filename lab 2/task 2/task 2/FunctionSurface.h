#pragma once

#include "IBody.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

const glm::vec3 PINK = { 1.f, 0.07f, 0.58f };

using Function2D = std::function<glm::vec3(float, float)>;

// Вершина с трёхмерной позицией и нормалью.
struct SVertexP3N
{
    glm::vec3 position;
    glm::vec3 normal;
	glm::vec3 diffuseColor;
	glm::vec3 color = PINK;

	float u;
	float v;

    SVertexP3N() = default;
    SVertexP3N(const glm::vec3 &position, float const& u, float const& v)
        : position(position)
		, u(u)
		, v(v)
    {
	}
};

class CDottedFunctionSurface final : public IBody
{
public:
    CDottedFunctionSurface(const Function2D &fn);

    /// Инициализирует сетку треугольников
    /// @param rangeX - диапазон, где x - нижняя граница, y - верхняя граница
    /// @param rangeZ - диапазон, где x - нижняя граница, y - верхняя граница
    void Tesselate(const glm::vec2 &rangeU, const glm::vec2 &rangeV, float step);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

private:
    Function2D m_fn;
    std::vector<SVertexP3N> m_vertices;

	bool m_isColored;
};

class CSolidFunctionSurface final : public IBody
{
public:
    CSolidFunctionSurface(const Function2D &fn);

    /// Инициализирует индексированную сетку треугольников
    /// @param rangeX - диапазон, где x - нижняя граница, y - верхняя граница
    /// @param rangeZ - диапазон, где x - нижняя граница, y - верхняя граница
    void Tesselate(const glm::vec2 &rangeU, const glm::vec2 &rangeV, float step);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

	void ChangeDrawMode();
	void ChangeColorMode();

private:
    Function2D m_fn;
    std::vector<SVertexP3N> m_vertices;
    std::vector<uint32_t> m_indicies;

	bool m_isColored;
	bool m_isFilled;
};
