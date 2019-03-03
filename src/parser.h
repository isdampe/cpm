#ifndef CPM_PARSER_H
#define CPM_PARSER_H
struct cpm_package_list cpm_parse_package_list(const char *package_fp);
static void walk_package_list(struct cpm_package_list *list);
static struct cpm_package_entry *walk_parse_line(const char *line);
#endif
