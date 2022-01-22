//
// Created by jw910731 on 2022/1/22.
//

#ifndef SCRN_ROTD_DDCUTIL_WRAPPER_H
#define SCRN_ROTD_DDCUTIL_WRAPPER_H

#include <ddcutil_c_api.h>
#include <ddcutil_status_codes.h>

struct display_identifier{
    DDCA_Display_Identifier val;
    operator DDCA_Display_Identifier() const {
        return val;
    }
    display_identifier(){}
    display_identifier(const display_identifier &) = delete;
    display_identifier(display_identifier &&) = delete;
    display_identifier &operator=(const display_identifier&) = delete;
    display_identifier &operator=(display_identifier&&) = delete;
    ~display_identifier(){
        ddca_free_display_identifier (val);
    }
};

struct display_ref{
    DDCA_Display_Ref val;
    operator DDCA_Display_Ref() const{
        return val;
    }
    display_ref(){}
    display_ref(const display_ref &) = delete;
    display_ref(display_ref &&) = delete;
    display_ref &operator=(const display_ref&) = delete;
    display_ref &operator=(display_ref&&) = delete;
    ~display_ref(){
        ddca_free_display_ref(val);
    }
};

struct display_handle{
    DDCA_Display_Handle val;
    operator DDCA_Display_Handle () const{
        return val;
    }
    display_handle(){}
    display_handle(const display_handle &) = delete;
    display_handle(display_handle &&) = delete;
    display_handle &operator=(const display_handle&) = delete;
    display_handle &operator=(display_handle&&) = delete;
    ~display_handle(){
        ddca_close_display(val);
    }
};

#endif //SCRN_ROTD_DDCUTIL_WRAPPER_H
