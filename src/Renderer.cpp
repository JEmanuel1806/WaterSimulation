#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Camera* cam)
    : m_waterPlane(500) 
{
    m_camera = cam;
    m_waterShader = nullptr;
}

Renderer::~Renderer() {
    delete m_waterShader;
}

void Renderer::Start() {

    m_waterShader = new Shader("src/shader/vertex_shader.vert", "src/shader/fragment_shader.frag");
    
    setupBuffers(m_waterPlane);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void Renderer::Run()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_camera->GetViewMatrix();
    glm::mat4 projection =
        glm::perspective(glm::radians(m_camera->m_zoom), float(1920) / float(1080), 0.01f, 100.0f);
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 1.0, 0.0));

    glm::vec3 viewPos = m_camera->m_vecPosition;
    float time = static_cast<float>(glfwGetTime());

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

    // blinn phong
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "viewPos"),1, glm::value_ptr(m_camera->m_vecPosition));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "lightPos"), 1, glm::value_ptr(u_lightPos));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "lightColor"), 1, glm::value_ptr(u_lightColor));
    glUniform3fv(glGetUniformLocation(m_waterShader->m_shaderID, "objectColor"), 1, glm::value_ptr(u_objectColor));


    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::setupBuffers(MeshGrid &meshGrid)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, meshGrid.vertices.size() * sizeof(Vertex),
        meshGrid.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        meshGrid.indices.size() * sizeof(unsigned int),
        meshGrid.indices.data(),
        GL_STATIC_DRAW);

    m_vertexCount = meshGrid.vertices.size();
    m_indexCount = static_cast<GLsizei>(meshGrid.indices.size());

    glBindVertexArray(0);
}
