#include "GameView.h"

#include <iostream>

GameView::GameView(std::shared_ptr<GameModel> model) : model(model), window(nullptr) {
    /* Initialize the library */
    glfwInit();

    this->window = glfwCreateWindow(this->model->windowX, this->model->windowY, "Asteroids: Remastered", glfwGetPrimaryMonitor(), NULL);

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

void GameView::updateResolution() {
    for (auto& actor : this->actors) {
        actor.second.setResolution(this->model->windowX, this->model->windowY);
    }
}

void GameView::checkWindowResize() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (width != this->model->windowX || height != this->model->windowY) {
        this->model->windowX = width;
        this->model->windowY = height;

        this->updateResolution();
    }
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
    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        keyboardEvents.push_back("SPACE");
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

struct ActorTypeData {
    std::string texturePath;
    std::string shaderPath;
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};

ActorTypeData getActorDataFromType(ActorType type) {
    ActorTypeData data;

    data.shaderPath = "res/shaders/Basic.shader";
    float scale = 1.0f;
    switch (type) {
    case ActorType::Triangle:
        data.indices = { 0, 1, 2 };
        data.positions = {
            -10.0f, -10.0f, 0.0f, 0.0f,
            10.0f, -10.0f, 1.0f, 0.0f,
            0.0f, 20.0f, 0.5f, 1.0f
        };

        return data;
    case ActorType::Quad:
        scale = 15.0f;
        break;
    case ActorType::Player:
        data.indices = { 0, 1, 2 };
        data.positions = {
            -10.0f, -10.0f, 0.0f, 0.0f,
            10.0f, -10.0f, 1.0f, 0.0f,
            0.0f, 20.0f, 0.5f, 1.0f
        };
        data.texturePath = "res/textures/spaceshipNeu.bmp";
        return data;
        break;
    case ActorType::AsteroidLarge:
        scale = 30.0f;
        data.texturePath = "res/textures/AsteroidBigTest.bmp";
        break;
    case ActorType::AsteroidMedium:
        scale = 20.0f;
        data.texturePath = "res/textures/AsteroidBigTest.bmp";
        break;
    case ActorType::AsteroidSmall:
        scale = 10.0f;
        data.texturePath = "res/textures/AsteroidBigTest.bmp";
        break;
    case ActorType::ShipLarge:
        scale = 15.0f;
        data.texturePath = "res/textures/spaceshipNeu.bmp";
        break;
    case ActorType::ShipSmall:
        scale = 15.0f;
        data.texturePath = "res/textures/spaceshipNeu.bmp";
        break;
    case ActorType::Projectile:
        scale = 2.0f;
        data.texturePath = "res/textures/projektil.bmp";
        break;
    }

    data.indices = { 0, 1, 2, 2, 3, 0 };
    data.positions = {
            -scale, -scale, 0.0f, 0.0f,
           scale, -scale, 1.0f, 0.0f,
            scale, scale, 1.0f, 1.0f,
            -scale, scale, 0.0f, 1.0f
    };

    return data;
}


void GameView::AddActor(unsigned int id, ActorType actorType) {
    ActorTypeData typeData = getActorDataFromType(actorType);

    if (this->actorDataPerType.count(actorType) == 1) {
        ActorView newActor(actorDataPerType.find(actorType)->second, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
        this->actors.insert({ id, std::move(newActor) });
    } else {
        if (actorType == ActorType::Triangle) {
            std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 6, typeData.positions.size(), typeData.texturePath);
            this->actorDataPerType.insert({ actorType, newActorType });

            ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
            this->actors.insert({ id, std::move(newActor) });
        } else {
            std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 8, typeData.positions.size(), typeData.texturePath);
            this->actorDataPerType.insert({ actorType, newActorType });

            ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
            this->actors.insert({ id, std::move(newActor) });
        }
    }
}

void GameView::Render()  {
    //TODO: make map for actor lists with their id as key instead of going by array index
    unsigned int count = 0;
    for (auto& actor : this->actors) {
        actor.second.data->texture.Bind();
        actor.second.shader.Bind();
        actor.second.data->va.Bind();
        actor.second.data->ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, actor.second.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        count++;
    }
}

void GameView::Update() {
    // delete actors in view
    for (auto actor = this->actors.cbegin(), next_actor = actor; actor != this->actors.cend(); actor = next_actor)
    {
        ++next_actor;
        if (model->actors.count(actor->first) != 1) {
            this->actors.erase(actor);
        }
    }

    // add new actors in view
    for (auto& currentActorModel : model->actors) {
        if (this->actors.count(currentActorModel.first) != 1) {
            this->AddActor(currentActorModel.second->id, currentActorModel.second->actorType);
        }

        // update actor position in view
        this->actors.find(currentActorModel.first)->second.SetPosition(currentActorModel.second->position, currentActorModel.second->rotation);
    }
}

void GameView::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}