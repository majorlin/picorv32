wvSetPosition -win $_nWave1 {("G1" 0)}
wvOpenFile -win $_nWave1 \
           {/proj/mcu_zebu/users/b51677/sverilog/zebu/pico_soc/zebu/monitor.fsdb}
wvGetSignalOpen -win $_nWave1
wvGetSignalSetScope -win $_nWave1 "/testbench"
wvGetSignalSetScope -win $_nWave1 "/testbench/uut/soc"
wvGetSignalSetScope -win $_nWave1 "/testbench/uut/soc/cpu"
wvSetPosition -win $_nWave1 {("G1" 10)}
wvSetPosition -win $_nWave1 {("G1" 10)}
wvAddSignal -win $_nWave1 -clear
wvAddSignal -win $_nWave1 -group {"G1" \
{/testbench/TXD} \
{/testbench/_I_RxD_} \
{/testbench/_I_clk_} \
{/testbench/_I_nrst_} \
{/testbench/clk} \
{/testbench/flash_csb} \
{/testbench/flash_io0} \
{/testbench/uut/soc/cpu/clk} \
{/testbench/uut/soc/cpu/mem_instr} \
{/testbench/uut/soc/cpu/reg_sh\[4:0\]} \
}
wvAddSignal -win $_nWave1 -group {"G2" \
}
wvSelectSignal -win $_nWave1 {( "G1" 10 )} 
wvSetPosition -win $_nWave1 {("G1" 10)}
wvZoomAll -win $_nWave1
wvZoomAll -win $_nWave1
wvZoomAll -win $_nWave1
wvZoomAll -win $_nWave1
wvZoomAll -win $_nWave1
wvSetCursor -win $_nWave1 15324.955655 -snap {("G1" 4)}
wvExit
