#include "stdafx.h"
#include "FunctionSurface.h"
#include <algorithm>


namespace
{
	const glm::vec3 MAX_COLOR = { 0, 0.5, 1 };
	const glm::vec3 MIN_COLOR = { 1, 0.5, 0 };
	const float DOT_SIZE = 5.f;

	void CalculateNormals(std::vector<SVertexP3N> &vertices, const Function2D &fn, float step)
	{
		for (SVertexP3N &ver : vertices)
		{
			const glm::vec3 &position = ver.position;
			glm::vec3 norm1 = fn(ver.u, ver.v + step) - position;
			glm::vec3 norm2 = fn(ver.u + step, ver.v) - position;
			ver.normal = glm::normalize(glm::cross(norm1, norm2));
		}
	}

	void SetDeffuseColors(std::vector<SVertexP3N> &vertices, glm::vec2 const& range)
	{
		for (SVertexP3N &ver : vertices)
		{
			const float factor = sqrt(0.5f);
			const float distance = std::hypotf(ver.position.x - 1, ver.position.y - 7) * factor;
			const float length = (std::abs(range.x - range.y));
			const float normalizedDistance = std::abs(0 - distance) / (length * factor);

			const auto color = MAX_COLOR * normalizedDistance + MIN_COLOR * (1 - normalizedDistance);
			ver.diffuseColor = color;
		}
	}

	/// Привязывает вершины к состоянию OpenGL,
	/// затем вызывает 'callback'.
	template <class T>
	void DoWithBindedArrays(const std::vector<SVertexP3N> &vertices, const bool & isColored, T && callback)
	{
		// Включаем режим vertex array и normal array и color array.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// Выполняем привязку vertex array и normal array и color array
		const size_t stride = sizeof(SVertexP3N);
		glNormalPointer(GL_FLOAT, stride, glm::value_ptr(vertices[0].normal));
		glVertexPointer(3, GL_FLOAT, stride, glm::value_ptr(vertices[0].position));
		auto color = (isColored) ? glm::value_ptr(vertices[0].color) : glm::value_ptr(vertices[0].diffuseColor);
		glColorPointer(3, GL_FLOAT, stride, color);

		// Выполняем внешнюю функцию.
		callback();

		// Выключаем режим vertex array и normal array и color array.
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void CalculateTriangleStripIndicies(std::vector<uint32_t> &indicies, unsigned columnCount, unsigned rowCount)
	{
		indicies.clear();
		indicies.reserve((columnCount - 1) * rowCount * 2);
		// вычисляем индексы вершин.
		for (unsigned ci = 0; ci < columnCount - 1; ++ci)
		{
			if (ci % 2 == 0)
			{
				for (unsigned ri = 0; ri < rowCount; ++ri)
				{
					unsigned index = ci * rowCount + ri;
					indicies.push_back(index + rowCount);
					indicies.push_back(index);
				}
			}
			else
			{
				for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
				{
					unsigned index = ci * rowCount + ri;
					indicies.push_back(index);
					indicies.push_back(index + rowCount);
				}
			}
		}
	}
}

CDottedFunctionSurface::CDottedFunctionSurface(const Function2D &fn)
    : m_fn(fn)
{
}

void CDottedFunctionSurface::Tesselate(const glm::vec2 &rangeU, const glm::vec2 &rangeV, float step)
{
    m_vertices.clear();
    for (float u = rangeU.x; u < rangeU.y; u += step)
    {
        for (float v = rangeV.x; v < rangeV.y; v += step)
        {
            m_vertices.push_back(SVertexP3N(m_fn( u, v), u, v));
        }
    }
	CalculateNormals(m_vertices, m_fn, step);
}

void CDottedFunctionSurface::Draw() const
{
	glPointSize(DOT_SIZE);
	DoWithBindedArrays(m_vertices, m_isColored, [this] {
		glDrawArrays(GL_POINTS, 0, GLsizei(m_vertices.size()));
	});
}

CSolidFunctionSurface::CSolidFunctionSurface(const Function2D &fn)
	: m_fn(fn)
	, m_isColored(true)
	, m_isFilled(true)
{
}

void CSolidFunctionSurface::Tesselate(const glm::vec2 &rangeU, const glm::vec2 &rangeV, float step)
{
	const unsigned uCount = unsigned((rangeU.y - rangeU.x) / step);
	const unsigned vCount = unsigned((rangeV.y - rangeV.x) / step);
	glm::vec2 range;

	m_vertices.clear();
	m_vertices.reserve(uCount * vCount);
	for (unsigned ui = 0; ui < uCount; ++ui)
	{
		const float u = rangeU.x + step * float(ui);
		for (unsigned vi = 0; vi < vCount; ++vi)
		{
			const float v = rangeV.x + step * float(vi);
			m_vertices.push_back(SVertexP3N(m_fn(u, v), u, v));

			const float x = m_vertices[m_vertices.size() - 1].position.x;
			range.y = std::min(x, range.y);
			range.x = std::max(x, range.x);
		}
	}
	SetDeffuseColors(m_vertices, range);
	CalculateNormals(m_vertices, m_fn, step);
	CalculateTriangleStripIndicies(m_indicies, uCount, vCount);
}

void CSolidFunctionSurface::Draw() const
{
	glCullFace(GL_FRONT);
	// TODO: use glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	DoWithBindedArrays(m_vertices, m_isColored, [this] {
		GLenum mode = (m_isFilled) ? GL_TRIANGLE_STRIP : GL_LINE_STRIP;
		glDrawElements(mode, GLsizei(m_indicies.size()),
			GL_UNSIGNED_INT, m_indicies.data());
	});

	glCullFace(GL_BACK);
	DoWithBindedArrays(m_vertices, m_isColored, [this] {
		GLenum mode = (m_isFilled) ? GL_TRIANGLE_STRIP : GL_LINE_STRIP;
		glDrawElements(mode, GLsizei(m_indicies.size()),
			GL_UNSIGNED_INT, m_indicies.data());
	});
}

void CSolidFunctionSurface::ChangeDrawMode()
{
	m_isFilled = !m_isFilled;
}

void CSolidFunctionSurface::ChangeColorMode()
{
	m_isColored = !m_isColored;
}
