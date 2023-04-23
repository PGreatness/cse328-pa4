#ifndef PA4_TORUS_H
#define PA4_TORUS_H

#include "Shape.h"

class Torus : public Shape
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
    Torus()
    {
        this->center = {DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z};
        this->radius = DEFAULT_SIZE;
        this->innerRadius = DEFAULT_SIZE / 2.0f;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;

        initShape();
    }

    Torus(glm::vec3 center, float radius)
    {
        this->center = {DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z};
        this->radius = radius;
        this->innerRadius = DEFAULT_SIZE / 2.0f;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;

        initShape();
        translate(center);
    }

    Torus(glm::vec3 center, float radius, float innerRadius)
    {
        this->center = {DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z};
        this->radius = radius;
        this->innerRadius = innerRadius;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;

        initShape();
        translate(center);
    }

    Torus(glm::vec3 center, float radius, float innerRadius, glm::vec3 color)
    {
        this->center = {DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z};
        this->radius = radius;
        this->innerRadius = innerRadius;
        this->color = color;
        this->oldColor = this->color;

        initShape();
        translate(center);
    }

    // getters
    virtual GLint getNumVertices() const override
    {
        return vertices.size();
    }

    virtual const glm::vec3 * getVertexData() const override
    {
        return vertices.data();
    }

    virtual const glm::vec3 * getNormalData() const override
    {
        glm::vec3 *normals = new glm::vec3[vertices.size()];
        for (int i = 0; i < vertices.size(); i += 3)
        {
            glm::vec3 v1 = glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]);
            glm::vec3 v2 = glm::vec3(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2]);
            glm::vec3 v3 = glm::vec3(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2]);
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            normals[i] = normal;
            normals[i + 1] = normal;
            normals[i + 2] = normal;
        }
        return normals;
    }

    glm::vec3 getCenter() const
    {
        return center;
    }

    GLfloat getSize() const
    {
        return radius;
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

    void setSize(float radius)
    {
        this->radius = radius;
        this->innerRadius = radius / 2.0f;
        initShape();
    }

    void setColor(glm::vec3 color)
    {
        this->color = color;
    }

    void setOldColor(glm::vec3 oldColor)
    {
        this->oldColor = oldColor;
    }

    // methods

    void translate(glm::vec3 translation)
    {
        center += translation;
        updateTorusLocation(translation);
    }

    void rotate(float angle, glm::vec3 rotation)
    {
        updateTorusOrientation(rotation, angle);
    }

    void scale(float scaleFactor)
    {
        radius += scaleFactor;
        innerRadius += scaleFactor;
        initShape();
    }

    void reflect(glm::vec3 planeA, glm::vec3 planeB)
    {
        updateTorusReflection(planeA, planeB);
    }

    void shear(glm::vec3 axis, glm::vec3 shearFactor)
    {
        updateTorusShear(axis, shearFactor);
    }

    void render(GLuint VAO, GLuint VBO, GLuint shaderID) const
    {
        render(VAO, VBO, shaderID, Options::DEFAULT);
    }

    void render(GLuint VAO, GLuint VBO, GLuint shaderID, uint options) const
    {
        GLuint torusNormals;
        initializeRender(&VAO, &VBO, &torusNormals);

        // set color
        GLint uniColor = glGetUniformLocation(shaderID, "torusColor");
        glUniform3f(uniColor, color.x, color.y, color.z);

        // draw
        if (options == Options::WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, getNumVertices());
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, getNumVertices());
        }

        // unbind VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    // fields
    static constexpr float PI = 3.14159265358979323846;
    std::vector<glm::vec3> vertices;

    static constexpr int INIT_NUM_FACETS = 15;
    static constexpr int INIT_NUM_VERTICES = INIT_NUM_FACETS * 3;

    // defaults
    static constexpr GLfloat DEFAULT_BASE_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_BASE_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_BASE_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // default color
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

    glm::vec3 center;
    GLfloat radius;
    GLfloat innerRadius;
    glm::vec3 color;
    glm::vec3 oldColor;

    void initShape()
    {
        std::vector<glm::vec3> tmp;
        for (int i = 1; i < INIT_NUM_VERTICES; i++)
        {
            for (int j = 1; j < INIT_NUM_VERTICES; j++)
            {
                auto u = j / (float)INIT_NUM_VERTICES * 2 * PI;
                auto v = i / (float)INIT_NUM_VERTICES * 2 * PI;
                auto x = (radius + innerRadius * cos(v)) * cos(u);
                auto y = (radius + innerRadius * cos(v)) * sin(u);
                auto z = innerRadius * sin(v);
                tmp.push_back(glm::vec3(x, y, z));
            }
        }

        // create triangles from vertices
        for (int i = 0; i < INIT_NUM_VERTICES; i++)
        {
            for (int j = 0; j < INIT_NUM_VERTICES; j++)
            {
                vertices.push_back(tmp[i * INIT_NUM_VERTICES + j]);
                vertices.push_back(tmp[i * INIT_NUM_VERTICES + (j + 1)]);
                vertices.push_back(tmp[(i + 1) * INIT_NUM_VERTICES + j]);

                vertices.push_back(tmp[i * INIT_NUM_VERTICES + (j + 1)]);
                vertices.push_back(tmp[(i + 1) * INIT_NUM_VERTICES + (j + 1)]);
                vertices.push_back(tmp[(i + 1) * INIT_NUM_VERTICES + j]);
            }
        }
    }

    void updateTorusLocation(glm::vec3 translation)
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i][0] += translation[0];
            vertices[i][1] += translation[1];
            vertices[i][2] += translation[2];
        }
    }

    void updateTorusSize(float scale)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i][0] += vertices[i][0] * scale;
            vertices[i][1] += vertices[i][1] * scale;
            vertices[i][2] += vertices[i][2] * scale;
        }
        this->translate(tmp);
    }

    void rotateX(float angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[1][1] = cos(angle_rad);
        rotationMatrix[1][2] = -sin(angle_rad);
        rotationMatrix[2][1] = sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i], 1.0f);
            vertices[i] = glm::vec3(rotatedVertex);
        }
    }

    void rotateY(float angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][2] = sin(angle_rad);
        rotationMatrix[2][0] = -sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i], 1.0f);
            vertices[i] = glm::vec3(rotatedVertex);
        }
    }

    void rotateZ(float angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][1] = -sin(angle_rad);
        rotationMatrix[1][0] = sin(angle_rad);
        rotationMatrix[1][1] = cos(angle_rad);

        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i], 1.0f);
            vertices[i] = glm::vec3(rotatedVertex);
        }
    }

    void updateTorusOrientation(glm::vec3 axis, float angle)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axis[0] != 0) { this->rotateX(angle); }
        if (axis[1] != 0) { this->rotateY(angle); }
        if (axis[2] != 0) { this->rotateZ(angle); }
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
        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 vertexData = glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertices[i][0] = vertexData.x;
            vertices[i][1] = vertexData.y;
            vertices[i][2] = vertexData.z;
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
        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 vertexData = glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertices[i][0] = vertexData.x;
            vertices[i][1] = vertexData.y;
            vertices[i][2] = vertexData.z;
        }
    }

    void shearZ(float shearAmountX, float shearAmountY)
    {
        // get the shear matrix
        glm::mat4 shear = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            shearAmountX, shearAmountY, 1, 0,
            0, 0, 0, 1
        };

        // multiply the shear matrix with each vertex
        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 vertexData = glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f);
            vertexData = shear * vertexData;
            vertices[i][0] = vertexData.x;
            vertices[i][1] = vertexData.y;
            vertices[i][2] = vertexData.z;
        }
    }

    void updateTorusShear(glm::vec3 axis, glm::vec3 shearAmount)
    {
        if (axis[0] != 0) { this->shearX(shearAmount[1], shearAmount[2]); }
        if (axis[1] != 0) { this->shearY(shearAmount[0], shearAmount[2]); }
        if (axis[2] != 0) { this->shearZ(shearAmount[0], shearAmount[1]); }
    }

    void updateTorusReflection(glm::vec3 planeA, glm::vec3 planeB)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);

        glm::vec3 planeNormal = glm::normalize(glm::cross(planeA, planeB));

        // get the reflection matrix
        glm::mat4 P = glm::outerProduct(glm::vec4(planeNormal, 0.0f), glm::mat4(planeNormal, 0.0f));
        glm::mat4 R = glm::mat4(1.0f) - 2.0f * P;

        // multiply the reflection matrix with each vertex
        for (int i = 0; i < this->getNumVertices(); i++) {
            glm::vec4 vertexData = glm::vec4(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f);
            vertexData = R * vertexData;
            vertices[i][0] = vertexData.x;
            vertices[i][1] = vertexData.y;
            vertices[i][2] = vertexData.z;
        }
        this->translate(tmp);
    }

    void initializeRender(GLuint * torusArray, GLuint * torusBuffer, GLuint * torusNormals) const
    {
        // create VAO
        glGenVertexArrays(1, torusArray);

        // create VBO
        glGenBuffers(1, torusBuffer);

        // bind VAO
        glBindVertexArray(*torusArray);

        // bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, *torusBuffer);

        // copy vertex data to VBO
        const glm::vec3 * vertexData = getVertexData();
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertexData, GL_STATIC_DRAW);

        // set vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

        // do the normals
        glGenBuffers(1, torusNormals);
        glBindBuffer(GL_ARRAY_BUFFER, *torusNormals);
        const glm::vec3 * normalData = getNormalData();
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), normalData, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    }
};

#endif // PA4_TORUS_H