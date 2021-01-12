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

#ifndef __EXPIDUS_RESOURCE_H__
#define __EXPIDUS_RESOURCE_H__

G_BEGIN_DECLS

/**
 * XfceResourceType:
 * @EXPIDUS_RESOURCE_DATA   : where applications store data.
 * @EXPIDUS_RESOURCE_CONFIG : configuration files.
 * @EXPIDUS_RESOURCE_CACHE  : cached information.
 * @EXPIDUS_RESOURCE_ICONS  : icon search path.
 * @EXPIDUS_RESOURCE_THEMES : themes search path.
 **/
typedef enum /*< prefix=EXPIDUS_RESOURCE_ >*/
{
  EXPIDUS_RESOURCE_DATA   = 0,
  EXPIDUS_RESOURCE_CONFIG = 1,
  EXPIDUS_RESOURCE_CACHE  = 2,
  EXPIDUS_RESOURCE_ICONS  = 3,
  EXPIDUS_RESOURCE_THEMES = 4,
} XfceResourceType;

/**
 * XfceMatchFunc:
 * @basedir   : basedir
 * @relpath   : relpath
 * @user_data : additional user data passed to expidus_resource_match_custom().
 *
 * Return value :
 **/
typedef gboolean (*XfceMatchFunc)   (const gchar *basedir,
                                     const gchar *relpath,
                                     gpointer     user_data);

gchar** expidus_resource_dirs          (XfceResourceType type) G_GNUC_MALLOC;
gchar*  expidus_resource_lookup        (XfceResourceType type,
                                     const gchar     *filename) G_GNUC_MALLOC;
gchar** expidus_resource_lookup_all    (XfceResourceType type,
                                     const gchar     *filename) G_GNUC_MALLOC;
gchar** expidus_resource_match         (XfceResourceType type,
                                     const gchar     *pattern,
                                     gboolean         unique) G_GNUC_MALLOC;
gchar** expidus_resource_match_custom  (XfceResourceType type,
                                     gboolean         unique,
                                     XfceMatchFunc    func,
                                     gpointer         user_data) G_GNUC_MALLOC;
void    expidus_resource_push_path     (XfceResourceType type,
                                     const gchar     *path);
void    expidus_resource_pop_path      (XfceResourceType type);
gchar*  expidus_resource_save_location (XfceResourceType type,
                                     const gchar     *relpath,
                                     gboolean         create) G_GNUC_MALLOC;

G_END_DECLS

#endif /* !__EXPIDUS_RESOURCE_H__ */
