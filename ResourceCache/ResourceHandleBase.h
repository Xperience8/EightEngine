#pragma once

class ResourceHandleBase
{
protected:
	std::string m_ResourceName;
	bool m_IsInitialized;

public:
	explicit ResourceHandleBase(const std::string &resourceName) : m_ResourceName(resourceName),
		m_IsInitialized(false) { }
	virtual ~ResourceHandleBase() { }

	virtual bool VIsThereOwner() const = 0;

	bool IsInitialized() const 
	{ 
		return m_IsInitialized; 
	}
	void PostInit() 
	{ 
		m_IsInitialized = true; 
	}

	const std::string &GetResourceName() const { return m_ResourceName; }
};