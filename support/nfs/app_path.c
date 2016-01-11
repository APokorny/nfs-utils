/*
 * support/nfs/app_path.c
 *
 * Handle file system path indirection, to support container like application deployments like
 * Ubuntu Snappy
 *
 * Authors:	Andreas Pokorny <andreas.pokorny@canonical.com>
 *
 *		This software maybe be used for any purpose provided
 *		the above copyright notice is retained.  It is supplied
 *		as is, with no warranty expressed or implied.
 */
#include <stdlib.h>
#include <string.h>
#include "app_path.h"

struct file_path
get_app_path(char const* expected_path)
{
	struct file_path ret = { (char *)expected_path, 0};
	char* app_path = getenv("SNAP_APP_DATA_PATH");

	if (app_path) {
		int app_len = strlen(app_path);
		ret.path = malloc(app_len + strlen(expected_path) + 1);
		strcpy(ret.path, app_path);
		strcpy(ret.path + app_len, expected_path);
	}

	return ret;
}

void
free_app_path(struct file_path* path)
{
	if (path->on_heap)
		free(path->path);
}

