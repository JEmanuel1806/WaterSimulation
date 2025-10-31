#pragma once

#include "Camera.h"
#include "Shader.h"
#include "MeshGrid.h"

#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(Camera* cam);
    ~Renderer();
    void Start();
    void Run();
    void setupBuffers(MeshGrid& meshGrid);

    // uniforms for GUI

    float u_amplitude = 0.2f;
    float u_frequency = 0.3f;
    float u_speed = 2.0f;

    glm::vec3 u_lightPos = glm::vec3(2.0f, 5.0f, 3.0f);
    glm::vec3 u_lightColor = glm::vec3(1.0f);
    glm::vec3 u_objectColor = glm::vec3(0.0f, 0.3f, 0.8f);

private:
    
    Camera* m_camera = nullptr;
    
    Shader* m_waterShader = nullptr;
    MeshGrid m_waterPlane;

    int m_vertexCount = 0;
    int m_indexCount = 0;

    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    GLsizei m_IndexCount = 0;

};
