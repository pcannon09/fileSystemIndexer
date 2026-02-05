#ifndef INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_
#define INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_

#include <string>

namespace fsi::modules::tc
{
	/**
	 * @brief Wrapper for `FSI_temperatureConvertCelTo()` function
	 * @param from Convert to from Celcius
	 * @param to The value of the temperature
	 * @return The exact temperature
	 */
	double convertTo(const std::string &from, const double &to);

	/**
	 * @brief Wrapper for `FSI_temperatureConvertCelFrom()` function
	 * @param from The value of the temperature
	 * @param to From Celcius to the temperature
	 * @return The exact temperature
	 */
	double convertFrom(const std::string &to, const double &from);
}

#endif  // INCLUDE_TEMPERATURE_FSI_TEMPERATURE_HPP_

