#ifndef PA4_CUBE_H
#define PA4_CUBE_H

#include "shape/Shape.h"


class Cube : public Shape
{
public:
    Cube()
    {
        // TODO
        
    }
    virtual GLint getNumVertices() const override {
        return NUM_VERTICES;
    }

    virtual const glm::vec3 * getVertexData() const override {
        return vertexData;
    }

    virtual const glm::vec3 * getNormalData() const override {
        // normalize the vertex data
        glm::vec3 *normalData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i++) {
            normalData[i] = glm::normalize(glm::vec3(vertexData[i]));
        }
        return normalData;
    }

private:
    static constexpr GLint NUM_FACETS = 6 * 2;  // 6 square faces, each composed of 2 triangular facets
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    // Each line represents a 3D vertex,
    // Each 3 lines represents a triangle in 3D space,
    // Each 2x3 lines represents a quad facet (composed of 2 triangles) of this cube.
    static constexpr GLfloat vertexData[NUM_VERTICES][3] =
            {
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,

                    -0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,

                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,

                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,

                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f, -0.5f,

                    -0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
            };
};


#endif  // PA4_CUBE_H
