#pragma once

#include <meshy.h>

namespace hurricane {
	namespace base {
		class Writable {
		public:
			virtual size_t read(meshy::ByteArrayReader& reader, Variant& variant) = 0;
			virtual size_t write(meshy::ByteArrayWriter& writer, Variant const& variant) = 0;
		};
	}
}
