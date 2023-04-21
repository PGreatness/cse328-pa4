#ifndef PA4_ICOSAHEDRON_H
#define PA4_ICOSAHEDRON_H

#include "shape/Shape.h"


class Icosahedron : public Shape
{

private:
    struct Options
    {
        static const uint DEFAULT = 0;
        static const uint SOLID = 0;
        static const uint WIREFRAME = 1;
        static const uint FLAT = 2;
        static const uint SMOOTH = 4;
    };

    static const Options options;

public:
    Icosahedron()
    {
        this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;
    }

    Icosahedron(glm::vec3 center, GLfloat size)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        this->translate(center);
        this->scale(size - 1);
    }

    Icosahedron(glm::vec3 center, GLfloat size, glm::vec3 color)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        this->translate(center);
        this->scale(size - 1);
        this->setColor(color);
    }

    // getters
    virtual GLint getNumVertices() const override
    {
        return INIT_NUM_VERTICES;
    }
    virtual const glm::vec3 * getVertexData() const override {
        glm::vec3 * tmp = new glm::vec3[INIT_NUM_VERTICES];
        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            tmp[i] = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
        }
        return tmp;
    }
    virtual const glm::vec3 * getNormalData() const override {
        glm::vec3 * tmp = new glm::vec3[INIT_NUM_VERTICES];
        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            tmp[i] = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
            tmp[i] = glm::normalize(tmp[i]);
        }
        return tmp;
    }

    // getters
    glm::vec3 getCenter() const
    {
        return center;
    }

    GLfloat getSize() const
    {
        return size;
    }

    glm::vec3 getColor() const
    {
        return color;
    }

    glm::vec3 getOldColor() const
    {
        return oldColor;
    }

    // setters
    void setCenter(glm::vec3 center)
    {
        this->translate(center - this->center);
    }

    void setSize(float size)
    {
        this->scale(size);
    }

    void setColor(glm::vec3 color)
    {
        this->oldColor = this->color;
        this->color = color;
    }

    // methods
    void scale(float scaleFactor)
    {
        this->size *= scaleFactor;
        updateIcosahedronSize(scaleFactor);
    }

    void translate(glm::vec3 translation)
    {
        this->center += translation;
        updateIcosahedronLocation(translation);
    }

    void rotate(float angle, glm::vec3 axis)
    {
        updateIcosahedronOrientation(angle, axis);
    }

    void render(GLuint icosaArray, GLuint icosaBuffer, uint shaderID) const
    {
        initializeRender(&icosaArray, &icosaBuffer);

        GLuint colorLocation = glGetUniformLocation(shaderID, "icosaColor");
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        glDrawArrays(GL_TRIANGLES, 0, INIT_NUM_VERTICES);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint icosaArray, GLuint icosaBuffer, uint shaderID, uint options) const
    {
        initializeRender(&icosaArray, &icosaBuffer);

        GLuint colorLocation = glGetUniformLocation(shaderID, "icosaColor");
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        if (options & Options::WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, INIT_NUM_VERTICES);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, INIT_NUM_VERTICES);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void subdivide()
    {
        GLfloat newVertexData[INIT_NUM_VERTICES * 4][3];

        for (int i = 0; i < INIT_NUM_VERTICES; i += 3)
        {
            glm::vec3 a = glm::vec3(this->vertexData[i][0],
                                    this->vertexData[i][1],
                                    this->vertexData[i][2]);

            glm::vec3 b = glm::vec3(this->vertexData[i + 1][0],
                                    this->vertexData[i + 1][1],
                                    this->vertexData[i + 1][2]);

            glm::vec3 c = glm::vec3(this->vertexData[i + 2][0],
                                    this->vertexData[i + 2][1],
                                    this->vertexData[i + 2][2]);

            std::vector<glm::vec3> newVertices = subdivision(a, b, c);

            for (int j = 0; j < newVertices.size(); j++)
            {
                newVertexData[i * 4 + j] = newVertices[j];
            }
        }
        this->vertexData = newVertexData;
    }

