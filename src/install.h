#ifndef CPM_INSTALL_H
#define CPM_INSTALL_H
#include <stdbool.h>
#include "cpm.h"
#include "session.h"

int cpm_execute_install(struct cpm_session *session);
static bool lib_exists(const struct cpm_session *session,
	const struct cpm_package_entry *entry);
static int process_install_entry(struct cpm_package_entry *entry);
static int process_update_entry(struct cpm_package_entry *entry);

#endif
