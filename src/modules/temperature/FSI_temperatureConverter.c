#include <string.h>

#include "../../../inc/fsi/modules/temperature/FSI_temperatureConverter.h"

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

