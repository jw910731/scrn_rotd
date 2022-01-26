//
// Created by jw910731 on 2022/1/23.
//

#ifndef SCRN_ROTD_XWINDOWS_H
#define SCRN_ROTD_XWINDOWS_H

#include <X11/Xlib.h>

#include <stdexcept>

struct xdisplay{
    Display *val;
    ~xdisplay(){
        XCloseDisplay(val);
    }
    xdisplay(){
        val = XOpenDisplay(nullptr);
        if(val == nullptr)
            throw std::invalid_argument("No X11 display provided (through environment variable)");
    }
    xdisplay(const xdisplay &) = delete;
    xdisplay(xdisplay &&) = delete;
    xdisplay operator=(const xdisplay&) = delete;
    xdisplay &operator=(xdisplay &&) = delete;
};

struct xscreen_config{
    XRRScreenConfiguration *val;
    xscreen_config(xdisplay &dpy, Window draw){
        val = XRRGetScreenInfo(dpy.val, draw);
        if(val == nullptr){
            throw std::invalid_argument("something went wrong");
        }
    }
    ~xscreen_config(){
        XRRFreeScreenConfigInfo(val);
    }
    xscreen_config() = delete;
    xscreen_config(const xscreen_config &) = delete;
    xscreen_config(xscreen_config &&) = delete;
    xscreen_config operator=(const xscreen_config &) = delete;
    xscreen_config &operator=(xscreen_config &&) = delete;
};


#endif //SCRN_ROTD_XWINDOWS_H
