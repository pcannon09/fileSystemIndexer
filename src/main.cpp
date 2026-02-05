#include "../inc/fsi/lib/FSI_Indexer.hpp"

#include "../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"

#include "../inc/fsi/modules/temperature/FSI_temperatureConverter.h"

#include "ciof/ciof.hpp"

namespace
{
	std::string __timeToStr(const fsi::utils::TimeUtils_DateTime &time)
	{
		return ciof::format("%1/%2/%3 - %4:%5:%6.%7",
				time.year, time.month, time.day,
				time.hour, time.min, time.sec, time.ms
		);
	}
}

int main()
{
	ciof::print(FSI_temperatureConvertCelTo("f", 33));
	ciof::print(FSI_temperatureConvertCelFrom("f", 33));

	fsi::Indexer indexer("main-indexer");

	{
		fsi::IndexerInfo info;
		info.path = ".";
		info.id = "current-dir";

		indexer.addExtendedInfo(info);
	}

	for (const auto &x : indexer.searchMatching("cpp"))
	{
		ciof::print("FILE NAME: %1 -- %2", x, __timeToStr(indexer.getFileDTInfo(x)));
	}

	ciof::print("ID `current-dir` has path: %1", indexer.findIndex("current-dir").path);

	return 0;
}

