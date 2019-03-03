#include "install.h"
#include "session.h"

int cpm_dispatch(struct cpm_session *session)
{
	switch (session->mode) {
		default:
		case CPM_MODE_UNKNOWN:
			return 1;
		case CPM_MODE_INSTALL:
			return cpm_execute_install(session);
			break;
		case CPM_MODE_REMOVE:
			return 1;
	}
	return 0;
}
