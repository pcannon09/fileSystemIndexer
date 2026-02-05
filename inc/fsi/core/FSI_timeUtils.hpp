#ifndef INCLUDE_CORE_FSI_TIMEUTILS_HPP_
#define INCLUDE_CORE_FSI_TIMEUTILS_HPP_

#include <filesystem>

namespace fs = std::filesystem;

#ifdef  FSI_OS_WIN32
# 	define __FSI_LOCALTIME(_tt, _tm) 		localtime_s(_tm, _tt)
#else
# 	define __FSI_LOCALTIME(_tt, _tm) 		localtime_r(_tt, _tm)
#endif // FSI_OS_WIN32

namespace fsi::utils
{
	typedef struct TimeUtils_DateTime
	{
		unsigned int hour;
		unsigned int min;
		unsigned int sec;
		unsigned int ms;

		unsigned int year;
		unsigned int month;
		unsigned int day;
	} TimeUtils_DateTime;

	/**
	 * @brief Set std::filesystem clock data to an actual usable Date and Time format
	 * @param time Get `std::filesystem::file_time_type` that is to be converted to a usuable time format
	 * @return Date-Time object to be used
	 */
	TimeUtils_DateTime fsClockDataToDT(const fs::file_time_type &time);
}

#endif  // INCLUDE_CORE_FSI_TIMEUTILS_HPP_
