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
        this->center = center;
        this->radius = radius;
        this->innerRadius = radius / 2.0f;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;

        initShape();
    }

    Torus(glm::vec3 center, float radius, float innerRadius)
    {
        this->center = center;
        this->radius = radius;
        this->innerRadius = innerRadius;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;

        initShape();
    }

    Torus(glm::vec3 center, float radius, float innerRadius, glm::vec3 color)
    {
        this->center = center;
        this->radius = radius;
        this->innerRadius = innerRadius;
        this->color = color;
        this->oldColor = this->color;

        initShape();
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
        this->center = center;
    }

    void setSize(float radius)
    {
        this->radius = radius;
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
        // Generate vertices on the torus
        for (int j = 0; j < INIT_NUM_VERTICES * 2; j++)
        {
            double phi = 2 * PI * j / INIT_NUM_VERTICES;
            double cosphi = cos(phi);
            double sinphi = sin(phi);
            for (int i = 0; i < INIT_NUM_VERTICES * 2; i++)
            {
                double theta = 2 * PI * i / INIT_NUM_VERTICES;
                double costheta = cos(theta);
                double sintheta = sin(theta);
                double x = (radius + innerRadius * cosphi) * costheta;
                double y = (radius + innerRadius * cosphi) * sintheta;
                double z = innerRadius * sinphi;
                vertices.push_back(glm::vec3(x, y, z));
            }
        }

        // Generate vertices on the inner tube
        for (int j = 0; j < INIT_NUM_VERTICES * 2; j++)
        {
            double phi = 2 * PI * j / INIT_NUM_VERTICES;
            double cosphi = cos(phi);
            double sinphi = sin(phi);
            for (int i = 0; i < INIT_NUM_VERTICES * 2; i++)
            {
                double theta = 2 * PI * i / INIT_NUM_VERTICES;
                double costheta = cos(theta);
                double sintheta = sin(theta);
                double x = (radius + innerRadius * cosphi * cos(theta)) * costheta;
                double y = (radius + innerRadius * cosphi * cos(theta)) * sintheta;
                double z = innerRadius * sinphi * sin(theta);
                vertices.push_back(glm::vec3(x, y, z));
            }
        }
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