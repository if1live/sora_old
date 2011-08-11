// Ŭnicode please 
#pragma once

namespace mio
{
	class Console;
	class ConsoleCore;
	
	class ConsoleCore {
	public:
		virtual void log(const std::string &msg) const = 0;
		virtual void timelog(const std::string &msg) const = 0;
	};
	
	class Console {
	public:
		static void log(const char *fmt, ...);
		static void log(const std::string &msg);
		
		static void timelog(const char *fmt, ...);
		static void timelog(const std::string &msg);
		
		static ConsoleCore &console();
	};

#if _WIN_
	class ConsoleCoreWin : public ConsoleCore {
	public:
		virtual void log(const std::string &msg) const;
		virtual void timelog(const std::string &msg) const;
	};
#endif
}