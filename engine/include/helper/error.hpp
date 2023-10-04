#pragma once

#include <vulkan/vulkan.h>

void glfw_error_callback(int error, const char* description);
void check_vk_result(VkResult err);