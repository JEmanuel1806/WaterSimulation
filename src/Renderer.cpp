#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Camera* cam)
    : m_waterPlane(500) 
{
    m_camera = cam;
}

void Renderer::Start() {
    m_skyShader = std::make_unique<Shader>("src/shader/render_sky.vert", "src/shader/render_sky.frag");
    m_waterShader = std::make_unique<Shader>("src/shader/render_water.vert","src/shader/render_water.frag");
    
    setupBuffers(m_waterPlane);
    setupSkyBuffers(m_skyBox);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // skybox texture
    std::vector<std::string> faces = {
    "res/textures/skybox/left.jpg",
    "res/textures/skybox/right.jpg",
    "res/textures/skybox/top.jpg",
    "res/textures/skybox/bottom.jpg",
    "res/textures/skybox/front.jpg",
    "res/textures/skybox/back.jpg"
    };

    m_skyBoxID = m_skyBox.generateCubeMap(faces);
}

void Renderer::Run()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewSky = glm::mat4(glm::mat3(m_camera->GetViewMatrix()));
    glm::mat4 projection =
        glm::perspective(glm::radians(m_camera->m_zoom), float(1920) / float(1080), 0.01f, 1000.0f);
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 1.0, 0.0));

    glm::vec3 viewPos = m_camera->m_vecPosition;
    float time = static_cast<float>(glfwGetTime());

    // ----------- PASS 1 -------------
    glDepthMask(GL_FALSE);      
    glDepthFunc(GL_LEQUAL);

    m_skyShader->Use();

    // skybox load
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBoxID);
    glUniform1i(glGetUniformLocation(m_skyShader->m_shaderID, "skybox"), 0);

    // stndard MVP
    glUniformMatrix4fv(glGetUniformLocation(m_skyShader->m_shaderID, "view"), 1, GL_FALSE,
        glm::value_ptr(viewSky));                
    glUniformMatrix4fv(glGetUniformLocation(m_skyShader->m_shaderID, "proj"), 1, GL_FALSE,
        glm::value_ptr(projection));    

    glBindVertexArray(m_vaoSky);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glm::mat4 view = m_camera->GetViewMatrix();

    // ----------- PASS 2 -------------
    glDepthMask(GL_TRUE);       
    glDepthFunc(GL_LESS);

    // skybox load
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyBoxID);
    glUniform1i(glGetUniformLocation(m_waterShader->m_shaderID, "skybox"), 0);

    // stndard MVP
    m_waterShader->Use();
    glUniformMatrix4fv(glGetUniformLocation(m_waterShader->m_shaderID, "view"), 1, GL_FALSE,
        glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_waterShader->m_shaderID, "proj"), 1, GL_FALSE,
        glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_waterShader->m_shaderID, "model"), 1, GL_FALSE,
        glm::value_ptr(model));

    // wave params
    glUniform1f(glGetUniformLocation(m_waterShader->m_shaderID, "time"), time);
    glUniform1f(glGetUniformLocation(m_waterShader->m_shaderID, "amplitude"), u_amplitude);
    glUniform1f(glGetUniformLocation(m_waterShader->m_shaderID, "frequency"), u_frequency);
    glUniform1f(glGetUniformLocation(m_waterShader->m_shaderID, "speed"), u_speed);
    glUniform1f(glGetUniformLocation(m_waterShader->m_shaderID, "reflectionFactor"), u_reflection);

    // blinn phong
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "viewPos"),1, glm::value_ptr(m_camera->m_vecPosition));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "lightPos"), 1, glm::value_ptr(u_lightPos));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "lightColor"), 1, glm::value_ptr(u_lightColor));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "objectColor"), 1, glm::value_ptr(u_objectColor));

    glBindVertexArray(m_vaoWater);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

void Renderer::setupBuffers(MeshGrid &meshGrid)
{
    glGenVertexArrays(1, &m_vaoWater);
    glBindVertexArray(m_vaoWater);

    glGenBuffers(1, &m_vboWater);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboWater);
    glBufferData(GL_ARRAY_BUFFER, meshGrid.vertices.size() * sizeof(Vertex),
        meshGrid.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_eboWater);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboWater);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        meshGrid.indices.size() * sizeof(unsigned int),
        meshGrid.indices.data(),
        GL_STATIC_DRAW);

    m_vertexCount = meshGrid.vertices.size();
    m_indexCount = static_cast<GLsizei>(meshGrid.indices.size());

    glBindVertexArray(0);
}

void Renderer::setupSkyBuffers(SkyBox& skyBox) {

    glGenVertexArrays(1, &m_vaoSky);
    glBindVertexArray(m_vaoSky);

    glGenBuffers(1, &m_vboSky);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboSky);
    glBufferData(GL_ARRAY_BUFFER, skyBox.skyboxVerts.size() * sizeof(float), skyBox.skyboxVerts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
