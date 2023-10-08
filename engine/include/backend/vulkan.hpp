#include <vector>

#include <imgui.h>
#include <imgui_impl_vulkan.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "backend/base.hpp"

namespace Anterle {
class VulkanBackend : public BaseBackend
{
public:
  VulkanBackend(int width, int height, const char *window_name);
  ~VulkanBackend();

  void PreRender();
  void PostRender();

private:
  void SetupVulkan(std::vector<const char *> instance_extensions);
  void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height);
  void FramePresent(ImGui_ImplVulkanH_Window* wd);
  void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

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
  int g_MinImageCount = 2;
  bool g_SwapChainRebuild = false;
};
}// namespace Anterle