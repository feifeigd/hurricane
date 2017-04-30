#pragma once

#include <exception>
#include <string>

namespace hurricane {
	namespace base {
		class TypeMismatchException : std::exception {
		public:
			TypeMismatchException(std::string const& message) : m_message(message){}

			char const* what()const noexcept override { return m_message.c_str(); }
		private:
			std::string	m_message;
		};
	}
}