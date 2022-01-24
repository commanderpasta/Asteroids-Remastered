#pragma once

#include "gl/Renderer.h"
#include <GLFW/glfw3.h>

#include "Text.h"
#include "ActorView.h"

#include "../model/GameModel.h"

#include <utility>
#include <vector>
#include <memory>
#include <map>


class GameView {
	GLFWwindow *window;
	std::shared_ptr<GameModel> model;
	std::multimap<ActorType, std::shared_ptr<ActorDataView>> actorDataPerType;
	std::map<unsigned int, ActorView> actors;
	std::map<unsigned int, Text> texts;
	unsigned int scoreTextId;
public:
	GameView(std::shared_ptr<GameModel> model);
	~GameView();
	
	void setup();

	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void Clear() const;
	void Render();
	void Update();
	void updateResolution();
	void checkWindowResize();

	void AddActor(unsigned int id, ActorType actorType);
	void AddText(unsigned int id, std::string text, float x, float y);
	void removeText(unsigned int id);

	std::vector<std::string> GetInput() ;
};