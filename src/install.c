#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "session.h"
#include "parser.h"
#include "install.h"

int cpm_execute_install(struct cpm_session *session)
{
	struct cpm_package_list package_list = cpm_parse_package_list(session->package_fp);	

	if (package_list.head != NULL) {
		struct cpm_package_entry *row = package_list.head;
		int lib_count = 0, error_count = 0;
		while (1) {
			
			//If the library doesn't exist yet, do install.
			if (! lib_exists(session, row)) {
				if (process_install_entry(row) < 0)
					error_count++;
				else
					lib_count++;
			} else {
				//Do update.
				printf("Package %s already installed, skipping\n", row->lib_name);
			}

			if (row->next == NULL)
				break;
			row = row->next;
		}

		printf("Finished installing %i libraries\n", lib_count);
		if (error_count > 0) {
			fprintf(stderr, "Encountered %i errors trying to install libraries\n", 
				error_count);
			return -1;
		} else {
			printf("When compiling, don't forget to include the cpm-libs/ directory\n");
			printf("For example: gcc -I ./cpm-libs *.c\n");
			printf("And then you can include libraries like #include \"isdampe/libotp/src/otp.h\"\n");
		}


	}

	//Clean up
	cpm_destroy_parser(&package_list);

	return (package_list.head == NULL ? -1 : 1);
}

static bool lib_exists(const struct cpm_session *session,
	const struct cpm_package_entry *entry)
{
	char *dir = calloc(strlen(session->working_dir) + sizeof("/cpm-libs/") + 
		strlen(entry->lib_name) + 1, sizeof(char));
	strcpy(dir, session->working_dir);
	strcat(dir, "/cpm-libs/");
	strcat(dir, entry->lib_name);

	bool result = (access(dir, F_OK) != -1);
	free(dir);

	return result;
}


static int process_install_entry(struct cpm_package_entry *entry)
{
	int did_free = 0;

	if (strcmp(entry->lib_version, "*") == 0) {
		// Make sure we manually free lib_version because it's dynamically allocated in parser.
		free(entry->lib_version);
		entry->lib_version = "";
		did_free = 1;
	}

	char *cmd = calloc((strlen(entry->lib_name) + strlen(entry->lib_version) + 
		strlen(entry->lib_name) + sizeof("git clone --depth=1 -b '' --single-branch https://github.com/ cpm-libs/")) * sizeof(char) + 128, 
		sizeof(char));

	sprintf(cmd, "git clone --depth=1 --single-branch \"https://github.com/%s\" cpm-libs/%s", 
		entry->lib_name, entry->lib_name);

	int result = system(cmd);

	free(cmd);

	// Set lib_version to NULL if it was overriden so destroy_parser doesn't try to free 
	// non-heap space.
	if (did_free)
		entry->lib_version = NULL;

	return result;
}
