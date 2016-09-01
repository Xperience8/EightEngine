#include "stdafx.h"
#include "PhysicsShape.h"
#include "EightEngine\GameLogic\GamePhysics.h"

void BoxPhysicsGeometry::CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
	std::vector<USHORT> &indices) const
{
	auto &halfExtents = m_BoxGeometry.halfExtents;

	USHORT basicVertexId = static_cast<USHORT>(vertices.size());
	vertices.resize(basicVertexId + 8);

	vertices[basicVertexId + 0] = DirectX::XMFLOAT3(
		-halfExtents.x, -halfExtents.y, -halfExtents.z);
	vertices[basicVertexId + 1] = DirectX::XMFLOAT3(
		-halfExtents.x, +halfExtents.y, -halfExtents.z);
	vertices[basicVertexId + 2] = DirectX::XMFLOAT3(
		+halfExtents.x, +halfExtents.y, -halfExtents.z);
	vertices[basicVertexId + 3] = DirectX::XMFLOAT3(
		+halfExtents.x, -halfExtents.y, -halfExtents.z);

	vertices[basicVertexId + 4] = DirectX::XMFLOAT3(
		-halfExtents.x, -halfExtents.y, +halfExtents.z);
	vertices[basicVertexId + 5] = DirectX::XMFLOAT3(
		-halfExtents.x, +halfExtents.y, +halfExtents.z);
	vertices[basicVertexId + 6] = DirectX::XMFLOAT3(
		+halfExtents.x, +halfExtents.y, +halfExtents.z);
	vertices[basicVertexId + 7] = DirectX::XMFLOAT3(
		+halfExtents.x, -halfExtents.y, +halfExtents.z);

	USHORT basicIndexId = static_cast<USHORT>(indices.size());
	indices.resize(basicIndexId + 36);

	//Front
	indices[basicIndexId + 0] = 0 + basicVertexId;
	indices[basicIndexId + 1] = 1 + basicVertexId;
	indices[basicIndexId + 2] = 2 + basicVertexId;
	indices[basicIndexId + 3] = 0 + basicVertexId;
	indices[basicIndexId + 4] = 2 + basicVertexId;
	indices[basicIndexId + 5] = 3 + basicVertexId;

	//Back
	indices[basicIndexId + 6] = 4 + basicVertexId;
	indices[basicIndexId + 7] = 5 + basicVertexId;
	indices[basicIndexId + 8] = 6 + basicVertexId;
	indices[basicIndexId + 9] = 4 + basicVertexId;
	indices[basicIndexId + 10] = 6 + basicVertexId;
	indices[basicIndexId + 11] = 7 + basicVertexId;

	//Top
	indices[basicIndexId + 12] = 1 + basicVertexId;
	indices[basicIndexId + 13] = 5 + basicVertexId;
	indices[basicIndexId + 14] = 6 + basicVertexId;
	indices[basicIndexId + 15] = 1 + basicVertexId;
	indices[basicIndexId + 16] = 6 + basicVertexId;
	indices[basicIndexId + 17] = 2 + basicVertexId;

	//Bottom
	indices[basicIndexId + 18] = 0 + basicVertexId;
	indices[basicIndexId + 19] = 4 + basicVertexId;
	indices[basicIndexId + 20] = 7 + basicVertexId;
	indices[basicIndexId + 21] = 0 + basicVertexId;
	indices[basicIndexId + 22] = 7 + basicVertexId;
	indices[basicIndexId + 23] = 3 + basicVertexId;

	//Left
	indices[basicIndexId + 24] = 0 + basicVertexId;
	indices[basicIndexId + 25] = 1 + basicVertexId;
	indices[basicIndexId + 26] = 5 + basicVertexId;
	indices[basicIndexId + 27] = 0 + basicVertexId;
	indices[basicIndexId + 28] = 5 + basicVertexId;
	indices[basicIndexId + 29] = 4 + basicVertexId;

	//Right
	indices[basicIndexId + 30] = 3 + basicVertexId;
	indices[basicIndexId + 31] = 2 + basicVertexId;
	indices[basicIndexId + 32] = 6 + basicVertexId;
	indices[basicIndexId + 33] = 3 + basicVertexId;
	indices[basicIndexId + 34] = 6 + basicVertexId;
	indices[basicIndexId + 35] = 7 + basicVertexId;
}
void SpherePhysicsGeometry::CreateRenderingGeometry(
	std::vector<DirectX::XMFLOAT3> &vertices, std::vector<USHORT> &indices) const
{
	const UINT stackCount = 30;
	const UINT sliceCount = 30;
	const float radius = m_SphereGeometry.radius;

	USHORT basicVertexId = static_cast<USHORT>(vertices.size());

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.

	vertices.push_back(DirectX::XMFLOAT3(0.f, radius, 0.f));

	float phiStep = DirectX::XM_PI / static_cast<float>(stackCount);
	float thetaStep = DirectX::XM_2PI / static_cast<float>(sliceCount);

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i < stackCount; ++i)
	{
		float phi = i*phiStep;

		// vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			vertices.push_back(DirectX::XMFLOAT3(
				radius * sinf(phi) * cosf(theta), 
				radius * cosf(phi),
				radius * sinf(phi) * sinf(theta)
				));
		}
	}

	vertices.push_back(DirectX::XMFLOAT3(0.f, -radius, 0.f));

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(basicVertexId);
		indices.push_back(basicVertexId + i + 1);
		indices.push_back(basicVertexId + i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = basicVertexId + 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}
