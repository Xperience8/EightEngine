#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class RasterizerStateBase
{
public:
	// This is used for setting rasterizer properties
	// Default properties are:
	// CullMode is CULL_BACK
	// FillMode is FILL_SOLID
	// Depth clip enable is true
	// and everything else is false or zero
	// Parameters:
	// fillMode -> it desribe rasterizer mode -> render filled triangles 
	//				or render in wireframe mode
	// cullMode -> it describe culling mode
	void SetProperties(FillMode fillMode, CullMode cullMode)
	{
		m_CullMode = cullMode;
		m_FillMode = fillMode;
	}

protected:
	FillMode m_FillMode;
	CullMode m_CullMode;

	RasterizerStateBase() : m_FillMode(FillMode::FILL_SOLID),
		m_CullMode(CullMode::CULL_BACK) { }
	~RasterizerStateBase() { }
};