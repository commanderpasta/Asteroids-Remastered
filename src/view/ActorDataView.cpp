#include "ActorDataView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>

/**
 * Initializes render data for an actor.
 * 
 * To render an actor there needs to be a <BaseDataView> object and a texture.
 * 
 * \param indices The indices for each vertex.
 * \param positions The coordinates for the vertex and UV-map.
 * \param vertexCount The amount of values that describe the vertices.
 * \param positionsCount The amount of values in <positions>.
 * \param texturePath The file path for the texture to use.
 */
ActorDataView::ActorDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount, std::string& texturePath)
    : BaseDataView(indices, positions, vertexCount, positionsCount), texture(texturePath) {
    this->texture.Bind();
}

ActorDataView::~ActorDataView() {

}