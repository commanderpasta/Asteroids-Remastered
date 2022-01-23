#include "Text.h"

Character::Character(std::shared_ptr<BaseDataView> data, std::string shaderPath, unsigned int id, float x, float y)
    : data(data), shader(shaderPath), id(id) {

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

void Character::SetPosition(float position[3], float angle) {
    this->shader.Bind();
    float translationMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    // apply translation to view matrix
    for (int i = 0; i < 3; i++) {
        translationMatrix[i][3] += position[i];
    }

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

Text::Text(unsigned int id, std::string text, float x, float y, float windowX, float windowY) : text(text), id(id) {
	int counter = 0;

	ActorTypeData typeData = getActorDataFromType(ActorType::Character);
	for (auto& c : this->text) {
        /*
                -width, -height, 0.0f, 0.0f,
               width, -height, 1.0f, 0.0f,
                width, height, 1.0f, 1.0f,
                -width, height, 0.0f, 1.0f
        */
        auto column = charCoordinates[c].second;
        auto row = charCoordinates[c].first;

        typeData.positions = {
                -30.0f, -60.0f, column * 0.125f, 0.875f - row * 0.125f,
               30.0f, -60.0f, column * 0.125f + 0.125f, 0.875f - row * 0.125f,
                30.0f, 60.0f, column * 0.125f + 0.125f, 0.875f - row  * 0.125f + 0.125f,
                -30.0f, 60.0f, column * 0.125f, 0.875f - row * 0.125f + 0.125f
        };

        auto vertexData = std::make_shared<BaseDataView>(typeData.indices.data(), typeData.positions.data(), 8, typeData.positions.size());

		Character charRenderData(std::move(vertexData), typeData.shaderPath, this->id + counter + 1, windowX, windowY);
        float position[3] = { x + counter * 60, y, 0 };
        charRenderData.SetPosition(position, 0.0f);
        this->characters.push_back(std::move(charRenderData));

		counter++;
	}
}

void Text::setup() {
    Text::texture = std::make_shared<Texture>("res/textures/font/ExportedFont.bmp");
}