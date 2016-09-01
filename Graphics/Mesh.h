#pragma once

#include "EightEngine\Graphics\ApiWrapper\Buffers\VertexBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\Buffers\\IndexBuffer.h"

class RenderingDevice;

struct MeshDesc
{
	UINT VertexCount;
	UINT VertexBufferStride;
	void *pVerticesMemory;

	UINT IndexCount;
	UINT IndexBufferStride;
	void *pIndicesMemory;

	MeshDesc() { }
	template <typename VERTEX, typename INDEX>
	MeshDesc(std::vector<VERTEX> &vertices, std::vector<INDEX> &indices) 
		: VertexCount(static_cast<UINT>(vertices.size())), VertexBufferStride(sizeof(VERTEX)),
		pVerticesMemory(vertices.data()), IndexCount(static_cast<UINT>(indices.size())), 
		IndexBufferStride(sizeof(INDEX)), pIndicesMemory(indices.data()) { }
};

class Mesh
{
public:
	Mesh() { }
	~Mesh() { }

	// Creating mesh from mesh description
	// Mesh description must be initialized, pointer to vertices and indices cannot be null
	// and vertex count, index count, vertex stride and index stride can not be zero too
	// This mesh create vertex and index buffer -> this mean, that you can't have only vertex buffer
	// After this call you can release memory from vertex and index buffer -> gpu has its own copy
	// Vertex and index buffer are created for hight performance -> only gpu accces(only reading)
	// Parameters:
	// meshDesc -> describing mesh, vertex buffer and index buffer(you can not skip vertex or index part)
	// pRenderingDevice -> smart pointer to rendering device, it can not be null
	bool Init(const MeshDesc &meshDesc, 
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	// Setting vertex and index buffer into input assembler stage
	// There is a control -> if mesh is allready binded it is not binded again
	// control is in vertex and index shader(for now)
	// Parameters:
	// pRenderingDeviceContext -> smart pointer to rendering device context
	void Apply(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		m_VertexBuffer.Bind(pRenderingDeviceContext);
		m_IndexBuffer.Bind(pRenderingDeviceContext);
	}

	// It returns how many indices are in index buffer
	UINT GetIndexCount() const { return m_IndexBuffer.GetIndexCount(); }
private:
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};