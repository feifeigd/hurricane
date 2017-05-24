#pragma once

namespace meshy {

	class HttpDataSink {
		virtual OnDataIndication(TcpStream* stream, HttpRequest const& reqauest) = 0;
	};
}
