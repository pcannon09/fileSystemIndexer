#include "../../../inc/fsi/modules/temperature/FSI_temperature.hpp"
#include "../../../inc/fsi/modules/temperature/FSI_temperatureConverter.h"

namespace fsi::modules::tc
{
	double convertTo(const std::string &from, const double &to)
	{ return FSI_temperatureConvertCelTo(from.c_str(), to); }

	double convertFrom(const std::string &to, const double &from)
	{ return FSI_temperatureConvertCelFrom(to.c_str(), from); }
}
