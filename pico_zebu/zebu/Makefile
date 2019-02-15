##---------------------------------------------------------------------
##  COPYRIGHT (C) 2018 SYNOPSYS INC.
## This software and the associated documentation are confidential and
## proprietary to Synopsys, Inc. Your use or disclosure of this software
## is subject to the terms and conditions of a written license agreement
## between you, or your company, and Synopsys, Inc. In the event of
## publications, the following notice is applicable:
##
## ALL RIGHTS RESERVED
##
## The entire notice above must be reproduced on all authorized copies.
##----------------------------------------------------------------------

help:
	@echo ""
	@echo "#################################################"
	@echo "# Available targets                             #"
	@echo "#################################################"
	@echo "#  SEM=[1/0] (default : 0)                      #"
	@echo "#                                               #"
	@echo "# ZeBu compilation:                             #"
	@echo "#  -> compil                                    #"
	@echo "#  LEGACY=[true/false] (default : false)        #"
	@echo "#  IFLESS=[true/false] (default : false)        #"
	@echo "#                                               #" 
	@echo "#                                               #"
	@echo "# Uart testbench:                               #"
	@echo "#  -> run_cb                                    #"
	@echo "#  -> run_nonblocking                           #"
	@echo "#  -> run_blocking                              #"
	@echo "#                                               #"
	@echo "# Uart MP testbench:                            #"
	@echo "#  -> run_cb_mp                                 #"
	@echo "#  -> run_nonblocking_mp                        #"
	@echo "#  -> run_blocking_mp                           #"
	@echo "#                                               #"
	@echo "# UartServer testbench:                         #"
	@echo "#  -> run_server                                #"
	@echo "#                                               #"
	@echo "# UartTerm testbench:                           #"
	@echo "#  -> run_xterm                                 #"
	@echo "#  -> run_xtermcb                               #"
	@echo "#                                               #"
	@echo "# UartTerm MP testbench:                        #"
	@echo "#  -> run_xterm_mp                              #"
	@echo "#  -> run_xtermcb_mp                            #"
	@echo "#                                               #"
	@echo "# Clean                                         #"
	@echo "#  -> clean_compil                              #"
	@echo "#  -> clean_run                                 #"
	@echo "#  -> clean                                     #"
	@echo "#################################################"
	@echo ""
	@echo " For run_server target you can set the followingvariable:"
	@echo " TCP_PORT   : TCP port number (default: 10000)"
	@echo " TCP_MODE   : TCP mode (client or server, default: server)"
	@echo " TCP_SERVER : TCP server for client mode (default: localhost)"
	@echo " For running in SEM flow add make <target> SEM=1 in command line"
	@echo ""

ifndef ZEBU_IP_ROOT
  $(error ZEBU_IP_ROOT variable not set. Please set ZEBU_IP_ROOT environnment variable)
endif

ifeq "$(MAKECMDGOALS)" "run_dmtcp"
ifndef DMTCP_HOME
$(error DMTCP_HOME variable not set. Please set DMTCP_HOME environnment variable)
endif
endif


ZEBUWORK ?= zebu.work

## UC compilation
LEGACY   ?= false
SEM      ?= 0
IFLESS   ?= 0

UTFFILE = ../src/env/pico_device_wrapper.utf
ZPFILE  = ../src/env/pico.zpf
FILE_CONF = ./zse_configuration.tcl

TCP_PORT    ?= 10000
TCP_SERVER  ?= localhost
TCP_MODE    ?= server

TBENCH_SUFFIX      ?= $(word 2,$(subst _, ,$(MAKECMDGOALS)))

# TCP server/client testbench
ifeq ("$(TBENCH_SUFFIX)", "server")
TBENCH = testbench_server
endif

# Xterm testbench
ifeq ("$(TBENCH_SUFFIX)", "xterm")
TBENCH = testbench_xterm
endif

# Xterm testbench
ifeq ("$(TBENCH_SUFFIX)", "xtermcb")
TBENCH = testbench_xterm_cb
endif

# blocking testbench
ifeq ("$(TBENCH_SUFFIX)", "blocking")
TBENCH = testbench_blocking
endif

