#pragma once
#include "AbstractWindow.h"
#include "ComplexShape.h"

class CWindow : public CAbstractWindow
{
public: 
	CWindow();
	~CWindow();
protected:
	void OnWindowEvent(const SDL_Event &event) override;

	void OnUpdateWindow(float deltaSeconds) override;

	void OnDrawWindow(const glm::ivec2 & size) override;

	void OnDragBegin(const glm::vec2 &pos);
	void OnDragMotion(const glm::vec2 &pos);
	void OnDragEnd(const glm::vec2 &pos);

private:
	void SetupView(const glm::ivec2 & size);

	CComplexShape m_complexShape;

	glm::ivec2 m_offSet;
	bool m_isMove = false;
};
