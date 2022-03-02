#pragma once

#include "model/ActorTypes.h"

#include <string>
#include <vector>


struct ActorTypeData {
    std::string texturePath;
    std::string shaderPath;
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};

inline std::string getRandomAsteroidTexture() {
    int randomInt = rand() % 4;
    switch (randomInt) {
    case 0:
        return "res/textures/Asteroid1.bmp";
    case 1:
        return "res/textures/Asteroid2.bmp";
    case 2:
        return "res/textures/Asteroid3.bmp";
    case 3:
        return "res/textures/Asteroid4.bmp";
    default:
        return "res/textures/Asteroid1.bmp";
    }
}

inline ActorTypeData getActorDataFromType(ActorType type) {
    ActorTypeData data;

    data.shaderPath = "res/shaders/Basic.shader";
    float width = 1.0f;
    float height = 1.0f;

    switch (type) {
    case ActorType::Character:
        width = 30.0f;
        height = 60.0f;
        data.indices = { 0, 1, 2, 2, 3, 0 };
        data.positions = {
                -width, -height, 0.0f, 0.0f,
               width, -height, 1.0f, 0.0f,
                width, height, 1.0f, 1.0f,
                -width, height, 0.0f, 1.0f
        };
        data.shaderPath = "res/shaders/Text.shader";
        return data;
    case ActorType::Player:
        data.indices = { 0, 1, 2 };
        data.positions = {
            -8.0f, -8.0f, 0.0f, 0.0f,
            8.0f, -8.0f, 1.0f, 0.0f,
            0.0f, 16.0f, 0.5f, 1.0f
        };
        data.texturePath = "res/textures/spaceshipNeu.bmp";
        return data;
        break;
    case ActorType::AsteroidLarge:
        width = 30.0f;
        height = 30.0f;
        data.texturePath = getRandomAsteroidTexture();
        break;
    case ActorType::AsteroidMedium:
        width = 15.0f;
        height = 15.0f;
        data.texturePath = getRandomAsteroidTexture();
        break;
    case ActorType::AsteroidSmall:
        width = 6.0f;
        height = 6.0f;
        data.texturePath = getRandomAsteroidTexture();
        break;
    case ActorType::ShipLarge:
        width = 20.0f;
        height = 10.0f;
        data.texturePath = "res/textures/Ufo.bmp";
        break;
    case ActorType::ShipSmall:
        width = 12.0f;
        height = 6.0f;
        data.texturePath = "res/textures/Ufo.bmp";
        break;
    case ActorType::Projectile:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projektil.bmp";
        break;
    case ActorType::Particle:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projektil.bmp";
        break;
    case ActorType::PlayerParticle:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projektil.bmp";
        break;
    case ActorType::Booster:
        data.indices = { 0, 1, 2 };
        data.positions = {
            -3.0f, -3.0f, 0.0f, 0.0f,
            3.0f, -3.0f, 1.0f, 0.0f,
            0.0f, 10.0f, 0.5f, 1.0f
        };
        data.texturePath = "res/textures/spaceshipNeu.bmp";
        return data;
    }

    data.indices = { 0, 1, 2, 2, 3, 0 };
    data.positions = {
            -width, -height, 0.0f, 0.0f,
           width, -height, 1.0f, 0.0f,
            width, height, 1.0f, 1.0f,
            -width, height, 0.0f, 1.0f
    };

    return data;
}