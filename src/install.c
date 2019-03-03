#include <stdio.h>
#include "session.h"
#include "parser.h"

int cpm_execute_install(struct cpm_session *session)
{
	struct cpm_package_list package_list = cpm_parse_package_list(session->package_fp);	

	return 0;
}
