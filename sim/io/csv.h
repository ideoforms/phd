/*--------------------------------------------------------------------*
 * csv.h: Comma-separated value file writer.
 *
 * $Id: csv.h 12 2010-08-18 10:31:37Z daniel $
 *--------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>

#include <string>

namespace sim
{

#define SEPARATOR ","

using namespace std;

class csvwriter
{

public:
	csvwriter(string filename);
	csvwriter(const char *filename);
	csvwriter(FILE *fd);
	void startn(int n, const char *prefix);
	void start(const char *fmt, ...);
	void format(const char *fmt);
	void write(...);
	void writen(int n, float *data);
	void close();

	char fmt[64];
	FILE *fd;

};

}
