#include "../inc/fsi/lib/FSI_Indexer.hpp"

#include "../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"

#include "ciof/ciof.hpp"

int main()
{
	fsi::Indexer indexer("main-indexer");

	{
		fsi::IndexerInfo info;
		info.path = ".";
		info.id = "current-dir";

		indexer.addExtendedInfo(info);
	}

	for (const auto &x : indexer.getIndexPaths())
	{
		ciof::print(x);
	}

	return 0;
}

