#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "parser.h"

int cpm_execute_install(struct cpm_session *session)
{
	struct cpm_package_list package_list = cpm_parse_package_list(session->package_fp);	

	if (package_list.head != NULL) {
		struct cpm_package_entry *row = package_list.head;
		int lib_count = 0, error_count = 0;
		while (1) {

			int did_free = 0;

			if (strcmp(row->lib_version, "*") == 0) {
				// Make sure we manually free lib_version because it's dynamically allocated in parser.
				free(row->lib_version);
				row->lib_version = "";
				did_free = 1;
			}

			char *cmd = calloc((strlen(row->lib_name) + strlen(row->lib_version) + strlen(row->lib_name) + 
				sizeof("git clone --depth=1 -b '' --single-branch https://github.com/ cpm-libs")) * sizeof(char) + 128, sizeof(char));

			sprintf(cmd, "git clone --depth=1 --single-branch \"https://github.com/%s\" cpm-libs/%s", row->lib_name, row->lib_name);
			if (system(cmd) < 0)
				error_count++;
			else
				lib_count++;

			free(cmd);

			// Set lib_version to NULL if it was overriden so destroy_parser doesn't try to free 
			// non-heap space.
			if (did_free)
				row->lib_version = NULL;

			if (row->next == NULL)
				break;
			row = row->next;
		}

		printf("Finished installing %i libraries\n", lib_count);
		if (error_count > 0) {
			fprintf(stderr, "Encountered %i errors trying to install libraries\n", error_count);
			return -1;
		}


	}

	//Clean up
	cpm_destroy_parser(&package_list);

	return (package_list.head == NULL ? -1 : 1);
}
