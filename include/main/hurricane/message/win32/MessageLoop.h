#pragma once

class Message;

class MessageLoop
{
	MessageLoop(MessageLoop const&) = delete;
	MessageLoop const& operator=(MessageLoop const&) = delete;
public:
	typedef std::function<void(Message*)>	MessageHandler;

	MessageLoop() m_threadId(GetCurrentThreadId()) { }
	
	template<typename ObjectType,typename MethodType>
	void MessageMap(int messageType, ObjectType* self, MethodType method) {
		MessageMap(messageType, std::bind(method, self, std::placeholders::_1));
	}

	void MessageMap(int messageType, MessageHandler handler) {
		m_messageHandlers.insert({ messageType , handler});
	}

	// 启动消息队列
	void run() {
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0) {
			std::cout << "Recived Message" << std::endl;
			auto handler = m_messageHandlers.find(msg.messsage);
			if (handler != m_messageHandlers.end())
			{
				handler->second((Message*)msg.wParam);
			}
			DispatchMessage(&msg);
			if (Message::Type::Stop == msg.message)
			{
				break;
			}
		}
	}

	// 停止消息队列
	void stop(){
		//TODO
	}

	// 向消息队列投递消息
	void post(Message* message) {
		PostThreadMessage(m_threadId, message->type(), WPARAM(message), 0);
	}

private:
	std::map<int, MessageHandler>	m_messageHandlers;
	DWORD	m_threadId;
};
