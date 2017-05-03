#include <hurricane/base/DataPackage.h>
#include <hurricane/message/Command.h>

using hurricane::base::DataPackage;
using hurricane::message::Command;

Command::Command(DataPackage const& dataPackage) {

}

DataPackage Command::ToDataPackage()const {
	DataPackage dataPackage;
	dataPackage.AddVariant(int(m_type));
	return dataPackage;
}
