
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Default camera values
// const float YAW = -90.0f;
// const float PITCH = 0.0f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;
// const float distance = 5.0f;  // 相机与目标点的初始距离
const float minDistance = 1.0f;
const float maxDistance = 10.0f;

class ModelCamera {
 public:
  // camera Attributes
  glm::vec3 position_;
  glm::vec3 target_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  float distance_;
  // euler Angles
  float yaw_;
  float pitch_;
  // camera options
  float movement_speed_;
  float mouse_sensitivity_;
  float zoom_;

 public:
 ModelCamera(float dis, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
    // position_ = position;
    distance_ = dis;
    world_up_ = up;
    target_ = glm::vec3(0.0f, 0.0f, 0.0f);
    yaw_ = -90.0f;
    pitch_ = 0.0f;
    movement_speed_ = 2.5f;
    mouse_sensitivity_ = 0.1f;
    zoom_ = 45.0f;
    updateCameraVectors();
  }

  ~ModelCamera() {}

 public:
  glm::mat4 GetViewMatrix() { return glm::lookAt(position_, front_, up_); }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    // float velocity = movement_speed_ * deltaTime;
    // if (direction == FORWARD) position_ += front_ * velocity;
    // if (direction == BACKWARD) position_ -= front_ * velocity;
    // if (direction == LEFT) position_ -= right_ * velocity;
    // if (direction == RIGHT) position_ += right_ * velocity;
    // position_.y = 0.25f;
  }

  // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ -= xoffset;
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
    distance_ += static_cast<float>(yoffset);
    if (distance_ < minDistance) distance_ = minDistance;
    if (distance_ > maxDistance) distance_ = maxDistance;
    updateCameraVectors();
  }

 private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    position_.x = distance_ * cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    position_.y = distance_ * sin(glm::radians(pitch_));
    position_.z = distance_ * sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    // calculate the new front_ vector
    front_ = glm::normalize(position_ - target_);
    // also re-calculate the right_ and up_ vector
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }
};
