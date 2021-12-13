#pragma once

#include "gl/Renderer.h"
#include <GLFW/glfw3.h>

#include "ActorView.h"
#include "ActorDataView.h"

#include "../model/GameModel.h"

#include <vector>
#include <memory>
#include <map>

class GameView {
	GLFWwindow *window;
	std::shared_ptr<GameModel> model;
	std::map<std::string, std::shared_ptr<ActorDataView>> actorTypes;
	std::map<unsigned int, ActorView> actors;
public:
	GameView(std::shared_ptr<GameModel> model); //todo: pass 'opengl' or 'sdl' and init renderer-object accordingly
	~GameView();

	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	void Clear() const;
	void Render();
	void Update();

	void AddActor(Shape shape, std::string& shaderPath, std::string& texturePath, std::string& type, unsigned int id);

	std::vector<std::string> GetInput() const;
};