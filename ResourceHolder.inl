#include "ResourceHolder.h"

template <typename Resource>
void ResourceHolder<Resource>::load(const string& key, const string& filename)
{
    shared_ptr<Resource> resource_ptr = make_shared<Resource>();
    resource_ptr->loadFromFile(filename);
    mResourcesMap.insert({ key, resource_ptr });
}

template <typename Resource>
Resource& ResourceHolder<Resource>::get(const string& key)
{
    return *(mResourcesMap[key]);
}