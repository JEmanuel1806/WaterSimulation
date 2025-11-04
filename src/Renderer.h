#pragma once

#include "Camera.h"
#include "Shader.h"
#include "MeshGrid.h"
#include "SkyBox.h"

#include <GLFW/glfw3.h>
#include <memory>

class Renderer {
public:
    Renderer(Camera *cam);
    void Start();
    void Run();
    void setupBuffers(MeshGrid& meshGrid);
    void setupSkyBuffers(SkyBox& skyBox);

    // uniforms for GUI

    float u_amplitude = 2.0f;
    float u_frequency = 0.12f;
    float u_speed = 1.0f;
    float u_reflection = 1.0f;

    glm::vec3 u_lightPos = glm::vec3(52.0f, 55.0f, 53.0f);
    glm::vec3 u_lightColor = glm::vec3(1.0f);
    glm::vec3 u_objectColor = glm::vec3(0.17f, 0.3f, 0.5f);

private:
    
    Camera* m_camera = nullptr;
    
    std::unique_ptr<Shader> m_skyShader;
    std::unique_ptr<Shader> m_waterShader;
 
    SkyBox m_skyBox;
    MeshGrid m_waterPlane;

    int m_skyBoxID = 0;
    int m_vertexCount = 0;
    int m_indexCount = 0;

    GLuint m_vaoWater = 0;
    GLuint m_vboWater = 0;
    GLuint m_eboWater = 0;
    GLsizei m_IndexCount = 0;

    GLuint m_vaoSky = 0;
    GLuint m_vboSky = 0;
    GLuint m_eboSky = 0;

};
