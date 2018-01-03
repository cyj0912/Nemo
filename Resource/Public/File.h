#pragma once
#include "Resource.h"
#include <Foundation.h>
#include <string>
#include <cstdio>

namespace tc
{

/*
 * RFile interface
 *
 * RFile serves as the generic interface to operate on any stream-like
 * objects. The most basic implementation is the RDiskFile.
 *
 * RFile is created in various ways and should always be tracked with
 * smart pointers. Hence, releasing a file is done through calling
 * the destructor. The same thing goes with any RResource.
 */

class RESOURCE_API RFile: public RResource
{
protected:
	RFile() = default;
public:
	enum class OpenMode
	{
		Read = 0,
		Write = 1,
        Append = 2
	};

	enum class SeekOrigin
	{
		Begin,
		End,
		Current
	};

    ~RFile() override = default;
	virtual size_t Read(void *dest, size_t size, size_t count) = 0;
	virtual size_t Write(void *src, size_t size, size_t count) = 0;
	virtual ptrdiff_t Seek(int offset, SeekOrigin origin) = 0;
	virtual ptrdiff_t Size() = 0;
	virtual ptrdiff_t Tell() = 0;
};

class RDiskFile: public RFile
{
	FILE *CStdIoFile;

    explicit RDiskFile(FILE* fp) : CStdIoFile(fp) {}

public:
    static TRefPtr<RDiskFile> NewFromPath(const string& path, OpenMode mode = OpenMode::Read);

	~RDiskFile() override;
	size_t Read(void *dest, size_t size, size_t count) override;
	size_t Write(void *src, size_t size, size_t count) override;
	ptrdiff_t Seek(int offset, SeekOrigin origin) override;
	ptrdiff_t Size() override;
	ptrdiff_t Tell() override;
};

}
