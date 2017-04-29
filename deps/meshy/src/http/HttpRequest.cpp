#include <http/HttpRequest.h>
#include <utils/String.h>

using meshy::HttpRequest;

void HttpRequest::ParseStdString(std::string const& text) {
	StdStringList stringList = SplitString(text, '\n');
	for (auto& line : stringList)line.pop_back();
	ParseStdStringList(stringList);
}

void HttpRequest::ParseStdStringList(StdStringList const& stringList) {
	std::string requestLine = stringList.front();
	ParseRequestLine(requestLine);
	StdStringList contextLines = stringList;
	contextLines.erase(contextLines.begin());
	HttpContext::ParseStdStringList(contextLines);
}

void HttpRequest::ParseRequestLine(std::string const& requestLine) {
	StdStringList words = SplitString(requestLine, ' ');
	if (words.empty())return;
	method(words[0]);
	path(words[1]);
	version(words[2]);
}

HttpRequest HttpRequest::FromStdString(std::string const& text) {
	HttpRequest request;
	request.ParseStdString(text);
	return request;
}

HttpRequest HttpRequest::FromStdStringList(StdStringList const& stringList) {
	HttpRequest request;
	request.ParseStdStringList(stringList);
	return request;
}

std::string const& HttpRequest::method()const {
	return m_method;
}

std::string const& HttpRequest::path()const {
	return m_path;
}

void HttpRequest::method(std::string const& m) {
	m_method = m;
}

void HttpRequest::path(std::string const& p) {
	m_path = p;
}
