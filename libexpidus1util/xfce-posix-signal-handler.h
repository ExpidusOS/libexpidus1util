/*
 * Copyright (c) 2007 Brian Tarricone <bjt23@cornell.edu>
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

#ifndef __EXPIDUS_POSIX_SIGNAL_HANDLER_H__
#define __EXPIDUS_POSIX_SIGNAL_HANDLER_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * XfcePosixSignalHandler:
 * @signal: The signal that was caught.
 * @user_data: The @user_data parameter passed when the handler was registered.
 */

typedef void (*XfcePosixSignalHandler)(gint signal, gpointer user_data);

gboolean expidus_posix_signal_handler_init(GError **error);
void expidus_posix_signal_handler_shutdown(void);

gboolean expidus_posix_signal_handler_set_handler(gint signal,
                                               XfcePosixSignalHandler handler,
                                               gpointer user_data,
                                               GError **error);
void expidus_posix_signal_handler_restore_handler(gint signal);

G_END_DECLS

#endif  /* __EXPIDUS_POSIX_SIGNAL_HANDLER_H__ */
