#! bin/csh -f

### Shell setup script for ZEBU ENV

#### Integration Make File Alias 
echo "-------------------------------------------------------"
echo "Setting Alias Commands"
setenv workarea $PWD


### Default project settings setup VCS tools
unsetenv VERDI_HOME
unsetenv NOVAS_HOME
unsetenv DEBUSSY_HOME 

setenv VIVADO_ENABLE_MULTITHREADING 2
### VCS backward compat
#setenv SNPS_SIM_BC_CONFIG_FILE $workarea/zebu/settings/synopsys_bc.setup

### Patch for ZDFI model
#eval "`add_pkg synopsys-zebu-/M-2017.03-SP1-1`"
#eval `add_pkg synopsys-vcs_mx-/M-2017.03-SP2-1-TZ-20171215-eng`
#eval `add_pkg synopsys-verdi3-/M-2017.03-SP2-1`

source /pkg/synopsys-zebu-/M-2017.03-SP1-4/x86_64-linux2.6-glibc2.12/env/cshrc
source /pkg/synopsys-vcs_mx-/M-2017.03-SP2-10-Z/x86_64-linux2.6-glibc2.12/env/cshrc
source /pkg/synopsys-verdi3-/M-2017.03-SP2-11.Z-2/x86_64-linux2.6-glibc2.12/env/cshrc

source /run/pkg/fs-emu2lsf-/2018.05/env/cshrc

setenv LD_LIBRARY_PATH $VERDI_HOME/share/FsdbWriter/LINUX64:$LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH $VERDI_HOME/share/FsdbReader/LINUX64:$LD_LIBRARY_PATH

#### Setup for Timing Failure seen in Vivado static timing analysis
setenv VIVADO_FORCE_REPORT_TIMING

#### Must Use make 3.1 or PAR fails
setenv PATH /pkg/gnu-make-/3.81/i686-linux/bin:/run/pkg/foundation-/frame/bin:$PATH 

# decrease FSDB reader thread count to 1 
setenv FFR_MT_THREAD_COUNT 1

### FSDB Updated fwc dumping
setenv  FFR_ENV_SKIP_CHECKSUM YES

#### python Setup
setenv PATH /run/pkg/OSS-anaconda-/1.8.0/bin:$PATH

if (${?LM_LICENSE_FILE} == 1) then
setenv LM_LICENSE_FILE ${LM_LICENSE_FILE}:26585@curl-e:26585@cde-az84-108va
else
setenv LM_LICENSE_FILE 26585@curl-e
endif

setenv SNPSLMD_LICENSE_FILE  26585@cde-az84-108va:26585@curl-a:26585@curl-b:26585@curl-c:26585@curl-d:26585@curl-e:26585@curl-f:26585@curl-h:26585@curl-k:26585@curl-x:26585@curl-y:26585@luge-b:26585@run-a:26585@run-c:26585@skate-c:9707@liv0019.nxdi.us-cdc01.nxp.com:9707@liv0057.nxdi.us-cdc01.nxp.com:9707@liv0057.nxdi.us-cdc01.nxp.com]9707@liv0061.nxdi.nl-cdc01.nxp.com:9707@liv0061.nxdi.nl-cdc01.nxp.com:9707@liv0062.nxdi.nl-cdc01.nxp.com:9707@liv0064.nxdi.us-cdc01.nxp.com:9707@liv6012.in-blr01.nxp.com:9707@liv9009.nxdi.de-hbg01.nxp.com:mailto

setenv XILINXD_LICENSE_FILE "mailto:9707@liv0057.nxdi.us-cdc01.nxp.com:9712@liv0057.nxdi.us-cdc01.nxp.com]9707@liv0057.nxdi.us-cdc01.nxp.com:9712@liv0057.nxdi.us-cdc01.nxp.com"

### Set ZEBU_RUN_CONIFG 
### Default SW_HW (Optional HDL_COSIM)
if (${?ZEBU_RUN_CONIFG} == 1) then
else
setenv ZEBU_RUN_CONIFG SW_HW 
endif

### Set ZEBU_IP_ROOT path
setenv ZEBU_IP_ROOT  /run/pkg/synopsys-zebu-/ip_root_2017.12/


### Append ZEBU lib32 path
if (${?LD_LIBRARY_PATH} == 1) then
setenv LD_LIBRARY_PATH ${ZEBU_IP_ROOT}/lib/amd64.lib:${ZEBU_IP_ROOT}/lib32:${ZEBU_IP_ROOT}/lib:${LD_LIBRARY_PATH}
else
setenv LD_LIBRARY_PATH ${ZEBU_IP_ROOT}/lib/amd64.lib:${ZEBU_IP_ROOT}/lib32:${ZEBU_IP_ROOT}/lib:
endif


### Update Zebu System DIR/ Default is zebu11
if (${?ZEBU_SYSTEM_DIR} == /proj/mcu_zebu_adm/ZEBU_SYSTEM_DIR/zebu11) then
else
setenv ZEBU_SYSTEM_DIR /proj/mcu_zebu_adm/ZEBU_SYSTEM_DIR/zebu11
endif


### Zebu Environment Varibles for HDL cosim and Transactors
setenv ZEBU_INTERNAL_SIGNAL urg

echo "Removing Possible Duplicates in ENV Variables"

### LD_Library_path
set var_no_duplicates = `echo $LD_LIBRARY_PATH | tr ':' '\n' | sort | uniq | tr '\n' ':'`
set var_clean_start   = `echo $var_no_duplicates | sed 's/^://'`
set var_tot_clean     = `echo $var_clean_start | sed 's/.$//'`
setenv LD_LIBRARY_PATH $var_tot_clean

### LM_LICENSE_FILE
set var_no_duplicates = `echo $LM_LICENSE_FILE | tr ':' '\n' | sort | uniq | tr '\n' ':'`
set var_clean_start   = `echo $var_no_duplicates | sed 's/^://'`
set var_tot_clean     = `echo $var_clean_start | sed 's/.$//'`
setenv LM_LICENSE_FILE $var_tot_clean

### SNPSLMD_LICENSE_FILE
set var_no_duplicates = `echo $SNPSLMD_LICENSE_FILE | tr ':' '\n' | sort | uniq | tr '\n' ':'`
set var_clean_start   = `echo $var_no_duplicates | sed 's/^://'`
set var_tot_clean     = `echo $var_clean_start | sed 's/.$//'`
setenv SNPSLMD_LICENSE_FILE $var_tot_clean

### gcc compiler
source $workarea/settings/setup_gcc_compiler.csh

echo "VERDI_HOME    =" $VERDI_HOME
echo "VCS_HOME      =" $VCS_HOME
echo "ZEBU_IP_ROOT  =" $ZEBU_IP_ROOT


