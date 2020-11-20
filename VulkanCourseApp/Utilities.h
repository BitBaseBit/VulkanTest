#pragma once

// Indices (locations) of Queue Families 

struct QueueFamilyIndices 
{
    int graphicsFamily = -1;        // Location of graphics Queue family

    // Check if queue families are valid;
    bool isValid()
    {
       return graphicsFamily >= 0; 
    }
};
