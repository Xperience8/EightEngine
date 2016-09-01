#pragma once

#include "GraphicsFlags.h"

class InputLayoutBase
{
public:
	// Adding input element to the input layout
	// This input element has to match exactly with what vertex shader is waiting for 
	// Parameters:
	// semanticName -> input element name -> must be same as in the shader
	// semanticIndex -> input element index 
	// format -> describing how many bytes it takes up
	// inputSlot -> input slot through which are data pushed
	// alignedByteOffset -> from which byte it starts
	// inputSlotClass -> if there are any data which you want to update per instance
	//				  -> and they are in vertex buffer you must specify INPUT_PER_INSTANCE_DATA here otherwise INPUT_PER_VERTEX_DATA
	// instanceDataStepRate -> specify only if you use instance data
	//						-> it specifies how many instances should be drown before going to next instance data
	void AddInputElement(const std::string semanticName, UINT semanticIndex,
		FormatFlag format, UINT inputSlot, UINT alignedByteOffset,
		InputClassification inputSlotClass, UINT instanceDataStepRate)
	{
		InputElementDesc desc;
		desc.SemanticName = semanticName;
		desc.SemanticIndex = semanticIndex;
		desc.Format = format;
		desc.InputSlot = inputSlot;
		desc.AlignedByteOffset = alignedByteOffset;
		desc.InputSlotClass = inputSlotClass;
		desc.InstanceDataStepRate = instanceDataStepRate;

		m_InputLayoutDesc.push_back(desc);
	}
protected:
	struct InputElementDesc
	{
		std::string SemanticName;
		UINT SemanticIndex;
		FormatFlag Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		InputClassification InputSlotClass;
		UINT InstanceDataStepRate;
	};

	std::vector<InputElementDesc> m_InputLayoutDesc;

	InputLayoutBase() { }
	~InputLayoutBase() { }
};