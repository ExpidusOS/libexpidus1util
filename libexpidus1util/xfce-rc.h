/* $Id$ */
/*-
 * Copyright (c) 2003-2007 Benedikt Meurer <benny@expidus.org>
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

#ifndef __EXPIDUS_RC_H__
#define __EXPIDUS_RC_H__

#include <glib-object.h>
#include <libexpidus1util/expidus-resource.h>

G_BEGIN_DECLS

typedef struct _XfceRc XfceRc;

#define EXPIDUS_TYPE_RC       (expidus_rc_get_type ())
#define EXPIDUS_RC(obj)       ((XfceRc *) (obj))
#define EXPIDUS_RC_CONST(obj) ((const XfceRc *) (obj))

GType        expidus_rc_get_type                (void);

XfceRc*      expidus_rc_simple_open             (const gchar     *filename,
                                              gboolean         readonly) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

XfceRc*      expidus_rc_config_open             (XfceResourceType type,
                                              const gchar     *resource,
                                              gboolean         readonly) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

void         expidus_rc_close                   (XfceRc *rc);
void         expidus_rc_flush                   (XfceRc *rc);
void         expidus_rc_rollback                (XfceRc *rc);

gboolean     expidus_rc_is_dirty                (const XfceRc *rc) G_GNUC_WARN_UNUSED_RESULT;
gboolean     expidus_rc_is_readonly             (const XfceRc *rc) G_GNUC_WARN_UNUSED_RESULT;

const gchar* expidus_rc_get_locale              (const XfceRc *rc) G_GNUC_WARN_UNUSED_RESULT;

gchar**      expidus_rc_get_groups              (const XfceRc *rc) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;
gchar**      expidus_rc_get_entries             (const XfceRc *rc,
                                              const gchar  *group) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;

void         expidus_rc_delete_group            (XfceRc       *rc,
                                              const gchar  *group,
                                              gboolean      global);
const gchar* expidus_rc_get_group               (const XfceRc *rc) G_GNUC_WARN_UNUSED_RESULT;
gboolean     expidus_rc_has_group               (const XfceRc *rc,
                                              const gchar  *group) G_GNUC_WARN_UNUSED_RESULT;
void         expidus_rc_set_group               (XfceRc       *rc,
                                              const gchar  *group);

void         expidus_rc_delete_entry            (XfceRc       *rc,
                                              const gchar  *key,
                                              gboolean      global);
gboolean     expidus_rc_has_entry               (const XfceRc *rc,
                                              const gchar  *key) G_GNUC_WARN_UNUSED_RESULT;

const gchar* expidus_rc_read_entry              (const XfceRc *rc,
                                              const gchar  *key,
                                              const gchar  *fallback) G_GNUC_WARN_UNUSED_RESULT;
const gchar* expidus_rc_read_entry_untranslated (const XfceRc *rc,
                                              const gchar  *key,
                                              const gchar  *fallback) G_GNUC_WARN_UNUSED_RESULT;
gboolean     expidus_rc_read_bool_entry         (const XfceRc *rc,
                                              const gchar  *key,
                                              gboolean      fallback) G_GNUC_WARN_UNUSED_RESULT;
gint         expidus_rc_read_int_entry          (const XfceRc *rc,
                                              const gchar  *key,
                                              gint          fallback) G_GNUC_WARN_UNUSED_RESULT;
gchar**      expidus_rc_read_list_entry         (const XfceRc *rc,
                                              const gchar  *key,
                                              const gchar  *delimiter) G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;
void         expidus_rc_write_entry             (XfceRc       *rc,
                                              const gchar  *key,
                                              const gchar  *value);
void         expidus_rc_write_bool_entry        (XfceRc       *rc,
                                              const gchar  *key,
                                              gboolean      value);
void         expidus_rc_write_int_entry         (XfceRc       *rc,
                                              const gchar  *key,
                                              gint          value);
void         expidus_rc_write_list_entry        (XfceRc       *rc,
                                              const gchar  *key,
                                              gchar       **value,
                                              const gchar  *separator);

G_END_DECLS

#endif /* !__EXPIDUS_RC_H__ */