private:

    // the default iscosahedron is centered at the origin and has a side length of 1
    static constexpr GLfloat DEFAULT_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // the deffault color is white
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

    static constexpr GLint INIT_NUM_FACETS = 20;
    static constexpr GLint INIT_NUM_VERTICES = INIT_NUM_FACETS * 3;

    static constexpr GLfloat X = 0.525731112119133606f;
    static constexpr GLfloat Z = 0.850650808352039932f;

    // Each line represents the 3 vertices of a triangular facet.
    // An icosahedron has 20 such facets, each occupies one line.
    GLfloat vertexData[INIT_NUM_VERTICES][3] =
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

    // user-defined center, size, and color
    glm::vec3 center;
    GLfloat size;
    glm::vec3 color;
    glm::vec3 oldColor;

    // updates the vertex data when the size of the icosahedron changes
    void updateIcosahedronSize(float scale) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            this->vertexData[i][0] += vertexData[i][0] * scale;
            this->vertexData[i][1] += vertexData[i][1] * scale;
            this->vertexData[i][2] += vertexData[i][2] * scale;
        }
        this->translate(tmp);
    }

    // updates the vertex data when the center of the icosahedron changes
    void updateIcosahedronLocation(glm::vec3 translation) {
        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            this->vertexData[i][0] += translation[0];
            this->vertexData[i][1] += translation[1];
            this->vertexData[i][2] += translation[2];
        }
    }

    void rotateX(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[1][1] = cos(angle_rad);
        rotationMatrix[1][2] = -sin(angle_rad);
        rotationMatrix[2][1] = sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            glm::vec4 tmp = rotationMatrix * glm::vec4(this->vertexData[i][0], this->vertexData[i][1], this->vertexData[i][2], 1.0f);
            this->vertexData[i][0] = tmp[0];
            this->vertexData[i][1] = tmp[1];
            this->vertexData[i][2] = tmp[2];
        }
    }

    void rotateY(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][2] = sin(angle_rad);
        rotationMatrix[2][0] = -sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            glm::vec4 tmp = rotationMatrix * glm::vec4(this->vertexData[i][0], this->vertexData[i][1], this->vertexData[i][2], 1.0f);
            this->vertexData[i][0] = tmp[0];
            this->vertexData[i][1] = tmp[1];
            this->vertexData[i][2] = tmp[2];
        }
    }

    void rotateZ(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][1] = -sin(angle_rad);
        rotationMatrix[1][0] = sin(angle_rad);
        rotationMatrix[1][1] = cos(angle_rad);

        for (int i = 0; i < INIT_NUM_VERTICES; i++) {
            glm::vec4 tmp = rotationMatrix * glm::vec4(this->vertexData[i][0], this->vertexData[i][1], this->vertexData[i][2], 1.0f);
            this->vertexData[i][0] = tmp[0];
            this->vertexData[i][1] = tmp[1];
            this->vertexData[i][2] = tmp[2];
        }
    }

    // updates the vertex data when the icosahedron is rotated
    void updateIcosahedronOrientation(float angle, glm::vec3 axes) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axes[0] != 0) { rotateX(angle); }
        if (axes[1] != 0) { rotateY(angle); }
        if (axes[2] != 0) { rotateZ(angle); }
        this->translate(tmp);
    }

    // initialize the render data and uniforms
    void initializeRender(GLuint * icosaArray, GLuint * icosaBuffer) const
    {
        // create buffers/arrays
        glGenVertexArrays(1, icosaArray);
        glGenBuffers(1, icosaBuffer);

        glBindVertexArray(*icosaArray);

        glBindBuffer(GL_ARRAY_BUFFER, *icosaBuffer);

        const glm::vec3 *dataStart = getVertexData();
        glBufferData(GL_ARRAY_BUFFER, INIT_NUM_VERTICES * sizeof(glm::vec3), dataStart, GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    }

    // Subdivide an icosahedron to approximate a unit sphere.
    // Takes as input a triangular facet on a unit icosahedron
    // and subdivide it into four new triangular facets.
    // Returns an array of glm::vec3. The 4 elements are the
    // new vertices
    std::vector<glm::vec3> subdivision(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
    {
        // sampling
        glm::vec3 v12 = glm::normalize((v1 + v2) / 2.0f);
        glm::vec3 v23 = glm::normalize((v2 + v3) / 2.0f);
        glm::vec3 v31 = glm::normalize((v3 + v1) / 2.0f);

        // return the new vertices
        std::vector<glm::vec3> newVertices;
        newVertices.push_back(v1);
        newVertices.push_back(v12);
        newVertices.push_back(v31);

        newVertices.push_back(v2);
        newVertices.push_back(v23);
        newVertices.push_back(v12);

        newVertices.push_back(v3);
        newVertices.push_back(v31);
        newVertices.push_back(v23);

        newVertices.push_back(v12);
        newVertices.push_back(v23);
        newVertices.push_back(v31);

        return newVertices;
    }
};


#endif  // PA4_ICOSAHEDRON_H
