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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <libexpidus1util/expidus-private.h>
#include <libexpidus1util/expidus-rc-private.h>
#include <libexpidus1util/libexpidus1util-alias.h>



static gchar **merge_arrays (gchar **source1,
                             gchar **source2);



struct _XfceRcConfig
{
  XfceRc  __parent__;
  XfceRc *save;
  GSList *rclist;
};



static gchar**
merge_arrays (gchar **source1,
              gchar **source2)
{
  gchar **result;
  gchar **rp;
  gchar **sp;
  guint   size;
  guint   pos;

  result = g_new (gchar *, 11);
  size   = 10;
  pos    = 0;

  for (sp = source1; *sp != NULL; ++sp)
    {
      if (pos == size)
        {
          size *= 2;
          result = g_realloc (result, (size + 1) * sizeof (*result));
        }
      result[pos] = *sp;
      ++pos;
    }

  for (sp = source2; *sp != NULL; ++sp)
    {
      for (rp = result; rp < result + pos; ++rp)
        if (strcmp (*rp, *sp) == 0)
          break;

      if (rp == result + pos)
        {
          g_free (*sp);
          continue;
        }

      if (pos == size)
        {
          size += 2;
          result = g_realloc (result, (size + 1) * sizeof (*result));
        }
      result[pos] = *sp;
      ++pos;
    }

  result[pos] = NULL;

  g_free (source1);
  g_free (source2);

  return result;
}



XfceRcConfig*
_expidus_rc_config_new (XfceResourceType type,
                     const gchar     *resource,
                     gboolean         readonly)
{
  XfceRcConfig *config;
  XfceRcSimple *simple = NULL;
  gboolean      user_present = FALSE;
  gchar        *user;
  gchar       **paths;
  gchar       **p;

  g_return_val_if_fail (resource != NULL && *resource != '\0', NULL);
  g_return_val_if_fail (resource[strlen (resource) - 1] != G_DIR_SEPARATOR, NULL);

  user = expidus_resource_save_location (type, resource, FALSE);
  paths = expidus_resource_lookup_all (type, resource);

  g_assert (user != NULL);
  g_assert (paths != NULL);

  config = g_new0 (XfceRcConfig, 1);

  /* system files first */
  for (p = paths; *p != NULL; ++p)
    {
      if (strcmp (*p, user) == 0)
        {
          user_present = TRUE;
          continue;
        }

      simple = _expidus_rc_simple_new (simple, *p, TRUE);
      if (!_expidus_rc_simple_parse (simple))
        {
          g_critical ("Failed to parse file %s, ignoring.", *p);
          expidus_rc_close (EXPIDUS_RC (simple));
          simple = NULL;
          continue;
        }
      config->rclist = g_slist_append (config->rclist, simple);
    }

  /* now the user file */
  simple = _expidus_rc_simple_new (simple, user, readonly);
  if (user_present && !_expidus_rc_simple_parse (simple))
    {
      g_critical ("Failed to parse file %s, ignoring.", user);
    }
  config->save   = EXPIDUS_RC (simple);
  config->rclist = g_slist_prepend (config->rclist, simple);

  /* attach callbacks */
  config->__parent__.close        = _expidus_rc_config_close;
  config->__parent__.get_groups   = _expidus_rc_config_get_groups;
  config->__parent__.get_entries  = _expidus_rc_config_get_entries;
  config->__parent__.delete_group = _expidus_rc_config_delete_group;
  config->__parent__.get_group    = _expidus_rc_config_get_group;
  config->__parent__.has_group    = _expidus_rc_config_has_group;
  config->__parent__.set_group    = _expidus_rc_config_set_group;
  config->__parent__.delete_entry = _expidus_rc_config_delete_entry;
  config->__parent__.has_entry    = _expidus_rc_config_has_entry;
  config->__parent__.read_entry   = _expidus_rc_config_read_entry;

  if (!readonly)
    {
      config->__parent__.flush       = _expidus_rc_config_flush;
      config->__parent__.rollback    = _expidus_rc_config_rollback;
      config->__parent__.is_dirty    = _expidus_rc_config_is_dirty;
      config->__parent__.is_readonly = _expidus_rc_config_is_readonly;
      config->__parent__.write_entry = _expidus_rc_config_write_entry;
    }

  g_strfreev (paths);
  g_free (user);

  return config;
}



void
_expidus_rc_config_close (XfceRc *rc)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);

  g_slist_foreach (config->rclist, (GFunc) (void (*)(void)) expidus_rc_close, NULL);
  g_slist_free (config->rclist);
}



