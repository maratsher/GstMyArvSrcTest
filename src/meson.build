gst_sources = [
	'gstmyarvtest.c'
]

gst_headers = [
	'gstmyarvtest.h'
]

gst_c_args = ['-DHAVE_CONFIG_H']

# создаем shared_library
gst_plugin = shared_library ('MyArvTestPlugin',
			     gst_sources, gst_headers,
			     c_args: gst_c_args,
			     dependencies: [gst_dep, arv_dep],
			     install: true,
			     install_dir: gst_plugin_dir)

