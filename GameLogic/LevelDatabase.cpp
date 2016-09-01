#include "stdafx.h"
#include "LevelDatabase.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

LevelDatabase g_LevelDatabase;

bool LevelDatabase::Init()
{
	if (!g_ResourceCache.LoadResource(PLAYER_LEVEL_DATABASE, XmlResourceLoader::s_SIGNATURE))
	{
		if (!g_ResourceCache.LoadResource(BASIC_LEVEL_DATABASE, XmlResourceLoader::s_SIGNATURE))
		{
			EIGHT_ERROR("There isn't basic level database: " + BASIC_LEVEL_DATABASE);
			return false;
		}

		// Create from basic level database player level database
		auto pBaseLevelDatabaseConfig = g_ResourceCache.RequestResource<XmlDocument>(
			BASIC_LEVEL_DATABASE);
		std::string str = pBaseLevelDatabaseConfig->SaveToString();

		if (!g_ResourceCache.LoadResource(reinterpret_cast<BYTE*>(const_cast<char*>(str.c_str())),
			str.size(), XmlResourceLoader::s_SIGNATURE, PLAYER_LEVEL_DATABASE))
		{
			EIGHT_ERROR("There is a problem from creating player level database");
			return false;
		}
	}

	m_pLevelDatabaseConfig = g_ResourceCache.RequestResource<XmlDocument>(PLAYER_LEVEL_DATABASE);
	return true;
}
void LevelDatabase::StartNewGame()
{
	if (!g_ResourceCache.LoadResource(BASIC_LEVEL_DATABASE, XmlResourceLoader::s_SIGNATURE))
	{
		EIGHT_ERROR("There isn't basic level database: " + BASIC_LEVEL_DATABASE);
		return;
	}
	m_pLevelDatabaseConfig = g_ResourceCache.RequestResource<XmlDocument>(BASIC_LEVEL_DATABASE);

	std::remove(PLAYER_LEVEL_DATABASE.c_str());
}
void LevelDatabase::LevelCompleted(const std::string &levelName, UINT numStars)
{
	auto levelConfigNode = GetLevelInfo(levelName);

	// If we're getting more stars then in previous try, save it
	auto starsNode = levelConfigNode.GetChildrenNode("Stars");
	if (numStars > static_cast<UINT>(std::stoi(starsNode.GetValue())))
	{
		starsNode.ChangeValue(std::to_string(numStars));
	}

	// Unlock next level
	levelConfigNode.GoToNextNode();
	if (levelConfigNode.Exist())
	{
		levelConfigNode.GetChildrenNode("Unlocked").ChangeValue("1");
	}
	// Send event that player completed whole game
	// else
}