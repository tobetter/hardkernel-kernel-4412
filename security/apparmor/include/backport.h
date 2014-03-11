/*
 * AppArmor security module
 *
 * This file contains AppArmor file mediation function definitions.
 *
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 *
 * This is a file of helper macros, defines for backporting newer versions
 * of apparmor to older kernels
 */
#ifndef __AA_BACKPORT_H
#define __AA_BACKPORT_H

/* commit 496ad9aa8ef448058e36ca7a787c61f2e63f0f54 */
#define file_inode(FILE) ((FILE)->f_path.dentry->d_inode)

/* commit d2b31ca644fdc8704de3367a6a56a5c958c77f53 */
#define kuid_t uid_t
#define kgid_t gid_t

/* commit 2db81452931eb51cc739d6e495cf1bd4860c3c99 */
#define GLOBAL_ROOT_UID 0
#define from_kuid(X, UID) UID
#define uid_eq(X, Y) ((X) == (Y))

/* commit d007794a182bc072a7b7479909dbd0d67ba341be */
#include <linux/capability.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include <linux/security.h>
static inline int cap_mmap_addr(unsigned long addr)
{
	int ret = 0;

	if (addr < dac_mmap_min_addr) {
		ret = cap_capable(current_cred(), &init_user_ns, CAP_SYS_RAWIO,
				  SECURITY_CAP_AUDIT);
		/* set PF_SUPERPRIV if it turns out we allow the low mmap */
		if (ret == 0)
			current->flags |= PF_SUPERPRIV;
	}
	return ret;
}

#endif /* __AA_BACKPORT_H */
