#pragma once

#include "HttpContext.h"

namespace meshy {
	class HttpRequest : public HttpContext {
	public:
		void ParseStdString(std::string const& text);
		virtual void ParseStdStringList(StdStringList const& stringList)override;

		static HttpRequest FromStdString(std::string const text);
		static HttpRequest FromStdStringList(StdStringList const& stringList);

		std::string const& method()const;
		void method(std::string const& m);
		std::string const& path()const;
		void path(std::string const& p);

	private:
		void ParseRequestLine(std::string const& requestLine);
	private:
		std::string m_method;
		std::string m_path;
	};
}