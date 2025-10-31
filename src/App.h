#pragma once

#include "Camera.h"
#include "Renderer.h"

#include "imgui.h";
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class App
{
public:
    App(int width = 1280, int height = 720);
    ~App();


    void Run();

private:

    bool Init();                                  
    void processInput();             
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:
    GLFWwindow* m_window = nullptr;
    int width = 1280;
    int height = 720;

    Renderer* m_Renderer = nullptr;
    Camera* camera = nullptr;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool key_pressed = false;
    bool left_mouse_pressed = false;
    bool right_mouse_pressed = false;
    bool firstMouse = true;

    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
};
