#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpm.h"
#include "parser.h"

struct cpm_package_list cpm_parse_package_list(const char *package_fp)
{
	struct cpm_package_list package_list = {
		.head = NULL,
		.tail = NULL,
		.package_src = NULL,
		.package_src_size = 0
	};

	FILE *fh = fopen(package_fp, "r");
	if (fh == NULL)
		return package_list;

	fseek(fh, 0, SEEK_END);
	package_list.package_src_size = ftell(fh);
	fseek(fh, 0, SEEK_SET);

	package_list.package_src = malloc(package_list.package_src_size * sizeof(char) + 1);
	package_list.package_src[package_list.package_src_size -1] = '\0';

	fread(package_list.package_src, sizeof(char), package_list.package_src_size, fh);
	fclose(fh);

	walk_package_list(&package_list);

	return package_list;
}

static void walk_package_list(struct cpm_package_list *list)
{
	if (list->package_src_size < 1)
		return;

	char *line_buffer[4096];
	int start_idx = 0, line_count = 0;

	// Split the source file into an array of lines.
	// Each line will then later be parsed and processed.
	for (int i=0; (i<list->package_src_size && line_count < 4096); ++i) {
		if (list->package_src[i] == '\n') {
			line_buffer[line_count] = malloc(((i - start_idx) + (2 * i)) * sizeof(char));
			memcpy(line_buffer[line_count++], list->package_src + start_idx, (i - start_idx));

			while (list->package_src[i] == '\n') {++i;}

			start_idx = i;
		}
	}

	for (int i=0; i<line_count; ++i) {
		struct cpm_package_entry *entry = walk_parse_line(line_buffer[i]);
		if (entry == NULL)
			continue;

		if (list->head == NULL) {
			list->head = entry;
			list->tail = entry;
		} else {
			list->tail->next = entry;
			list->tail = entry;
		}
	}

	// Tidy stuff back up.
	if (line_count > 0)
		for (int i=0; i<line_count; ++i)
			if (line_buffer[i] != NULL)
				free(line_buffer[i]);

	struct cpm_package_entry *row = list->head;
	while (1) {
		printf("Lib: %s, Version: %s\n", row->lib_name, row->lib_version);
		if (row->next == NULL)
			break;
		row = row->next;
	}


}

static struct cpm_package_entry *walk_parse_line(const char *line)
{
	int mid_idx = -1;
	int line_len = strlen(line);

	// Find the : middle point.
	for (int i=0; i<line_len; ++i) {
		if (line[i] == ':') {
			mid_idx = i;
			break;
		}
	}

	if (mid_idx < 1)
		return NULL;

	// Allocate enough space for version and lib name.
	struct cpm_package_entry *entry = malloc(sizeof(struct cpm_package_entry));
	entry->lib_name = calloc(mid_idx, sizeof(char));
	entry->lib_version = calloc(strlen(line) - mid_idx, sizeof(char));
	entry->next = NULL;

	int idx = 0;
	char pc = '\0';
	for (int i=0; i<mid_idx; ++i) {
		if (line[i] == '"' || line[i] == '\\') {
			if (pc == '\\')
				entry->lib_name[idx++] = line[i];
		} else {
			entry->lib_name[idx++] = line[i];
		}
		pc = line[i];
	}

	idx = 0;
	for (int i=(mid_idx +1); i<line_len; ++i) {
		if (line[i] == '"' && pc != '\\')
			continue;
		else
			entry->lib_version[idx++] = line[i];
	}

	return entry;
}
