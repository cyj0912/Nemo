#pragma once
#include "Resource.h"
#include <Foundation.h>
#include <BoundingBox.h>
#include <Sphere.h>

struct aiScene;

namespace tc
{

class RFile;

class RESOURCE_API RStaticMesh : public RResource
{
	BoundingBox AABB;
	Sphere BoundingSphere;

public:
	// Create an empty mesh
	RStaticMesh();

	// Create from file
	RStaticMesh(RFile* file);

	// Create from file path on disk
    RStaticMesh(const string& path, bool onDisk = true);

	~RStaticMesh() override;

	unsigned short CountVertices() const;

	void CalculateAABB();
	const BoundingBox& GetAABB() const;
	void UpdateBoundingSphere();
	const Sphere& GetBoundingSphere() const;

	void AddVertex(const Vector3& p, const Vector2& uv, const Vector3& normal, const Vector3& tan, const Vector3& bitan);
	void AddTriangle(const Vector3& a, float ua, float va, const Vector3& b, float ub, float vb, const Vector3& c, float uc, float vc);

	// Public mesh data
	vector<float> VertexBuffer; //x, y, z
	vector<float> UVBuffer; //u, v
	vector<float> NormalBuffer; //x, y, z
	vector<float> TangentBuffer; //x, y, z
	vector<float> BitangentBuffer; //x, y, z
	vector<unsigned short> IndexBuffer;

protected:
	void AssimpImportFromFile(const string& path);
	void AssimpImportScene(const aiScene* scene);
};

} /* namespace tc */
