#ifndef INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_
#define INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_

#include <string>

namespace fsi::modules::tc
{
	double convertTo(
			const std::string &from,
			const double &to
			);
}

#endif  // INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_

