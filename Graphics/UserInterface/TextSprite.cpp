#include "stdafx.h"
#include "TextSprite.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\String\LocalizationMap.h"

void TextSprite::Init(const DirectX::XMFLOAT3 &position, const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	m_pScreen = pScreen;

	// In config file we had position stored in NDC space 
	// so we do not have to set for every resolution new positions
	// so from NDC position we calculate position on screen in pixels
	// and this position is used in calculating vertices
	// which are in the end converting back to NDC space
	DirectX::XMFLOAT3 initPosition = pScreen->PointFromNDCToPx(position);

	// Because we're loading text from xml file 
	// and our parser has a problem with reading escape sequences
	// we have to repair it manually or we get wrong results if we render text
	std::string text = g_LocalizationMap[configNode.GetChildrenNode("Text").GetValue()];
	RepairEscapeSequences(text);

	m_SizeInNDC = std::stof(configNode.GetChildrenNode("SizeInNDC").GetValue());

	auto colorNode = configNode.GetChildrenNode("Color");
	DirectX::XMFLOAT4 color;
	color.x = std::stof(colorNode.GetAttribute("r").GetValue());
	color.y = std::stof(colorNode.GetAttribute("g").GetValue());
	color.z = std::stof(colorNode.GetAttribute("b").GetValue());
	color.w = std::stof(colorNode.GetAttribute("a").GetValue());

	m_pFontSheet = g_ResourceCache.RequestResource<FontSheet>(
		configNode.GetChildrenNode("Font").GetValue());

	CreateVertices(text, initPosition, color);
}

void TextSprite::ChangeText(const std::string &text)
{
	EIGHT_ASSERT(!m_Vertices.empty(),
		"You can not change text if text was not previously initialized");

	DirectX::XMFLOAT3 initPosition = m_pScreen->PointFromNDCToPx(m_Vertices[1].Position);
	DirectX::XMFLOAT4 color = m_Vertices[1].Color;

	m_Vertices.clear();

	CreateVertices(text, initPosition, color);
}

void TextSprite::CreateVertices(const std::string &text,
	const DirectX::XMFLOAT3 &initPosition, const DirectX::XMFLOAT4 &color)
{
	DirectX::XMFLOAT3 pos = initPosition;

	// Getting texture information needed for calculating texture coordinates
	auto &fontSheetDesc = m_pFontSheet->GetTexture()->GetTextureDesc();
	const float textureWidth = static_cast<float>(fontSheetDesc.TextureWidth);
	const float textureHeight = static_cast<float>(fontSheetDesc.TextureHeight);

	// This mean that text will be scaled based on resolution
	// It means that text will cover the same portion of screen in every resolution
	float fontSize = m_SizeInNDC * m_pScreen->GetScreenHeight();

	// This is a difference between native font size and font size needed for selected resolution 
	float differenceX = fontSize / static_cast<float>(
		m_pFontSheet->GetCharacterHeight());

	for (UINT i = 0; i < text.size(); i++)
	{
		if (text[i] == ' ')
		{
			pos.x += m_pFontSheet->GetSpaceWidth() * differenceX;
		}
		else if (text[i] == '\n')
		{
			pos.x = initPosition.x;
			pos.y += fontSize;
		}
		else
		{
			// This is a rectangle which describes exactly one character
			// This means that character can be put into this rectangle
			auto &rect = m_pFontSheet->GetCharacterRectangle(text[i]);

			const float characterHeight = static_cast<float>(fontSize);
			const float characterWidth = static_cast<float>(
				rect.right - rect.left) * differenceX;

			// Creating vertices for one character
			// Indices are created automatically in rendering pass

			// Bottom left corner
			PosColTexVertex vertex1;
			vertex1.Position = m_pScreen->PointToNDC(
				DirectX::XMFLOAT3(pos.x, pos.y + characterHeight, pos.z));
			vertex1.TexCoord = DirectX::XMFLOAT2(
				static_cast<float>(rect.left) / textureWidth,
				static_cast<float>(rect.bottom) / textureHeight);
			vertex1.Color = color;

			// Top left corner
			PosColTexVertex vertex2;
			vertex2.Position = m_pScreen->PointToNDC(pos);
			vertex2.TexCoord = DirectX::XMFLOAT2(
				static_cast<float>(rect.left) / textureWidth,
				static_cast<float>(rect.top) / textureHeight);
			vertex2.Color = color;

			// Top right corner
			PosColTexVertex vertex3;
			vertex3.Position = m_pScreen->PointToNDC(
				DirectX::XMFLOAT3(pos.x + characterWidth, pos.y, pos.z));
			vertex3.TexCoord = DirectX::XMFLOAT2(
				static_cast<float>(rect.right) / textureWidth,
				static_cast<float>(rect.top) / textureHeight);
			vertex3.Color = color;

			// Bottom right corner
			PosColTexVertex vertex4;
			vertex4.Position = m_pScreen->PointToNDC(
				DirectX::XMFLOAT3(pos.x + characterWidth, pos.y + characterHeight, pos.z));
			vertex4.TexCoord = DirectX::XMFLOAT2(
				static_cast<float>(rect.right) / textureWidth,
				static_cast<float>(rect.bottom) / textureHeight);
			vertex4.Color = color;

			m_Vertices.push_back(vertex1);
			m_Vertices.push_back(vertex2);
			m_Vertices.push_back(vertex3);
			m_Vertices.push_back(vertex4);

			pos.x += characterWidth;
		}
	}
}
void TextSprite::RepairEscapeSequences(std::string &text)
{
	std::string tempText;
	tempText.reserve(text.size());

	for (UINT i = 0; i < text.size(); i++)
	{
		if (text[i] == '\\')
		{
			switch (text[i + 1])
			{
			case 'n':
				tempText += '\n';
				break;
			case 't':
				tempText += '\t';
				break;
			}
			i++;
		}
		else
		{
			tempText += text[i];
		}
	}

	text.swap(tempText);
}
