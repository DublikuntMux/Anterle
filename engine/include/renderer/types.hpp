#pragma once

#include <optional>
#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct Camera
{
  glm::vec3 cameraPosition;
  glm::vec3 cameraDirection;
  glm::vec3 up;
  float yaw;
  float pitch;
  glm::mat4 viewProj;
  glm::mat4 view;
  glm::mat4 proj;
};
