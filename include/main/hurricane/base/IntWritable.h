#pragma once

#include "Writable.h"

namespace hurricane {
	namespace base {
		class IntWritable : public Writable {
		public:
			virtual size_t read(meshy::ByteArrayReader& reader, Variant& variant)override {
				int32_t intValue = reader.readInt32BE();
				variant.SetIntValue(intValue);
				return sizeof(int32_t);
			}

			virtual size_t write(meshy::ByteArrayWriter& writer, Variant const& variant)override {
				writer.writeInt32BE(variant.GetIntValue());
				return sizeof(int32_t);
			}
		};
	}
}
