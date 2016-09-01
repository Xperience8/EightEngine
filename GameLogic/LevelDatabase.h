#pragma once

#include "EightEngine\Xml\XmlDocument.h"

class LevelDatabase
{
public:
	LevelDatabase() { }
	~LevelDatabase() { }

	bool Init();

	void StartNewGame();

	void LevelCompleted(const std::string &levelName, UINT numStars);

	XmlNode GetLevelInfo(const std::string &levelName)
	{
		return m_pLevelDatabaseConfig->GetRootNode().GetChildrenNode(levelName);
	}

	void Save()
	{
		m_pLevelDatabaseConfig->SaveToFile(PLAYER_LEVEL_DATABASE);
	}

private:
	const std::string BASIC_LEVEL_DATABASE = "LevelDatabase.xml";
	const std::string PLAYER_LEVEL_DATABASE = "LevelDatabaseAutosave.xml";

	std::shared_ptr<XmlDocument> m_pLevelDatabaseConfig;
};

extern LevelDatabase g_LevelDatabase;