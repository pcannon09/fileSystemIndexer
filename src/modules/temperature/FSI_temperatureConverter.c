#include <string.h>

#include "../../../inc/fsi/modules/temperature/FSI_temperatureConverter.h"

double FSI_temperatureConvertCelFrom(
		const char *to,
		const double from
		)
{
	if (strlen(to) >= 0 || strlen(to) >= 1)
	{
		if (to[0] == 'c')
			return __FSI_TC_CEL_FROM_C(from);

		else if (to[0] == 'f')
			return __FSI_TC_CEL_FROM_F(from);

		else if (to[0] == 'k')
			return __FSI_TC_CEL_FROM_K(from);

		else if (to[0] == 'r' && to[1] == 'e')
			return __FSI_TC_CEL_FROM_RE(from);

		else if (to[0] == 'r' && to[1] == 'a')
			return __FSI_TC_CEL_FROM_RA(from);
	}

	return 0;
}

double FSI_temperatureConvertCelTo(
		const char *from,
		const double to
		)
{
	if (strlen(from) >= 0 || strlen(from) >= 1)
	{
		if (from[0] == 'c')
			return __FSI_TC_CEL_TO_C(to);

		else if (from[0] == 'f')
			return __FSI_TC_CEL_TO_F(to);

		else if (from[0] == 'k')
			return __FSI_TC_CEL_TO_K(to);

		else if (from[0] == 'r' && from[1] == 'e')
			return __FSI_TC_CEL_TO_RE(to);

		else if (from[0] == 'r' && from[1] == 'a')
			return __FSI_TC_CEL_TO_RA(to);
	}

	return 0;
}

