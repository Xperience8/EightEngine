#pragma once

class XmlAttribute
{
public:
	XmlAttribute(rapidxml::xml_attribute<> *pAttribute)
		: m_pWorkingAttribute(pAttribute) { }
	~XmlAttribute() { }

	// This method return whether this attribute exist
	// You can get a wrong attribute if:
	//		You use XmlNode::GetAttribute(name) with wrong name
	//		You use XmlNode::GetAttribute(), but node has not any attributes
	bool Exist() const 
	{ 
		return m_pWorkingAttribute != nullptr; 
	}

	// Changing value of the attribute
	// It finds XmlDocument, allocates new data and replaces old ones
	void ChangeValue(const std::string &value)
	{
		EIGHT_ASSERT(m_pWorkingAttribute, "Attribute doesn't exist");
		auto pDocument = m_pWorkingAttribute->document();
		m_pWorkingAttribute->value(pDocument->allocate_string(value.c_str()));
	}

	// Return name of the attribute
	std::string GetName() const 
	{ 
		EIGHT_ASSERT(m_pWorkingAttribute, "Attribute doesn't exist");
		//return m_pWorkingAttribute->name(); 
		return std::string(m_pWorkingAttribute->name(), m_pWorkingAttribute->name_size());
	}

	// Return value of the attribute
	std::string GetValue() const 
	{ 
		EIGHT_ASSERT(m_pWorkingAttribute, "Attribute doesn't exist");
		return std::string(m_pWorkingAttribute->value(), m_pWorkingAttribute->value_size());
		//return m_pWorkingAttribute->value(); 
	}
private:
	rapidxml::xml_attribute<> *m_pWorkingAttribute;
};