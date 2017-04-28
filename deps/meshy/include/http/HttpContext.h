#pragma once

#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> StdStringList;

class HttpContext {
public:
	std::string const& header(std::string const& name)const;
	void header(std::string const& name, std::string const& value);
	bool HasHeader(std::string const& name)const;

	StdStringList GetHeaderNames()const;

	std::string const& content()const;
	void content(std::string const& ct);
	
	std::string const& version()const;
	void version(std::string const& ver);

	virtual void ParseStdStringList(StdStringList const& stringList);
	virtual std::string ToStdString()const;

	static HttpContext FromStdStringList(StdStringList const& stringList);
protected:
	std::string							m_version;
private:
	std::map<std::string, std::string>	m_headers;
	std::string							m_content;
};