void
_expidus_rc_config_flush (XfceRc *rc)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);
  const gchar  *filename;
  gchar        *dir;

  /* create the base directory for the local rc file on demand */
  if (!_expidus_rc_simple_is_readonly (EXPIDUS_RC_CONST (config->save)))
    {
      filename = _expidus_rc_simple_get_filename (EXPIDUS_RC_CONST (config->save));
      dir = g_path_get_dirname (filename);
      if (!expidus_mkdirhier (dir, 0700, NULL))
  {
    g_critical ("Unable to create base directory %s. "
          "Saving to file %s is likely to fail.",
          dir, filename);
  }
      g_free (dir);
    }

  _expidus_rc_simple_flush (EXPIDUS_RC (config->save));
}



void
_expidus_rc_config_rollback (XfceRc *rc)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);

  _expidus_rc_simple_rollback (EXPIDUS_RC (config->save));
}



gboolean
_expidus_rc_config_is_dirty (const XfceRc *rc)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);

  return _expidus_rc_simple_is_dirty (EXPIDUS_RC_CONST (config->save));
}



gboolean
_expidus_rc_config_is_readonly (const XfceRc *rc)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);

  return _expidus_rc_simple_is_readonly (EXPIDUS_RC_CONST (config->save));
}



gchar**
_expidus_rc_config_get_groups (const XfceRc *rc)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);
  gchar             **result = NULL;
  gchar             **tmp;
  GSList             *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      tmp = _expidus_rc_simple_get_groups (EXPIDUS_RC_CONST (list->data));
      if (tmp == NULL)
        continue;

      if (result == NULL)
        result = tmp;
      else
        result = merge_arrays (result, tmp);
    }

  return result;
}


gchar**
_expidus_rc_config_get_entries (const XfceRc *rc,
                             const gchar  *name)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);
  gchar             **result = NULL;
  gchar             **tmp;
  GSList             *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      tmp = _expidus_rc_simple_get_entries (EXPIDUS_RC_CONST (list->data), name);
      if (tmp == NULL)
        continue;

      if (result == NULL)
        result = tmp;
      else
        result = merge_arrays (result, tmp);
    }

  return result;
}



void
_expidus_rc_config_delete_group (XfceRc       *rc,
                              const gchar  *name,
                              gboolean      global)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);

  _expidus_rc_simple_delete_group (EXPIDUS_RC (config->save), name, global);
}



const gchar*
_expidus_rc_config_get_group (const XfceRc *rc)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);

  return _expidus_rc_simple_get_group (EXPIDUS_RC_CONST (config->save));
}



gboolean
_expidus_rc_config_has_group (const XfceRc *rc,
                           const gchar  *name)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);
  GSList             *list;

  /* atleast one has to have the specified group! */
  for (list = config->rclist; list != NULL; list = list->next)
    if (_expidus_rc_simple_has_group (EXPIDUS_RC_CONST (list->data), name))
      return TRUE;

  return FALSE;
}



void
_expidus_rc_config_set_group (XfceRc      *rc,
                           const gchar *name)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);
  GSList       *list;

  for (list = config->rclist; list != NULL; list = list->next)
    _expidus_rc_simple_set_group (EXPIDUS_RC (list->data), name);
}



void
_expidus_rc_config_delete_entry (XfceRc       *rc,
                              const gchar  *key,
                              gboolean      global)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);

  _expidus_rc_simple_delete_entry (EXPIDUS_RC (config->save), key, global);
}



gboolean
_expidus_rc_config_has_entry (const XfceRc *rc,
                           const gchar  *key)
{
  const XfceRcConfig *config = EXPIDUS_RC_CONFIG_CONST (rc);
  GSList             *list;

  /* atleast one has to have the specified entry! */
  for (list = config->rclist; list != NULL; list = list->next)
    if (_expidus_rc_simple_has_entry (EXPIDUS_RC_CONST (list->data), key))
      return TRUE;

  return FALSE;
}



const gchar*
_expidus_rc_config_read_entry (const XfceRc *rc,
                            const gchar  *key,
                            gboolean      translated)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);
  const gchar  *value;
  GSList       *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      value = _expidus_rc_simple_read_entry (EXPIDUS_RC_CONST (list->data),
            key,
            translated);
      if (value != NULL)
        return value;
    }

  return NULL;
}



void
_expidus_rc_config_write_entry (XfceRc      *rc,
                             const gchar *key,
                             const gchar *value)
{
  XfceRcConfig *config = EXPIDUS_RC_CONFIG (rc);

  /* XXX - don't write default values */
  if (!_expidus_rc_simple_is_readonly (EXPIDUS_RC (config->save)))
    _expidus_rc_simple_write_entry (EXPIDUS_RC (config->save), key, value);
}



#define __EXPIDUS_RC_CONFIG_C__
#include <libexpidus1util/libexpidus1util-aliasdef.c>
