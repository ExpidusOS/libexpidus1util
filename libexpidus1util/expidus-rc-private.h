/* $Id$ */
/*-
 * Copyright (c) 2003-2005 Benedikt Meurer <benny@expidus.org>
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

#ifndef __LIBEXPIDUS1UTIL_EXPIDUS_RC_PRIVATE_H__
#define __LIBEXPIDUS1UTIL_EXPIDUS_RC_PRIVATE_H__

#include <libexpidus1util/libexpidus1util.h>

typedef struct _ExpidusRcConfig ExpidusRcConfig;
typedef struct _ExpidusRcSimple ExpidusRcSimple;

struct _ExpidusRc
{
  void          (*close)        (ExpidusRc       *rc);
  void          (*flush)        (ExpidusRc       *rc);
  void          (*rollback)     (ExpidusRc       *rc);
  gboolean      (*is_dirty)     (const ExpidusRc *rc);
  gboolean      (*is_readonly)  (const ExpidusRc *rc);
  gchar       **(*get_groups)   (const ExpidusRc *rc);
  gchar       **(*get_entries)  (const ExpidusRc *rc,
                                 const gchar  *group);
  void          (*delete_group) (ExpidusRc       *rc,
                                 const gchar  *group, gboolean global);
  const gchar  *(*get_group)    (const ExpidusRc *rc);
  gboolean      (*has_group)    (const ExpidusRc *rc,
                                 const gchar  *group);
  void          (*set_group)    (ExpidusRc       *rc,
                                 const gchar  *group);
  void          (*delete_entry) (ExpidusRc       *rc,
                                 const gchar *key,
                                 gboolean     global);
  gboolean      (*has_entry)    (const ExpidusRc *rc,
                                 const gchar  *key);
  const gchar  *(*read_entry)   (const ExpidusRc *rc,
                                 const gchar  *key,
                                 gboolean      translated);
  /* write_entry == NULL means readonly */
  void          (*write_entry)  (ExpidusRc       *rc,
                                 const gchar  *key,
                                 const gchar  *value);

  gchar   *locale;
};

#define EXPIDUS_RC_CONFIG(obj)       ((ExpidusRcConfig *) (obj))
#define EXPIDUS_RC_CONFIG_CONST(obj) ((const ExpidusRcConfig *) (obj))

#define EXPIDUS_RC_SIMPLE(obj)       ((ExpidusRcSimple *) (obj))
#define EXPIDUS_RC_SIMPLE_CONST(obj) ((const ExpidusRcSimple *) (obj))


G_GNUC_INTERNAL void          _expidus_rc_init               (ExpidusRc       *rc);

G_GNUC_INTERNAL ExpidusRcSimple* _expidus_rc_simple_new         (ExpidusRcSimple *shared,
                                                           const gchar  *filename,
                                                           gboolean      readonly);
G_GNUC_INTERNAL gboolean      _expidus_rc_simple_parse       (ExpidusRcSimple *simple);
G_GNUC_INTERNAL void          _expidus_rc_simple_close       (ExpidusRc       *rc);
G_GNUC_INTERNAL void          _expidus_rc_simple_flush       (ExpidusRc       *rc);
G_GNUC_INTERNAL void          _expidus_rc_simple_rollback    (ExpidusRc       *rc);
G_GNUC_INTERNAL gboolean      _expidus_rc_simple_is_dirty    (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gboolean      _expidus_rc_simple_is_readonly (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL const gchar*  _expidus_rc_simple_get_filename(const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gchar**       _expidus_rc_simple_get_groups  (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gchar**       _expidus_rc_simple_get_entries (const ExpidusRc *rc,
                                                           const gchar  *name) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_simple_delete_group(ExpidusRc       *rc,
                                                           const gchar  *name,
                                                           gboolean      global);
G_GNUC_INTERNAL const gchar*  _expidus_rc_simple_get_group   (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gboolean      _expidus_rc_simple_has_group   (const ExpidusRc *rc,
                                                           const gchar  *name) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_simple_set_group   (ExpidusRc       *rc,
                                                           const gchar  *name);
G_GNUC_INTERNAL void          _expidus_rc_simple_delete_entry(ExpidusRc       *rc,
                                                           const gchar  *key,
                                                           gboolean      global);
G_GNUC_INTERNAL gboolean      _expidus_rc_simple_has_entry   (const ExpidusRc *rc,
                                                           const gchar  *key) G_GNUC_CONST;
G_GNUC_INTERNAL const gchar*  _expidus_rc_simple_read_entry  (const ExpidusRc *rc,
                                                           const gchar  *key,
                                                           gboolean      translated) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_simple_write_entry (ExpidusRc       *rc,
                                                           const gchar  *key,
                                                           const gchar  *value);

G_GNUC_INTERNAL ExpidusRcConfig* _expidus_rc_config_new         (ExpidusResourceType type,
                                                           const gchar  *resource,
                                                           gboolean      readonly);
G_GNUC_INTERNAL void          _expidus_rc_config_close       (ExpidusRc       *rc);
G_GNUC_INTERNAL void          _expidus_rc_config_flush       (ExpidusRc       *rc);
G_GNUC_INTERNAL void          _expidus_rc_config_rollback    (ExpidusRc       *rc);
G_GNUC_INTERNAL gboolean      _expidus_rc_config_is_dirty    (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gboolean      _expidus_rc_config_is_readonly (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gchar**       _expidus_rc_config_get_groups  (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gchar**       _expidus_rc_config_get_entries (const ExpidusRc *rc,
                                                           const gchar  *name) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_config_delete_group(ExpidusRc       *rc,
                                                           const gchar  *name,
                                                           gboolean      global);
G_GNUC_INTERNAL const gchar*  _expidus_rc_config_get_group   (const ExpidusRc *rc) G_GNUC_CONST;
G_GNUC_INTERNAL gboolean      _expidus_rc_config_has_group   (const ExpidusRc *rc,
                                                           const gchar  *name) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_config_set_group   (ExpidusRc       *rc,
                                                           const gchar  *name);
G_GNUC_INTERNAL void          _expidus_rc_config_delete_entry(ExpidusRc       *rc,
                                                           const gchar  *key,
                                                           gboolean      global);
G_GNUC_INTERNAL gboolean      _expidus_rc_config_has_entry   (const ExpidusRc *rc,
                                                           const gchar  *key) G_GNUC_CONST;
G_GNUC_INTERNAL const gchar*  _expidus_rc_config_read_entry  (const ExpidusRc *rc,
                                                           const gchar  *key,
                                                           gboolean      translated) G_GNUC_CONST;
G_GNUC_INTERNAL void          _expidus_rc_config_write_entry (ExpidusRc       *rc,
                                                           const gchar  *key,
                                                           const gchar  *value);


#endif /* !__LIBEXPIDUS1UTIL_EXPIDUS_RC_PRIVATE_H__ */
