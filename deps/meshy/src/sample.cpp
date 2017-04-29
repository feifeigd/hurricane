
#include <EventQueueLoop.h>
#include <PackageDataSink.h>
#include <http/HttpServer.h>
#include <iostream>
using namespace meshy;
using namespace std;

class SampleEventQueueLoop : public meshy::EventQueueLoop {
public:
	SampleEventQueueLoop(EventQueue* queue) :EventQueueLoop(queue){

	}

protected:
	virtual void OnEvent(std::shared_ptr<BaseEvent> event)override {

	}
};

int main(){
	meshy::EventQueue mainEventQueue(5);
	IoLoop::get().start();

	meshy::PackageDataSink dataSink(&mainEventQueue);

	meshy::HttpServer server(&dataSink);
	server.OnConnection([&](meshy::HttpConnection* connection) {
		connection->connection()->SetDataSink(&dataSink);
		connection->OnRequest([connection](HttpRequest const& request) {
			cout << "Request arrived\n"
				<< request.method() << "\n"
				<< request.path() << "\n"
				<< request.version() << endl;

			HttpResponse response;
			response.content("Hello, world!");
			connection->SendResponse(response);
		});
		connection->OnData([connection](std::string const& data) {
			cout << "Data arrived" << "\n" << data << endl;
		});
	});
	server.listen("127.0.0.1", 9000);

	SampleEventQueueLoop sampleQueue(&mainEventQueue);
	sampleQueue.start();
	return 0;
}

