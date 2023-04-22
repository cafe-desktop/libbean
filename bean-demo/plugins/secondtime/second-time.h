/*
 * second-time.h
 * This file is part of libbean
 *
 * Copyright (C) 2010 Steve Frécinaux
 *
 * libbean is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libbean is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */
#ifndef __BEANDEMO_SECOND_TIME_H__
#define __BEANDEMO_SECOND_TIME_H__

#include <gtk/gtk.h>
#include <libbean/bean.h>

G_BEGIN_DECLS

#define BEANDEMO_TYPE_SECOND_TIME         (beandemo_second_time_get_type ())
#define BEANDEMO_SECOND_TIME(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), BEANDEMO_TYPE_SECOND_TIME, BeanDemoSecondTime))
#define BEANDEMO_SECOND_TIME_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), BEANDEMO_TYPE_SECOND_TIME, BeanDemoSecondTime))
#define BEANDEMO_IS_SECOND_TIME(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), BEANDEMO_TYPE_SECOND_TIME))
#define BEANDEMO_IS_SECOND_TIME_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), BEANDEMO_TYPE_SECOND_TIME))
#define BEANDEMO_SECOND_TIME_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), BEANDEMO_TYPE_SECOND_TIME, BeanDemoSecondTimeClass))

typedef struct _BeanDemoSecondTime       BeanDemoSecondTime;
typedef struct _BeanDemoSecondTimeClass  BeanDemoSecondTimeClass;

struct _BeanDemoSecondTime {
  BeanExtensionBase parent_instance;

  GtkWidget *window;
  GtkWidget *label;
};

struct _BeanDemoSecondTimeClass {
  BeanExtensionBaseClass parent_class;
};

GType                 beandemo_second_time_get_type               (void) G_GNUC_CONST;
G_MODULE_EXPORT void  bean_register_types                         (BeanObjectModule *module);

G_END_DECLS

#endif /* __BEANDEMO_SECOND_TIME_H__ */
