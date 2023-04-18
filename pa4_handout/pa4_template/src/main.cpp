#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shape/Cube.h"
#include "shape/Dodecahedron.h"
#include "shape/Icosahedron.h"
#include "shape/Tetrahedron.h"
#include "shape/Octahedron.h"
#include "util/Camera.h"
#include "util/Shader.h"


void cursorPosCallback(GLFWwindow * window, double xpos, double ypos);

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);

void perFrameKeyInput(GLFWwindow * window);

void perFrameTimeLogic();

void initializeContext();

void initializeCubes();

namespace Colors
{
    const glm::vec3 WIREFRAME = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 FLAT = glm::vec3(1.0f, 0.5f, 0.31f);
    const glm::vec3 SMOOTH = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 currentColor = WIREFRAME;
} // namespace Colors
namespace Context
{

// screen size
constexpr unsigned int kWindowWidth = 1024U;
constexpr unsigned int kWindowHeight = 1024U;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// lighting
glm::vec3 lightPos(10.0f, -10.0f, 10.0f);

// mouse
bool mousePressed = false;
bool firstMouse = true;
float lastX = kWindowWidth / 2.0f;
float lastY = kWindowHeight / 2.0f;

// timing
float deltaTime = 0.0f;     // time between current frame and last frame
float lastFrame = 0.0f;

std::shared_ptr<Shader> axisShader;       // shader for x, y, z axis

// TODO: Add other context configurations
std::shared_ptr<Shader> cubeShader;       // shader for cubes
Cube cube(glm::vec3(0.0f,0.0f,0.0f), 1.0f, Colors::currentColor); // default cube object

std::shared_ptr<Shader> tetrahedronShader;       // shader for tetrahedron
Tetrahedron tetrahedron(glm::vec3(0.0f,0.0f,0.0f), 1.0f, Colors::currentColor); // default tetrahedron object
struct cubeOptions
{
    static const uint DEFAULT = 0;
    static const uint SOLID = 0;
    static const uint WIREFRAME = 1;
    static const uint FLAT = 2;
    static const uint SMOOTH = 4;
}; // cube options

uint options = 0;

void setOptions(uint newOptions)
{
    options = newOptions;
}

}  // namespace Context


namespace Primitive
{

GLuint axisVertexArray;
GLuint axisVertexBuffer;

// TODO: Add other OpenGL objects for other primitives
GLuint cubeVertexArray;
GLuint cubeVertexBuffer;

GLuint tetrahedronVertexArray;
GLuint tetrahedronVertexBuffer;

}  // namespace Primitive


namespace Context
{

// display function for x, y, z axis
void displayAxis()
{
    Context::axisShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::axisShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::axisShader->setMat4("view", view);
    Context::axisShader->setMat4("model", glm::mat4(1.0f));

    glBindVertexArray(Primitive::axisVertexArray);
    glDrawArrays(GL_LINES, 0, 6);  // 3 axis to show (x, y, z), each has 2 vertices
    glBindVertexArray(0);
}

void displayCube()
{
    Context::cubeShader->use();

    // set lighting uniforms
    Context::cubeShader->setVec3("lightPos", Context::lightPos);
    Context::cubeShader->setVec3("viewPos", Context::camera.position);
    Context::cubeShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::cubeShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::cubeShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::cubeShader->setMat4("view", view);
    Context::cubeShader->setMat4("model", glm::mat4(1.0f));

    cube.render(Primitive::cubeVertexArray,
                Primitive::cubeVertexBuffer,
                Context::cubeShader->getShaderProgramHandle(),
                options);
}

void displayTetrahedron()
{
    Context::tetrahedronShader->use();

    // set lighting uniforms
    Context::tetrahedronShader->setVec3("lightPos", Context::lightPos);
    Context::tetrahedronShader->setVec3("viewPos", Context::camera.position);
    Context::tetrahedronShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::tetrahedronShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::tetrahedronShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::tetrahedronShader->setMat4("view", view);
    Context::tetrahedronShader->setMat4("model", glm::mat4(1.0f));

    tetrahedron.render(Primitive::tetrahedronVertexArray,
                        Primitive::tetrahedronVertexBuffer,
                        Context::tetrahedronShader->getShaderProgramHandle(),
                        options);
}

// TODO: Add display functions for other primitives

}  // namespace Context


