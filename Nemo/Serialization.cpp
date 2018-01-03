#include "Serialization.h"
#include <yaml-cpp/yaml.h>
#include <Vector3.h>
#include <Quaternion.h>

namespace tc
{

template<typename T>
T FYamlDeserializer::Deserialize(const string &name) const
{
    return nullptr;
}

template<>
string FYamlDeserializer::Deserialize<string>(const string& name) const
{
    return Node[name].IsNull() ? string() : Node[name].as<string>();
}

template<>
Vector3 FYamlDeserializer::Deserialize<Vector3>(const string& name) const
{
    Vector3 result;
    auto collection = Node[name];
    for (size_t i = 0; i < 3; ++i)
        result[i] = collection[i].as<float>();
    return result;
}

template<>
Quaternion FYamlDeserializer::Deserialize<Quaternion>(const string& name) const
{
    Quaternion result;
    auto collection = Node[name];
    result.w_ = collection[0].as<float>();
    result.x_ = collection[1].as<float>();
    result.y_ = collection[2].as<float>();
    result.z_ = collection[3].as<float>();
    return result;
}

vector<FYamlDeserializer*> FYamlDeserializer::GetArray(const string &name) const
{
    vector<FYamlDeserializer*> result;
    auto& collection = Node[name];
    for (size_t i = 0; i < collection.size(); ++i)
    {
        result.push_back(new FYamlDeserializer(collection[i]));
    }
    return result;
}

}
