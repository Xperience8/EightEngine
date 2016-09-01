# pragma once

#include "EightEngine\Graphics\Vertex.h"
#include "FontSheet.h"
#include "EightEngine\GameAppLayer\Screen.h"

class XmlNode;
class ResourceCache;

class TextSprite
{
public:
	TextSprite() : m_SizeInNDC(0.f) { }
	~TextSprite() { }

	// Parameters:
	// position -> position in NDC space
	// configNode -> it holds information such as text, color, font ...
	// pScreen -> smart pointer to screen, it is used for converting to and from NDC space
	void Init(const DirectX::XMFLOAT3 &position, const XmlNode &configNode, 
		const std::shared_ptr<Screen> &pScreen);

	// Changing only text, position, color, font etc. will stay same
	void ChangeText(const std::string &text);
	// Changing only color, position, text etc. will stay same
	void ChangeColor(const DirectX::XMFLOAT4 &color)
	{
		EIGHT_ASSERT(!m_Vertices.empty(),
			"TextSprite is not initialized! You can not change color of text, if text was not created!");
		for (auto &vertex : m_Vertices)
		{
			vertex.Color = color;
		}
	}

	// Returning text dimensions in pixels
	void FindTextDimension(DirectX::XMFLOAT3 &firstVertexPos,
		DirectX::XMFLOAT3 &lastVertexPos)
	{
		EIGHT_ASSERT(!m_Vertices.empty(),
			"TextSprite is not initialized! You can not get text dimension if text was not created!");

		firstVertexPos = m_pScreen->PointFromNDCToPx(m_Vertices[1].Position);
		lastVertexPos = m_pScreen->PointFromNDCToPx(m_Vertices[m_Vertices.size() - 1].Position);
	}

	const std::shared_ptr<FontSheet> &GetFont() const { return m_pFontSheet; }
	const std::vector<PosColTexVertex> &GetVertices() const { return m_Vertices; }
	const DirectX::XMFLOAT4 &GetColor() const
	{
		EIGHT_ASSERT(!m_Vertices.empty(),
			"TextSprite is not initialized! You can not get color of text if text was not created!");
		return m_Vertices[0].Color;
	}
private:
	std::vector<PosColTexVertex> m_Vertices;
	std::shared_ptr<FontSheet> m_pFontSheet;
	std::shared_ptr<Screen> m_pScreen;

	float m_SizeInNDC;

	void CreateVertices(const std::string &text, const DirectX::XMFLOAT3 &initPosition,
		const DirectX::XMFLOAT4 &color);
	void RepairEscapeSequences(std::string &text);
};