int main()
{
    // initialize OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Special care will be need to be taken for normal vectors if you want to support viewport scaling;
    // just forbid it for convenience.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(Context::kWindowWidth,
                                           Context::kWindowHeight,
                                           "PA4",
                                           nullptr,
                                           nullptr);

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        throw std::runtime_error("failed to initialize GLAD");
    }

    // initial the global program context
    initializeContext();

    // TODO: Add more contents to fit your own ideas

    Context::cubeShader = std::make_shared<Shader>("src/shader/Cube/vert.glsl",
                                                    "src/shader/Cube/frag.glsl");

    Context::tetrahedronShader = std::make_shared<Shader>("src/shader/Tetrahedron/vert.glsl",
                                                            "src/shader/Tetrahedron/frag.glsl");

    // render loop
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, Context::kWindowWidth, Context::kWindowHeight);

    while (!glfwWindowShouldClose(window))
    {
        perFrameKeyInput(window);
        perFrameTimeLogic();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: Display or hide axis based on keyboard functions
        Context::displayAxis();

        // TODO: Render

        Context::displayCube();

        // Context::displayTetrahedron();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    // TODO: Modify this function to fit your own ideas
    
    if (Context::firstMouse)
    {
        Context::lastX = static_cast<float>(xpos);
        Context::lastY = static_cast<float>(ypos);
        Context::firstMouse = false;
    }

    // reversed since y-coordinates go from bottom to top
    auto xoffset = static_cast<float>(xpos - Context::lastX);
    auto yoffset = static_cast<float>(Context::lastY - ypos);

    Context::lastX = static_cast<float>(xpos);
    Context::lastY = static_cast<float>(ypos);

    if (Context::mousePressed)
    {
        Context::camera.processMouseMovement(xoffset, yoffset);
    }
}


void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    // TODO: Modify this function to fit your own ideas
    // check if key pressed is 1
    Context::cube.setColor(Colors::currentColor);
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        Context::cube.setColor(Colors::WIREFRAME);
        Colors::currentColor = Colors::WIREFRAME;
        Context::setOptions(Context::cubeOptions::WIREFRAME);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        Colors::currentColor = Colors::FLAT;
        Context::setOptions(Context::cubeOptions::FLAT);
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        Colors::currentColor = Colors::SMOOTH;
        Context::setOptions(Context::cubeOptions::SMOOTH);
    }
}


void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Context::mousePressed = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        Context::mousePressed = false;
    }
}


void scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    Context::camera.processMouseScroll(static_cast<float>(yoffset));
}


void perFrameKeyInput(GLFWwindow * window)
{
    // camera control
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kLeft, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kRight, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kBackWard, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kForward, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kUp, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kDown, Context::deltaTime);
    }
}


void perFrameTimeLogic()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    Context::deltaTime = currentFrame - Context::lastFrame;
    Context::lastFrame = currentFrame;
}


void initializeContext()
{
    // TODO: Modify this function to fit your own ideas
    
    Context::axisShader = std::make_shared<Shader>("src/shader/axis.vert.glsl",
                                                   "src/shader/axis.frag.glsl");

    // initialize axis
    glGenVertexArrays(1, &Primitive::axisVertexArray);
    glBindVertexArray(Primitive::axisVertexArray);
    std::vector<glm::vec3> axisVertexData
            {
                    // x+ axis in red
                    {0, 0, 0}, {1, 0, 0},
                    {3, 0, 0}, {1, 0, 0},
                    // y+ axis in green
                    {0, 0, 0}, {0, 1, 0},
                    {0, 3, 0}, {0, 1, 0},
                    // z+ axis in green
                    {0, 0, 0}, {0, 0, 1},
                    {0, 0, 3}, {0, 0, 1},
            };

    glGenBuffers(1, &Primitive::axisVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Primitive::axisVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(axisVertexData.size() * sizeof(glm::vec3)),
                 axisVertexData.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(sizeof(glm::vec3)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
