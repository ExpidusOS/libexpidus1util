/* $Id$ */
/*-
 * Copyright (c) 2003-2006 Benedikt Meurer <benny@expidus.org>
 * Copyright (c) 2004      Jasper Huijsman <jasper@expidus.org>
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

#ifndef __EXPIDUS_I18N_H__
#define __EXPIDUS_I18N_H__

#include <glib.h>

#if defined(GETTEXT_PACKAGE)
#include <glib/gi18n-lib.h>
#else
#include <glib/gi18n.h>
#endif

#if !defined(GETTEXT_PACKAGE)

#ifdef gettext
#undef gettext
#endif
#ifdef dgettext
#undef dgettext
#endif
#ifdef dcgettext
#undef dcgettext
#endif
#ifdef ngettext
#undef ngettext
#endif
#ifdef dngettext
#undef dngettext
#endif
#ifdef dcngettext
#undef dcngettext
#endif

#define gettext(s)                                            (s)
#define dgettext(domain,s)                                    (s)
#define dcgettext(domain,s,type)                              (s)
#define ngettext(msgid, msgid_plural, n)                      (((n) > 0) ? (msgid) : (msgid_plural))
#define dngettext(domainname, msgid, msgid_plural, n)         (((n) > 0) ? (msgid) : (msgid_plural))
#define dcngettext(domainname, msgid, msgid_plural, n, type)  (((n) > 0) ? (msgid) : (msgid_plural))

#endif /* !defined(GETTEXT_PACKAGE) */

void                  expidus_textdomain           (const gchar *package,
                                                 const gchar *localedir,
                                                 const gchar *encoding);

gchar*                expidus_get_file_localized   (const gchar *filename) G_GNUC_MALLOC;
gchar*                expidus_get_file_localized_r (gchar       *buffer,
                                                 gsize        length,
                                                 const gchar *filename);

gchar*                expidus_get_dir_localized    (const gchar *directory) G_GNUC_MALLOC;
gchar*                expidus_get_dir_localized_r  (gchar       *buffer,
                                                 gsize        length,
                                                 const gchar *directory);

gchar*                expidus_get_path_localized   (gchar       *dst,
                                                 gsize        size,
                                                 const gchar *paths,
                                                 const gchar *filename,
                                                 GFileTest    test);

#define EXPIDUS_LOCALE_FULL_MATCH 50
#define EXPIDUS_LOCALE_NO_MATCH    0

guint                 expidus_locale_match         (const gchar *locale1,
                                                 const gchar *locale2);

#endif  /* !__EXPIDUS_I18N_H__ */
