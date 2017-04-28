#pragma once

#include "HttpContext.h"

namespace meshy {
	class HttpResponse : public HttpContext{
	public:
		HttpResponse();
		virtual std::string ToStdString()const override;

		size_t GetStatusCode()const;
		void SetStatusCode(size_t statusCode);
		
		std::string const& GetStatusMessage()const;
		void SetStatusMessage(std::string const& message);

	private:
		std::string GetResponseLine()const;

		size_t m_statusCode;
		std::string	m_status_Message;
	};
}
