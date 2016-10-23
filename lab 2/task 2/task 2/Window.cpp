#include "stdafx.h"
#include "Window.h"
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace
{
const glm::vec4 BLACK = {0, 0, 0, 1};
const float MATERIAL_SHININESS = 30.f;
const glm::vec4 FADED_WHITE_RGBA = {0.3f, 0.3f, 0.3f, 1.f};
const glm::vec4 BLUE = { 0.f, 0.f, 1.f , 1.f };
const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
const glm::vec3 SUNLIGHT_DIRECTION = { -1, 0.2f, 0.7f };
const float CAMERA_INITIAL_ROTATION = 0;
const float CAMERA_INITIAL_DISTANCE = 15.f;

const glm::vec2 U_RANGE = { -M_PI / 2, 3 * M_PI / 2 + 0.2f };
const glm::vec2 V_RANGE = { 0, 2 * M_PI * 1.025f };
const float STEP = 0.1f;

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // включаем систему освещения
    glEnable(GL_LIGHTING);
}

glm::vec3 GetCoordinatesFromVariables(float u, float v)
{
	const float r = 1.f;
	glm::vec3 coordinates;
	if ((0.f <= u) && (u < M_PI))
	{
		coordinates.x = 6 * std::cos(u) * (1 + std::sin(u)) + 4 * r * (1 - (std::cos(u) / 2)) * std::cos(u) * std::cos(v);
		coordinates.y = 16 * std::sin(u) + 4 * r * (1 - (std::cos(u) / 2)) * std::sin(u) * std::cos(v);
		coordinates.z = 4 * r * (1 - (std::cos(u) / 2)) * std::sin(v);
	}
	else if ((- 2 * M_PI / 3 <= u) && (u <= 0.f))
	{
		coordinates.x = 6 * std::cos(u) * (1 + std::sin(u)) + 4 * r * (1 - (std::cos(u) / 2)) * std::cos(v);
		coordinates.y = 16 * std::sin(u);
		coordinates.z = 4 * r * (1 - (std::cos(u) / 2)) * std::sin(v);
	}
	else if ((M_PI <= u) && (u <= M_PI * 2 ))
	{
		coordinates.x = 6 * std::cos(u) * (1 + std::sin(u)) - 4 * r * (1 - (std::cos(u) / 2)) * std::cos(v);
		coordinates.y = 16 * std::sin(u);
		coordinates.z = 4 * r * (1 - (std::cos(u) / 2)) * std::sin(v);
	}
	return coordinates;
}
}

CWindow::CWindow()
    : m_surface(GetCoordinatesFromVariables)
    , m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
    , m_sunlight(GL_LIGHT0)
{
    SetBackgroundColor(BLACK);
 
    m_material.SetAmbient(BLUE);
    m_material.SetDiffuse(BLUE);
    m_material.SetSpecular(FADED_WHITE_RGBA);
    m_material.SetShininess(MATERIAL_SHININESS);

	m_surface.Tesselate(U_RANGE, V_RANGE, STEP);
    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetAmbient(0.1f * WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);
}

void CWindow::OnWindowInit(const glm::ivec2 &size)
{
    (void)size;
    SetupOpenGLState();
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);
    m_surface.Update(deltaSeconds);
}

void CWindow::OnDrawWindow(const glm::ivec2 &size)
{
    SetupView(size);
    m_sunlight.Setup();
    m_material.Setup();
    m_surface.Draw();
}

void CWindow::SetupView(const glm::ivec2 &size)
{
    glViewport(0, 0, size.x, size.y);

    // Матрица вида возвращается камерой и составляет
    // начальное значение матрицы GL_MODELVIEW.
    glLoadMatrixf(glm::value_ptr(m_camera.GetViewTransform()));

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;
    const glm::mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
}

void CWindow::OnKeyDown(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyDown(event);
}

void CWindow::OnKeyUp(const SDL_KeyboardEvent &event)
{
	m_camera.OnKeyUp(event);
	if (event.keysym.sym == SDLK_SPACE)
	{
		m_surface.ChangeDrawMode();
	}
	else if (event.keysym.sym == SDLK_c)
	{
		m_surface.ChangeColorMode();
	}
}
void CWindow::OnDragBegin(const glm::vec2 & pos)
{
	m_camera.OnDragBegin(pos);
}

void CWindow::OnDragMotion(const glm::vec2 & pos)
{
	m_camera.OnDragMotion(pos);
}

void CWindow::OnDragEnd(const glm::vec2 & pos)
{
	m_camera.OnDragEnd(pos);
}

