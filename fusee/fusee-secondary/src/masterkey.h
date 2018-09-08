/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef FUSEE_MASTERKEY_H
#define FUSEE_MASTERKEY_H

/* This is glue code to enable master key support across versions. */

/* TODO: Update to 0x6 on release of new master key. */
#define MASTERKEY_REVISION_MAX 0x5

#define MASTERKEY_REVISION_100_230     0x00
#define MASTERKEY_REVISION_300         0x01
#define MASTERKEY_REVISION_301_302     0x02
#define MASTERKEY_REVISION_400_410     0x03
#define MASTERKEY_REVISION_500_CURRENT 0x04

#define MASTERKEY_NUM_NEW_DEVICE_KEYS (MASTERKEY_REVISION_MAX - MASTERKEY_REVISION_400_410)

/* This should be called during initialization. */
int mkey_detect_revision(void);

unsigned int mkey_get_revision(void);

unsigned int mkey_get_keyslot(unsigned int revision);

#endif
