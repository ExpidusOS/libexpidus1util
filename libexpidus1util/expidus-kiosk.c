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
 * SECTION: expidus-kiosk
 * @title: Expidus Kiosk functions
 * @short_description: Expidus Kiosk mode support functions.
 *
 * This module provides a simple Kiosk mode for Expidus.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_GRP_H
#include <grp.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libexpidus1util/libexpidus1util.h>
#include <libexpidus1util/libexpidus1util-alias.h>



#define KIOSKRC (KIOSKDIR "/kioskrc")


struct _ExpidusKiosk
{
  GObject __parent__;

  gchar  *module_name;
  ExpidusRc *module_rc;
};


static const gchar *expidus_kiosk_lookup  (const ExpidusKiosk *kiosk,
                                        const gchar     *capability);
static gboolean     expidus_kiosk_chkgrp  (const gchar     *group);
static time_t       mtime              (const gchar     *path);
static void         expidus_kiosk_finalize (GObject        *object);


static gchar        *usrname = NULL;
static gchar       **groups;
static time_t        kiosktime = 0;
static const gchar  *kioskdef = NULL;
static ExpidusRc       *kioskrc = NULL;

G_DEFINE_TYPE (ExpidusKiosk, expidus_kiosk, G_TYPE_OBJECT)
G_LOCK_DEFINE_STATIC (kiosk_lock);


static void
expidus_kiosk_class_init (ExpidusKioskClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = expidus_kiosk_finalize;
}

/**
 * expidus_kiosk_new: (constructor)
 * @module: The Expidus project to query about
 *
 * Creates and returns a new instance of #ExpidusKiosk.
 *
 * Return value: (transfer full): a new instance of #ExpidusKiosk.
 *
 * Since: 4.2
 **/
ExpidusKiosk*
expidus_kiosk_new (const gchar *module)
{
  ExpidusKiosk *kiosk;
  gchar      path[1024];

  g_return_val_if_fail (module != NULL, NULL);
  g_return_val_if_fail (g_strcmp0 (module, "General") != 0, NULL);

  g_snprintf (path, 1024, "%s/%s.kioskrc", KIOSKDIR, module);

  kiosk               = g_object_new (EXPIDUS_TYPE_KIOSK, NULL);
  kiosk->module_name  = g_strdup (module);
  kiosk->module_rc    = expidus_rc_simple_open (path, TRUE);

  return kiosk;
}


/**
 * expidus_kiosk_query:
 * @kiosk:      A #ExpidusKiosk.
 * @capability: The name of the capability to check.
 *
 * Queries the @kiosk object for a given capability and returns %TRUE if
 * the current user has the @capability, else %FALSE.
 *
 * Return value: %TRUE if the current user has the @capability, else %FALSE.
 *
 * Since: 4.2
 **/
gboolean
expidus_kiosk_query (const ExpidusKiosk *kiosk,
                  const gchar     *capability)
{
  const gchar *value;
  gboolean     result;
  gchar      **vector;
  gchar       *string;
  gint         n;

  g_return_val_if_fail (kiosk != NULL, FALSE);
  g_return_val_if_fail (capability != NULL, FALSE);

  if (G_UNLIKELY (usrname == NULL))
    return FALSE;

  value = expidus_kiosk_lookup (kiosk, capability);

  /* most common case first! */
  if (G_LIKELY (value[0] == 'A'
            &&  value[1] == 'L'
            &&  value[2] == 'L'
            && (value[3] == '\0' || value[4] == ' ')))
    {
      return TRUE;
    }

  if (G_LIKELY (value[0] == 'N'
            &&  value[1] == 'O'
            &&  value[2] == 'N'
            &&  value[3] == 'E'
            && (value[4] == '\0' || value[4] == ' ')))
    {
      return FALSE;
    }

  vector = g_strsplit (value, ",", -1);
  for (n = 0, result = FALSE; vector[n] != NULL; ++n)
    {
      string = vector[n];
      if (*string == '%' && expidus_kiosk_chkgrp (string + 1))
        {
          result = TRUE;
          break;
        }
      else if (strcmp (usrname, string) == 0)
        {
          result = TRUE;
          break;
        }
    }
  g_strfreev (vector);

  return result;
}


static void
expidus_kiosk_finalize (GObject *object)
{
  ExpidusKiosk *kiosk = EXPIDUS_KIOSK (object);

  g_return_if_fail (kiosk != NULL);

  if (kiosk->module_rc != NULL)
    expidus_rc_close (kiosk->module_rc);
  g_free (kiosk->module_name);
}


