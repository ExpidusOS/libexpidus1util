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

#if !defined(LIBEXPIDUS1UTIL_INSIDE_LIBEXPIDUS1UTIL_H) && !defined(LIBEXPIDUS1UTIL_COMPILATION)
#error "Only <libexpidus1util/libexpidus1util.h> can be included directly, this file may disappear or change contents"
#endif

#ifndef __EXPIDUS_FILEUTILS_H__
#define __EXPIDUS_FILEUTILS_H__

#include <glib.h>

G_BEGIN_DECLS

gboolean expidus_mkdirhier (const gchar *whole_path,
                         gulong       mode,
                         GError     **error);

G_END_DECLS

#endif /* !__EXPIDUS_FILEUTILS_H__ */
