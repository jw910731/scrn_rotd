

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <X11/extensions/Xrandr.h>
#include <fstream>

#include "ddcutil_wrapper.h"
#include "XWindows.h"

static bool saved_report_ddc_errors = false;
static bool saved_verify_setvcp = false;

void set_standard_settings() {
    // Report any DDC data errors to the terminal
    saved_report_ddc_errors = ddca_enable_report_ddc_errors(true);
    // Read value after setting it as verification
    saved_verify_setvcp = ddca_enable_verify(true);
}

void restore_standard_settings() {
    ddca_enable_verify(saved_verify_setvcp);
    ddca_enable_report_ddc_errors(saved_report_ddc_errors);
}

sig_atomic_t int_flag = 1;
void int_handler(int){
    int_flag = 0;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << argv[0] << ' ' << "<config file>" << std::endl;
        return EXIT_FAILURE;
    }
    long screen_num;
    std::string SN_number;
    std::fstream conf_fs(argv[1]);
    if(!conf_fs.is_open()){
        std::cerr << argv[0] << ' ' << "<config file>" << std::endl;
        return EXIT_FAILURE;
    }
    conf_fs >> SN_number >> screen_num;

    display_identifier ident;
    display_ref dref;
    display_handle dh;
    DDCA_Status ddcastat;
    set_standard_settings();
    ddcastat = ddca_create_mfg_model_sn_display_identifier(nullptr, nullptr, SN_number.c_str(), &ident.val);
    if(ddcastat != 0){
        return EXIT_FAILURE;
    }
    ddcastat = ddca_get_display_ref(ident, &dref.val);
    if(ddcastat != 0){
        return EXIT_FAILURE;
    }
    ddcastat = ddca_open_display2(dref, true, &dh.val);
    if(ddcastat != 0){
        return EXIT_FAILURE;
    }

    xdisplay display;
    Window root = RootWindow(display.val, int(screen_num));
    xscreen_config config(display, root);

    signal(SIGINT, int_handler);

    Rotation prev_rot = RR_Rotate_0;
    for(;int_flag;) {
        DDCA_Non_Table_Vcp_Value valrec;
        // 0xAA is for orientation
        ddcastat = ddca_get_non_table_vcp_value(dh, 0xAA, &valrec);
        if(ddcastat != 0){
            ddca_error_detail *err = ddca_get_error_detail();
            ddca_report_error_detail(err, 1);
            ddca_free_error_detail(err);
            continue;
        }
        uint16_t max_val = valrec.mh << 8 | valrec.ml;
        uint16_t cur_val = valrec.sh << 8 | valrec.sl;
        Rotation rot;
        SizeID size = XRRConfigCurrentConfiguration(config.val, &rot);

        switch(cur_val){
            case 1:
                if(rot != RR_Rotate_0){ // rotate screen
                    std::cout << "Rotate horizon" << std::endl;
                    XRRSetScreenConfig(display.val, config.val, root, size, RR_Rotate_0, CurrentTime);
                }
                break;
            case 2:
                if(rot != RR_Rotate_90){ // rotate screen
                    std::cout << "Rotate vertical" << std::endl;
                    XRRSetScreenConfig(display.val, config.val, root, size, RR_Rotate_90, CurrentTime);
                }
                break;
            default:
                break;
        }
        sleep(2);
    }

    restore_standard_settings();
    return EXIT_SUCCESS;
}
