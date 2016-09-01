#pragma once

enum class ProcessState { RUNNING, SUCCEDED, FAILED };

// Singlethreaded version
class Process
{
protected:
	ProcessState m_ProcessState;
	std::shared_ptr<Process> m_pChildProcess;
public:
	Process() : m_ProcessState(ProcessState::RUNNING) { }
	virtual ~Process() { }

	virtual void VUpdate(float DeltaTime) = 0;

	virtual void VOnSucced() = 0;
	virtual void VOnFailed() = 0;

	bool HasChildProcess() const { return (m_pChildProcess != nullptr); }
	void AddChildProcess(std::shared_ptr<Process> &pProcess) { m_pChildProcess = pProcess; }
	std::shared_ptr<Process> RemoveChildProcess()
	{
		auto pProcess = m_pChildProcess;
		m_pChildProcess.reset();
		return pProcess;
	}

	ProcessState GetProcessState() const { return m_ProcessState; }
};