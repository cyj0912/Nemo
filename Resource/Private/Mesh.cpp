#include "Mesh.h"
#include "File.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace tc
{

FMeshLoaderSettings RMesh::Settings;

void RMesh::FSubMesh::Read(aiMesh *mesh)
{
	LOGINFO("Mesh: %s", mesh->mName.C_Str());
	if (mesh->HasFaces()) {
		LOGINFO("HasFaces: %d", mesh->mNumFaces);
		for (unsigned fi = 0; fi < mesh->mNumFaces; fi++) {
			if (mesh->mFaces[fi].mNumIndices != 3)
				LOGERROR("WTF");
			for (unsigned ii = 0; ii < mesh->mFaces[fi].mNumIndices; ii++)
				Indices.push_back(mesh->mFaces[fi].mIndices[ii]);
		}
	}
	for (unsigned vi = 0; vi < mesh->mNumVertices; vi++) {
		FVertexData vert;
		if (mesh->HasPositions()) {
			vert.x = mesh->mVertices[vi].x;
			vert.y = mesh->mVertices[vi].y;
			vert.z = mesh->mVertices[vi].z;
		}
		if (mesh->HasNormals()) {
			vert.nx = mesh->mNormals[vi].x;
			vert.ny = mesh->mNormals[vi].y;
			vert.nz = mesh->mNormals[vi].z;
		}
		if (mesh->HasTextureCoords(0)) {
			vert.TexX = mesh->mTextureCoords[0]->x;
			vert.TexY = mesh->mTextureCoords[0]->y;
		}
		Vertices.push_back(vert);
	}
	if (mesh->HasTangentsAndBitangents())
		LOGINFO("HasTangentsAndBitangents");
	if (mesh->HasVertexColors(0))
		LOGINFO("HasVertexColors");
	if (mesh->HasBones()) {
		LOGINFO("HasBones");
		for (unsigned bi = 0; bi < mesh->mNumBones; bi++) {
			auto index = pOwner->BoneNameToIndex[mesh->mBones[bi]->mName.C_Str()];
			LOGINFO("\t%s, %d", mesh->mBones[bi]->mName.C_Str(), index);
			pOwner->Bones[index].OffsetMatrix = Matrix3x4(mesh->mBones[bi]->mOffsetMatrix[0]);
			pOwner->Bones[index].Bound = true;
			for (unsigned bvi = 0; bvi < mesh->mBones[bi]->mNumWeights; bvi++)
				Vertices[mesh->mBones[bi]->mWeights[bvi].mVertexId]
					.AddWeight(index, mesh->mBones[bi]->mWeights[bvi].mWeight);
		}
	}
}

void RMesh::Read(void *buffer, size_t len)
{
	unsigned int aiFlags = aiProcessPreset_TargetRealtime_Fast;
	if (Settings.HighestQuality)
		aiFlags = aiProcessPreset_TargetRealtime_MaxQuality;
	else if (Settings.HighQuality)
		aiFlags = aiProcessPreset_TargetRealtime_Quality;
	else if (Settings.SmoothNormal) {
		aiFlags &= !aiProcess_GenNormals;
		aiFlags |= aiProcess_GenSmoothNormals;
	}
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFileFromMemory(buffer, len, aiFlags, nullptr);
	if (!scene || !scene->HasMeshes())
		return;
	if (scene->mMeshes[0]->HasBones() || scene->HasAnimations())
		ReadComplex(scene);
	else
		ReadSimple(scene);
}

void RMesh::ReadSimple(const aiScene *scene)
{
	aiMesh *mesh = scene->mMeshes[0];
	Vertices.reserve(mesh->mNumVertices * 3);
	Normals.reserve(mesh->mNumVertices * 3);
	UVs.reserve(mesh->mNumVertices * 2);
	Indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertices.push_back(mesh->mVertices[i].x);
		Vertices.push_back(mesh->mVertices[i].y);
		Vertices.push_back(mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			Normals.push_back(mesh->mNormals[i].x);
			Normals.push_back(mesh->mNormals[i].y);
			Normals.push_back(mesh->mNormals[i].z);
		}
		if (mesh->HasTextureCoords(0)) {
			UVs.push_back(mesh->mTextureCoords[0][i].x);
			UVs.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			UVs.push_back((float) i * (1 / (float) mesh->mNumVertices));
			UVs.push_back((mesh->mNumVertices - i) * (1 / (float) mesh->mNumVertices));
		}
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		Indices.push_back(mesh->mFaces[i].mIndices[0]);
		Indices.push_back(mesh->mFaces[i].mIndices[1]);
		Indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
}

void RMesh::ReadComplex(const aiScene *scene)
{
	Bones.clear();
	BoneNameToIndex.clear();
	ReadBones(scene->mRootNode, 0);

	for (unsigned i = 0; i < scene->mNumMeshes; i++) {
		SubMeshes.push_back(FSubMesh(this));
		SubMeshes.back().Read(scene->mMeshes[i]);
	}
}

size_t RMesh::ReadBones(aiNode *node, size_t parent)
{
	Bones.push_back(FBone());
	size_t thisIndex = Bones.size() - 1;
	BoneNameToIndex[node->mName.C_Str()] = thisIndex;
	Bones[thisIndex].Name = node->mName.C_Str();
	Bones[thisIndex].Transformation = Matrix3x4(node->mTransformation[0]);
	Bones[thisIndex].Parent = parent;
	for (size_t i = 0; i < node->mNumChildren; i++) {
		size_t c = ReadBones(node->mChildren[i], thisIndex);
		Bones[thisIndex].Children.push_back(c);
	}
	return thisIndex;
}

RMesh::RMesh()
{
}

RMesh::RMesh(RResource *file)
{
	SetParent(file);
	RFile *par = static_cast<RFile *>(GetParent());
	size_t len = par->Size();
	char *buffer = new char[len];
	par->Read(buffer, sizeof(char), len);
	Read(buffer, len);
}

RMesh::RMesh(const string &identifier, bool onDisk)
{
	if (onDisk) {
		unsigned int aiFlags = aiProcessPreset_TargetRealtime_Fast;
		if (Settings.HighestQuality)
			aiFlags = aiProcessPreset_TargetRealtime_MaxQuality;
		else if (Settings.HighQuality)
			aiFlags = aiProcessPreset_TargetRealtime_Quality;
		else if (Settings.SmoothNormal) {
			aiFlags &= !aiProcess_GenNormals;
			aiFlags |= aiProcess_GenSmoothNormals;
		}

		Assimp::Importer importer;
		auto *scene = importer.ReadFile(identifier.c_str(), aiFlags);
		ReadComplex(scene);
	}
	else {
		// TODO: Implement this
		void* buffer = nullptr;
		int size = 0;
		Read(buffer, size);
	}
}

RMesh::~RMesh()
{
}

}
