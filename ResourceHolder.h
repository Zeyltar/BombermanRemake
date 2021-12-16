#pragma once

#include <string>
#include <memory>
using namespace std;

#include <SFML/Graphics.hpp>

template <typename Resource>
class ResourceHolder
{
public:
    void load(const string& key, const string& filename);
    Resource& get(const string& key);
private:
    map< string, shared_ptr<Resource> > mResourcesMap;
};

#include "ResourceHolder.inl"

