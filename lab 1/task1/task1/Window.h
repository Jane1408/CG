#pragma once
#include "AbstractWindow.h"

class CWindow : public CAbstractWindow
{
public: 
	CWindow();
	~CWindow();
protected:
	void OnWindowEvent(const SDL_Event &event) override;

	void OnUpdateWindow(float deltaSeconds) override;

	void OnDrawWindow(const glm::ivec2 & size) override;

private:
	void SetupView(const glm::ivec2 & size);
};
