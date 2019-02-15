# /run/pkg/synopsys-vcs_mx-/M-2017.03-SP2-10-Z/bin/utftran -u ../src/env/pico_device_wrapper.utf -target zebu.work/utf_generatefiles -sum utf_summary.log -l utf_fulllog.log
/run/pkg/synopsys-vcs_mx-/M-2017.03-SP2-10-Z/bin/utftran -u ../src/env/pico_device_wrapper.utf -target zebu.work/utf_generatefiles -sum utf_summary.log -l utf_fulllog.log
exitcode=$?
echo "command exit code is '${exitcode}'"
exit ${exitcode}
