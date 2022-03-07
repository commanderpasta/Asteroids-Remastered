#include "BaseDataView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>

/**
 * Creates buffers for an <ActorType>.
 * 
 * Can be reused across multiple objects of the same type.
 * 
 * \param indices The indices for each vertex.
 * \param positions The coordinates for the vertex and UV-map.
 * \param vertexCount The amount of values that describe the vertices.
 * \param positionsCount The amount of values in <positions>.
 */
BaseDataView::BaseDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount)
    : va(), ib(indices, vertexCount), vb(positions, positionsCount * sizeof(float)) {

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    this->va.AddBuffer(this->vb, layout);

    this->va.Unbind();
    this->vb.Unbind();
    this->ib.Unbind();
}

BaseDataView::~BaseDataView() {

}