#pragma once

#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"
#include "EightEngine\Graphics\Vertex.h"

class FontSheet
{
public:
	FontSheet() : m_CharacterHeight(0), m_SpaceWidth(0) { }
	~FontSheet() { }

	// Parameters:
	// charHeight -> character height a.k.a native font size in pixels
	// spaceWidth -> how many pixels should we skip if we want to render one space
	// charRectangles -> rectangle about every character, can not be null, it is copied into internal structures
	// pTexture -> texture which hold graphical information about every character, can not be null
	void Init(UINT charHeight, UINT spaceWidth, CD3D11_RECT *charRectangles,
		std::shared_ptr<Texture2D> &pTexture)
	{
		EIGHT_ASSERT(charRectangles,
			"Character's rectangles can not be null");
		EIGHT_ASSERT(pTexture,
			"Texture with font information can not be null");

		m_CharacterHeight = charHeight;
		m_SpaceWidth = spaceWidth;
		m_pTexture = pTexture;

		memcpy(m_CharacterRects, charRectangles, sizeof(CD3D11_RECT) * NUM_CHARACTERS);
	}

	const CD3D11_RECT &GetCharacterRectangle(char character) const
	{
		EIGHT_ASSERT(static_cast<UINT>(character)-CHAR_START < NUM_CHARACTERS,
			"Requested character rectangle from font is out of range");

		return m_CharacterRects[static_cast<UINT>(character)-CHAR_START];
	}
	UINT GetSpaceWidth() const { return m_SpaceWidth; }
	UINT GetCharacterHeight() const { return m_CharacterHeight; }
	std::shared_ptr<Texture2D> &GetTexture() { return m_pTexture; }

private:
	static const UINT CHAR_START = 33U;
	static const UINT CHAR_END = 127U;
public:
	// How many characters are in texture map(only english alphabet now)
	static const UINT NUM_CHARACTERS = CHAR_END - CHAR_START;
private:
	// This holds rectangle about every character
	// Rectangle specifies character's position inside texture
	// and it specifies character's width too
	CD3D11_RECT m_CharacterRects[NUM_CHARACTERS];

	// This is a texture with every character
	std::shared_ptr<Texture2D> m_pTexture;

	UINT m_CharacterHeight;
	UINT m_SpaceWidth;
};