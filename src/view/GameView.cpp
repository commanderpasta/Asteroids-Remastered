#include "GameView.h"
#include "Shapes.h"

#include <iostream>

GameView::GameView(std::shared_ptr<GameModel> model) : model(model), window(nullptr) {
    /* Initialize the library */
    glfwInit();

    this->window = glfwCreateWindow(this->model->windowX, this->model->windowY, "Asteroids: Remastered", NULL, NULL);

    if (!this->window)
    {
        glfwTerminate();
        //return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);

    /* Tie frame rate to monitor refresh rate */
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
        //return -1;
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

GameView::~GameView() {

}

bool GameView::ShouldWindowClose() const {
    return glfwWindowShouldClose(this->window);
}

void GameView::SwapBuffers() const {
    glfwSwapBuffers(this->window);
}

std::vector<std::string> GameView::GetInput() const {
    glfwPollEvents();

    std::vector<std::string> keyboardEvents;

    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
        keyboardEvents.push_back("FORWARD");
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
        keyboardEvents.push_back("RIGHT");
    }
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
        keyboardEvents.push_back("LEFT");
    }
    if (glfwGetKey(this->window, GLFW_KEY_H) == GLFW_PRESS) {
        keyboardEvents.push_back("H");
    }

    return keyboardEvents;
}

void GameView::AddActor(Shape shape, std::string& shaderPath, std::string& texturePath, unsigned int id) { 
    if (shape == Shape::Triangle) {
        ActorView newActor(TriangleShape::indices.data(), TriangleShape::positions.data(), 6, TriangleShape::positions.size(), shaderPath, texturePath, id);
        this->actors.insert({ id, std::move(newActor) });
    } else if (shape == Shape::Quad) {
        ActorView newActor(QuadShape::indices.data(), QuadShape::positions.data(), 8, QuadShape::positions.size(), shaderPath, texturePath, id);
        this->actors.insert({ id, std::move(newActor) });
    }
}

void GameView::Render()  {
    //TODO: make map for actor lists with their id as key instead of going by array index
    unsigned int count = 0;
    for (auto& actor : this->actors) {
        actor.second.shader.Bind();
        actor.second.va.Bind();
        actor.second.ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, actor.second.ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        count++;
    }
}

void GameView::Update() {
    // delete actors in view
    for (auto actor = model->actors.cbegin(), next_actor = actor; actor != model->actors.cend(); actor = next_actor)
    {
        ++next_actor;
        if (model->actors.count(actor->first) != 1) {
            model->actors.erase(actor);
        }
    }

    // add new actors in view
    for (auto& currentActorModel : model->actors) {
        if (this->actors.count(currentActorModel.first) != 1) {
            this->AddActor(currentActorModel.second->shape, currentActorModel.second->shaderPath, currentActorModel.second->texturePath, currentActorModel.second->id);
        }

        // update actor position in view
        this->actors.find(currentActorModel.first)->second.SetPosition(currentActorModel.second->position, currentActorModel.second->rotation);
    }
}

void GameView::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}