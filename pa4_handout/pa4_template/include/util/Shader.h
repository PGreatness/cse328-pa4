#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


class Shader
{
public:
    Shader(const char * vertShaderPath, const char * fragShaderPath)
    {
        // 1. retrieve the vertexShader/fragmentShader source code from filePath

        std::string vertShaderCode;
        std::string fragShaderCode;

        if (std::ifstream fin {vertShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            vertShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("vertex shader file not successfully read");
        }

        if (std::ifstream fin {fragShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            fragShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("fragment shader file not successfully read");
        }

        const char * vertShaderPtr = vertShaderCode.data();
        const char * fragShaderPtr = fragShaderCode.data();

        // 2. compile Shader

        // vertexShader shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertShaderPtr, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // fragmentShader shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragShaderPtr, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        // shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        checkCompileErrors(shaderProgram, "PROGRAM");

        // delete the Shader as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader(const char * vertShaderPath, const char * ctrlShaderPath, const char * evalShaderPath, const char * fragShaderPath)
    {
        // 1. retrieve the vertShader/fragShader source code from filePath

        std::string vertShaderCode;
        std::string ctrlShaderCode;
        std::string evalShaderCode;
        std::string fragShaderCode;

        if (std::ifstream fin {vertShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            vertShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("vertex shader file not successfully read");
        }

        if (std::ifstream fin {ctrlShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            ctrlShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("tessellation control shader file not successfully read");
        }

        if (std::ifstream fin {evalShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            evalShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("tessellation evaluation shader file not successfully read");
        }

        if (std::ifstream fin {fragShaderPath, std::ifstream::in})
        {
            std::ostringstream sout;
            sout << fin.rdbuf();
            fragShaderCode = sout.str();
        }
        else
        {
            glfwTerminate();
            throw std::runtime_error("fragment shader file not successfully read");
        }

        const char * vertShaderPtr = vertShaderCode.data();
        const char * ctrlShaderPtr = ctrlShaderCode.data();
        const char * evalShaderPtr = evalShaderCode.data();
        const char * fragShaderPtr = fragShaderCode.data();

        // 2. compile Shader

        // vertShader shader
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertShaderPtr, nullptr);
        glCompileShader(vertShader);
        checkCompileErrors(vertShader, "VERTEX");

        // tessellation control shader
        GLuint ctrlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(ctrlShader, 1, &ctrlShaderPtr, nullptr);
        glCompileShader(ctrlShader);
        checkCompileErrors(ctrlShader, "TESSELLATION CONTROL");

        // tessellation evaluation shader
        GLuint evalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(evalShader, 1, &evalShaderPtr, nullptr);
        glCompileShader(evalShader);
        checkCompileErrors(evalShader, "TESSELLATION EVALUATION");

        // fragShader shader
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragShaderPtr, nullptr);
        glCompileShader(fragShader);
        checkCompileErrors(fragShader, "FRAGMENT");

        // shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, ctrlShader);
        glAttachShader(shaderProgram, evalShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
        checkCompileErrors(shaderProgram, "PROGRAM");

        // delete the Shader as they're linked into our program now and no longer necessary
        glDeleteShader(vertShader);
        glDeleteShader(ctrlShader);
        glDeleteShader(evalShader);
        glDeleteShader(fragShader);
    }

    void use() const
    {
        glUseProgram(shaderProgram);
    }

    void setBool(const std::string & name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), static_cast<GLint>(value));
    }

    void setInt(const std::string & name, GLint value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void setFloat(const std::string & name, GLfloat value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void setVec2(const std::string & name, const glm::vec2 & value) const
    {
        glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string & name, GLfloat x, GLfloat y) const
    {
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
    }

    void setVec3(const std::string & name, const glm::vec3 & value) const
    {
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string & name, GLfloat x, GLfloat y, GLfloat z) const
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
    }

    void setVec4(const std::string & name, const glm::vec4 & value) const
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string & name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string & name, const glm::mat2 & mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat2x3(const std::string & name, const glm::mat2x3 & mat) const
    {
        glUniformMatrix2x3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string & name, const glm::mat3 & mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string & name, const glm::mat4 & mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    [[nodiscard]] GLuint getShaderProgramHandle() const
    {
        return shaderProgram;
    }

private:
    // utility function for checking shader compilation/linking errors.
    static void checkCompileErrors(GLuint shader, const std::string & type)
    {
        GLint success;
        char infoLog[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

                std::ostringstream errorInfo;
                errorInfo << type << " shader compilation failed: " << infoLog;

                glfwTerminate();
                throw std::runtime_error(errorInfo.str());
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);

                std::ostringstream errorInfo;
                errorInfo << type << "shader program linking failed" << infoLog;

                glfwTerminate();
                throw std::runtime_error(errorInfo.str());
            }
        }
    }

private:
    GLuint shaderProgram;
};


#endif  // LEARNOPENGL_SHADER_H
