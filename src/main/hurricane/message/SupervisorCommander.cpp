#include <meshy.h>
#include <hurricane/base/Values.h>
#include <hurricane/base/Variant.h>
#include <hurricane/base/DataPackage.h>
#include <hurricane/message/Command.h>
#include <hurricane/message/SupervisorCommander.h>
#include <iostream>

using hurricane::base::DataPackage;
using hurricane::base::Values;
using hurricane::base::Variants;
using hurricane::base::NetAddress;
using hurricane::message::SupervisorCommander;
using meshy::ByteArray;

size_t const DATA_BUFFER_SIZE = 65535;

SupervisorCommander::SupervisorCommander(NetAddress const& nimbusAddress, std::string const& supervisorName)
	: m_nimbusAddress(nimbusAddress), m_supervisorName(supervisorName)
{

}

void SupervisorCommander::connect() {
	if (!m_connector) {
		m_connector = std::make_shared<NetConnector>(m_nimbusAddress);
		m_connector->connect();
	}
}

void SupervisorCommander::join() {
	connect();

	Command command(Command::Type::Join, {m_supervisorName});
	DataPackage messagePackage = command.ToDataPackage();
	ByteArray message = messagePackage.serialize();
	char resultBuffer[DATA_BUFFER_SIZE];
	size_t resultSize = m_connector->SendAndReceive(message.data(), message.size(), resultBuffer, DATA_BUFFER_SIZE);
	ByteArray result(resultBuffer, resultSize);
	DataPackage resultPackage;
	resultPackage.deserialize(result);
	command = Command(resultPackage);
	std::cout << command.type() << "\n"
		<< command.arg(0).GetStringValue() << std::endl;
}

void SupervisorCommander::alive() {
	connect();
	Command command(Command::Type::Alive, {m_supervisorName});
	DataPackage messagePackage = command.ToDataPackage();
	ByteArray message = messagePackage.serialize();
	char resultBuff[DATA_BUFFER_SIZE];
	size_t resultSize = m_connector->SendAndReceive(message.data(), message.size(), resultBuff, DATA_BUFFER_SIZE);
	ByteArray result(resultBuff, resultSize);
	DataPackage resultPackage;
	resultPackage.deserialize(result);
	command = Command(resultPackage);
}

void SupervisorCommander::SendTuple(int taskIndex, Values const& values) {
	connect();
	Variants args{ m_supervisorName, taskIndex };
	for (auto const& value : values) {
		args.push_back(value.ToVariant());
	}
}

void SupervisorCommander::RandomDestination() {

}
