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

#define __FSI_TC_CEL_TO_C(value) 		value
#define __FSI_TC_CEL_TO_F(value) 		(value - 32) * 5 / 9
#define __FSI_TC_CEL_TO_K(value) 		value - 273.15
#define __FSI_TC_CEL_TO_RE(value) 		value * 5 / 4
#define __FSI_TC_CEL_TO_RA(value) 		(value - 491.67) * 5 / 9

#define __FSI_TC_CEL_FROM_C(value) 			value
#define __FSI_TC_CEL_FROM_F(value) 			(value * 9 / 2) + 32
#define __FSI_TC_CEL_FROM_K(value) 			value + 273.15
#define __FSI_TC_CEL_FROM_RE(value) 		value * 4 / 5
#define __FSI_TC_CEL_FROM_RA(value) 		(value - 273.15) * 9 / 5

double FSI_temperatureConvertCelTo(
		const char *from,
		const double to
		);

__FSI_DIRUTILS_TC_CPP_CLOSE

#endif  // INCLUDE_TEMPERATURE_FSI_TEMPERATURECONVERTER_H_