void HeightFieldPhysicsGeometry::CreateRenderingGeometry(
	std::vector<DirectX::XMFLOAT3> &vertices, std::vector<USHORT> &indices) const
{
	
}
void TriangleMeshPhysicsGeometry::CreateRenderingGeometry(
	std::vector<DirectX::XMFLOAT3> &vertices, std::vector<USHORT> &indices) const
{
	auto pTriangleMesh = m_TriangleMeshGeometry.triangleMesh;

	// vertices
	USHORT basicVertexId = static_cast<USHORT>(vertices.size());
	
	vertices.resize(basicVertexId + pTriangleMesh->getNbVertices());
	
	memcpy(vertices.data() + basicVertexId, pTriangleMesh->getVertices(),
		pTriangleMesh->getNbVertices() * sizeof(DirectX::XMFLOAT3));

	// indices
	USHORT basicIndexId = static_cast<USHORT>(indices.size());
	indices.resize(basicIndexId + pTriangleMesh->getNbTriangles() * 3);

	USHORT ibStride = pTriangleMesh->getTriangleMeshFlags() == physx::PxTriangleMeshFlag::e16_BIT_INDICES
		? sizeof(USHORT) : sizeof(UINT);

	const BYTE *rawMemory = reinterpret_cast<const BYTE*>(
		pTriangleMesh->getTriangles());

	for (USHORT i = basicIndexId, k = 0; i < indices.size(); i++, k++)
	{
		indices[i] = *reinterpret_cast<const USHORT*>(rawMemory + k * ibStride);
	}
}
void PhysicsShape::CreateRenderingShape(std::vector<DirectX::XMFLOAT3> &vertices,
	std::vector<USHORT> &indices) const
{
	UINT startVertex = static_cast<UINT>(vertices.size());

	m_pPhysicsGeometry->CreateRenderingGeometry(vertices, indices);

	physx::PxTransform transform = m_pShape->getLocalPose();

	if (!transform.p.isZero() || !(transform.q.x == 0.f && transform.q.y == 0.f &&
		transform.q.z == 0.f && transform.q.w == 1.f))
	{
		DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(
			transform.p.x, transform.p.y, transform.p.z);
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(
			DirectX::XMVectorSet(transform.q.x, transform.q.y, transform.q.z, transform.q.w));

		DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(
			rotationMatrix, translationMatrix);

		for (UINT i = startVertex; i < vertices.size(); i++)
		{
			auto &vertex = vertices[i];

			DirectX::XMStoreFloat3(&vertex, DirectX::XMVector3TransformCoord(
				DirectX::XMLoadFloat3(&vertex), localTransform));
		}
	}
}