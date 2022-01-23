#pragma once

#include "gl/Renderer.h"
#include <GLFW/glfw3.h>

#include "Text.h"
#include "ActorView.h"

#include "../model/GameModel.h"

#include <vector>
#include <memory>
#include <map>

class GameView {
	GLFWwindow *window;
	std::shared_ptr<GameModel> model;
	std::map<ActorType, std::shared_ptr<ActorDataView>> actorDataPerType;
	std::map<unsigned int, ActorView> actors;
	std::map<unsigned int, Text> texts;
public:
	GameView(std::shared_ptr<GameModel> model); //todo: pass 'opengl' or 'sdl' and init renderer-object accordingly
	~GameView();

	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void Clear() const;
	void Render();
	void Update();
	void updateResolution();
	void checkWindowResize();
	//void erase(unsigned int id);

	void AddActor(unsigned int id, ActorType actorType);
	void AddText(unsigned int id, std::string text, float x, float y);

	std::vector<std::string> GetInput() ;
};