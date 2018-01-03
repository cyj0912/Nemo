#pragma once
#include <Foundation.h>
#include <yaml-cpp/yaml.h>

namespace YAML
{
class Node;
}

namespace tc
{

class FYamlSerializer
{
public:
};

class FYamlDeserializer
{
    const YAML::Node Node;
public:
    FYamlDeserializer(const YAML::Node& node) : Node(node) {}
    template <typename T> T Deserialize(const string& name) const;
    vector<FYamlDeserializer*> GetArray(const string& name) const;
};

}
