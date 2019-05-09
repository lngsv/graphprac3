#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace glm;

#pragma GCC diagnostic ignored "-Wunused-parameter"
void debugCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        cerr << "OpenGL debug. Message: " << endl <<
            string(message, length) << endl;
    }
}
#pragma GCC diagnostic pop

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    int screenWidth = 1900, screenHeight = 1210;
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Screensaver", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        cerr << "Error!" << endl;
    }

    const char *ver = (const char *) glGetString(GL_VERSION);
    cout << ver << endl;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // Set the debug callback function
    // TODO: DISABLE THIS FOR RELEASE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);

    double prevTime;
    double curTime = prevTime = glfwGetTime();
    double deltaTime = 0.0;

    // Init meshes
    vector<Vertex> vertices;
    vector<VertexN> verticesN;

    vertices = {
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}, 
        {{ -1.0, 1.0, 0.0 }, {0.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, -1.0, 0.0 }, {1.0, 0.0 }}, 
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}
    };
    Mesh meshSquare(vertices);

    verticesN = {
        {{ 0.0, -1.0, 2.0 }, {}, { 0.0, -1.0, 0.0 }}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}, {0.0, -1.0, 0.0}}, 
        {{ sqrt(3), -1.0, -1.0 }, {},  {0.0, -1.0, 0.0}},

        {{ 0.0, 2.0, 0.0 }, {}, { 0.0, 1.0, -3.0 }}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}, {0.0, 1.0, -3.0}}, 
        {{ sqrt(3), -1.0, -1.0 }, {}, {0.0, 1.0, -3.0}},

        {{ 0.0, 2.0, 0.0 }, {}, { 3*sqrt(3), 2.0, 3.0 }}, 
        {{ 0.0, -1.0, 2.0 }, {}, {3*sqrt(3), 2.0, 3.0}}, 
        {{ sqrt(3), -1.0, -1.0 }, {}, {3*sqrt(3), 2.0, 3.0}},

        {{ 0.0, 2.0, 0.0 }, {}, { -3*sqrt(3), 2.0, 3.0 }}, 
        {{ 0.0, -1.0, 2.0 }, {}, { -3*sqrt(3), 2.0, 3.0}}, 
        {{ -sqrt(3), -1.0, -1.0 }, {}, { -3*sqrt(3), 2.0, 3.0}},
    };
    Mesh meshTetra(verticesN);

    fstream fs;
    fs.open("meshes/cube.txt");
    verticesN = {};
    for (int i = 0; i < 36; ++i) {
        float vx, vy, vz, tx, ty, nx, ny, nz;
        fs >> vx >> vy >> vz >> tx >> ty >> nx >> ny >> nz;
        verticesN.push_back({vec3(vx, vy, vz), vec2(tx, ty), vec3(nx, ny, nz)});
    }
    Mesh meshCube(verticesN);

    vec3 tetraPos = {-4.0, -2.0, 2.0};
    vec3 tetraRotAxis = normalize(vec3{-3.3, 1.2, -0.1});
    float tetraRotAngle = 0.0; // in radians

    vec3 cubePos = {7.0, 1.0, 2.0};
    vec3 cubeRotAxis = normalize(vec3{1.0, 1.0, -1.0});
    float cubeRotAngle = 0.0; // in radians

    Shader shaderTextured("shaders/textured.vert", "shaders/textured.frag");
    shaderTextured.SetUniform("tex", 0);
    Texture textureMetal("textures/metal.jpg");

    Shader shaderBg("shaders/background.vert", "shaders/textured.frag");
    shaderBg.SetUniform("tex", 0);
    Texture textureBg("textures/bg.jpg");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    vec3 viewPos = {0.0, 0.0, 15.0};
    vec3 lightPos = {1.0, 5.0, 5.0};

    Shader shaderLightened("shaders/lighting.vert", "shaders/lighting.frag");
    shaderLightened.SetUniform("lightPos", lightPos);
    shaderLightened.SetUniform("lightColor", vec3(1.0, 1.0, 1.0));
    shaderLightened.SetUniform("viewPos", viewPos);
    shaderLightened.SetUniform("texture0", 0);

    // triangle 
    verticesN = {
        {{-2.0, -2*sqrt(3.0)/3.0, 0.0}, {0.0, 0.0}, {0.0, 0.0, 1.0}},
        {{ 2.0, -2*sqrt(3.0)/3.0, 0.0}, {1.0, 0.0}, {0.0, 0.0, 1.0}},
        {{ 0.0, 4*sqrt(3.0)/3.0, 0.0}, {0.5, sqrt(3.0)/4.0}, {0.0, 0.0, 1.0}}
    };
    Mesh meshTriangleN(verticesN);

    vec3 trPos(1.0, 3.0, 0.0);


    // main loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // Update
        glfwPollEvents();
        prevTime = curTime;
        curTime = glfwGetTime();
        deltaTime = curTime - prevTime;

        tetraPos.y = sin(curTime);
        tetraRotAngle += deltaTime * 2.0;

        cubePos.x = 5.0 + cos(curTime);
        cubePos.y = 0.0 + sin(curTime);
        cubeRotAngle += deltaTime * 4.0;

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // P * V * M
        // Projection
        mat4 PV = perspective((float)radians(45.0),
                (float)(screenWidth) / screenHeight,
                0.1f, 100.0f);
        // View
        PV = translate(PV, -viewPos);
        // Model
        
        // background
        shaderBg.Use();
        textureBg.Bind();
        meshSquare.Draw();

        // tetra
        shaderLightened.SetUniform("myColor", vec3(fabs(sin(curTime)), 0.6, 0.6));
        mat4 transMat = translate(mat4(1.0), tetraPos + (float)(sin(curTime) * 2.0) * tetraRotAxis);
        transMat = rotate(transMat, tetraRotAngle, tetraRotAxis);
        shaderLightened.SetUniform("modelTransform", transMat);
        shaderLightened.SetUniform("fullTransform", PV * transMat);
        shaderLightened.Use();
        meshTetra.Draw();
        
        // cube
        shaderLightened.SetUniform("myColor", vec3(0.3, 0.6, 0.3));
        transMat = translate(mat4(1.0), cubePos + (float)(sin(curTime) * 3.2) * cubeRotAxis);
        transMat = rotate(transMat, cubeRotAngle, cubeRotAxis);
        shaderLightened.SetUniform("modelTransform", transMat);
        shaderLightened.SetUniform("fullTransform", PV * transMat);
        shaderLightened.Use();
        meshCube.Draw();


        // triangle 
        //transMat = translate(mat4(1.0), trPos);
        transMat = translate(mat4(1.0), trPos + 
                (float)(sin(curTime) * 3.0) * vec3(-1.0, 0.0, 0.0));
        shaderLightened.SetUniform("modelTransform", transMat);
        shaderLightened.SetUniform("fullTransform", PV * transMat);
        shaderLightened.SetUniform("myColor", vec3(0.0));
        shaderLightened.Use();
        textureMetal.Bind();
        meshTriangleN.Draw();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
