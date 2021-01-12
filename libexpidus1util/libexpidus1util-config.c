/* $Id$ */
/*-
 * Copyright (c) 2003-2006 Benedikt Meurer <benny@expidus.org>
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */

/**
 * SECTION: libexpidus1util-config
 * @title: Version Information
 * @short_description: Variables and functions to check the libexpidus1util version
 * @see_also: https://developer.gnome.org/glib/stable/glib-Standard-Macros.html
 *
 * These macros provide a few commonly-used features.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libexpidus1util/libexpidus1util.h>
#include <libexpidus1util/libexpidus1util-alias.h>



const guint libexpidus1util_major_version = LIBEXPIDUS1UTIL_MAJOR_VERSION;
const guint libexpidus1util_minor_version = LIBEXPIDUS1UTIL_MINOR_VERSION;
const guint libexpidus1util_micro_version = LIBEXPIDUS1UTIL_MICRO_VERSION;



#define __LIBEXPIDUS1UTIL_CONFIG_C__
#include <libexpidus1util/libexpidus1util-aliasdef.c>
