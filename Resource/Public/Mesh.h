#pragma once
#include "Resource.h"
#include <Foundation.h>
#include <Matrix3x4.h>

struct aiScene;
struct aiMesh;
struct aiNode;

namespace tc
{

class FMeshLoaderSettings
{
public:
	FMeshLoaderSettings()
	{ Reset(); }

	bool SmoothNormal;
	bool HighQuality;
	bool HighestQuality;

	void Reset()
	{
		SmoothNormal = false;
		HighQuality = false;
		HighestQuality = false;
	}
};

class RESOURCE_API RMesh: public RResource
{
	class FSubMesh
	{
		struct FVertexData
		{
			float x, y, z;
			float nx, ny, nz;
			float TexX, TexY;
			int Bones[4];
			float Weights[4];

			FVertexData()
			{
				x = y = z = 0.0f;
				nx = ny = nz = 0.0f;
				TexX = TexY = 0.0f;
				Bones[0] = Bones[1] = Bones[2] = Bones[3] = 0;
				Weights[0] = Weights[1] = Weights[2] = Weights[3] = 0.0f;
			}

			void AddWeight(int bone, float weight)
			{
				int ibone = 0;
				while (Weights[ibone] != 0.0f)
					ibone++;
				Bones[ibone] = bone;
				Weights[ibone] = weight;
			}
		};

		vector<FVertexData> Vertices;
		vector<unsigned short> Indices;
		RMesh *pOwner;

	public:
		FSubMesh(RMesh *owner)
			: pOwner(owner)
		{}
		void Read(aiMesh *mesh);
	};

	struct FBone
	{
		bool Bound = false;
		string Name;
		Matrix3x4 OffsetMatrix;
		Matrix3x4 Transformation;
		size_t Parent;
		vector<size_t> Children;
	};

	//Simple Mesh
	vector<float> Vertices;
	vector<float> UVs;
	vector<float> Normals;
	vector<unsigned short> Indices;

	//Complex Mesh
	vector<FSubMesh> SubMeshes;
	vector<FBone> Bones;
	unordered_map<string, size_t> BoneNameToIndex;

	void Read(void *buffer, size_t len);
	void ReadSimple(const aiScene *scene);
	void ReadComplex(const aiScene *scene);
	size_t ReadBones(aiNode *node, size_t parent);

public:
	RMesh();
	RMesh(RResource *file);
	RMesh(const string &identifier, bool onDisk = true);
	~RMesh();
	static FMeshLoaderSettings Settings;
};

}
