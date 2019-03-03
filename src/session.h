#ifndef CPM_SESSION_H
#define CPM_SESSION_H
#include <stdbool.h>
#include "cpm.h"
static enum cpm_mode _translate_cpm_mode(const char *command);
struct cpm_session cpm_create_session(const char *command);
bool cpm_validate_session(struct cpm_session *session);
int cpm_dispatch(struct cpm_session *session);
#endif
