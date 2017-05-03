#pragma once

namespace hurricane {
	namespace base {
		class Node {
		public:
			enum class Status {
				Dead,
				Alived,
			};

			Node(std::string const& name = "", NetAddress const& address = {"", 0})
				: m_name(name), m_address(address), m_status(Status::Dead){}

			void alive() { m_lastLiveTime = time(nullptr); }

			void status(Status status) { m_status = status; }

			Status UpdateStatus() {
				time_t currentTime = 0;
				if (m_lastLiveTime - currentTime > 9) { m_status = Status::Dead; }
				return m_status;
			}

		private:
			std::string		m_name;
			NetAddress		m_address;
			Status			m_status;
			time_t			m_lastLiveTime;
		};
	}
}
