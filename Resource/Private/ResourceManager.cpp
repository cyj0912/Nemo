#include "ResourceManager.h"
#include "ResourceFactory.h"
#include "File.h"
#include "StaticMesh.h"

#include <PathTools.h>

#if TC_OS == TC_OS_WINDOWS_NT
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <iostream>

namespace tc
{

void FResourceManager::Init()
{
    SearchPaths.insert(FPathTools::StripFilename(FPathTools::GetExecutablePath()));
    auto wd = FPathTools::GetWorkingDirectory();
    for (int i = 0; i < 4; i++)
    {
        SearchPaths.insert(wd);
        auto dataDir = FPathTools::Join(wd, "Data");
        if (FPathTools::Exists(dataDir))
            SearchPaths.insert(dataDir);
        wd = FPathTools::StripFilename(wd);
        wd = FPathTools::RemoveTrailingSlash(wd);
    }
    for (auto s : SearchPaths)
        LOGINFO("Search path: %s\n", s.c_str());
}

void FResourceManager::Shutdown()
{
    SearchPaths.clear();
}

string FResourceManager::ProcessPath(const string &path)
{
    if (FPathTools::IsAbsolute(path))
        return path;

    // Test each search dir + path
    for (auto searchPath : SearchPaths)
        if (FPathTools::Exists(FPathTools::Join(searchPath, path)))
            return FPathTools::Join(searchPath, path);

    return FindFile(FPathTools::StripDirectory(path));
}

string FResourceManager::FindFile(const string &name)
{
    for (auto searchPath : SearchPaths)
        if (FPathTools::Exists(FPathTools::Join(searchPath, name)))
            return FPathTools::Join(searchPath, name);
    return string();
}

TRefPtr<RFile> FResourceManager::Open(const string& name)
{
    return TRefPtr<RFile>();
}

char* FResourceManager::ReadEntireFile(const string& name, size_t& retSize)
{
    return nullptr;
}

string FResourceManager::ReadTextFile(const string& name)
{
    auto str = ProcessPath(name);
    return FPathTools::ReadTextFile(str);
}

void FResourceManager::ReleaseEntireFile(char* buffer)
{
}

TRefPtr<RResource> FResourceManager::GetResource(const string &identifier)
{
    auto iter = ResourceMap.find(identifier);
    if (iter != ResourceMap.end())
        return iter->second;

    // Not found in map
    string path = ProcessPath(identifier);
    TRefPtr<RResource> res = GResourceFactory.NewResource(path);
    ResourceMap.insert(make_pair(identifier, res));
    return res;
}

template <>
RESOURCE_API TRefPtr<RStaticMesh> FResourceManager::GetResource<RStaticMesh>(const string &identifier)
{
    auto iter = ResourceMap.find(identifier);
    if (iter != ResourceMap.end())
        return dynamic_cast<RStaticMesh*>(iter->second.Get());

    // Not found in map
    string path = ProcessPath(identifier);
    TRefPtr<RStaticMesh> res(new RStaticMesh(path));
    ResourceMap.insert(make_pair(identifier, res));
    return res;
}

RESOURCE_API FResourceManager GResourceManager;

}
