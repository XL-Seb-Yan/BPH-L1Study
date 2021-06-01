#!/bin/bash
for line in `ls /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_1622377355/`
do
	echo "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_1622377355/${line}" >> "KeeMC.txt"
done