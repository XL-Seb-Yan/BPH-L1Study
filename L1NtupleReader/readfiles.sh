#!/bin/bash
for line in `ls /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_1622619017/`
do
	echo "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_1622619017/${line}" >> "KeeMC.txt"
done