#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class DepthStencilStateBase
{
public:
	// Setting depth description
	// Default settings are: depth is enabled, depth writing is enabled 
	// and comparison function is less
	// Parameters:
	// enableDepth -> set true if you want to have depth test enabled
	// depthWriteMask -> you can enable / disable writing to depth buffer
	//				  -> if you disable, depth test is performed, but value stored in buffer
	//				  -> will not be replaced
	// depthFunc -> depth function used in depth test
	// This method you can call only before Create is called
	void SetDepthDesc(bool enableDepth, DepthWriteMask depthWriteMask,
		ComparisonFunc depthFunc)
	{
		m_DepthEnable = enableDepth;
		m_DepthWriteMask = depthWriteMask;
		m_DepthFunc = depthFunc;
	}
	// Setting stencil description
	// Default settings are: stencil is disabled and stencil read and write mask is 0xff
	// Parameters:
	// enableStencil -> set true if you want to have stencil test enabled
	// stencilReadMask -> identify a portion of stencil buffer for reading stencil data
	// stencilWriteMask -> identify a portion of stencil buffer for writing stencil data
	// This method you can call only before Create is called
	void SetStencilDesc(bool enableStencil, BYTE stencilReadMask, BYTE stencilWriteMask)
	{
		m_StencilEnable = enableStencil;
		m_StencilReadMask = stencilReadMask;
		m_StencilWriteMask = stencilWriteMask;
	}
	// Setting stencil operation description for front face polygons
	// Default parameters are: if stencil test or depth test failed stencil value stays unchanged
	// if stencil test pass stencil value is incremented and comparison function for stencil
	// test is always, this mean that comparison function always return true
	// Parameters:
	// stencilFailOp -> what should stencil buffer do, if stencil test failed
	// depthFailOp -> what should stencil buffer do, if depth test failed
	// stencilpassOp -> what should stencil buffer do, if stencil test passed
	// stencilFunc -> stencil function used in stencil test
	// This method you can call only before Create is called
	void SetFrontFaceStencilOpDesc(StencilOp stencilFailOp, StencilOp depthFailOp,
		StencilOp stencilPassOp, ComparisonFunc stencilFunc)
	{
		m_FrontFace.StencilDepthFailOp = depthFailOp;
		m_FrontFace.StencilFailOp = stencilFailOp;
		m_FrontFace.StencilPassOp = stencilPassOp;
		m_FrontFace.StencilFunc = stencilFunc;
	}
	// Setting stencil operation description for back face polygons
	// Default parameters are: if stencil test or depth test failed stencil value stays unchanged
	// if stencil test pass stencil value is incremented and comparison function for stencil
	// test is always, this mean that comparison function always return true
	// Parameters:
	// stencilFailOp -> what should stencil buffer do, if stencil test failed
	// depthFailOp -> what should stencil buffer do, if depth test failed
	// stencilpassOp -> what should stencil buffer do, if stencil test passed
	// stencilFunc -> stencil function used in stencil test
	// This method you can call only before Create is called
	void SetBackFaceStencilOpDesc(StencilOp stencilFailOp, StencilOp depthFailOp,
		StencilOp stencilPassOp, ComparisonFunc stencilFunc)
	{
		m_BackFace.StencilDepthFailOp = depthFailOp;
		m_BackFace.StencilFailOp = stencilFailOp;
		m_BackFace.StencilPassOp = stencilPassOp;
		m_BackFace.StencilFunc = stencilFunc;
	}
	// Parameters:
	// stencilRef -> stencil reference value used in stencil test
	void SetStencilRef(UINT stencilRef)
	{
		m_StencilRef = stencilRef;
	}
protected:
	struct StencilOpDesc
	{
		StencilOp StencilFailOp;
		StencilOp StencilDepthFailOp;
		StencilOp StencilPassOp;
		ComparisonFunc StencilFunc;

		StencilOpDesc() : StencilFailOp(StencilOp::STENCIL_OP_KEEP),
			StencilDepthFailOp(StencilOp::STENCIL_OP_KEEP),
			StencilPassOp(StencilOp::STENCIL_OP_INCR),
			StencilFunc(ComparisonFunc::COMPARISON_ALWAYS)
		{

		}
	};

	bool m_DepthEnable;
	DepthWriteMask m_DepthWriteMask;
	ComparisonFunc m_DepthFunc;

	bool m_StencilEnable;
	UINT m_StencilRef;
	BYTE m_StencilReadMask;
	BYTE m_StencilWriteMask;

	StencilOpDesc m_FrontFace;
	StencilOpDesc m_BackFace;

	DepthStencilStateBase() : m_DepthEnable(true), m_DepthFunc(ComparisonFunc::COMPARISON_LESS),
		m_DepthWriteMask(DepthWriteMask::DEPTH_WRITE_MASK_ALL), m_StencilEnable(false),
		m_StencilReadMask(0xff), m_StencilWriteMask(0xff), m_StencilRef(0)
	{

	}
	~DepthStencilStateBase() { }
};