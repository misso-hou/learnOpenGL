#include "input_handler.h"

#include <iostream>

namespace Callbacks {

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLFWwindow* window;
float deltaTime = 0.f;
float lastFrame = 0.f;
bool leftMouseButtonPressed = false;
double lastX = 0.0;
double lastY = 0.0;

void key_callback(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (leftMouseButtonPressed) {
    float xoffset = xpos - (float)lastX;
    float yoffset = (float)lastY - ypos;  // rev

    lastX = (double)xpos;
    lastY = (double)ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { camera.ProcessMouseScroll(static_cast<float>(yoffset)); }

// 鼠标按钮回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      leftMouseButtonPressed = true;
      glfwGetCursorPos(window, &lastX, &lastY);
    } else if (action == GLFW_RELEASE) {
      leftMouseButtonPressed = false;
    }
  }
}

}  // namespace Callbacks