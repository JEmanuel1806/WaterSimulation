#include "App.h"
#include <iostream>


// ---------- Debug Output ------------- //
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    std::cerr << "[OpenGL DEBUG] " << message << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH)
        std::cerr << "Severity: HIGH\n";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        std::cerr << "Severity: MEDIUM\n";
    else if (severity == GL_DEBUG_SEVERITY_LOW)
        std::cerr << "Severity: LOW\n";
}

App::App(int w, int h)
    : m_window(nullptr), m_Renderer(nullptr), camera(nullptr),
    width(w), height(h),
    lastFrame(0.0f), deltaTime(0.0f), key_pressed(false),
    left_mouse_pressed(false), right_mouse_pressed(false), firstMouse(true)
{}

App::~App() {
    ImGui::SaveIniSettingsToDisk("imgui_layout.ini");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete m_Renderer;
    delete camera;
    glfwTerminate();
}

bool App::Init() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "WaterSim", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return false;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* win, double x, double y) {
        static_cast<App*>(glfwGetWindowUserPointer(win))->mouse_callback(win, x, y);
        });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* win, int button, int action, int mods) {
        static_cast<App*>(glfwGetWindowUserPointer(win))->mouse_button_callback(win, button, action, mods);
        });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = "imgui_layout.ini";

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    camera = new Camera(
        glm::vec3(125.0f, 350.0f, 10.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        -90.0f, -65.0f
    );

    m_Renderer = new Renderer(camera);
    return true;
}


void App::Run() {
    if (!Init()) return;

    m_Renderer->Start();

    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float fps = 1.0f / deltaTime;

        processInput();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SetupGUI();

        m_Renderer->Run();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void App::SetupGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ---------- DOCKSPACE ---------- //
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Transparent!
    ImGui::Begin("DockSpaceRoot", nullptr, window_flags);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
        ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode);

    ImGui::End();

    ImGui::Begin("Water Controls");
    ImGui::Text("Wave Parameters");
    ImGui::SliderFloat("Amplitude", &m_Renderer->u_amplitude, 0.0f, 1.0f);
    ImGui::SliderFloat("Frequency", &m_Renderer->u_frequency, 0.0f, 2.0f);
    ImGui::SliderFloat("Speed", &m_Renderer->u_speed, 0.0f, 5.0f);

    ImGui::Separator();
    ImGui::Text("Lighting");
    ImGui::SliderFloat3("Light Pos", glm::value_ptr(m_Renderer->u_lightPos), -100.0f, 100.0f);
    ImGui::ColorEdit3("Light Color", glm::value_ptr(m_Renderer->u_lightColor));
    ImGui::ColorEdit3("Object Color", glm::value_ptr(m_Renderer->u_objectColor));

    ImGui::End();
}

// ---------- Control Handling (Keyboard & Mouse) ------------- //
void App::processInput() {
    auto isPressed = [&](int key) { return glfwGetKey(m_window, key) == GLFW_PRESS; };

    if (isPressed(GLFW_KEY_W)) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (isPressed(GLFW_KEY_S)) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (isPressed(GLFW_KEY_A)) camera->ProcessKeyboard(LEFT, deltaTime);
    if (isPressed(GLFW_KEY_D)) camera->ProcessKeyboard(RIGHT, deltaTime);
    if (isPressed(GLFW_KEY_Q)) camera->ProcessKeyboard(ROTATE_LEFT, deltaTime);
    if (isPressed(GLFW_KEY_E)) camera->ProcessKeyboard(ROTATE_RIGHT, deltaTime);

    auto toggle = [&](int key, bool& flag) {
        if (isPressed(key) && !key_pressed) {
            flag = !flag;
            key_pressed = true;
        }
        if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
            key_pressed = false;
        }
        };

    if (glfwGetKey(m_window, GLFW_KEY_KP_ADD) == GLFW_RELEASE) key_pressed = false;

    if (glfwGetKey(m_window, GLFW_KEY_KP_SUBTRACT) == GLFW_RELEASE) key_pressed = false;

}

void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        left_mouse_pressed = (action == GLFW_PRESS);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        right_mouse_pressed = (action == GLFW_PRESS);
}

void App::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
}
