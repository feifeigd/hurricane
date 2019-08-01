#include <hurricane/message/Message.h>
#include <hurricane/message/MessageLoop.h>

#include <iostream>

#ifdef OS_WIN32
#include <Windows.h>
#endif // OS_WIN32

using hurricane::message::Message;
using hurricane::message::MessageLoop;

MessageLoop::MessageLoop() : m_threadId(
#ifdef OS_WIN32
	GetCurrentThreadId()
#else
	0
#endif // OS_LINUX
) 
{ }

void MessageLoop::run() {
#ifdef OS_WIN32
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		std::cout << "Recived Message" << std::endl;
		Message* pMsg = (Message*)msg.wParam;
		auto handler = m_messageHandlers.find(msg.message);
		if (handler != m_messageHandlers.end())
		{
			handler->second(pMsg);
		}
		DispatchMessage(&msg);
		if (Message::Type::Stop == msg.message)
		{
			delete pMsg;
			break;
		}
	}
#else
	/*
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		std::cout << "Recived Message" << std::endl;
		auto handler = m_messageHandlers.find(msg.message);
		Message* pMessage = (Message*)msg.wParam;
		if (handler != m_messageHandlers.end())
		{
			handler->second(pMessage);
		}
		DispatchMessage(&msg);
		delete pMessage;
		if (Message::Type::Stop == msg.message)
		{
			break;
		}
	}
	*/

#endif // OS_WIN32
}

void MessageLoop::post(Message* message) {
#ifdef OS_WIN32
	PostThreadMessage(m_threadId, message->type(), WPARAM(message), 0);
#endif // OS_WIN32
}

void MessageLoop::stop() {
	post(new Message(Message::Type::Stop));
}
