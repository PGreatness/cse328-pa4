#ifndef PA4_ICOSAHEDRON_H
#define PA4_ICOSAHEDRON_H

#include "shape/Shape.h"


class Icosahedron : public Shape
{
public:
    Icosahedron()
    {
        // TODO
    }

private:
    static constexpr GLint INIT_NUM_FACETS = 20;
    static constexpr GLint INIT_NUM_VERTICES = INIT_NUM_FACETS * 3;

    static constexpr GLfloat X = 0.525731112119133606f;
    static constexpr GLfloat Z = 0.850650808352039932f;

    // Each line represents the 3 vertices of a triangular facet.
    // An icosahedron has 20 such facets, each occupies one line.
    static constexpr GLfloat vertexData[INIT_NUM_VERTICES][3] =
            {
                    {X,   0.0,  Z}, {0.0, Z,    X}, {-X,  0.0,  Z},
                    {0.0, Z,    X}, {-Z,  X,  0.0}, {-X,  0.0,  Z},
                    {0.0, Z,    X}, {0.0, Z,   -X}, {-Z,  X,  0.0},
                    {Z,   X,  0.0}, {0.0, Z,   -X}, {0.0, Z,    X},
                    {X,   0.0,  Z}, {Z,   X,  0.0}, {0.0, Z,    X},
                    {X,   0.0,  Z}, {Z,   -X, 0.0}, {Z,   X,  0.0},
                    {Z,   -X, 0.0}, {X,   0.0, -Z}, {Z,   X,  0.0},
                    {Z,   X,  0.0}, {X,   0.0, -Z}, {0.0, Z,   -X},
                    {X,   0.0, -Z}, {-X,  0.0, -Z}, {0.0, Z,   -X},
                    {X,   0.0, -Z}, {0.0, -Z,  -X}, {-X,  0.0, -Z},
                    {X,   0.0, -Z}, {Z,   -X, 0.0}, {0.0, -Z,  -X},
                    {Z,   -X, 0.0}, {0.0, -Z,   X}, {0.0, -Z,  -X},
                    {0.0, -Z,   X}, {-Z,  -X, 0.0}, {0.0, -Z,  -X},
                    {0.0, -Z,   X}, {-X,  0.0,  Z}, {-Z,  -X, 0.0},
                    {0.0, -Z,   X}, {X,   0.0,  Z}, {-X,  0.0,  Z},
                    {Z,   -X, 0.0}, {X,   0.0,  Z}, {0.0, -Z,   X},
                    {-Z,  -X, 0.0}, {-X,  0.0,  Z}, {-Z,  X,  0.0},
                    {-X,  0.0, -Z}, {-Z,  -X, 0.0}, {-Z,  X,  0.0},
                    {0.0, Z,   -X}, {-X,  0.0, -Z}, {-Z,  X,  0.0},
                    {-Z,  -X, 0.0}, {-X,  0.0, -Z}, {0.0, -Z,  -X}
            };
};


#endif  // PA4_ICOSAHEDRON_H
