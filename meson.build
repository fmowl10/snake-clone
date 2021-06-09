project('snake-clone', 'cpp', version:'0.0.1')
src=['src/board.cpp', 'src/main.cpp', 'src/snake.cpp',]
dep=[dependency('ncurses')]

conf_data = configuration_data()
conf_data.set('name', meson.project_name())

configure_file(input : 'config.h.in', output : 'config.h', configuration: conf_data)
conf_inc = include_directories('.')


install_data(['stage-1.map', 'stage-2.map', 'stage-3.map', 'stage-4.map', 'stage-5.map' ])
executable(meson.project_name(), src, dependencies:dep, install:true, include_directories : conf_inc)

