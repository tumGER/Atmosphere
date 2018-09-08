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
 
#ifndef EXOSPHERE_DBG_LOG_DEVICE_NULL_H
#define EXOSPHERE_DBG_LOG_DEVICE_NULL_H

#include "log.h"

typedef struct {
    debug_log_device_t super;
    /* Additonnal attributes go here */
} debug_log_device_null_t;

extern debug_log_device_null_t g_debug_log_device_null;

#endif
