## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libnfs.a
libnfs_a_SOURCES = exports.c rmtab.c xio.c rpcmisc.c rpcdispatch.c \
		   xlog.c xcommon.c wildmat.c mydaemon.c nfsclient.c \
		   nfsexport.c getfh.c nfsctl.c rpc_socket.c getport.c \
		   svc_socket.c cacheio.c closeall.c nfs_mntent.c conffile.c \
		   svc_create.c atomicio.c strlcpy.c strlcat.c app_path.c
libnfs_a_CPPFLAGS = $(LIBTIRPC_CFLAGS) $(AM_CPFLAGS)
libnfs_a_CFLAGS = $(LIBTIRPC_CFLAGS) $(AM_CFLAGS)

MAINTAINERCLEANFILES = Makefile.in

