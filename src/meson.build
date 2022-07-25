# cdata = configuration_data()
# cdata.set_quoted('PACKAGE_VERSION', gst_version)
# cdata.set_quoted('PACKAGE', 'gst-template-plugin')
# cdata.set_quoted('GST_LICENSE', 'LGPL')
# cdata.set_quoted('GST_API_VERSION', api_version)
# cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer template Plug-ins')
# cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')


gst_sources = [
	'gstmyarvtest.c'
]

gst_headers = [
	'gstmyarvtest.h'
]

# gst_c_args = ['-DHAVE_CONFIG_H']

gst_c_args = [
	'-DPACKAGE_LOCALE_DIR=@0@'.format(get_option ('prefix')),
	'-DG_LOG_DOMAIN="Aravis"'
]

# создаем library
gst_plugin = library ('gstmyarvsrc',
			     gst_sources, gst_headers,
			     c_args: gst_c_args,
			     dependencies: [gst_dep, arv_dep, gstbase_dep],
			     install: true,
			     install_dir: gst_plugin_dir)

