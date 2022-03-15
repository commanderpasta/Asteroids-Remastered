#pragma once

#include "model/ActorTypes.h"

#include <string>
#include <vector>

/**
 * Contains all data necessary to create a game object in OpenGL for an <ActorType>.
 */
struct ActorTypeData {
    std::string texturePath;
    std::string shaderPath;
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};

/**
 * Get a random texture path when creating an asteroid for more variety.
 * 
 * \return The file path for the randomly selected texture.
 */
inline std::string getRandomAsteroidTexture() {
    int randomInt = rand() % 4;
    switch (randomInt) {
    case 0:
        return "res/textures/asteroid1.bmp";
    case 1:
        return "res/textures/asteroid2.bmp";
    case 2:
        return "res/textures/asteroid3.bmp";
    case 3:
        return "res/textures/asteroid4.bmp";
    default:
        return "res/textures/asteroid1.bmp";
    }
}

/**
 * Get the related texture, shader and vertex data for an <ActorType>.
 * 
 * \param type The <ActorType> to get the data for.
 * \return An <ActorTypeData> structure containing the necessary data for OpenGL.
 */
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
        data.texturePath = "res/textures/spaceship.bmp";
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
        data.texturePath = "res/textures/ufo.bmp";
        break;
    case ActorType::ShipSmall:
        width = 12.0f;
        height = 6.0f;
        data.texturePath = "res/textures/ufo.bmp";
        break;
    case ActorType::Projectile:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projectile.bmp";
        break;
    case ActorType::Particle:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projectile.bmp";
        break;
    case ActorType::PlayerParticle:
        width = 2.0f;
        height = 2.0f;
        data.texturePath = "res/textures/projectile.bmp";
        break;
    case ActorType::Booster:
        data.indices = { 0, 1, 2 };
        data.positions = {
            -3.0f, -3.0f, 0.0f, 0.0f,
            3.0f, -3.0f, 1.0f, 0.0f,
            0.0f, 10.0f, 0.5f, 1.0f
        };
        data.texturePath = "res/textures/spaceship.bmp";
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