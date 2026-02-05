#ifndef INCLUDE_TEMPERATURE_FSI_TEMPERATURECONVERTER_H_
#define INCLUDE_TEMPERATURE_FSI_TEMPERATURECONVERTER_H_

// NOTES:
// TC; In this context it means ("Temperature Converter")

#ifdef __cplusplus
# 	define __FSI_DIRUTILS_TC_CPP_OPEN 		extern "C" {
# 	define __FSI_DIRUTILS_TC_CPP_CLOSE 		}
#else
# 	define __FSI_DIRUTILS_TC_CPP_OPEN
# 	define __FSI_DIRUTILS_TC_CPP_CLOSE
#endif // __cplusplus

__FSI_DIRUTILS_TC_CPP_OPEN

#include <stdbool.h>

// Celcius to others
#define __FSI_TC_CEL_TO_C(value)      (value)
#define __FSI_TC_CEL_TO_F(value)      ((value) * 9.0 / 5.0 + 32.0)
#define __FSI_TC_CEL_TO_K(value)      ((value) + 273.15)
#define __FSI_TC_CEL_TO_RE(value)     ((value) * 4.0 / 5.0)
#define __FSI_TC_CEL_TO_RA(value)     (((value) + 273.15) * 9.0 / 5.0)

// Others to Celcius
#define __FSI_TC_CEL_FROM_C(value)    (value)
#define __FSI_TC_CEL_FROM_F(value)    (((value) - 32.0) * 5.0 / 9.0)
#define __FSI_TC_CEL_FROM_K(value)    ((value) - 273.15)
#define __FSI_TC_CEL_FROM_RE(value)   ((value) * 5.0 / 4.0)
#define __FSI_TC_CEL_FROM_RA(value)   (((value) - 491.67) * 5.0 / 9.0)

/**
 * @brief Convert Celcius to `from` as a double
 * @param from Convert to from celcius
 * @param to The value of the temperature
 * @return The exact converted temperature
 */
double FSI_temperatureConvertCelTo(const char *from, const double to);

/**
 * @brief Convert from `from` to Celcius as a double
 * @param from The value of the temperature
 * @param to From celcius to the temperature
 * @return The exact converted temperature
 */
double FSI_temperatureConvertCelFrom(const char *to, const double from);

__FSI_DIRUTILS_TC_CPP_CLOSE

#endif  // INCLUDE_TEMPERATURE_FSI_TEMPERATURECONVERTER_H_

