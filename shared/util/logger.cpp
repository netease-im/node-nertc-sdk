#include "logger.h"


namespace nertc_node {
Logger* Logger::m_instance_ = nullptr;

std::string currTime()
{
	char tmp[64];
	time_t ptime;
	time(&ptime);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&ptime));
	return tmp;
}

Logger::Logger()
{
}

void Logger::initPath(const std::string& path) {
	m_path = path;
	std::string strContent = currTime() + " : " + "---electron sdk log start---\n";
	m_outfile.open(path, std::ios::out | std::ios::app);
	m_outfile << strContent;
}


Logger::~Logger()
{
	std::string  strContent = currTime() + " : " + "---electron sdk log end---\r\n";
	if (m_outfile.is_open())
	{
		m_outfile << strContent;
	}
	m_outfile.flush();
	m_outfile.close();
}

void Logger::debug(const std::string& text)
{
	write2File(text, 0);
}

void Logger::info(const std::string& text)
{
	write2File(text, 1);
}

void Logger::warning(const std::string& text)
{
	write2File(text, 2);
}

void Logger::error(const std::string& text)
{
	write2File(text, 3);
}

void Logger::write2File(const std::string &text, int level)
{
	std::string prefix;
	switch (level)
	{
	case 0:
		prefix = "[DEBUG] ";
		break;
	case 1:
		prefix = "[INFO] ";
		break;
	case 2:
		prefix = "[WARNING] ";
		break;
	case 3:
		prefix = "[ERROR] ";
		break;
	default:
		break;
	}
	std::string outputContent = prefix + currTime() + " : " + text + "\n";
    m_outfile << outputContent;
	m_outfile.flush();
}

}