#pragma once

class XmlDocument;
class XmlNode;

class GameApp
{
protected:
	GameApp() : m_Running(false) { }
	~GameApp() { }

	bool CheckRequirements(const XmlNode &configNode);
	bool CheckOneInstance(const std::string &appName);

	std::shared_ptr<XmlDocument> m_pConfig;

	bool m_Running;
private:
	bool CheckRAMSize(ULONG neededPhysMemory, ULONG neededVirtualMemory);
	bool CheckHDDSize(ULONG neededMemory);
};