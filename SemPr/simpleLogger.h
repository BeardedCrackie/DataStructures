#pragma once
#include <string>
#include <iostream>

enum LogLevel {LOG_DEBUG = 1, LOG_INFO = 2, LOG_ERROR = 5};

extern LogLevel LOGLEVEL = LOG_INFO;

class SimpleLogger
{
public:
	static void log(LogLevel level, std::string message) {
		if (level >= LOGLEVEL) {
			std::string lvl_name;
			switch (level) {
			case LOG_INFO:
				lvl_name = "INFO";
				break;
			case LOG_ERROR:
				lvl_name = "ERROR";
				break;
			default: //LOG_DEBUG
				lvl_name = "DEBUG";
				break;
			}
			std::cout << "[" << lvl_name << "]: " << message << std::endl;
		}
	}
};

