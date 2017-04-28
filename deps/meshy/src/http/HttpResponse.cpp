#include <http/HttpResponse.h>
#include <utils/String.h>

using meshy::HttpResponse;

HttpResponse::HttpResponse()
	: m_statusCode(0)
{

}

std::string HttpResponse::ToStdString()const {
	return GetResponseLine() + HttpContext::ToStdString();
}

size_t HttpResponse::GetStatusCode()const {
	return m_statusCode;
}

void HttpResponse::SetStatusCode(size_t statusCode) {
	m_statusCode = statusCode;
}

std::string const& HttpResponse::GetStatusMessage()const {
	return m_status_Message;
}

void HttpResponse::SetStatusMessage(std::string const& message) {
	m_status_Message = message;
}

std::string HttpResponse::GetResponseLine()const {
	return m_version + ' ' + itos(m_statusCode) + ' ' + m_status_Message;
}