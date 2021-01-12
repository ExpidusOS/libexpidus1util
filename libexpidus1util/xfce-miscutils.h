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

#ifndef __EXPIDUS_MISCUTILS_H__
#define __EXPIDUS_MISCUTILS_H__

#include <glib.h>

#include <libexpidus1util/expidus-debug.h>

G_BEGIN_DECLS

static inline gboolean expidus_str_is_empty (const gchar *str)
{
  return str == NULL || *str == '\0';
}

const gchar* expidus_version_string (void) G_GNUC_PURE;

const gchar* expidus_get_homedir    (void) G_GNUC_PURE;

gchar*       expidus_get_homefile_r (gchar *buffer,
                                  size_t length,
                                  const gchar *format,
                                  ...);

const gchar* expidus_get_userdir    (void) G_GNUC_PURE;

gchar*       expidus_get_userfile_r (gchar *buffer,
                                  size_t length,
                                  const gchar *format,
                                  ...);

#if defined(G_HAVE_ISO_VARARGS)

#define expidus_get_homefile(...)                             \
  (g_build_filename (expidus_get_homedir (), __VA_ARGS__))

#define expidus_get_userfile(...)                             \
  (g_build_filename (expidus_get_userdir (), __VA_ARGS__))

#elif defined(G_HAVE_GNUC_VARARGS)

#define expidus_get_homefile(first_element...)                \
  (g_build_filename (expidus_get_homedir (), ## first_element))

#define expidus_get_userfile(first_element...)                \
  (g_build_filename (expidus_get_userdir (), ## first_element))

#else

static gchar*
expidus_get_homefile (const gchar *first_element, ...)
{
#error "Implement this, if you see this error!"
}

static gchar*
expidus_get_userfile (const gchar *first_element, ...)
{
#error "Implement this, if you see this error!"
}

#endif

gchar* expidus_gethostname                      (void) G_GNUC_MALLOC;

gchar* expidus_expand_variables                 (const gchar  *command,
                                              gchar       **envp) G_GNUC_MALLOC;

void   expidus_append_quoted                    (GString      *string,
                                              const gchar  *unquoted);

gchar* expidus_expand_desktop_entry_field_codes (const gchar  *command,
                                              GSList       *uri_list,
                                              const gchar  *icon,
                                              const gchar  *name,
                                              const gchar  *uri,
                                              gboolean      requires_terminal) G_GNUC_MALLOC;

G_END_DECLS

#endif /* __EXPIDUS_MISCUTILS_H__ */
