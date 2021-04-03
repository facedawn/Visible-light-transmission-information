#include "uncompress.h"
#include <vector>

using namespace std;

void UnCompress::append(Record r)
{
	int first = buffer.size() - tonum(r.off);
	for (int i = 0; i < tonum(r.len); ++i)
	{
		buffer.push_back(buffer[first + i]);
	}
}

