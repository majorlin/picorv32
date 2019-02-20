#------------------------------------------------------------------------------
# SYNOPSYS CONFIDENTIAL -- This is an unpublished, proprietary work of
# Synopsys, Inc., and is fully protected under copyright and trade secret
# laws. You may not view, use, disclose, copy, or distribute this file or
# any information contained herein except pursuant to a valid written
# license from Synopsys.
#------------------------------------------------------------------------------
# Configuration script created by zConfig

puts "# Configuration used is:"
puts "#   valid machines   : "
puts "#   created by       : b37173"
puts "#   date             : Wed Jan 24 11:59:41 2018"
puts "#   based on release : /run/pkg/synopsys-zebu-/M-2017.03-SP1-1"
puts "#   platform type    : ZS3"

proc loadfile { file } {
  if { ![file exist $file] || ![file readable $file] } {
    puts "### Error: Cannot access file $file. Please check your installation."
  }
  uplevel source $file
}
System quiet

set path_cfg "/proj/mcu_zebu_adm/ZEBU_SYSTEM_DIR/XC7V-503_81_81_81_81_81_A/config"

loadfile $env(ZEBU_ROOT)/etc/sys/ZSE/configs/zse_basic_conf.tcl
loadfile $env(ZEBU_ROOT)/etc/sys/ZSE/configs/zse_sbp_5s_v3.tcl

Target set zebu.frequency 450
Target set zebu.trace "on"



### Module 0
loadfile $env(ZEBU_ROOT)/etc/sys/ZSE/configs/zse_xc7v_8c_2000_v1_conf.tcl
Target set zebu.module 0 U0.M0 zse_xc7v_8c_2000_v1 { 07C2}

### Module 1
Target set zebu.module 1 U0.M1 zse_xc7v_8c_2000_v1 { 1172}

### Module 2
Target set zebu.module 2 U0.M2 zse_xc7v_8c_2000_v1 { 1152}

### Module 3
Target set zebu.module 3 U0.M3 zse_xc7v_8c_2000_v1 { 1762}

### Module 4
Target set zebu.module 4 U0.M4 zse_xc7v_8c_2000_v1 { 1642}

puts "#"
puts "#  pos | name  | module"
puts "# -----+-------+--------------------------"
puts "#    0 | U0.M0 | zse_xc7v_8c_2000_v1 "
puts "#    1 | U0.M1 | zse_xc7v_8c_2000_v1 "
puts "#    2 | U0.M2 | zse_xc7v_8c_2000_v1 "
puts "#    3 | U0.M3 | zse_xc7v_8c_2000_v1 "
puts "#    4 | U0.M4 | zse_xc7v_8c_2000_v1 "
puts "#"
