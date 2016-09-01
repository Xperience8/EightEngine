#pragma once

class SubsetTable
{
private:
	UINT m_MaterialID;
	UINT m_MeshID;
	UINT m_IndexStart;
	UINT m_IndexCount;
public:
	SubsetTable() { }
	SubsetTable(UINT materialID, UINT meshID, UINT IndexStart, UINT IndexCount) : 
		m_MaterialID(materialID), m_MeshID(meshID), m_IndexCount(IndexCount), m_IndexStart(IndexStart) { }
	~SubsetTable() { }

	UINT GetMaterialID() const { return m_MaterialID; }
	UINT GetMeshID() const { return m_MeshID; }
	UINT GetIndexStart() const { return m_IndexStart; }
	UINT GetIndexCount() const { return m_IndexCount; }
};