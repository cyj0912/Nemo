#include "Image.h"
#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace tc
{

RImage::RImage(int w, int h, int comp) :
	Type(ImageType::Image2D),
	Width(w), Height(h), Depth(1), NumComponent(comp)
{
	Data = new ComponentType[w * h * comp];
}

RImage::RImage(const string id) : Type(ImageType::Image2D), Depth(1)
{
    string path = GResourceManager.FindFile(id);
	auto* imgData = stbi_load(path.c_str(), &Width, &Height, &NumComponent, 3);
	Data = new ComponentType[Width * Height * NumComponent];
	memcpy(Data, imgData, Width * Height * NumComponent * sizeof(ComponentType));
	stbi_image_free(imgData);
}

RImage::~RImage()
{
	if (Data)
		delete[] Data;
}

int RImage::GetDataSize() const
{
	int numElements = 0;
	switch (Type)
	{
	case ImageType::Image1D:
		numElements = Width * NumComponent;
		break;
	case ImageType::Image2D:
		numElements = Width * Height * NumComponent;
		break;
	case ImageType::Image3D:
		numElements = Width * Height * Depth * NumComponent;
		break;
	case ImageType::Image1DArray:
		numElements = Width * Height * NumComponent;
		break;
	case ImageType::Image2DArray:
		numElements = Width * Height * Depth * NumComponent;
		break;
	case ImageType::CubeMap:
		numElements = Width * Height * 6 * NumComponent;
		break;
	case ImageType::CubeMapArray:
		numElements = Width * Height * 6 * NumComponent;
		break;
	default: ;
	}
	return numElements * sizeof(ComponentType);
}

void RImage::ZeroOut()
{
	memset(Data, 0, GetDataSize());
}

} /* namespace tc */
