#pragma once

// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>
#include "camera.h"

namespace Callbacks {

extern Camera camera;
extern GLFWwindow* window;
extern bool leftMouseButtonPressed;
extern float deltaTime;  // time between current frame and last frame
extern float lastFrame;
extern double lastX;
extern double lastY;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
}  // namespace Callbacks