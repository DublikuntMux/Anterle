#pragma once

#include <vector>
#include <vulkan/vulkan.h>

void check_vk_result(VkResult err);
bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);
VkPhysicalDevice SelectPhysicalDevice(VkInstance instance);
