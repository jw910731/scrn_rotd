project(scrn_rotd_src)

link_libraries(ddcutil Xrandr X11)

add_executable(scrn_rotd_src main.cpp XWindows.h ddcutil_wrapper.h)

