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
		while (1) {

			//Do command.
			if (strcmp(row->lib_version, "*") == 0)
				row->lib_version = "";

			char *cmd = calloc((strlen(row->lib_name) + strlen(row->lib_version) + strlen(row->lib_name) + 
				sizeof("git clone --depth=1 -b '' --single-branch https://github.com/ cpm-libs")) * sizeof(char) + 128, sizeof(char));

			sprintf(cmd, "git clone --depth=1 --single-branch \"https://github.com/%s\" cpm-libs/%s", row->lib_name, row->lib_name);
			system(cmd);

			free(cmd);

			if (row->next == NULL)
				break;
			row = row->next;
		}
	}

	//Clean up

	return (package_list.head == NULL ? -1 : 1);
}
