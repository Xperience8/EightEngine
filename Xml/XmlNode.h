#pragma once

#include "XmlAttribute.h"

class XmlNode
{
public:
	XmlNode() : m_pWorkingNode(nullptr), m_pParentDocument(nullptr) { }
	XmlNode(rapidxml::xml_node<> *pWorkingNode, rapidxml::xml_document<> *pParentDoc) 
		: m_pWorkingNode(pWorkingNode), m_pParentDocument(pParentDoc) { }
	~XmlNode() { }

	// Internally switch to next node(sibling node)
	void GoToNextNode()
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		m_pWorkingNode = m_pWorkingNode->next_sibling();
	}
	
	// Internally switch to first children node
	void GoToChildrenNode()
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		m_pWorkingNode = m_pWorkingNode->first_node();
	}

	XmlNode CreateChildNode(const std::string &name, const std::string &value = std::string())
	{
		EIGHT_ASSERT(m_pParentDocument, "This node has not set xml document");
		EIGHT_ASSERT(m_pWorkingNode, "This node isn't initialized");

		rapidxml::xml_node<> *pNode = m_pParentDocument->allocate_node(
			rapidxml::node_type::node_element, 
			m_pParentDocument->allocate_string(name.c_str(), name.size()), 
			m_pParentDocument->allocate_string(value.c_str(), value.size()),
			name.size(), value.size());

		m_pWorkingNode->append_node(pNode);

		return XmlNode(pNode, m_pParentDocument);
	}

	void CreateChildNode(const XmlNode &node)
	{
		EIGHT_ASSERT(node.m_pWorkingNode, "Node which you want to clone is not initialized");
		EIGHT_ASSERT(m_pParentDocument, "New node has not set xml document");

		m_pWorkingNode->append_node(m_pParentDocument->clone_node(node.m_pWorkingNode));
	}

	void CreateAttribute(const std::string &name, const std::string &value = std::string())
	{
		EIGHT_ASSERT(m_pParentDocument, "This node has not set xml document");
		EIGHT_ASSERT(m_pWorkingNode, "This node isn't initialized");

		m_pWorkingNode->append_attribute(m_pParentDocument->allocate_attribute(
			m_pParentDocument->allocate_string(name.c_str(), name.size()),
			m_pParentDocument->allocate_string(value.c_str(), value.size()),
			name.size(), value.size()));
	}

	// This method return whether this node exist
	// You can get a wrong node if:
	//		You use GoToNextNode(), but the node has not sibling
	//		You use GoToChildrenNode(), but the node has not childrens
	//		You use GetChildrenNode(name) with wrong name
	//		etc
	bool Exist() const
	{ 
		return m_pWorkingNode != nullptr; 
	}

	// Changing value of the actual node
	// It finds XmlDocument, allocates new data and replaces old ones
	void ChangeValue(const std::string &value)
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		auto pDocument = m_pWorkingNode->document();
		m_pWorkingNode->value(pDocument->allocate_string(value.c_str()));
	}

	// Return name of the actual node
	std::string GetName() const 
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		//return m_pWorkingNode->name(); 
		return std::string(m_pWorkingNode->name(), m_pWorkingNode->name_size());
	}

	// Return value of the actual node
	std::string GetValue() const 
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		//return m_pWorkingNode->value();
		return std::string(m_pWorkingNode->value(), m_pWorkingNode->value_size());
	}

	// Return the first attribute
	// If attribute doesn't exist it returns empty attribute(check with exist method)
	XmlAttribute GetAttribute() const
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		return XmlAttribute(m_pWorkingNode->first_attribute());
	}

	// Return attribute with specific name
	// If attribute doesn't exist it returns empty attribute(check with exist method)
	XmlAttribute GetAttribute(const std::string &attributeName) const
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		return XmlAttribute(m_pWorkingNode->first_attribute(attributeName.c_str(),
			attributeName.size()));
	}
	
	// Return the first children node
	// If node doesn't exist it returns empty node(check with exist method)
	XmlNode GetChildrenNode() const
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		return XmlNode(m_pWorkingNode->first_node(), m_pParentDocument);
	}
	
	// Return children node with specific name
	// If node doesn't exist it returns empty node(check with exist method)
	// Only First Level(i.e. it doesn't go deeper)
	XmlNode GetChildrenNode(const std::string &nodeName) const
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlNode is not initialize or Working node is out of range");
		return XmlNode(m_pWorkingNode->first_node(nodeName.c_str(),
			nodeName.size()), m_pParentDocument);
	}
private:
	rapidxml::xml_document<> *m_pParentDocument;
	rapidxml::xml_node<> *m_pWorkingNode;
};