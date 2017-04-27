
#include <http/HttpContext.h>
#include <utils/String.h>

std::string const& HttpContext::header(std::string const& name)const {
	return m_headers.at(name);
}

void HttpContext::header(std::string const& name, std::string const& value) {
	m_headers[name] = value;
}

bool HttpContext::HasHeader(std::string const& name)const {
	return  m_headers.find(name) != m_headers.end();
}

StdStringList HttpContext::GetHeaderNames()const {
	StdStringList header_names;
	for (auto const& pair : m_headers) {
		header_names.push_back(pair.first);
	}
	return header_names;
}

std::string const& HttpContext::content()const {
	return m_content;
}

void HttpContext::content(std::string const& ct) {
	m_content = ct;
	size_t contentLen = ct.size();
	if (contentLen)
	{
		header("Content-Length", itos(contentLen));
	}
}

std::string const& HttpContext::version()const {
	return m_version;
}

void HttpContext::version(std::string const& ver) {
	m_version = ver;
}

void HttpContext::ParseStdStringList(StdStringList const& stringList) {
	for (std::string const& line : stringList) {
		StdStringList words = SplitString(line, ':');
		if (words.empty())return;
		std::string& headerName = words[0];
		std::string& headerValue = words[1];
		if (words.size() > 2)
		{
			for (size_t i = 2; i < words.size(); ++i) {
				headerValue += ':';
				headerValue += words[i];
			}
		}
		headerValue.erase(headerValue.begin());
		header(headerName, headerValue);
	}
}

std::string HttpContext::ToStdString()const {
	std::string result;
	for (auto const& headerPair : m_headers)
	{
		result += (headerPair.first + ':' + headerPair.second + "\r\n");
	}
	if (m_content.size())
	{
		result += "\r\n";
		result += m_content;
	}
	return result;
}

HttpContext HttpContext::FromStdStringList(StdStringList const& stringList) {
	HttpContext context;
	context.ParseStdStringList(stringList);
	return context;
}