#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <vector>

class VulkanRenderer 
{
public: 
                VulkanRenderer();
    int         init(GLFWwindow* newWindow);
                ~VulkanRenderer();

private:
    GLFWwindow* window;

    // Vulkan Components
    VkInstance  instance;
    void        CreateInstance();
    // Support Funcitons
    bool        checkInstanceExtensionSupport(std::vector<const char*> checkExtenstions);
};
