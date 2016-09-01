#pragma once

template <typename VERTEX>
class RenderingBatch
{
private:
	std::vector<VERTEX> m_Vertices;
	float m_MinZ;
public:
	RenderingBatch() : m_MinZ(1.f) { }
	~RenderingBatch() { }

	void Reset()
	{
		m_MinZ = 1.f;
		m_Vertices.clear();
	}

	void Batch(const std::vector<VERTEX> &vertices)
	{
		UINT startPosition = static_cast<UINT>(m_Vertices.size());
		m_Vertices.resize(startPosition + vertices.size());

		memcpy(m_Vertices.data() + startPosition, vertices.data(),
			vertices.size() * sizeof(VERTEX));

		m_MinZ = DirectX::XMMin(m_Vertices[0].Position.z, m_MinZ);
	}

	float GetMinZ() const { return m_MinZ; }

	void *GetVertices() { return m_Vertices.data(); }
	UINT GetVertexCount() const { return static_cast<UINT>(m_Vertices.size()); }
	UINT GetVertexSize() const { return sizeof(VERTEX); }
};