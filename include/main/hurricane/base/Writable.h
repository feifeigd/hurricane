#pragma once

namespace hurricane {
	namespace base {
		class Writable {
		public:
			virtual size_t read(ByteArrayreader& reader, Variant& variant) = 0;
			virtual size_t write(ByteArrayWriter& writer, Variant const& variant) = 0;
		};
	}
}
