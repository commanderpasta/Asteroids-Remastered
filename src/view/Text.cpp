#include "Text.h"

/**
 * Creates the buffer data and shader program for a text character.
 * 
 * \param data The buffer data for this character
 * \param shaderPath The file path to use for creating a <Shader> program
 * \param x The absolute x coordinate of this character
 * \param y The absolute y coordinate of this character 
 */
Character::Character(std::shared_ptr<BaseDataView> data, std::string shaderPath, float x, float y)
    : data(data), shader(shaderPath) {

    this->shader.Bind();

    this->setResolution(x, y);

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
 * Changes the projection matrix of this <Character>.
 * 
 * If the window resolution changes, the <Shader>'s projection matrix needs to be adapted
 * for the new resolution.
 * 
 * \param x The width of the new resolution
 * \param y The height of the new resolution
 */
void Character::setResolution(float x, float y) {
    float LEFT = 0.0f;
    float RIGHT = x;
    float BOTTOM = 0.0f;
    float TOP = y;
    float NEAR = -1.0f;
    float FAR = 1.0f;

    float projectionMatrix[4][4] = {
        {2.0f / (RIGHT - LEFT), 0.0f, 0.0f, -1.0f * (RIGHT + LEFT) / (RIGHT - LEFT)},
        {0.0f, 2.0f / (TOP - BOTTOM), 0.0f, -1.0f * (TOP + BOTTOM) / (TOP - BOTTOM)},
        {0.0f, 0.0f, -2.0f / (FAR - NEAR), -1.0f * ((FAR + NEAR) / (FAR - NEAR))},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_P", projectionMatrix);
}

Character::~Character() {

}

/**
 * Changes the position of the <Character>.
 * 
 * \param position The character's new x,y coordinates in the game space.
 * \param angle The orientation of the character.
 */
void Character::setPosition(float position[2], float angle) {
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
 * Creates a new text that can be displayed on screen.
 * 
 * \param id The id of the new text.
 * \param text The text it represents.
 * \param x The x-coordinate of the lower left corner of the text.
 * \param y The y-coordinate of the lower left corner of the text.
 * \param windowX The width of the window of the game instance.
 * \param windowY The height of the window of the game instance.
 */
Text::Text(unsigned int id, std::string text, float x, float y, float windowX, float windowY) : text(text), id(id) {
	int counter = 0;

	ActorTypeData typeData = getActorDataFromType(ActorType::Character);
	for (auto& c : this->text) {
        auto column = charCoordinates[c].second;
        auto row = charCoordinates[c].first;

        //todo optimize
        typeData.positions = {
            -15.0f, -30.0f, 0.0f, 0.875f,
            15.0f, -30.0f, 0.125f, 0.875f,
            15.0f, 30.0f, 0.125f, 1.0f,
            -15.0f, 30.0f, 0.0f, 1.0f
        };

        auto vertexData = std::make_shared<BaseDataView>(typeData.indices.data(), typeData.positions.data(), 8, typeData.positions.size());

		Character charRenderData(std::move(vertexData), typeData.shaderPath, windowX, windowY);

        charRenderData.shader.Bind();
        charRenderData.shader.SetUniform2f("u_texCoordShift", column * 0.125f, - (row * 0.125f));

        float position[2] = { x + counter * 30, y };
        charRenderData.setPosition(position, 0.0f);
        this->characters.push_back(std::move(charRenderData));

		counter++;
	}
}

/**
 * Initializes a sample font.
 */
void Text::setup() {
    Text::texture = std::make_shared<Texture>("res/textures/font/ExportedFont.bmp");
}