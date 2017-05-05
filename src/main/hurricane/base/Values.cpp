#include <hurricane/base/Values.h>

using hurricane::base::Value;
using hurricane::base::Values;

Values::Values(std::initializer_list<Value> values) : std::vector<Value>(values)
{

}

