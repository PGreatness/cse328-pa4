#ifndef PA4_TETRAHEDRON_H
#define PA4_TETRAHEDRON_H

#include "shape/Shape.h"


class Tetrahedron : public Shape
{
public:
    Tetrahedron()
    {
        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3);
        vertex.emplace_back(V1);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V2);
        vertex.emplace_back(V1);

        vertex.emplace_back(V1);
        vertex.emplace_back(V2);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V4);
        vertex.emplace_back(V2);

        // TODO
    }

private:
    // 4 triangular facets
    static constexpr GLint NUM_FACETS = 4;
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    static constexpr GLfloat RADIUS = 0.5773502691896258f;

    static constexpr glm::vec3 V1 { RADIUS,  RADIUS,  RADIUS};
    static constexpr glm::vec3 V2 {-RADIUS, -RADIUS,  RADIUS};
    static constexpr glm::vec3 V3 {-RADIUS,  RADIUS, -RADIUS};
    static constexpr glm::vec3 V4 { RADIUS, -RADIUS, -RADIUS};

    // vertices organized into triangular facets
    std::vector<glm::vec3> vertex;
};


#endif  // PA4_TETRAHEDRON_H
