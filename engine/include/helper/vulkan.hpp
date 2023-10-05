#pragma once

#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "renderer/types.hpp"

std::vector<const char *> getRequiredExtensions();
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<const char *> deviceExtensions);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char *> deviceExtensions);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *glfw_window);
VkShaderModule createShaderModule(VkDevice device, const std::vector<char> &code);
VkFormat findSupportedFormat(VkPhysicalDevice device,
  const std::vector<VkFormat> &candidates,
  VkImageTiling tiling,
  VkFormatFeatureFlags features);
VkFormat findDepthFormat(VkPhysicalDevice device);
bool hasStencilComponent(VkFormat format);
VkImageView
  createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
void createImage(uint32_t width,
  uint32_t height,
  uint32_t mipLevels,
  VkSampleCountFlagBits numSamples,
  VkFormat format,
  VkImageTiling tiling,
  VkImageUsageFlags usage,
  VkMemoryPropertyFlags properties,
  VkImage &image,
  VkDeviceMemory &imageMemory,
  VkDevice device,
  VkPhysicalDevice physicalDevice);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
void createBuffer(VkDeviceSize size,
  VkBufferUsageFlags usage,
  VkMemoryPropertyFlags properties,
  VkBuffer &buffer,
  VkDeviceMemory &bufferMemory,
  VkDevice device,
  VkPhysicalDevice physicalDevice);
void transitionImageLayout(VkImage image,
  VkImageLayout oldLayout,
  VkImageLayout newLayout,
  uint32_t mipLevels,
  VkCommandPool commandPool,
  VkDevice device,
  VkQueue graphicsQueue);
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
void endSingleTimeCommands(VkCommandBuffer commandBuffer,
  VkQueue graphicsQueue,
  VkDevice device,
  VkCommandPool commandPool);
void copyBufferToImage(VkBuffer buffer,
  VkImage image,
  uint32_t width,
  uint32_t height,
  VkCommandPool commandPool,
  VkQueue graphicsQueu,
  VkDevice device);
void copyBuffer(VkBuffer srcBuffer,
  VkBuffer dstBuffer,
  VkDeviceSize size,
  VkCommandPool commandPool,
  VkDevice device,
  VkQueue graphicsQueue);
VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
void generateMipmaps(VkImage image,
  VkFormat imageFormat,
  int32_t texWidth,
  int32_t texHeight,
  uint32_t mipLevels,
  VkPhysicalDevice physicalDevice,
  VkCommandPool commandPool,
  VkDevice device,
  VkQueue graphicsQueue);
