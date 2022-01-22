

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include "ddcutil_wrapper.h"

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
        std::cerr << argv[0] << ' ' << "<SN Number>" << std::endl;
        return EXIT_FAILURE;
    }
    display_identifier ident;
    display_ref dref;
    display_handle dh;
    DDCA_Status ddcastat;
    set_standard_settings();
    ddcastat = ddca_create_mfg_model_sn_display_identifier(nullptr, nullptr, argv[1], &ident.val);
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

    signal(SIGINT, int_handler);

    for(;int_flag;) {
        DDCA_Non_Table_Vcp_Value valrec;
        // 0xAA is for orientation
        ddcastat = ddca_get_non_table_vcp_value(dh, 0xAA, &valrec);
        if(ddcastat != 0){
            ddca_error_detail *err = ddca_get_error_detail();
            ddca_report_error_detail(err, 1);
            ddca_free_error_detail(err);
            return EXIT_FAILURE;
        }
        uint16_t max_val = valrec.mh << 8 | valrec.ml;
        uint16_t cur_val = valrec.sh << 8 | valrec.sl;
        std::cout << cur_val << std::endl;
        sleep(1);
    }

    restore_standard_settings();
    return EXIT_SUCCESS;
}