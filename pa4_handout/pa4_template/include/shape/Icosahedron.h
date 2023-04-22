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
        this->rotationDegrees = {0.0f, 0.0f, 0.0f};
    }

    Icosahedron(glm::vec3 center, GLfloat size)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;
        this->rotationDegrees = {0.0f, 0.0f, 0.0f};

        this->translate(center);
        this->scale(size - 1);
    }

    Icosahedron(glm::vec3 center, GLfloat size, glm::vec3 color)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;
        this->rotationDegrees = {0.0f, 0.0f, 0.0f};

        this->translate(center);
        this->scale(size - 1);
        this->setColor(color);
    }

    // getters
    virtual GLint getNumVertices() const override
    {
        return this->vertexData.size();
    }
    virtual const glm::vec3 * getVertexData() const override {
        glm::vec3 * tmp = new glm::vec3[this->vertexData.size()];
        for (int i = 0; i < this->vertexData.size(); i++) {
            tmp[i] = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
        }
        return tmp;
    }
    virtual const glm::vec3 * getNormalData() const override {
        glm::vec3 * faceNormals = new glm::vec3[this->vertexData.size()];
        std::vector<glm::vec3> tmp;
        for (int i = 0; i < this->vertexData.size(); i += 3)
        {
            glm::vec3 v1 = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
            glm::vec3 v2 = glm::vec3(vertexData[i + 1][0], vertexData[i + 1][1], vertexData[i + 1][2]);
            glm::vec3 v3 = glm::vec3(vertexData[i + 2][0], vertexData[i + 2][1], vertexData[i + 2][2]);
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            tmp.push_back(normal);
            tmp.push_back(normal);
            tmp.push_back(normal);
        }
        for (int i = 0; i < tmp.size(); i++)
        {
            faceNormals[i][0] = tmp[i][0];
            faceNormals[i][1] = tmp[i][1];
            faceNormals[i][2] = tmp[i][2];
        }
        return faceNormals;
    }

    const glm::vec3 * getCombinedData() const {
        glm::vec3 * tmp = new glm::vec3[this->vertexData.size() * 2];
        for (int i = 0; i < this->vertexData.size(); i++) {
            tmp[i] = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
        }
        for (int i = 0; i < this->vertexData.size(); i++) {
            tmp[i + this->vertexData.size()] = glm::vec3(vertexData[i][0], vertexData[i][1], vertexData[i][2]);
            tmp[i + this->vertexData.size()] = glm::normalize(tmp[i + this->vertexData.size()]);
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

    glm::vec3 getRotationDegrees() const
    {
        return rotationDegrees;
    }

    // setters
    void setCenter(glm::vec3 center)
    {
        this->translate(center - this->center);
    }

    void setSize(float size)
    {
        this->scale(size / this->size);
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

    void reflect(glm::vec3 planeA, glm::vec3 planeB)
    {
        updateIcosahedronReflection(planeA, planeB);
    }

    void render(GLuint icosaArray, GLuint icosaBuffer, uint shaderID) const
    {
        GLuint vbo_normals;
        initializeRender(&icosaArray, &icosaBuffer, &vbo_normals);

        GLuint colorLocation = glGetUniformLocation(shaderID, "icosaColor");
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        glDrawArrays(GL_TRIANGLES, 0, INIT_NUM_VERTICES);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint icosaArray, GLuint icosaBuffer, uint shaderID, uint options) const
    {
        GLuint vbo_normals;
        initializeRender(&icosaArray, &icosaBuffer, &vbo_normals);

        GLuint colorLocation = glGetUniformLocation(shaderID, "icosaColor");
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        if (options & Options::FLAT) {
            GLuint flatLocation = glGetUniformLocation(shaderID, "isFlat");
            glUniform1i(flatLocation, 1);
        }

        if (options & Options::WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, this->getNumVertices());
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void subdivide()
    {
        subdivision();
        this->subdivisions++;
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

    GLint subdivisions = 1;

    static constexpr GLfloat X = 0.525731112119133606f;
    static constexpr GLfloat Z = 0.850650808352039932f;

    // Each line represents the 3 vertices of a triangular facet.
    // An icosahedron has 20 such facets, each occupies one line.
    std::vector<std::array<GLfloat, 3>> vertexData =
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
    glm::vec3 rotationDegrees;

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
        if (axes[0] != 0) { this->rotationDegrees[0] += angle; rotateX(angle); }
        if (axes[1] != 0) { this->rotationDegrees[1] += angle; rotateY(angle); }
        if (axes[2] != 0) { this->rotationDegrees[2] += angle; rotateZ(angle); }
        this->translate(tmp);
    }

    void updateIcosahedronReflection(glm::vec3 planeA, glm::vec3 planeB)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);

        glm::vec3 planeNormal = glm::normalize(glm::cross(planeA, planeB));

        // Create reflection matrix
        glm::mat4 P = glm::outerProduct(glm::vec4(planeNormal, 0.0f), glm::vec4(planeNormal, 0.0f));
        glm::mat4 R = glm::mat4(1.0f) - 2.0f * P;

        // Apply reflection matrix to each vertex of the cube
        for (int i = 0; i < this->getNumVertices(); i++)
        {
            glm::vec4 tmp = R * glm::vec4(this->vertexData[i][0], this->vertexData[i][1], this->vertexData[i][2], 1.0f);
            this->vertexData[i][0] = tmp[0];
            this->vertexData[i][1] = tmp[1];
            this->vertexData[i][2] = tmp[2];
        }
        this->translate(tmp);
    }

    // initialize the render data and uniforms
    void initializeRender(GLuint * icosaArray, GLuint * icosaBuffer, GLuint * icosaNormals) const
    {
        // create buffers/arrays
        glGenVertexArrays(1, icosaArray);
        glGenBuffers(1, icosaBuffer);
        glGenBuffers(1, icosaNormals);

        glBindVertexArray(*icosaArray);

        glBindBuffer(GL_ARRAY_BUFFER, *icosaBuffer);

        const glm::vec3 *dataStart = getVertexData();
        const auto size = this->getNumVertices() * sizeof(glm::vec3);
        const glm::vec3 *normalsStart = getNormalData();
        const auto normalsSize = this->getNumVertices() * sizeof(glm::vec3);

        glBufferData(GL_ARRAY_BUFFER, size, dataStart, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, *icosaNormals);
        glBufferData(GL_ARRAY_BUFFER, normalsSize, normalsStart, GL_STATIC_DRAW);


        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);

    }

    // Subdivide an icosahedron to approximate a unit sphere.
    // Takes as input a triangular facet on a unit icosahedron
    // and subdivide it into four new triangular facets.
    // Returns an array of glm::vec3. The 4 elements are the
    // new vertices
    void subdivision()
    {
        auto tmp = this->getCenter();
        auto tmp2 = this->getRotationDegrees();
        this->translate(-tmp);
        this->rotateX(-tmp2[0]);
        this->rotateY(-tmp2[1]);
        this->rotateZ(-tmp2[2]);

        std::vector<std::array<GLfloat, 3>> newVertices;
        for (int i = 0; i < this->getNumVertices(); i += 3)
        {
            std::array<GLfloat, 3> v1 = { this->vertexData[i][0], this->vertexData[i][1], this->vertexData[i][2] };
            std::array<GLfloat, 3> v2 = { this->vertexData[i + 1][0], this->vertexData[i + 1][1], this->vertexData[i + 1][2] };
            std::array<GLfloat, 3> v3 = { this->vertexData[i + 2][0], this->vertexData[i + 2][1], this->vertexData[i + 2][2] };

            std::array<GLfloat, 3> v12;
            std::array<GLfloat, 3> v23;
            std::array<GLfloat, 3> v31;

            getHalfVertex(&v1, &v2, &v12);
            getHalfVertex(&v2, &v3, &v23);
            getHalfVertex(&v3, &v1, &v31);

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
        }
        this->vertexData.clear();
        for (auto &vertex : newVertices)
        {
            this->vertexData.push_back(vertex);
        }
        this->translate(tmp);
        this->rotateX(tmp2[0]);
        this->rotateY(tmp2[1]);
        this->rotateZ(tmp2[2]);
    }

    void getHalfVertex(std::array<GLfloat,3> * v1, std::array<GLfloat,3> * v2, std::array<GLfloat,3> * v12)
    {
        (*v12)[0] = ((*v1)[0] + (*v2)[0]) / 2;
        (*v12)[1] = ((*v1)[1] + (*v2)[1]) / 2;
        (*v12)[2] = ((*v1)[2] + (*v2)[2]) / 2;

        // scale it so that the vertex is on the radius of the icosahedron
        (*v12)[0] = (*v12)[0] * (1 / sqrt(pow((*v12)[0], 2) + pow((*v12)[1], 2) + pow((*v12)[2], 2)));
        (*v12)[1] = (*v12)[1] * (1 / sqrt(pow((*v12)[0], 2) + pow((*v12)[1], 2) + pow((*v12)[2], 2)));
        (*v12)[2] = (*v12)[2] * (1 / sqrt(pow((*v12)[0], 2) + pow((*v12)[1], 2) + pow((*v12)[2], 2)));
    }
};


#endif  // PA4_ICOSAHEDRON_H
