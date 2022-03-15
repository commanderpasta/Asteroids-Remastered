#include "ActorView.h"

/**
 * Initializes a single view instance of an actor.
 * 
 * \param data The render data needed for OpenGL.
 * \param shaderPath The file path of the shader code to generate a <Shader> program with.
 * \param id The id of the new actor.
 * \param windowX The game window's width.
 * \param windowY The game window's height.
 */
ActorView::ActorView(std::shared_ptr<ActorDataView> data, std::string shaderPath, unsigned int id, float windowX, float windowY)
    : data(data), shader(shaderPath), id(id), flickerFrequency(0), isVisible(false) {

    this->shader.Bind();

    this->setResolution(windowX, windowY);

    float identityMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_T", identityMatrix);
    this->shader.SetUniformMat4f("u_R", identityMatrix);

    this->shader.SetUniform1i("u_Texture", 0);
    this->shader.Unbind();
}

/**
 * Change projectionMatrix of this actor after a window resize.
 * 
 * \param x The new width for the game's window.
 * \param y The new height for the game's window.
 */
void ActorView::setResolution(float x, float y) {
    float LEFT = 0.0f;
    float RIGHT = x;
    float BOTTOM = 0.0f;
    float TOP = y;
    float NEAR = -1.0f;
    float FAR = 1.0f;

    // Following the standard in OpenGL for defining a projection matrix.
    float projectionMatrix[4][4] = {
        {2.0f / (RIGHT - LEFT), 0.0f, 0.0f, -1.0f * (RIGHT + LEFT) / (RIGHT - LEFT)},
        {0.0f, 2.0f / (TOP - BOTTOM), 0.0f, -1.0f * (TOP + BOTTOM) / (TOP - BOTTOM)},
        {0.0f, 0.0f, -2.0f / (FAR - NEAR), -1.0f * ((FAR + NEAR) / (FAR - NEAR))},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_P", projectionMatrix);
}

ActorView::~ActorView() {

}

/**
 * Changes the actor's position.
 *
 * \param position The actor's new x,y coordinates in the game space.
 * \param angle The orientation of the actor.
 */
void ActorView::setPosition(float position[2], float angle) {
    this->shader.Bind();
    float translationMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, position[0]},
        {0.0f, 1.0f, 0.0f, position[1]},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    float rotationMatrix[4][4] = {
        {cosf(angle), sinf(angle), 0.0f, 0.0f},
        {-sinf(angle), cosf(angle), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_T", translationMatrix);
    this->shader.SetUniformMat4f("u_R", rotationMatrix);
    this->shader.Unbind();
}

/**
 * Hide an actor from the screen without unloading it entirely.
 * 
 * Notice: This function is required for flickering the booster and does not work consistently.
 * 
 * \param isHidden Whether the actor should be hidden or not.
 */
void ActorView::setFlickerFrequency(float frequency) {
    this->flickerFrequency = frequency;
}

void ActorView::updateFlickering(steady_clock::time_point currentTickTime) {
    if (this->flickerFrequency == 0.0f) {
        return;
    }

    duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->lastFlicker);

    if (timeSpan.count() > 0.1f) {
        this->lastFlicker = currentTickTime;
        this->isVisible = !this->isVisible;
    }
}
