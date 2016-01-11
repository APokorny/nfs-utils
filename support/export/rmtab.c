/*
 * support/export/rmtab.c
 *
 * Interface to the rmtab file.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "misc.h"
#include "nfslib.h"
#include "exportfs.h"
#include "xio.h"
#include "xlog.h"
#include "app_path.h"

/*
 * See if the entry already exists.  If not,
 * this was an instantiated wild card, and we
 * must add it.
 */
static void
rmtab_read_wildcard(struct rmtabent *rep)
{
	nfs_export *exp, *exp2;
	struct addrinfo *ai;

	ai = host_addrinfo(rep->r_client);
	if (ai == NULL)
		return;

	exp = export_allowed(ai, rep->r_path);
	freeaddrinfo(ai);
	if (exp == NULL)
		return;

	exp2 = export_lookup(rep->r_client, exp->m_export.e_path, 0);
	if (exp2 == NULL) {
		struct exportent ee;

		memset(&ee, 0, sizeof(ee));
		dupexportent(&ee, &exp->m_export);

		ee.e_hostname = rep->r_client;
		exp2 = export_create(&ee, 0);
		exp2->m_changed = exp->m_changed;
	}
	exp2->m_mayexport = 1;
}

int
rmtab_read(void)
{
	struct rmtabent		*rep;

	setrmtabent("r");
	while ((rep = getrmtabent(1, NULL)) != NULL) {
		int			htype;

		htype = client_gettype(rep->r_client);
		if (htype == MCL_FQDN || htype == MCL_SUBNETWORK)
			rmtab_read_wildcard(rep);
	}

	if (errno == EINVAL) {
		/* Something goes wrong. We need to fix the rmtab
		   file. */
		int	lockid;
		FILE	*fp;
		struct file_path rmtab_lock = get_app_path(_PATH_RMTABLCK);
		struct file_path rmtab_tmp = get_app_path(_PATH_RMTABTMP);
		struct file_path rmtab = get_app_path(_PATH_RMTAB);
		if ((lockid = xflock(rmtab_lock.path, "w")) < 0) {
			free_app_path(&rmtab_lock);
			free_app_path(&rmtab_tmp);
			free_app_path(&rmtab);
			return -1;
		}
		rewindrmtabent();
		if (!(fp = fsetrmtabent(rmtab_tmp.path, "w"))) {
			free_app_path(&rmtab_lock);
			free_app_path(&rmtab_tmp);
			free_app_path(&rmtab);
			endrmtabent ();
			xfunlock(lockid);
			return -1;
		}
		while ((rep = getrmtabent(0, NULL)) != NULL) {
			fputrmtabent(fp, rep, NULL);
		}
		if (rename(rmtab_tmp.path, rmtab.path) < 0) {
			xlog(L_ERROR, "couldn't rename %s to %s",
			     rmtab_tmp.path, rmtab.path);
		}
		endrmtabent();
		fendrmtabent(fp);
		xfunlock(lockid);

		free_app_path(&rmtab_lock);
		free_app_path(&rmtab_tmp);
		free_app_path(&rmtab);
	}
	else {
		endrmtabent();
	}
	return 0;
}
