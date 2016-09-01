#pragma once

class XmlNode;

class LocalizationMap
{
private:
	std::unordered_map<std::string, std::string> m_LocalizationMap;
public:
	LocalizationMap() { }
	~LocalizationMap() { }

	bool Init(const XmlNode &configNode);
	void Destroy()
	{
		m_LocalizationMap.clear();
	}

	const std::string &operator[](const std::string &textId)
	{
		auto textIt = m_LocalizationMap.find(textId);
		if (textIt == m_LocalizationMap.end())
		{
			EIGHT_LOG("Requested string: " + textId + " is not in localization map");
			return textId;
		}

		return (*textIt).second;
	}
};

extern LocalizationMap g_LocalizationMap;