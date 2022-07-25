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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstmyarvtest
 *
 * The myarvtest element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! myarvtest ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#include "gst/gstversion.h"
#ifdef HAVE_CONFIG_H
#endif

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>
#include "gstmyarvtest.h"
#include "gst/base/gstpushsrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_my_arv_test_debug_category);
#define GST_CAT_DEFAULT gst_my_arv_test_debug_category

/* функции для работы с камерой */
static gboolean myarvtest_init_camera (GstMyArvTest *myarvtest);

/* Переопределнные функции класа GstBasesrc */

static void gst_my_arv_test_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_my_arv_test_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_my_arv_test_finalize (GObject * object);

static gboolean gst_my_arv_test_start (GstBaseSrc * src);
static gboolean gst_my_arv_test_stop (GstBaseSrc * src);
static void gst_my_arv_test_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static GstFlowReturn gst_my_arv_test_create (GstPushSrc * push_src,
                                             GstBuffer ** buffer);

/* =================== */

enum
{
  PROP_0,
  PROP_CAMERA_NAME,
  PROP_CAMERA,

};

// определенение типа нвшего объекта
G_DEFINE_TYPE (GstMyArvTest, gst_my_arv_test, GST_TYPE_PUSH_SRC);


/*  Описание пэдов */

static GstStaticPadTemplate gst_my_arv_test_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );



/* функция инициализации камеры, если успешно, то TRUE */
static gboolean
myarvtest_init_camera (GstMyArvTest *myarvtest)
{
	GError *error = NULL;

	if (myarvtest->camera != NULL)
		g_object_unref (myarvtest->camera);

  // получаем ссылку на обект ArvCamera  
	myarvtest->camera = arv_camera_new (myarvtest->camera_name, &error);

  if (error) return FALSE;

	return TRUE;
}


// G_DEFINE_TYPE_WITH_CODE (GstMyArvTest, gst_my_arv_test, GST_TYPE_BASE_SRC,
//   GST_DEBUG_CATEGORY_INIT (gst_my_arv_test_debug_category, "myarvtest", 0,
//   "debug category for myarvtest element"));

