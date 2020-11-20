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
        getPhysicalDevice();
        CreateLogicalDevice();
    }
    catch (const std::runtime_error &e)
    {
        printf("ERROR: %s \n", e.what());
        return EXIT_FAILURE;
    }

    return 0;
}

void VulkanRenderer::cleanup() 
{
    vkDestroyInstance(instance, nullptr);
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

    // Check if instance extensions are supported;
    if (!checkInstanceExtensionSupport(&instanceExtensions))
    {
        throw std::runtime_error("VkInstance does not support required extensions");
    }

                            createInfo.enabledExtensionCount    = (uint32_t)instanceExtensions.size();
                            createInfo.ppEnabledExtensionNames  = instanceExtensions.data();
                            createInfo.enabledLayerCount        = 0;
                            createInfo.ppEnabledLayerNames      = nullptr;
    // Create Instance
    VkResult                result                              = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a Vulkan Instance");
    }
}

void VulkanRenderer::CreateLogicalDevice() 
{
    // Get the queue family indices for the chose physical device
    QueueFamilyIndices      indices                             = getQueueFamilies(mainDevice.physicalDevice);
    VkDeviceQueueCreateInfo queueCreateInfo                     = {};
                            queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                            queueCreateInfo.queueFamilyIndex    = indices.graphicsFamily;
                            queueCreateInfo.queueCount          = 1;                    // Number of Queues to create
    float                   priority                            = 1.0f;
                            queueCreateInfo.pQueuePriorities    = &priority;
    VkDeviceCreateInfo      deviceCreateInfo                    = {};
                            deviceCreateInfo.sType              = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                            deviceCreateInfo.queueCreateInfoCount= 1;
                            deviceCreateInfo.pQueueCreateInfos  = &queueCreateInfo; // List of queue create info so device can create required queues;
                            deviceCreateInfo.enabledExtensionCount=0;               // Number of enabled logical device extensions
                            deviceCreateInfo.ppEnabledExtensionNames=nullptr;
    // Physical device features that the logical device will be using;
    VkPhysicalDeviceFeatures deviceFeatures                     = {};
                            deviceCreateInfo.pEnabledFeatures   = &deviceFeatures;

    // Create the logical device for the given physical device;
    VkResult                result                              = vkCreateDevice(mainDevice.physicalDevice, 
                                                                                 &deviceCreateInfo, 
                                                                                 nullptr,
                                                                                 &mainDevice.logicalDevice);
}

void VulkanRenderer::getPhysicalDevice() 
{
    uint32_t                        deviceCount = 0;
                                    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice>   deviceList(deviceCount);
                                    vkEnumeratePhysicalDevices(instance, &deviceCount,deviceList.data()); 
    // Check if no availabe devices

    if (deviceCount == 0)
    {
        throw std::runtime_error("No Physical devices found");
    }
    
    for (const auto &device : deviceList)
    {
        if (checkDeviceSuitable(device))
        {
            mainDevice.physicalDevice = device;
        }
    }
}

bool VulkanRenderer::checkInstanceExtensionSupport(
                     std::vector<const char*>* checkExtensions)
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    // Create a list of VkExtensionProperties using count;
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    for(const auto &checkExtension: *checkExtensions)
    {
       bool hasExtension = false ;

       for (auto const &extension : extensions)
       {
           if (strcmp(checkExtension, extension.extensionName)) 
           {
                hasExtension = true;
                break;
           }
       }

       if (!hasExtension)
       {
            return false;
       }       

    }
    return true;
}

bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device) 
{
    // Information about the device itself
    VkPhysicalDeviceProperties  deviceProperties;
    vkGetPhysicalDeviceProperties(device,&deviceProperties);
    std::cout << deviceProperties.deviceType << '\n';
    std::cout << deviceProperties.deviceName << '\n';

    // Information about what the device can do?

    VkPhysicalDeviceFeatures    deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices  indices = getQueueFamilies(device);
    return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device) 
{
    QueueFamilyIndices                      indices;
    uint32_t                                queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties>    queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    // Go through each queue family and check if it has at least 1 of the required types of queue
    int i = 0;
    for (const auto &queueFamily : queueFamilyList)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
           indices.graphicsFamily = i;
        }

        if (indices.isValid())
        {
            break;
        }
        i++;
    }

    return indices;
}
