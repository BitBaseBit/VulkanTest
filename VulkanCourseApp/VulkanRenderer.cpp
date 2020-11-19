#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
}
VulkanRenderer::~VulkanRenderer()
{
}

int VulkanRenderer::init(GLFWwindow* newWindow) 
{
    
    window = newWindow;

    try 
    {
        CreateInstance();
    }
    catch (const std::runtime_error &e)
    {
        printf("ERROR: %s \n", e.what());
        return EXIT_FAILURE;
    }

    return 0;
}

void VulkanRenderer::CreateInstance() 
{
    // Information about the application itself
    // Most data here doesn't affect the program and is for dev convenience.
    VkApplicationInfo       appInfo                             = {};
                            appInfo.sType                       = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                            appInfo.pApplicationName            = "Vulkan App";
                            appInfo.applicationVersion          = VK_MAKE_VERSION(1,0,0);
                            appInfo.pEngineName                 = "Noe Engine";
                            appInfo.engineVersion               = VK_MAKE_VERSION(1,0,0);
                            appInfo.apiVersion                  = VK_API_VERSION_1_0;
        
    VkInstanceCreateInfo    createInfo                          = {};
                            createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                            createInfo.pApplicationInfo         = &appInfo;
    // Create a list to hold instance extensions.        
    std::vector<const char*>instanceExtensions                  = std::vector<const char*>();
    uint32_t                glfwExtensionCount                  = 0; //glfw may requrie multiple extensions;
    const char**            glfwExtensions;                          // Extensions passed as array of cstrings, so need a pointer to the pointer(cstring
                            glfwExtensions                      = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    // Add glfw extensions to list of all extensions
    for (size_t i = 0; i < glfwExtensionCount; i++) 
    {
        instanceExtensions.push_back(glfwExtensions[i]);
    }
                            createInfo.enabledExtensionCount    = (uint32_t)instanceExtensions.size();
                            createInfo.ppEnabledExtensionNames  = instanceExtensions.data();
                            createInfo.enabledLayerCount        = 0;
                            createInfo.ppEnabledLayerNames      = nullptr;
    // Create Instance
    VkResult result                                             = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a Vulkan Instance");
    }
}

bool VulkanRenderer::checkInstanceExtensionSupport(
                     std::vector<const char*> checkExtenstions)
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    return false;
}
