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

#ifndef __EXPIDUS_KIOSK_H__
#define __EXPIDUS_KIOSK_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define EXPIDUS_TYPE_KIOSK expidus_kiosk_get_type ()
G_DECLARE_FINAL_TYPE (ExpidusKiosk, expidus_kiosk, EXPIDUS, KIOSK, GObject)

ExpidusKiosk *expidus_kiosk_new   (const gchar     *module) G_GNUC_MALLOC;
gboolean   expidus_kiosk_query (const ExpidusKiosk *kiosk,
                             const gchar     *capability);
void       expidus_kiosk_free  (ExpidusKiosk       *kiosk);

G_END_DECLS

#endif /* !__EXPIDUS_KIOSK_H__ */
