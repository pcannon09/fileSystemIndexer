#include <iostream>

#include <fsi/lib/FSI_Indexer.hpp>

int main()
{
	fsi::Indexer indexer("main-indexer");

	{
		fsi::IndexerInfo info;
		info.path = ".";

		indexer.addExtendedInfo(info);
	}

	for (const auto &x : indexer.searchMatching("cpp"))
	{
		std::cout << x << "\n";
	}

	return 0;
}

