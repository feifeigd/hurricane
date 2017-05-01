#pragma once

#include <functional>
#include <cstdint>
#include <map>

namespace hurricane {
	namespace message {

		class Message;

		class MessageLoop
		{
			MessageLoop(MessageLoop const&) = delete;
			MessageLoop const& operator=(MessageLoop const&) = delete;
		public:
			typedef std::function<void(Message*)>	MessageHandler;

			MessageLoop();

			template<typename ObjectType, typename MethodType>
			void MessageMap(int messageType, ObjectType* self, MethodType method) {
				MessageMap(messageType, std::bind(method, self, std::placeholders::_1));
			}

			void MessageMap(int messageType, MessageHandler handler) {
				m_messageHandlers.insert({ messageType , handler });
			}

			// 启动消息队列
			void run();

			// 停止消息队列
			void stop();

			// 向消息队列投递消息
			void post(Message* message);

		private:
			std::map<int, MessageHandler>	m_messageHandlers;
			uint32_t						m_threadId;
		};
	}
}
