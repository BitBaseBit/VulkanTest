#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include "Utilities.h"

class VulkanRenderer 
{
public: 
                        VulkanRenderer();
    int                 init(GLFWwindow* newWindow);
    void                cleanup();
                        ~VulkanRenderer();

private:
    GLFWwindow* window;
    struct 
    {
       VkPhysicalDevice physicalDevice;
       VkDevice         logicalDevice;
    } mainDevice;

    // Vulkan Components
    // create funcitons
    VkInstance          instance;
    void                CreateInstance();
    void                CreateLogicalDevice();

    // Get functions
    void                getPhysicalDevice();

    // Support Funcitons
    bool                checkInstanceExtensionSupport(std::vector<const char*>* checkExtenstions);
    bool                checkDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices  getQueueFamilies(VkPhysicalDevice device);
};
