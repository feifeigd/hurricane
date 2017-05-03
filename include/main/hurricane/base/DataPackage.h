#pragma once

#include "Variant.h"

#include <meshy.h>

#include <memory>

namespace hurricane {
	namespace base {
		
		class Variant;
		
		class DataPackage {
		public:
			DataPackage()  : m_version(0), m_length(0){}

			void AddVariant(Variant const& variant) { m_variants.push_back(variant); }

			meshy::ByteArray serialize();

			void deserialize(meshy::ByteArray const& data);
		private:
			meshy::ByteArray SerializeBody()const;
			meshy::ByteArray SerializeHead(size_t bodySize);

			void SerializeVariant(meshy::ByteArrayWriter& writer, Variant const& variant)const;

		private:
			int8_t		m_version;
			int32_t		m_length;
			Variants	m_variants;
		};
	}
}