#written by zCui
set zcui_ztime_constraint_file {../utf_generatefiles/zTime_config.tcl}
set zcui_ztime_clock_combi true
set zcui_ztime_routing_async true
set zcui_ztime_save_run_param true
set post_fpga_mode false
set place_fpga_mode false
set vivado_filter_time false
set vivado_skew_time false
set fpga_family V7
source $env(ZEBU_ROOT)/etc/ztime/zTime_from_zcui.tcl
