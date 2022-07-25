/* GStreamer
 * Copyright (C) 2022 FIXME <fixme@example.com>
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
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_MY_ARV_TEST_H_
#define _GST_MY_ARV_TEST_H_

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include <arv.h>

G_BEGIN_DECLS

#define GST_TYPE_MY_ARV_TEST   (gst_my_arv_test_get_type())
#define GST_MY_ARV_TEST(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MY_ARV_TEST,GstMyArvTest))
#define GST_MY_ARV_TEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MY_ARV_TEST,GstMyArvTestClass))
#define GST_IS_MY_ARV_TEST(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MY_ARV_TEST))
#define GST_IS_MY_ARV_TEST_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MY_ARV_TEST))

typedef struct _GstMyArvTest GstMyArvTest;
typedef struct _GstMyArvTestClass GstMyArvTestClass;

struct _GstMyArvTest
{
  GstPushSrc base_myarvtest;

  char *camera_name; // имя камеры
  ArvCamera *camera; // укзаатель на объект камеры
  ArvStream *stream; // указатель на обект потока

};

struct _GstMyArvTestClass
{
  GstPushSrcClass base_myarvtest_class;

};

GType gst_my_arv_test_get_type (void);

G_END_DECLS

#endif
