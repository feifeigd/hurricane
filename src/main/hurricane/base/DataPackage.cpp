#include <ByteArrayWriter.h>
#include <hurricane/base/DataPackage.h>
#include <hurricane/base/IntWritable.h>

using hurricane::base::DataPackage;
using hurricane::base::Variant;

using meshy::ByteArray;
using meshy::ByteArrayWriter;

namespace hurricane {
	namespace base {
		std::map<int8_t, std::shared_ptr<Writable> > Writables = {
			{0, std::shared_ptr<Writable>(new IntWritable)},
		};
	}
}

ByteArray DataPackage::serialize() {
	ByteArray body = SerializeBody();
	ByteArray head = SerializeHead(body.size());
	return head + body;
}

void DataPackage::deserialize(meshy::ByteArray const& data) {

}

ByteArray DataPackage::SerializeBody()const {
	ByteArrayWriter bodyWriter;
	for (auto const& variant : m_variants)SerializeVariant(bodyWriter, variant);
	return bodyWriter.ToByteArray();
}

meshy::ByteArray DataPackage::SerializeHead(size_t bodySize) {
	ByteArrayWriter headWriter;
	m_length = sizeof(int32_t) + sizeof(m_version) + bodySize;
	return headWriter.ToByteArray();
}

void DataPackage::SerializeVariant(ByteArrayWriter& writer, Variant const& variant)const {
	Variant::Type type = variant.type();
	int8_t typeCode = Variant::TypeCodes[type];
	std::shared_ptr<Writable> writable = Writables[typeCode];

	writer.write<int8_t>(typeCode);
	writable->write(writer, variant);
}