/*--------------------------------------------------------------------*
 * csv.cpp: Comma-separated value file writer.
 *
 * $Id: csv.cpp 16 2010-08-20 11:38:44Z daniel $
 *--------------------------------------------------------------------*/

#include "csv.h"
#include <string.h>

namespace sim
{

/*--------------------------------------------------------------------*
 * csvwriter(FILE): Create object, writing to the given filehandle.
 *--------------------------------------------------------------------*/
csvwriter::csvwriter(FILE *fd)
{
	this->fd = fd;
}

/*--------------------------------------------------------------------*
 * csvwriter(string): Create object, writing to the given filename.
 *--------------------------------------------------------------------*/
csvwriter::csvwriter(string filename) 
{
	this->fd = fopen(filename.c_str(), "w");
	assert(this->fd != NULL);
}
csvwriter::csvwriter(const char *filename) 
{
	this->fd = fopen(filename, "w");
	assert(this->fd != NULL);
}


/*--------------------------------------------------------------------*
 * format(): Prepare writing records to file without header.
 *    fmt - format of fields contained in subsequent records
 *          'f' = double
 *          'i' = int
 *
 * writer->start("iff")
 *--------------------------------------------------------------------*/
void csvwriter::format(const char *fmt)
{
	strncpy(this->fmt, fmt, sizeof(this->fmt));
}

/*--------------------------------------------------------------------*
 * start(): Write CSV header, and prepare writing records to file.
 *    fmt - format of fields contained in subsequent records
 *          'f' = double
 *          'i' = int
 *    ... - one or more const char * values containing header names.
 *          Must have the same number of values as fields in fmt.
 *
 * writer->start("iff", "index", "width", "height");
 *--------------------------------------------------------------------*/
void csvwriter::start(const char *fmt, ...)
{
	char *header;
	strncpy(this->fmt, fmt, sizeof(this->fmt));

	va_list args;
	va_start(args, fmt);

	while (*fmt++)
	{
		header = va_arg(args, char *);
		fprintf(this->fd, "%s", header);
		if (*fmt)
			fprintf(this->fd, SEPARATOR);
	}
	fprintf(this->fd, "\n");

	va_end(args);
}

/*--------------------------------------------------------------------*
 * startn(): Write CSV header containing a fixed number of fields
 * named sequentially with the specified prefix.
 *
 * writer->startn(5, "x")
 *--------------------------------------------------------------------*/
void csvwriter::startn(int n, const char *prefix)
{
	char header[strlen(prefix) + 8];
	strncpy(this->fmt, fmt, sizeof(this->fmt));

	for (int i = 0; i < n; i++)
	{
		snprintf(header, strlen(prefix) + 8, "%s%04d", prefix, i);
		fprintf(this->fd, "%s", header);
		if (i < n - 1)
			fprintf(this->fd, SEPARATOR);
	}

	fprintf(this->fd, "\n");
}

/*--------------------------------------------------------------------*
 * write(): Write record to CSV file according to established format.
 *
 * writer->write(21, 34.5, 48.1);
 *--------------------------------------------------------------------*/
void csvwriter::write(...)
{
	va_list args;

	va_start(args, NULL);

	for (unsigned int i = 0; i < strlen(this->fmt); i++)
	{
		switch (this->fmt[i])
		{
			case 'f':
				fprintf(this->fd, "%f", va_arg(args, double));
				break;
			case 'i':
				fprintf(this->fd, "%d", va_arg(args, int));
				break;
		}
		if (this->fmt[i + 1])
			fprintf(this->fd, SEPARATOR);
	}
	fprintf(this->fd, "\n");
	fflush(this->fd);

	va_end(args);
}

/*--------------------------------------------------------------------*
 * write(): Write n double values to CSV file, pointed to by data.
 *
 * writer->writen(8, data);
 *--------------------------------------------------------------------*/
void csvwriter::writen(int n, float *data)
{
	for (int i = 0; i < n; i++)
	{
		fprintf(this->fd, "%f", data[i]);
		if (i < n - 1)
			fprintf(this->fd, SEPARATOR);
	}

	fprintf(this->fd, "\n");
	fflush(this->fd);
}

void csvwriter::close()
{
	fclose(this->fd);
}

} // namespace sim