/**
 * expidus_kiosk_free:
 * @kiosk: A #ExpidusKiosk.
 *
 * Frees the @kiosk object.
 * In 4.13 and above, this is equivalent to calling g_clear_object.
 *
 * Since: 4.2
 */
void
expidus_kiosk_free (ExpidusKiosk *kiosk)
{
  /* finalize takes care of things in case the consumer calls unref
   * themselves instead of this function */
  g_clear_object (&kiosk);
}


static const gchar*
expidus_kiosk_lookup (const ExpidusKiosk *kiosk,
                   const gchar     *capability)
{
  const gchar *value;

  if (G_UNLIKELY (kiosk->module_rc != NULL))
    {
      value = expidus_rc_read_entry (kiosk->module_rc, capability, NULL);
      if (value != NULL)
        return value;
    }

  if (G_UNLIKELY (kioskrc != NULL))
    {
      G_LOCK (kiosk_lock);

      expidus_rc_set_group (kioskrc, kiosk->module_name);
      value = expidus_rc_read_entry (kioskrc, capability, NULL);

      G_UNLOCK (kiosk_lock);

      if (value != NULL)
        return value;
    }

  return kioskdef;
}


static gboolean
expidus_kiosk_chkgrp (const gchar *group)
{
  int n;

  for (n = 0; groups[n] != NULL; ++n)
    if (strcmp (group, groups[n]) == 0)
      return TRUE;

  return FALSE;
}


static void
expidus_kiosk_init (ExpidusKiosk *kiosk)
{
  struct passwd *pw;
  struct group  *gr;
  gid_t         *gidset;
  int            gidsetlen;
  int            n;
  int            m;
  time_t         timestamp;

  G_LOCK (kiosk_lock);

  /* reload kioskrc */
  timestamp = mtime (KIOSKRC);
  if (timestamp > kiosktime || kioskdef == NULL)
    {
      if (kioskrc != NULL)
        expidus_rc_close (kioskrc);

      kiosktime = timestamp;
      kioskrc = expidus_rc_simple_open (KIOSKRC, TRUE);
      if (kioskrc != NULL)
        {
          expidus_rc_set_group (kioskrc, "General");
          kioskdef = expidus_rc_read_entry (kioskrc, "Default", KIOSKDEF);
        }
      else
        {
          kioskdef = KIOSKDEF;
        }
    }

  if (G_LIKELY (usrname != NULL))
    {
      G_UNLOCK (kiosk_lock);
      return;
    }

  /* determine user name */
  pw = getpwuid (getuid ());
  if (G_UNLIKELY (pw == NULL))
    {
      g_warning ("Unable to determine your username, all kiosk protected features "
                 "will be disabled for you. Please check your system setup "
                 "or ask your administrator.");
      G_UNLOCK (kiosk_lock);
      return;
    }
  usrname = g_strdup (pw->pw_name);

  /* query number of user groups */
  gidsetlen = getgroups (0, NULL);
  if (G_UNLIKELY (gidsetlen < 0))
    {
      g_warning ("Unable to determine the number of groups for your user account, "
                 "all kiosk protected features will be disabled for you. Please "
                 "check your system setup or ask your administrator.");
      g_free (usrname); usrname = NULL;
      G_UNLOCK (kiosk_lock);
      return;
    }

  /* query user groups */
  gidset = g_malloc (gidsetlen * sizeof (*gidset));
  gidsetlen = getgroups (gidsetlen, gidset);
  if (G_UNLIKELY (gidsetlen < 0))
    {
      g_warning ("Unable to determine your current group access list, all kiosk "
                 "protected features will be disabled for you. Please check "
                 "your system setup or ask your administrator.");
      g_free (usrname); usrname = NULL;
      G_UNLOCK (kiosk_lock);
      g_free (gidset);
      return;
    }
  groups = g_new (gchar *, gidsetlen + 1);
  for (n = m = 0; n < gidsetlen; ++n)
    {
      gr = getgrgid (gidset[n]);
      if (G_LIKELY (gr != NULL))
        groups[m++] = g_strdup (gr->gr_name);
    }
  groups[m] = NULL;
  g_free (gidset);

  G_UNLOCK (kiosk_lock);
}


static time_t
mtime (const gchar *path)
{
  struct stat sb;

  if (G_UNLIKELY (path == NULL) || stat (path, &sb) < 0)
    return (time_t) 0;

  return sb.st_mtime;
}



#define __EXPIDUS_KIOSK_C__
#include <libexpidus1util/libexpidus1util-aliasdef.c>
