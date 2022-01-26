project(scrn_rotd)

link_libraries(ddcutil Xrandr X11)

add_executable(scrn_rotd main.cpp XWindows.h ddcutil_wrapper.h)

install(TARGETS scrn_rotd DESTINATION bin)

