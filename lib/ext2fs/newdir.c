/*
 * newdir.c --- create a new directory block
 * 
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#include <stdio.h>
#include <string.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>

#include <linux/ext2_fs.h>

#include "ext2fs.h"

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ino_t dir_ino, ino_t parent_ino,
			       char **block)
{
	struct ext2_dir_entry 	*dir = NULL;
	errcode_t		retval;
	char			*buf;
	int			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, (void **) &buf);
	if (retval)
		return retval;
	memset(buf, 0, fs->blocksize);
	dir = (struct ext2_dir_entry *) buf;
	dir->rec_len = fs->blocksize;

	if (dir_ino) {
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		dir->name_len = 1;
		dir->name[0] = '.';
		rec_len = dir->rec_len - EXT2_DIR_REC_LEN(dir->name_len);
		dir->rec_len = EXT2_DIR_REC_LEN(dir->name_len);

		/*
		 * Set up entry for '..'
		 */
		dir = (struct ext2_dir_entry *) (buf + dir->rec_len);
		dir->rec_len = rec_len;
		dir->inode = parent_ino;
		dir->name_len = 2;
		dir->name[0] = '.';
		dir->name[1] = '.';
		
	}
	*block = buf;
	return 0;
}
