#pragma once

#include "Process.h"

class ProcessManager
{
protected:
	std::list<std::shared_ptr<Process> > m_ProcessList;

	ULONG m_NumProcesses;
	ULONG m_FailedProcesses;
	ULONG m_SuccededProcesses;

	void OnSucced(std::shared_ptr<Process> &pProcess) { pProcess->VOnSucced(); ++m_SuccededProcesses; }
	void OnFailed(std::shared_ptr<Process> &pProcess) { pProcess->VOnFailed(); ++m_FailedProcesses; }
public:
	ProcessManager() : m_FailedProcesses(0), m_SuccededProcesses(0), m_NumProcesses(0) { }
	~ProcessManager() { }

	void StartUp();
	void ShutDown();

	void Update(float DeltaTime);
	
	void AttachProcess(std::shared_ptr<Process> &pProcess) 
	{
		m_ProcessList.push_front(pProcess); 
		++m_NumProcesses; 
	}
	void RemoveProcess(std::shared_ptr<Process> &pProcess)
	{
		pProcess->VOnFailed();
		m_ProcessList.remove(pProcess);
	}
};

extern ProcessManager g_ProcessManager;
