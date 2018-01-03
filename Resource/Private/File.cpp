#include "File.h"
#include <sys/stat.h>

namespace tc
{

TRefPtr<RDiskFile> RDiskFile::NewFromPath(const string &path, RFile::OpenMode mode)
{
    FILE *fp = nullptr;
#if TC_OS == TC_OS_WINDOWS_NT
    switch (mode)
    {
    case OpenMode::Read:
        fopen_s(&fp, path.c_str(), "rb");
        break;
    case OpenMode::Write:
        fopen_s(&fp, path.c_str(), "wb");
        break;
    case OpenMode ::Append:
        fopen_s(&fp, path.c_str(), "ab");
        break;
    }
#else
    switch (mode)
    {
    case OpenMode::Read:
        fp = fopen(path.c_str(), "rb");
        break;
    case OpenMode::Write:
        fp = fopen(path.c_str(), "wb");
        break;
    case OpenMode ::Append:
        fp = fopen(path.c_str(), "ab");
        break;
    }
#endif
    if (!fp)
        return TRefPtr<RDiskFile>();
    return TRefPtr<RDiskFile>(new RDiskFile(fp));
}

RDiskFile::~RDiskFile()
{
    fclose(CStdIoFile);
}

size_t RDiskFile::Read(void *dest, size_t size, size_t count)
{
	return fread(dest, size, count, CStdIoFile);
}

size_t RDiskFile::Write(void *src, size_t size, size_t count)
{
	return fwrite(src, size, count, CStdIoFile);
}

ptrdiff_t RDiskFile::Seek(int offset, SeekOrigin origin)
{
	int fseekOrigin = SEEK_CUR;
	switch (origin) {
	case SeekOrigin::Begin: fseekOrigin = SEEK_SET;
		break;
	case SeekOrigin::End: fseekOrigin = SEEK_END;
		break;
	case SeekOrigin::Current: fseekOrigin = SEEK_CUR;
		break;
	default: break;
	}
	fflush(CStdIoFile);
	if (fseek(CStdIoFile, offset, fseekOrigin) != 0)
		throw "Something is wrong with seek";
	return ftell(CStdIoFile);
}

ptrdiff_t RDiskFile::Size()
{
#if TC_OS == TC_OS_WINDOWS_NT
	int fd = _fileno(CStdIoFile);
	struct stat sb;
	fstat(fd, &sb);
	return sb.st_size;
#else
    int fd = fileno(CStdIoFile);
    struct stat sb;
    fstat(fd, &sb);
    return sb.st_size;
#endif
}

ptrdiff_t RDiskFile::Tell()
{
	return ftell(CStdIoFile);
}

}
