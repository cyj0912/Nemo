#pragma once
#include "Resource.h"

#include <stb_image.h>

namespace tc
{

enum class ImageType
{
	Image1D,
	Image2D,
	Image3D,
	Image1DArray,
	Image2DArray,
	CubeMap,
	CubeMapArray
};

struct FPixelRGB8
{
	unsigned char R, G, B;
};

struct FPixelRGBA8
{
	unsigned char R, G, B, A;
};
/*
template <typename PixelType>
class TBitmap : public RResource
{
	ImageType Type;
    int Width, Height, Depth, NumComponent;
	PixelType* Data;
public:
	TImage(const string id)
	{
		auto* helper = SUBSYS(FResourceHelper);
		string path = helper->GetPathOnDisk(id);
		auto* imgData = stbi_load(path.c_str(), &Width, &Height, &NumComponent, 3);
		Data = new ComponentType[Width * Height * NumComponent];
		memcpy(Data, imgData, Width * Height * NumComponent * sizeof(ComponentType));
		stbi_image_free(imgData);
	}

	virtual ~TImage()
	{
		if (Data)
			delete[] Data;
	}

	ImageType GetType() const
	{
		return Type;
	}

	int GetWidth() const
	{
		return Width;
	}

	int GetHeight() const
	{
		return Height;
	}

	int GetDepth() const
	{
		return Depth;
	}

	PixelType* GetData() const
	{
		return Data;
	}
};
*/
class RESOURCE_API RImage : public RResource
{
	using ComponentType = unsigned char;

	ImageType Type;
	int Width, Height, Depth, NumComponent;
	ComponentType* Data;
public:
	RImage(int w, int h, int comp);
	RImage(const string id);
	virtual ~RImage();

	ImageType GetType() const
	{
		return Type;
	}

	int GetWidth() const
	{
		return Width;
	}

	int GetHeight() const
	{
		return Height;
	}

	int GetDepth() const
	{
		return Depth;
	}

	int GetNumComponent() const
	{
		return NumComponent;
	}

	unsigned char* GetData() const
	{
		return Data;
	}

	int GetDataSize() const;
	void ZeroOut();
};

} /* namespace tc */
