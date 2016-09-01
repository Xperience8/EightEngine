#include "stdafx.h"
#include "ProcessManager.h"

ProcessManager g_ProcessManager;

void ProcessManager::Update(float DeltaTime)
{
	auto processIT = m_ProcessList.begin();
	while (processIT != m_ProcessList.end())
	{
		auto pProcess = *processIT;
		if (pProcess->GetProcessState() == ProcessState::RUNNING)
		{
			pProcess->VUpdate(DeltaTime);
			processIT++;
		}
		else {
			if (pProcess->GetProcessState() == ProcessState::SUCCEDED)
			{
				OnSucced(pProcess);
				if (pProcess->HasChildProcess())
					AttachProcess(pProcess->RemoveChildProcess());
			}
			else if (pProcess->GetProcessState() == ProcessState::FAILED)
			{
				OnFailed(pProcess);
			}
			processIT = m_ProcessList.erase(processIT);
		}
	}
}

void ProcessManager::StartUp()
{
	
}
void ProcessManager::ShutDown()
{
	m_ProcessList.clear();

	EIGHT_LOG("Process manager processed " + std::to_string(m_NumProcesses) + " processes");
	EIGHT_LOG(std::to_string(m_SuccededProcesses) + " processes succeded");
	EIGHT_LOG(std::to_string(m_FailedProcesses) + " processes failed");
	
	m_NumProcesses = 0;
	m_SuccededProcesses = 0;
	m_FailedProcesses = 0;
}