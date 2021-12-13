#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

namespace debug {

	enum class Type {
		FATAL, ERROR, WARNING, INFO
	};

	constexpr auto error = Type::ERROR;
	constexpr auto warning = Type::WARNING;
	constexpr auto info = Type::INFO;
	constexpr auto fatal = Type::FATAL;

	std::string_view toString(Type type) {
		switch (type) {
			case Type::ERROR:   return "[Error] ";   break;
			case Type::WARNING: return "[Warning] "; break;
			case Type::INFO:    return "[Info] ";    break;
			case Type::FATAL:   return "[Fatal] ";   break;
		}
		return "";
	}
	
	class Log {
		class LogLine {
		public:
			LogLine(auto & outs, Type type) : outs(outs) { 
				ss << toString(type);
			}
			LogLine(const LogLine &) = delete;
			LogLine(LogLine &&) = delete;
			LogLine& operator=(const LogLine &) = delete;
			LogLine& operator=(LogLine &&) = delete;
			~LogLine() {
				ss << "\n";
				for(auto out : outs) {
					(*out) << ss.str();
				}
			}
			std::ostream & operator<<(const auto & value) {
				return this->ss << value;
			}
		private:
			std::vector<std::ostream*> & outs; 
			std::stringstream ss;
		};
	public:
		Log() = default;
		Log(const Log &) = delete;
		Log(Log &&) = delete;
		Log operator=(const Log &) = delete;
		Log operator=(Log &&) = delete;

		void registerOutput(std::ostream & out) {
			outs.push_back(&out);
		}

		auto operator<<(Type type) {
			return LogLine(outs, type);
		}
	private:
		std::vector<std::ostream*> outs;
	} log;
}

