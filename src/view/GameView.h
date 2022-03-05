#pragma once

#include "gl/Renderer.h"
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Text.h"
#include "ActorView.h"

#include "../model/GameModel.h"

#include <utility>
#include <vector>
#include <memory>
#include <map>

/**
 * View component of the game loop following the MVC pattern.
 * 
 * Includes rendering game object and UI, as well as a window context using GLFW and GLEW.
 */
class GameView {
	GLFWwindow *window;
	std::shared_ptr<GameModel> model;
	std::multimap<ActorType, std::shared_ptr<ActorDataView>> actorDataPerType;
	std::map<unsigned int, ActorView> actors;
	std::map<unsigned int, Text> texts;
	std::vector<ActorView> playerLivesUIObjects;
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
	void addPlayerLife(unsigned int id);
	void AddText(unsigned int id, std::string text, float x, float y);
	void removeText(unsigned int id);

	HWND getHwnd();
	std::vector<std::string> GetInput();
};