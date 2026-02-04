#include "../../inc/fsi/core/FSI_timeUtils.hpp"

#include <chrono>
#include <ctime>

namespace fsi::utils
{
	TimeUtils_DateTime fsClockDataToDT(const fs::file_time_type &time)
	{
		TimeUtils_DateTime dt{0};

		const auto &sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				time - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
		);

		auto epochSec = std::chrono::time_point_cast<std::chrono::seconds>(sctp);
		auto epochMs = std::chrono::duration_cast<std::chrono::microseconds>(sctp - epochSec).count();

		std::time_t timeInfo = std::chrono::system_clock::to_time_t(epochSec);
		std::tm tm{};

		__FSI_LOCALTIME(&timeInfo, &tm);

		// Time
		dt.hour = tm.tm_hour;
		dt.min = tm.tm_min;
		dt.sec = tm.tm_sec;
		dt.ms = (unsigned int)(epochMs);

		// Date
		dt.year = tm.tm_year + 1900;
		dt.month = tm.tm_mon + 1;
		dt.day = tm.tm_mday;

		return dt;
	}
}

