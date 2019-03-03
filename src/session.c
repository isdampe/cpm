#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpm.h"

static enum cpm_mode _translate_cpm_mode(const char *command)
{
	if (strcmp(command, "install") == 0)
		return CPM_MODE_INSTALL;
	if (strcmp(command, "remove") == 0)
		return CPM_MODE_REMOVE;
	return CPM_MODE_UNKNOWN;
}

struct cpm_session cpm_create_session(const char *command)
{
	struct cpm_session session = {
		.mode = _translate_cpm_mode(command)
	};

	getcwd(session.working_dir, sizeof(session.working_dir));
	strcpy(session.package_fp, session.working_dir);
	strcat(session.package_fp, "/package.cpm");

	return session;
}

bool cpm_validate_session(struct cpm_session *session)
{
	return (access(session->package_fp, F_OK) != -1);
}

