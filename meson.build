project('snake-clone', 'cpp', version:'0.0.1', default_options : ['cpp_std=c++14', 'prefix=/usr'])
src=['src/board.cpp', 'src/main.cpp', 'src/snake.cpp',]
dep=[dependency('ncurses')]

if get_option('buildtype').startswith('debug')
    add_project_arguments('-DDEBUG', language:'cpp')
endif

conf_data = configuration_data()
conf_data.set('name', meson.project_name())

configure_file(input : 'config.h.in', output : 'config.h', configuration: conf_data)
conf_inc = include_directories('.')


install_data(['stage-1.map', 'stage-2.map', 'stage-3.map', 'stage-4.map', 'stage-5.map' ], install_dir : join_paths(get_option('datadir'), meson.project_name()))
executable(meson.project_name(), src, dependencies:dep, install:true, include_directories : conf_inc)
