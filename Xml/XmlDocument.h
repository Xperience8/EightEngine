#pragma once

#include "XmlNode.h"

class XmlDocument
{
public:
	XmlDocument() : m_pXmlFile(nullptr), m_XmlFileSize(0), m_pWorkingNode(nullptr) { }
	~XmlDocument() 
	{ 
		delete[] m_pXmlFile; 
	}

	// It saves pointer to data and data size(data must stay available for all times)
	// It creates new graph and set working node to root
	void Init(char *pXmlFile, UINT xmlFileSize)
	{
		m_pXmlFile = pXmlFile;
		m_XmlFileSize = xmlFileSize;
		m_Document.parse<rapidxml::parse_no_data_nodes>(m_pXmlFile);
		m_pWorkingNode = m_Document.first_node();
	}

	void Init(const std::string &name)
	{
		if (m_Document.first_node())
		{
			m_Document.clear();
		}

		auto pNode = m_Document.allocate_node(rapidxml::node_type::node_element,
			m_Document.allocate_string(name.c_str(), name.size()), nullptr, name.size());

		m_Document.append_node(pNode);
		m_pWorkingNode = pNode;
	}

	// Save xml document to file
	void SaveToFile(const std::string &filename)
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlDocument is not initialized");
		//EIGHT_ASSERT(m_pXmlFile, "XmlDocument is not initialized");

		std::ofstream fout(filename.c_str());
		fout << m_Document;
		fout.close();
	}

	// Save xml document to file
	std::string SaveToString()
	{
		EIGHT_ASSERT(m_pWorkingNode, "XmlDocument is not initialized");
		//EIGHT_ASSERT(m_pXmlFile, "XmlDocument is not initialized");

		std::ostringstream os;
		os << m_Document;

		return os.str();
	}

	// Return root node
	XmlNode GetRootNode()
	{
		return XmlNode(m_pWorkingNode, &m_Document); 
	}
private:
	rapidxml::xml_document<> m_Document;
	char *m_pXmlFile;
	UINT m_XmlFileSize;

	rapidxml::xml_node<> *m_pWorkingNode;
};