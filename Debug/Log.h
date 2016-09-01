#pragma once

class Logger
{
private:

public:
	Logger() { }
	~Logger() { }

	static void Log(const std::string &message, const std::string &title)
	{
		std::string finalMessage = title + ": "  + message + "\n";

		OutputDebugStringA(finalMessage.c_str());
	}

};

#ifndef EIGHT_ERROR
	#if defined(_DEBUG)
		#define EIGHT_ERROR(message) Logger::Log(message, "Error")
	#else
		#define EIGHT_ERROR(message) 
	#endif
#endif

#ifndef EIGHT_WARNING
	#if defined(_DEBUG)
		#define EIGHT_WARNING(message) Logger::Log(message, "Warning")
	#else
		#define EIGHT_WARNING(message) 
	#endif
#endif

#ifndef EIGHT_LOG
	#if defined(_DEBUG)
		#define EIGHT_LOG(message) Logger::Log(message, "Log")
	#else
		#define EIGHT_LOG(message)
	#endif
#endif

#ifndef EIGHT_ASSERT

	#if defined(_DEBUG)
		#define EIGHT_ASSERT(expression, message) \
			if (!(expression)) \
			{ \
				std::string finalMessage = "\nFile: " + std::string(__FILE__) + "\nLine: " + std::to_string(__LINE__); \
				finalMessage += "\nFunction: " + std::string(__FUNCTION__) + "\nExpression: " + message; \
				finalMessage += "\nDo you want to start debugging?"; \
				int answer = MessageBoxA(nullptr, finalMessage.c_str(), "Assert", MB_YESNO | MB_ICONERROR); \
				if (answer == IDYES) \
				{ \
					DebugBreak(); \
				} \
			} 
	#else
		#define EIGHT_ASSERT(expression, message) 
	#endif
#endif