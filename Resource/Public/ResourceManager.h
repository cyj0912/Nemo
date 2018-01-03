#pragma once
#include "ResourceAPI.h"
#include <Foundation.h>

namespace tc
{

class RResource;
class RFile;

class RESOURCE_API FResourceManager
{
#if TC_OS == TC_OS_WINDOWS_NT
    static const char PathDelimChar = '\\';
#else
    static const char PathDelimChar = '/';
#endif
    string PathDelimiter;
    set<string> SearchPaths;
    unordered_map<string, TRefPtr<RResource>> ResourceMap;
public:
    void Init();
    void Shutdown();

    string ProcessPath(const string& path);
    string FindFile(const string& name);

    TRefPtr<RFile> Open(const string& name);

    char* ReadEntireFile(const string& name, size_t& retSize);
    string ReadTextFile(const string& name);
    void ReleaseEntireFile(char* buffer);

    TRefPtr<RResource> GetResource(const string& identifier);

    template <typename T> TRefPtr<T> GetResource(const string& identifier);
};

RESOURCE_API extern FResourceManager GResourceManager;

}
