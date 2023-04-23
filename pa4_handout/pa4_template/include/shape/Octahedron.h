#ifndef PA4_OCTAHEDRON_H
#define PA4_OCTAHEDRON_H

#include "shape/Shape.h"


class Octahedron : public Shape
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
    Octahedron()
    {
        this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3); vertex.emplace_back(V5); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V2); vertex.emplace_back(V5);
        vertex.emplace_back(V2); vertex.emplace_back(V4); vertex.emplace_back(V5);
        vertex.emplace_back(V5); vertex.emplace_back(V4); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V1); vertex.emplace_back(V6);
        vertex.emplace_back(V3); vertex.emplace_back(V6); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V4); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V1); vertex.emplace_back(V4);

    }

    Octahedron(glm::vec3 center, GLfloat size) {
        this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3); vertex.emplace_back(V5); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V2); vertex.emplace_back(V5);
        vertex.emplace_back(V2); vertex.emplace_back(V4); vertex.emplace_back(V5);
        vertex.emplace_back(V5); vertex.emplace_back(V4); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V1); vertex.emplace_back(V6);
        vertex.emplace_back(V3); vertex.emplace_back(V6); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V4); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V1); vertex.emplace_back(V4);

        this->translate(center);
        this->scale(size - 1);
    }

    Octahedron(glm::vec3 center, GLfloat size, glm::vec3 color)
    {
        this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
        this->size = DEFAULT_SIZE;
        this->color = color;
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3); vertex.emplace_back(V5); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V2); vertex.emplace_back(V5);
        vertex.emplace_back(V2); vertex.emplace_back(V4); vertex.emplace_back(V5);
        vertex.emplace_back(V5); vertex.emplace_back(V4); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V1); vertex.emplace_back(V6);
        vertex.emplace_back(V3); vertex.emplace_back(V6); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V4); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V1); vertex.emplace_back(V4);

        this->translate(center);
        this->scale(size - 1);
    }

    // getters
    virtual GLint getNumVertices() const override { return NUM_VERTICES; }

    virtual const glm::vec3 * getVertexData() const override {
        glm::vec3 *vertexData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertexData[i] = glm::vec3(this->vertex[i][0],
                                        this->vertex[i][1],
                                        this->vertex[i][2]);
        }
        return vertexData;
    }

    virtual const glm::vec3 * getNormalData() const override {
        glm::vec3 *normalData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i += 3) {
            glm::vec3 v1 = glm::vec3(this->vertex[i][0],
                                        this->vertex[i][1],
                                        this->vertex[i][2]);
            glm::vec3 v2 = glm::vec3(this->vertex[i + 1][0],
                                        this->vertex[i + 1][1],
                                        this->vertex[i + 1][2]);
            glm::vec3 v3 = glm::vec3(this->vertex[i + 2][0],
                                        this->vertex[i + 2][1],
                                        this->vertex[i + 2][2]);
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            normalData[i] = normal;
        }
        return normalData;
    }

    glm::vec3 getCenter() const { return this->center; }
    GLfloat getSize() const { return this->size; }
    glm::vec3 getColor() const { return this->color; }
    glm::vec3 getOldColor() const { return this->oldColor; }

    // setters
    void setCenter(glm::vec3 center) { this->translate(center - this->center); }
    void setSize(GLfloat size) { this->scale(size); }
    void setColor(glm::vec3 color) { this->oldColor = this->color; this->color = color; }

    void translate(glm::vec3 translation) {
        this->center += translation;
        // center changed, so we need to update the vertex data
        updateOctaLocation(translation);
    }

    void scale(GLfloat scale) {
        this->size += scale;
        // size changed, so we need to update the vertex data
        updateOctaSize(scale);
    }

    void rotate(GLfloat angle, glm::vec3 axis) {
        updateOctaOrientation(angle, axis);
    }

    void reflect(glm::vec3 planeA, glm::vec3 planeB) {
        updateOctahedronReflection(planeA, planeB);
    }

    void shear(glm::vec3 axis, glm::vec3 amount)
    {
        updateOctahedronShear(axis, amount);
    }

    void render(GLuint octaArray, GLuint octaBuffer, GLuint shaderID) const {
        GLuint octaNormals;
        initializeRender(&octaArray, &octaBuffer, &octaNormals);

        // set the color
        GLint colorID = glGetUniformLocation(shaderID, "octaColor");
        glUniform3f(colorID, this->color[0], this->color[1], this->color[2]);

        // draw the octa
        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        // unbind the buffer
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint octaArray, GLuint octaBuffer, GLuint shaderID, uint options) const {
        GLuint octaNormals;
        initializeRender(&octaArray, &octaBuffer, &octaNormals);

        // set the color
        GLint colorID = glGetUniformLocation(shaderID, "octaColor");
        glUniform3f(colorID, this->color[0], this->color[1], this->color[2]);

        GLuint flatLocation = glGetUniformLocation(shaderID, "isFlat");
        if (options == Options::FLAT) {
            glUniform1i(flatLocation, 1);
        } else {
            glUniform1i(flatLocation, 0);
        }

        // draw the octa
        if (options & Options::WIREFRAME) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, NUM_VERTICES);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
        }

        // unbind the buffer
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    // 8 triangular facets
    static constexpr GLint NUM_FACETS = 8;
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    static constexpr glm::vec3 V1 { 1,  0,  0};
    static constexpr glm::vec3 V2 {-1, 0,  0};
    static constexpr glm::vec3 V3 {0,  1, 0};
    static constexpr glm::vec3 V4 { 0, -1, 0};
    static constexpr glm::vec3 V5 {0,  0, 1};
    static constexpr glm::vec3 V6 { 0, 0, -1};

    // vertices organized into triangular facets
    std::vector<glm::vec3> vertex;

    // the default octa is centered at the origin and has a side lengths of 1
    static constexpr GLfloat DEFAULT_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // the deffault color is white
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

    glm::vec3 center;
    GLfloat size;
    glm::vec3 color;
    glm::vec3 oldColor;

    void updateOctaLocation(glm::vec3 translation) {
        // update the vertex data
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i][0] += translation[0];
            vertex[i][1] += translation[1];
            vertex[i][2] += translation[2];
        }
    }

    void updateOctaSize(GLfloat scale) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        // update the vertex data
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i][0] += vertex[i][0] * scale;
            vertex[i][1] += vertex[i][1] * scale;
            vertex[i][2] += vertex[i][2] * scale;
        }
        this->translate(tmp);
    }

    void rotateX(GLfloat angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[1][1] = cos(angle_rad);
        rotationMatrix[1][2] = -sin(angle_rad);
        rotationMatrix[2][1] = sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertex4 = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[i][0] = rotatedVertex[0];
            vertex[i][1] = rotatedVertex[1];
            vertex[i][2] = rotatedVertex[2];
        }
    }

    void rotateY(GLfloat angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][2] = sin(angle_rad);
        rotationMatrix[2][0] = -sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertex4 = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[i][0] = rotatedVertex[0];
            vertex[i][1] = rotatedVertex[1];
            vertex[i][2] = rotatedVertex[2];
        }
    }

    void rotateZ(GLfloat angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][1] = -sin(angle_rad);
        rotationMatrix[1][0] = sin(angle_rad);
        rotationMatrix[1][1] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertex4 = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[i][0] = rotatedVertex[0];
            vertex[i][1] = rotatedVertex[1];
            vertex[i][2] = rotatedVertex[2];
        }
    }

    void updateOctaOrientation(GLfloat angle, glm::vec3 axis) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axis[0] != 0) { rotateX(angle); }
        if (axis[1] != 0) { rotateY(angle); }
        if (axis[2] != 0) { rotateZ(angle); }
        this->translate(tmp);
    }

    void updateOctahedronReflection(glm::vec3 planeA, glm::vec3 planeB)
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
            glm::vec4 vertices = glm::vec4(this->vertex[i][0], this->vertex[i][1], this->vertex[i][2], 1.0f);
            glm::vec4 reflectedVertices = R * vertices;
            this->vertex[i][0] = reflectedVertices[0];
            this->vertex[i][1] = reflectedVertices[1];
            this->vertex[i][2] = reflectedVertices[2];
        }
        this->translate(tmp);
    }

     void shearX(float shearAmountY, float shearAmountZ)
    {
        // get the shear matrix
        glm::mat4 shear = {
            1, shearAmountY, shearAmountZ, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        // multiply the shear matrix with each vertex
        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertexData = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertex[i][0] = vertexData.x;
            vertex[i][1] = vertexData.y;
            vertex[i][2] = vertexData.z;
        }
    }

    void shearY(float shearAmountX, float shearAmountZ)
    {
        // get the shear matrix
        glm::mat4 shear = {
            1, 0, 0, 0,
            shearAmountX, 1, shearAmountZ, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        // multiply the shear matrix with each vertex
        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertexData = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertex[i][0] = vertexData.x;
            vertex[i][1] = vertexData.y;
            vertex[i][2] = vertexData.z;
        }
    }

    void shearZ(float shearAmountX, float shearAmountY)
    {
        // get the shear matrix
        glm::mat4 shear = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            shearAmountX, shearAmountY, 1, 0,
            0, 0, 0, 1};

        // multiply the shear matrix with each vertex
        for (int i = 0; i < NUM_VERTICES; i++)
        {
            glm::vec4 vertexData = glm::vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertex[i][0] = vertexData.x;
            vertex[i][1] = vertexData.y;
            vertex[i][2] = vertexData.z;
        }
    }

    void updateOctahedronShear(glm::vec3 axis, glm::vec3 amount)
    {
        if (axis[0] != 0)
        {
            shearX(amount[1], amount[2]);
        }
        if (axis[1] != 0)
        {
            shearY(amount[0], amount[2]);
        }
        if (axis[2] != 0)
        {
            shearZ(amount[0], amount[1]);
        }
    }

    void initializeRender(GLuint * octaArray, GLuint * octaBuffer, GLuint * octaNormals) const {
        // create the vertex array object
        glGenVertexArrays(1, octaArray);
        glBindVertexArray(*octaArray);

        // create the vertex buffer object
        glGenBuffers(1, octaBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *octaBuffer);

        // get the vertex data
        const glm::vec3 *dataStart = getVertexData();
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), dataStart, GL_STATIC_DRAW);

        // set up the vertex attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // create the vertex buffer object
        glGenBuffers(1, octaNormals);
        glBindBuffer(GL_ARRAY_BUFFER, *octaNormals);

        // get the vertex data
        const glm::vec3 *dataStart2 = getNormalData();
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), dataStart2, GL_STATIC_DRAW);

        // set up the vertex attributes
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
};


#endif  // PA4_OCTAHEDRON_H
