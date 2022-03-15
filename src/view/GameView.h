#pragma once

#include "gl/Renderer.h"
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32 //get the HWND context from GLFW to use in DirectSound
#include <GLFW/glfw3native.h>

#include "Text.h"
#include "ActorView.h"
#include "../IDGen.h"

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
	GLFWwindow *window; /**< The GLFW window context for this game. */
	std::shared_ptr<GameModel> model; /**< A reference to the model to get data from. */
	std::multimap<ActorType, std::shared_ptr<ActorDataView>> actorDataPerType; /**< A map of all actor types with their buffer data, so the buffers can be accessed. */
	std::map<unsigned int, ActorView> actors; /**< A map of all actors that are drawn in the game. */
	std::map<unsigned int, Text> texts; /**< A map of all texts displayed in the game at the moment. */
	std::vector<ActorView> playerLivesUIObjects; /**< A list of UI objects that show the player's current lives. */

	steady_clock::time_point currentFrameTime;

	unsigned int boosterId;

	unsigned int gameOverId;

	void updateFrameTime();
public:
	GameView(std::shared_ptr<GameModel> model);
	~GameView();
	
	void setup();

	bool shouldWindowClose() const;
	void swapFrameBuffers() const;
	void clearFrameBuffer() const;
	void render();
	void update();
	void updateResolution();
	void checkWindowResize();
	
	void addBooster();
	void addActor(unsigned int id, ActorType actorType);
	void addPlayerLifeUI(unsigned int id);
	void addText(unsigned int id, std::string text, float x, float y);
	void removeText(unsigned int id);

	void gameOver();

	HWND getHwnd();
	std::vector<std::string> getInput();
};