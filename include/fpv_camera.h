
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class FpvCamera {
 public:
  // camera Attributes
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  // euler Angles
  float yaw_;
  float pitch_;
  // camera options
  float movement_speed_;
  float mouse_sensitivity_;
  float zoom_;

 public:
  FpvCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
    position_ = position;
    world_up_ = up;
    front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    yaw_ = YAW;
    pitch_ = PITCH;
    movement_speed_ = SPEED;
    mouse_sensitivity_ = SENSITIVITY;
    zoom_ = ZOOM;
    updateCameraVectors();
  }

  FpvCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    position_ = glm::vec3(posX, posY, posZ);
    world_up_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    movement_speed_ = SPEED;
    mouse_sensitivity_ = SENSITIVITY;
    zoom_ = ZOOM;
    updateCameraVectors();
  }
  ~FpvCamera() {}

 public:
  glm::mat4 GetViewMatrix() { return glm::lookAt(position_, position_ + front_, up_); }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed_ * deltaTime;
    if (direction == FORWARD) position_ += front_ * velocity;
    if (direction == BACKWARD) position_ -= front_ * velocity;
    if (direction == LEFT) position_ -= right_ * velocity;
    if (direction == RIGHT) position_ += right_ * velocity;
    position_.y = 0.25f;
  }

  // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (pitch_ > 89.0f) pitch_ = 89.0f;
      if (pitch_ < -89.0f) pitch_ = -89.0f;
    }

    // update front_, right_ and up_ Vectors using the updated Euler angles
    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f) zoom_ = 1.0f;
    if (zoom_ > 45.0f) zoom_ = 45.0f;
  }

 private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    // calculate the new front_ vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    // std::cout << "debug:" << front.x << "; " << front.y << "; " << front.z << std::endl;
    front_ = glm::normalize(front);
    // also re-calculate the right_ and up_ vector
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }
};
