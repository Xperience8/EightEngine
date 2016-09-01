#pragma once

#include "EightEngine\Graphics\Vertex.h"

class Texture2D;
class Screen;
class XmlNode;
class ResourceCache;

class Sprite
{
public:
	Sprite() : m_Vertices(4) { }
	~Sprite() { }

	// Parameters:
	// position -> position in NDC space
	// configNode -> it holds information such as size, texture ...
	// pScreen -> smart pointer to screen, it is used for converting to and from NDC space
	void Init(const DirectX::XMFLOAT3 &position, const XmlNode &configNode, 
		const std::shared_ptr<Screen> &pScreen);

	const std::vector<PosTexVertex> &GetVertices() const
	{
		return m_Vertices;
	}
	const std::shared_ptr<Texture2D> &GetTexture() const
	{
		return m_pTexture;
	}

	void ChangeTexture(const std::shared_ptr<Texture2D> &pTexture)
	{
		m_pTexture = pTexture;
	}
private:
	std::vector<PosTexVertex> m_Vertices;
	std::shared_ptr<Texture2D> m_pTexture;
};