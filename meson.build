project('ArvTestPlugin', 'c', version : '1.20.0')

gst_plugin_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'

# подключаем зависимости gst и arv 
gst_dep = dependency('gstreamer-1.0', version : '>=1.19',
    required : true, fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-1.0', version : '>=1.19',
  fallback : ['gstreamer', 'gst_base_dep'])
arv_dep = dependency('aravis-0.8', version : '>=0.8',
  fallback : ['aravis', 'arv_dep'])

subdir('src')

