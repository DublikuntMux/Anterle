#pragma once

#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "imgui_impl_vulkan.h"

#include "backend/base.hpp"

class VulkanBackend: public BaseBackend
{
public:
  VulkanBackend(const char* window_name);
  ~VulkanBackend();

  void PreRender();
  void PostRender();

private:
  bool IsExtensionAvailable(const std::vector<VkExtensionProperties> &properties, const char *extension);
  VkPhysicalDevice SelectPhysicalDevice();

  void SetupVulkan(std::vector<const char *> instance_extensions);
  void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height);

  void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data);
  void FramePresent(ImGui_ImplVulkanH_Window *wd);

  VkSurfaceKHR surface;

  VkAllocationCallbacks *g_Allocator;
  VkInstance g_Instance;
  VkPhysicalDevice g_PhysicalDevice;
  VkDevice g_Device;
  uint32_t g_QueueFamily;
  VkQueue g_Queue;
  VkDebugReportCallbackEXT g_DebugReport;
  VkPipelineCache g_PipelineCache;
  VkDescriptorPool g_DescriptorPool;
  ImGui_ImplVulkanH_Window g_MainWindowData;
  uint32_t g_MinImageCount;
  bool g_SwapChainRebuild;
};
