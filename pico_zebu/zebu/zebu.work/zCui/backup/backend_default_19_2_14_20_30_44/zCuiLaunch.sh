source /run/pkg/synopsys-zebu-/M-2017.03-SP1-4/zebu_env.sh
source /proj/mcu_zebu/ZEBU_IP_2017.12/zebu_env.sh
export ZEBU_IP_ROOT=/proj/mcu_zebu/ZEBU_IP_2017.12
cd /proj/mcu_zebu/users/b51677/sverilog/zebu/pico_soc/zebu
zCui -p /proj/mcu_zebu/users/b51677/sverilog/zebu/pico_soc/zebu/utf_generatefiles/gen_ufe_project.zpf --nogui --compile -u ../src/env/pico_device_wrapper.utf -w zebu.work 