/* Инициализация класса */
static void
gst_my_arv_test_class_init (GstMyArvTestClass * klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GstBaseSrcClass *gstbasesrc_class = GST_BASE_SRC_CLASS (klass);
	GstPushSrcClass *gstpushsrc_class = GST_PUSH_SRC_CLASS (klass);


  /* переопределнные функции */
  gobject_class->set_property = GST_DEBUG_FUNCPTR(gst_my_arv_test_set_property);
  gobject_class->get_property = GST_DEBUG_FUNCPTR(gst_my_arv_test_get_property);
  gobject_class->finalize = GST_DEBUG_FUNCPTR(gst_my_arv_test_finalize);

  g_object_class_install_property
		(gobject_class,
		 PROP_CAMERA_NAME,
		 g_param_spec_string ("camera_name",
				      "Имя камеры",
				      "Введенное пользователем имя камеры",
				      NULL,
				      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property
		(gobject_class,
		 PROP_CAMERA,
		 g_param_spec_object ("camera",
				      "ArvCamera",
				      "Объект типа ArvCamera указывающий на камеру",
				              ARV_TYPE_CAMERA,
				      G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_my_arv_test_src_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gstbasesrc_class->start = GST_DEBUG_FUNCPTR (gst_my_arv_test_start);
  gstbasesrc_class->stop = GST_DEBUG_FUNCPTR (gst_my_arv_test_stop);
  // ???????
  gstbasesrc_class->get_times = GST_DEBUG_FUNCPTR (gst_my_arv_test_get_times);
  gstpushsrc_class->create = GST_DEBUG_FUNCPTR (gst_my_arv_test_create);
}

/* инициализация объекта класса */
static void
gst_my_arv_test_init (GstMyArvTest *myarvtest)
{
   gst_base_src_set_live (GST_BASE_SRC (myarvtest), TRUE);
   // ????
	 gst_base_src_set_format (GST_BASE_SRC (myarvtest), GST_FORMAT_TIME);

	myarvtest->camera_name = NULL;
  myarvtest->camera = NULL;
  myarvtest->stream = NULL;
}

/* установка свойств - только имя */
void
gst_my_arv_test_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMyArvTest *myarvtest = GST_MY_ARV_TEST (object);
  GST_DEBUG_OBJECT (myarvtest, "set_property");

  switch (property_id) {
      case PROP_CAMERA_NAME:
        GST_OBJECT_LOCK (myarvtest);
        g_free (myarvtest->camera_name);
        if(myarvtest->stream == NULL){
          /* читаем название камеры и подключаемся к ней */ 
          myarvtest->camera_name = g_strdup (g_value_get_string (value));
          myarvtest_init_camera (myarvtest);
        }

        GST_LOG_OBJECT (myarvtest, "Имя камеры: %s", myarvtest->camera_name);
        GST_OBJECT_UNLOCK (myarvtest);
        break;

      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* вернуть свойство property_id в value */
void
gst_my_arv_test_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMyArvTest *myarvtest = GST_MY_ARV_TEST(object);

	// GST_DEBUG_OBJECT (myarvtest, "Полечений свойсва %s", pspec->name);

	switch (property_id) {
		case PROP_CAMERA_NAME:
			g_value_set_string (value, myarvtest->camera_name);
			break;
		case PROP_CAMERA:
			g_value_set_object (value, myarvtest->camera);
			break;

    default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
  }
}

/* освобождение памяти и очитска всех объектов */
void
gst_my_arv_test_finalize (GObject * object)
{

	GstMyArvTest *myarvtest = GST_MY_ARV_TEST(object);
	ArvCamera *camera;
  ArvStream *stream;

	GST_OBJECT_LOCK (myarvtest);
	camera = g_steal_pointer (&myarvtest->camera);
  stream = g_steal_pointer(&myarvtest->stream);
	g_clear_pointer (&myarvtest->camera_name, g_free);
	GST_OBJECT_UNLOCK (myarvtest);

	if (camera != NULL) g_object_unref (camera);
  if (stream != NULL) g_object_unref (stream);

	G_OBJECT_CLASS (gst_my_arv_test_parent_class)->finalize (object);

}


/* start and stop processing, ideal for opening/closing the resource */
static gboolean
gst_my_arv_test_start (GstBaseSrc * src)
{
  GError *error = NULL;
	gboolean result = TRUE;
  size_t payload;
  GstMyArvTest *myarvtest = GST_MY_ARV_TEST (src);

	GST_LOG_OBJECT (myarvtest, "Подключение к камере '%s'", myarvtest->camera_name);

	GST_OBJECT_LOCK (myarvtest);
  // если камеры еще нет, подключаемся к ней
	if (myarvtest->camera == NULL){
    result = myarvtest_init_camera (myarvtest);
  }

  myarvtest->stream = arv_camera_create_stream (myarvtest->camera, NULL, NULL, &error);

  payload = arv_camera_get_payload (myarvtest->camera, &error);
	for (int i = 0; i < 50; i++)
		arv_stream_push_buffer (myarvtest->stream,
                          arv_buffer_new (payload, NULL));

	GST_LOG_OBJECT (myarvtest, "Запуск потока...");
	arv_camera_start_acquisition (myarvtest->camera, &error);
	GST_OBJECT_UNLOCK (myarvtest);

  // В случае ошибки с инициализацей камеры
	if (error)  result = FALSE;

	return result;

}

// остновка процесса
static gboolean
gst_my_arv_test_stop (GstBaseSrc * src)
{
  GError *error = NULL;
  GstMyArvTest *myarvtest = GST_MY_ARV_TEST (src);
  ArvStream *stream;

  GST_OBJECT_LOCK (myarvtest);
	arv_camera_stop_acquisition (myarvtest->camera, &error);
	stream = g_steal_pointer (&myarvtest->stream);
	GST_OBJECT_UNLOCK (myarvtest);

	if (stream != NULL)
		g_object_unref (stream);

  GST_DEBUG_OBJECT (myarvtest, "Остоновка потока...");

	return TRUE;
}

/* given a buffer, return start and stop time when it should be pushed
 * out. The base class will sync on the clock using these times. */
static void
gst_my_arv_test_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
  GstMyArvTest *myarvtest = GST_MY_ARV_TEST (src);
  GST_DEBUG_OBJECT (myarvtest, "get_times");

}

/*   Вернуть буффер из источника */
static GstFlowReturn
gst_my_arv_test_create (GstPushSrc * push_src, GstBuffer ** buffer)
{
    GstMyArvTest *myarvtest = GST_MY_ARV_TEST (push_src);
    int arv_row_stride;
    int width, height;
    char *buffer_data;
    size_t buffer_size;
    ArvBuffer *arv_buffer = NULL;
    GError *error = NULL;


    GST_OBJECT_LOCK (myarvtest);

    // /* Читаем в буффер одно изображение */ 
    // arv_buffer = arv_camera_acquisition(myarvtest->camera, 0, &error);
  	do {
      // начало потока, 
      if (arv_buffer) arv_stream_push_buffer (myarvtest->stream, arv_buffer);
      arv_buffer = arv_stream_pop_buffer (myarvtest->stream);
    } while (arv_buffer != NULL && arv_buffer_get_status (arv_buffer) != ARV_BUFFER_STATUS_SUCCESS);

    if (arv_buffer == NULL)
      goto error;

    /* кладем данные из буффера в массив */
    buffer_data = (char *) arv_buffer_get_data (arv_buffer, &buffer_size);
    arv_buffer_get_image_region (arv_buffer, NULL, NULL, &width, &height);
    arv_row_stride = width * ARV_PIXEL_FORMAT_BIT_PER_PIXEL (arv_buffer_get_image_pixel_format (arv_buffer)) / 8;

    /* ШАБЛОН: Из arvbuffer создает gstbuffer*/
    if ((arv_row_stride & 0x3) != 0) {
      int gst_row_stride;
      size_t size;
      char *data;
      int i;

      gst_row_stride = (arv_row_stride & ~(0x3)) + 4;

      size = height * gst_row_stride;
      data = g_malloc (size);

      for (i = 0; i < height; i++)
        memcpy (data + i * gst_row_stride, buffer_data + i * arv_row_stride, arv_row_stride);
        
      //полученный буффер
      *buffer = gst_buffer_new_wrapped (data, size);
    } else {
      *buffer = gst_buffer_new_wrapped_full (0, buffer_data, buffer_size, 0, buffer_size, NULL, NULL);
    }
    /*=============================*/

    arv_stream_push_buffer (myarvtest->stream, arv_buffer);
    GST_OBJECT_UNLOCK (myarvtest);

    return GST_FLOW_OK;

  error:
    GST_OBJECT_UNLOCK (myarvtest);
    return GST_FLOW_ERROR;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "myarvsrc", GST_RANK_NONE,
      GST_TYPE_MY_ARV_TEST);
}

// /* FIXME: these are normally defined by the GStreamer build system.
//    If you are creating an element to be included in gst-plugins-*,
//    remove these, as they're always defined.  Otherwise, edit as
//    appropriate for your external plugin package. */
// #ifndef VERSION
// #define VERSION "0.0.FIXME"
// #endif
// #ifndef PACKAGE
// #define PACKAGE "FIXME_package"
// #endif
// #ifndef PACKAGE_NAME
// #define PACKAGE_NAME "FIXME_package_name"
// #endif
// #ifndef GST_PACKAGE_ORIGIN
// #define GST_PACKAGE_ORIGIN "http://FIXME.org/"
// #endif



#define PACKAGE "myarvsrc"

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
		   GST_VERSION_MINOR,
		   myarvsrc,
		   "Aravis Video Source",
		   plugin_init,
		   ARAVIS_VERSION,
		   "LGPL",
		   "myarvsrc",
		   "http://google.com")
