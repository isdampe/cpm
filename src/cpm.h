#ifndef CPM_H
#define CPM_H
#include <stddef.h>

#define CPM_MODULE_PATH ".cpm_modules"

enum cpm_mode {
	CPM_MODE_UNKNOWN,
	CPM_MODE_INSTALL,
	CPM_MODE_REMOVE
};

struct cpm_session {
	enum cpm_mode mode;
	char working_dir[4096];
	char package_fp[4096];
};

struct cpm_package_entry {
	char *lib_name;
	char *lib_version;
	struct cpm_package_entry *next;
};

struct cpm_package_list {
	char *package_src;
	int package_src_size;
	struct cpm_package_entry *head;	
	struct cpm_package_entry *tail;
};

#endif
