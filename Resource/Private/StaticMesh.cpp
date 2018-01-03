#include "StaticMesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace tc
{

RStaticMesh::RStaticMesh()
{
}

RStaticMesh::RStaticMesh(RFile* file)
{
	throw std::logic_error("Unimplemented");
}

RStaticMesh::RStaticMesh(const string& path, bool onDisk)
{
    if (onDisk)
        AssimpImportFromFile(path);
    else
    {
		//TODO Implement this
		throw -1;
    }
}

RStaticMesh::~RStaticMesh()
{
}

unsigned short RStaticMesh::CountVertices() const
{
	return (unsigned short)(VertexBuffer.size() / 3);
}

void RStaticMesh::CalculateAABB()
{
	AABB.Clear();
	for (int i = 0; i < VertexBuffer.size(); i += 3)
	{
		AABB.Merge(Vector3(&VertexBuffer[i]));
	}
}

const BoundingBox& RStaticMesh::GetAABB() const
{
	return AABB;
}

void RStaticMesh::UpdateBoundingSphere()
{
	BoundingSphere.Clear();
	for (int i = 0; i < VertexBuffer.size(); i += 3)
	{
		BoundingSphere.Merge(Vector3(&VertexBuffer[i]));
	}
}

const Sphere& RStaticMesh::GetBoundingSphere() const
{
	return BoundingSphere;
}

// TODO: Debug and test the validity of the following two functions
void RStaticMesh::AddVertex(const Vector3& p, const Vector2& uv, const Vector3& normal, const Vector3& tan, const Vector3& bitan)
{
	VertexBuffer.push_back(p.Data()[0]);
	VertexBuffer.push_back(p.Data()[1]);
	VertexBuffer.push_back(p.Data()[2]);
	if (true)
	{
		UVBuffer.push_back(uv.Data()[0]);
		UVBuffer.push_back(uv.Data()[1]);
	}
	if (NormalBuffer.size() != 0)
	{
		NormalBuffer.push_back(normal.Data()[0]);
		NormalBuffer.push_back(normal.Data()[1]);
		NormalBuffer.push_back(normal.Data()[2]);
	}
	if (TangentBuffer.size() != 0)
	{
		TangentBuffer.push_back(tan.Data()[0]);
		TangentBuffer.push_back(tan.Data()[1]);
		TangentBuffer.push_back(tan.Data()[2]);
	}
	if (BitangentBuffer.size() != 0)
	{
		BitangentBuffer.push_back(bitan.Data()[0]);
		BitangentBuffer.push_back(bitan.Data()[1]);
		BitangentBuffer.push_back(bitan.Data()[2]);
	}
}

void RStaticMesh::AddTriangle(
	const Vector3& a, float ua, float va,
	const Vector3& b, float ub, float vb,
	const Vector3& c, float uc, float vc)
{
	unsigned short vertIndexStart = (unsigned short)(VertexBuffer.size() / 3);
	Vector3 surfaceNormal = (c - a).CrossProduct(c - b);
	AddVertex(a, Vector2(ua, va), surfaceNormal, Vector3::ZERO, Vector3::ZERO);
	AddVertex(b, Vector2(ub, vb), surfaceNormal, Vector3::ZERO, Vector3::ZERO);
	AddVertex(c, Vector2(uc, vc), surfaceNormal, Vector3::ZERO, Vector3::ZERO);
	IndexBuffer.push_back(vertIndexStart);
	IndexBuffer.push_back(vertIndexStart + 1);
	IndexBuffer.push_back(vertIndexStart + 2);
}

void RStaticMesh::AssimpImportFromFile(const string& path)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);
	AssimpImportScene(scene);
}

void RStaticMesh::AssimpImportScene(const aiScene* scene)
{
    LOGDEBUG("RStaticMesh import mMeshes[0], name: %s\n", scene->mMeshes[0]->mName.C_Str());
	aiMesh* mesh = scene->mMeshes[0];
	VertexBuffer.reserve(mesh->mNumVertices * 3);
	if (mesh->HasNormals()) NormalBuffer.reserve(mesh->mNumVertices * 3);
	if (mesh->HasTangentsAndBitangents())
	{
		TangentBuffer.reserve(mesh->mNumVertices * 3);
		BitangentBuffer.reserve(mesh->mNumVertices * 3);
	}
	if (mesh->HasTextureCoords(0)) UVBuffer.reserve(mesh->mNumVertices * 2);
	IndexBuffer.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexBuffer.push_back(mesh->mVertices[i].x);
		VertexBuffer.push_back(mesh->mVertices[i].y);
		VertexBuffer.push_back(mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			NormalBuffer.push_back(mesh->mNormals[i].x);
			NormalBuffer.push_back(mesh->mNormals[i].y);
			NormalBuffer.push_back(mesh->mNormals[i].z);
		}
		if(mesh->HasTangentsAndBitangents())
		{
			TangentBuffer.push_back(mesh->mTangents[i].x);
			TangentBuffer.push_back(mesh->mTangents[i].y);
			TangentBuffer.push_back(mesh->mTangents[i].z);
			BitangentBuffer.push_back(mesh->mBitangents[i].x);
			BitangentBuffer.push_back(mesh->mBitangents[i].y);
			BitangentBuffer.push_back(mesh->mBitangents[i].z);
		}
		if (mesh->HasTextureCoords(0))
		{
			UVBuffer.push_back(mesh->mTextureCoords[0][i].x);
			UVBuffer.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			UVBuffer.push_back((float)i * (1 / (float)mesh->mNumVertices));
			UVBuffer.push_back((mesh->mNumVertices - i) * (1 / (float)mesh->mNumVertices));
		}
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		IndexBuffer.push_back(mesh->mFaces[i].mIndices[0]);
		IndexBuffer.push_back(mesh->mFaces[i].mIndices[1]);
		IndexBuffer.push_back(mesh->mFaces[i].mIndices[2]);
	}
}

} /* namespace tc */