# blocking testbench
ifeq ("$(TBENCH_SUFFIX)", "nonblocking")
TBENCH = testbench_nonblocking
endif

# blocking testbench
ifeq ("$(TBENCH_SUFFIX)", "cb")
TBENCH = testbench_cb
endif


# default testbench
TBENCH ?= testbench_cb


ifdef USE_DRIVER_ITERATOR
 DRIVER_TER_OPT = -DUSE_DRIVER_ITERATOR=YES
endif

############################
# detect used ZeBu version
############################

# Use 64 bit compilation for ZeBu 4.3 and higher
ARCH ?= 64

ifeq ("$(LEGACY)","true")
DFFILE  = "$(CURDIR)/../src/env/designFeatures.legacy"
ZEBUWORK = zebu.work.legacy
else
ifndef VCS_HOME
  $(error VCS_HOME variable not set. Please set VCS_HOME environnement variable)
endif
DFFILE = "$(CURDIR)/../src/env/designFeatures"
endif

# Version > 4.2 ZeBu Work Dir = <ZEBU_WORK>/zebu.work
#ZWORK ?=../$(ZEBUWORK)/zebu.work
ZWORK ?=./$(ZEBUWORK)

ifeq ("$(findstring compil,$(MAKECMDGOALS))","compil")
ifndef FILE_CONF
  $(error FILE_CONF: ZeBu target hardware configuration file is not set. Please set FILE_CONF environnment variable or in GUI. \
  Example files in $(ZEBU_ROOT)/etc/configurations)
endif
endif

ZEBU_FILE_CONF=$(basename $(FILE_CONF)).tcl

ifeq ("$(IFLESS)","1")
  IF_LESSS:=ZEBU_NO_RTB
endif

############################
## COMPILATION
############################
ifdef GUI
  ZCUI_OPT    :=
else
  ZCUI_OPT    :=--nogui --compile
endif

ifeq ("$(SEM)","1")
  SEM_MACRO   = -DSEM
  ZCUI_OPT   := --sem --nogui
endif 

../src/dut/dut.edf:
	@$(MAKE) -C ../src/dut synth_dut 

compil:
ifneq ("$(LEGACY)","true")
	REMOTECMD="$(REMOTECMD)" FILE_CONF=$(ZEBU_FILE_CONF) SYNTH_TOOLS=$(SYNTH_TOOLS) IFLESS=$(IF_LESSS) tr.pkg synopsys-zebu-/M-2017.03-SP1-4 zCui $(ZCUI_OPT) -u $(UTFFILE) -w $(ZEBUWORK)
else
	REMOTECMD="$(REMOTECMD)" FILE_CONF=$(ZEBU_FILE_CONF) SYNTH_TOOLS=$(SYNTH_TOOLS) ZEBUWORK=$(ZEBUWORK) tr.pkg synopsys-zebu-/M-2017.03-SP1-4 zCui $(ZCUI_OPT) -p $(ZPFILE)
endif

clean_compil:
	rm -rf zebu.work*

############################
## EMULATION 
############################

