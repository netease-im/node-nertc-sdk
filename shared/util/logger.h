#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <time.h>
#include <mutex>
#include <string>

#pragma warning (disable:4996)

namespace nertc_node {

	class Logger
	{

	public:
		static Logger* Instance() {
			if (nullptr == m_instance_)
			{
				static std::recursive_mutex m_instance__lock_;
				std::lock_guard<std::recursive_mutex> auto_lock(m_instance__lock_);
				if (nullptr == m_instance_) {
					m_instance_ = new Logger();
				}
			}
			return m_instance_;
		}
	public:
		Logger();
		~Logger();

		void initPath(const std::string& path);
		void debug(const std::string& text);
		void info(const std::string& text);
		void warning(const std::string& text);
		void error(const std::string& text);

		static std::string boolToStr(bool var) {return std::to_string(var);};
		static std::string int32ToStr(int  var) { return  std::to_string(var); };
		static std::string uint32ToStr(unsigned int var) { return  std::to_string(var); };
		static std::string int64ToStr(long long  var) { return  std::to_string(var); };
		static std::string uint64ToStr(unsigned long long  var) { return  std::to_string(var); };
		static std::string floatToStr(float  var) { return  std::to_string(var); };
		static std::string doubleToStr(double var) { return  std::to_string(var); };
		static std::string longDoubleToStr(long double var) { return  std::to_string(var); };


	private:
		void write2File(const std::string &text, int level);

	private:
		std::ofstream m_outfile; 
		std::string m_path;

	private:
		static Logger * m_instance_;
	};

}



#endif//_LOGGER_H_