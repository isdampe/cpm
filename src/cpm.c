#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <unistd.h>
#endif
#ifdef __unix__
#include <getopt.h>
#endif

#include "cpm.h"
#include "session.h"
#include "dispatch.h"

static void usage()
{
	printf("Usage: cpm [options] command\nCommands:\n");
	printf("\tinstall    Install packages defined in package.cpm\n");
	printf("\tremove     Install packages defined in package.cpm\n");
	exit(EXIT_FAILURE);
}

int main(const int argc, char **argv)
{
	if (argc < 2)
		usage();

	int opt = -1;
	while((opt = getopt(argc, argv, ":h")) != -1) {
		switch (opt) {
			case 'h':
				usage();
				break;
		}
	}

	struct cpm_session session = cpm_create_session(argv[argc -1]);
	if (session.mode == CPM_MODE_UNKNOWN) {
		fprintf(stderr, "Error: unknown command '%s'\n", argv[argc -1]);
		usage();
	}

	if (! cpm_validate_session(&session)) {
		fprintf(stderr, "Error: '%s' doesn't exist or is not readable\n", session.package_fp);
		exit(EXIT_FAILURE);
	}

	return cpm_dispatch(&session);
}