############################
CXX       = g++
CXX_VER   = $(shell $(CXX) -dumpversion | sed -e s/[^0-9]//g | head -c 2 )
INCL_PATH = -I$(ZEBU_ROOT)/include -I$(ZEBU_IP_ROOT)/include  
LIB_PATH  = -m$(ARCH) -L$(ZEBU_ROOT)/lib -lZebu -L$(ZEBU_IP_ROOT)/lib -lUart -lZebuXtor
CXXFLAGS  = -m$(ARCH) -O $(INCL_PATH) -Wall -fexceptions                                     \
            -DZEBUWORK=\"$(ZWORK)\"  -DDFFILE=\"$(DFFILE)\" -DLEGACY=$(LEGACY) $(SEM_MACRO)  \
            -DTCP_MODE=\"$(TCP_MODE)\" -DTCP_SERVER=\"$(TCP_SERVER)\" -DTCP_PORT=$(TCP_PORT) \
            $(DRIVER_TER_OPT)
            
            #-DTCP_MODE=\"$(TCP_MODE)\" -DTCP_SERVER=\"$(TCP_SERVER)\" -DTCP_PORT=$(TCP_PORT) -std=gnu++11 
ifeq "$(MAKECMDGOALS)" "run_dmtcp"
CXXFLAGS += -DRUN_DMTCP
TBENCH = testbench_nonblocking
endif

ifeq "$(MAKECMDGOALS)" "dmtcp_save"
CXXFLAGS += -DRUN_DMTCP
TBENCH = testbench_nonblocking
endif


############################

rundir_mp rundir:
	mkdir -p $@

rundir/testbench.o : rundir ../src/bench/$(TBENCH).cc
	$(CXX) $(CXXFLAGS) -c ../src/bench/$(TBENCH).cc -o rundir/testbench.o

rundir/testbench : $(ZEBU_IP_ROOT)/lib/libUart.so rundir/testbench.o
	$(CXX) $(LIB_PATH) -o rundir/testbench rundir/testbench.o
	rm rundir/testbench.o

 run: rundir/testbench
 	cd rundir && ZEBU_USE_SCHEDULER=OK LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH && ./testbench
 
# run_gdb: rundir/testbench
# 	cd rundir && ZEBU_USE_SCHEDULER=OK LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH && gdb ./testbench
# 
# dmtcp_save: rundir/testbench
# 	cd rundir && ZEBU_USE_SCHEDULER=OK LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH &&  $(DMTCP_HOME)/bin/dmtcp_launch ./testbench
# 
# dmtcp_restore:
# 	cd rundir && ./CP1.0/dmtcp_restart_script.sh
# 	
# run_dmtcp:
# 	make dmtcp_save
# 	@echo "Restoring DMTCP process ..."
# 	@sleep 2
# 	make dmtcp_restore	
# #Run MP


rundir_mp/testbench.o : rundir_mp ../src/bench/$(TBENCH).cc
	cd rundir_mp && $(CXX) $(CXXFLAGS) -c ../../src/bench/$(TBENCH).cc -o testbench.o

rundir_mp/testbench : $(ZEBU_IP_ROOT)/lib/libUart.so rundir_mp/testbench.o
	cd rundir_mp && $(CXX) $(LIB_PATH) -o testbench testbench.o
	rm rundir_mp/testbench.o

run_mp: rundir_mp rundir_mp/testbench
	cd rundir_mp && ZEBU_USE_SCHEDULER=OK && LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH && ./testbench

run_gdb_mp: rundir_mp rundir_mp/testbench
	cd rundir && ZEBU_USE_SCHEDULER=OK && LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH && gdb ./testbench

	
ifdef DEBUG
DEBUGDRIVERCLK = -debugDriverClk
endif

#run_gui: testbench
#	ZEBU_USE_SCHEDULER=OK && LD_LIBRARY_PATH=$(ZEBU_IP_ROOT)/lib:$$LD_LIBRARY_PATH && zRun $(DEBUGDRIVERCLK) -testBench './testbench' 

run_zebu: 
	zsub -D 0 -P imxrt1170.verif -q zebu_mcu2 tr.pkg synopsys-zebu-/M-2017.03-SP1-1 zRun -testBench './rundir/testbench' & 


comp_xterm : run
#run_cb run_nonblocking run_blocking run_server run_xterm run_xtermcb: run
run_cb_mp run_nonblocking_mp run_blocking_mp run_xterm_mp run_xtermcb_mp: run_mp
run_cb_gdb run_nonblocking_gdb run_blocking_gdb run_server_gdb run_xterm_gdb: run_gdb
run_cb_gui run_nonblocking_gui run_blocking_gui run_server_gui run_xterm_gui: run_gui

clean_run:
	rm -rf rundir rundir_mp zebu.work zcui.work tools *.log *.help *cmds.tcl

setup_dir:
	ln -s ./zebu.work/zebu.work ./zcui.work
	ln -s ./zebu.work/zebu.work/tools ./

############################
## CLEAN
############################
clean: clean_run clean_compil

.PHONY: compil run clean clean_run clean_compil run_gui run_gdb run_mp 
