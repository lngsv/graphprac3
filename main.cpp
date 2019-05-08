#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
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

extern pair<vector<Vertex>, vector<GLushort>> ReadMesh(const string &path);


int main()
{
    srand(time(nullptr));

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
    vector<GLushort> emptyInds = {};

    // Loading mesh from file
    //FILE *f = fopen


    // triangle
    vertices = {
        {{-1.0, -sqrt(3.0)/3.0, 0.0 }, {}},
        {{ 1.0, -sqrt(3.0)/3.0, 0.0 }, {}},
        {{ 0.0, 2.0 * sqrt(3.0)/3.0, 0.0 }, {}}, 
    };
    Mesh meshTriangle(vertices, emptyInds);

    vertices = {
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}, 
        {{ -1.0, 1.0, 0.0 }, {0.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, 1.0, 0.0 }, {1.0, 1.0}}, 
        {{ 1.0, -1.0, 0.0 }, {1.0, 0.0 }}, 
        {{ -1.0, -1.0, 0.0 }, {0.0, 0.0 }}
    };
    Mesh meshSquare(vertices, emptyInds);

    vertices = {
        {{ -1.0, -1.0, -1.0 },  {0.0, 0.0 }},
        {{1.0, -1.0, -1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{-1.0,  1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 0.0 }},

        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 1.0 }},
        {{-1.0,  1.0,  1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},

        {{-1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0,  1.0,  1.0 },  {1.0, 0.0 }},

        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},

        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0, -1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0, -1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0, -1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0, -1.0, -1.0 },  {0.0, 1.0 }},

        {{-1.0,  1.0, -1.0 },  {0.0, 1.0 }},
        {{1.0,  1.0, -1.0 },  {1.0, 1.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{1.0,  1.0,  1.0 },  {1.0, 0.0 }},
        {{-1.0,  1.0,  1.0 },  {0.0, 0.0 }},
        {{-1.0,  1.0, -1.0 },  {0.0, 1.0}}
    };
    Mesh meshCube(vertices, emptyInds);

    Shader shaderBasic("shaders/basic.vert", "shaders/basic.frag");
    
    vec3 trPos = {0.0, 0.0, 0.0};
    vec3 trRotAxis = normalize(vec3{1.0, 0.3, 1.5});
    float trRotAngle = 0.0; // in radians

    vec3 sqPos = {-1.5, 0.0, 0.0};
    vec3 sqRotAxis = normalize(vec3{3.3, 1.2, 0.1});
    float sqRotAngle = 0.0; // in radians

    vec3 cubePos = {7.0, 1.0, 2.0};
    vec3 cubeRotAxis = normalize(vec3{1.0, 1.0, -1.0});
    float cubeRotAngle = 0.0; // in radians

    Shader shaderTextured("shaders/textured.vert", "shaders/textured.frag");
    shaderTextured.SetUniform("tex", 0);
    Texture textureMetal("textures/metal.jpg");
    Texture textureCmc("textures/cmc.png");

    Shader shaderBg("shaders/background.vert", "shaders/textured.frag");
    shaderBg.SetUniform("tex", 0);
    Texture textureBg("textures/bg.jpg");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    

    // main loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // Update
        glfwPollEvents();
        prevTime = curTime;
        curTime = glfwGetTime();
        deltaTime = curTime - prevTime;

        trPos.x = sin(curTime);
        trRotAngle += deltaTime * 2.0;

        sqPos.y = sin(curTime);
        sqRotAngle += deltaTime * 2.0;

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
        PV = translate(PV, vec3(0.0, 0.0, -15.0));
        // Model
        
        // background
        shaderBg.Use();
        textureBg.Bind();
        meshSquare.Draw();
    

        // triangle
        shaderBasic.SetUniform("myColor", vec4(0.3, 1.0, 0.1, 0.5));
        mat4 transMat = translate(PV, trPos);
        transMat = rotate(transMat, trRotAngle, trRotAxis);
        shaderBasic.SetUniform("transformMat", transMat);

        shaderBasic.Use();
        meshTriangle.Draw();

        // square
        transMat = translate(PV, sqPos);
        transMat = rotate(transMat, sqRotAngle, sqRotAxis);
        shaderTextured.SetUniform("transformMat", transMat);

        shaderTextured.Use();
        textureMetal.Bind();
        meshSquare.Draw();
        
        // cube
        //shaderBasic.SetUniform("myColor", vec4(1.0, 1.0, 1.0, 1.0));
        transMat = translate(PV, cubePos);
        transMat = rotate(transMat, cubeRotAngle, cubeRotAxis);
        shaderTextured.SetUniform("transformMat", transMat);

        shaderTextured.Use();
        textureCmc.Bind();
        meshCube.Draw();


        glfwSwapBuffers(window);


    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
