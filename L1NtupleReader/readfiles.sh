# !/bin/bash
for line in `ls /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_Yuta_1623697354/`
do
	echo "file:/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/xuyan/condor/KeeMC_Yuta_1623697354/${line}" >> "KeeMC_Yuta.txt"
done

# for line in `ls /eos/user/y/ytakahas/BparkingTriggerStudy_nomufilter/`
# do
	# echo "/eos/user/y/ytakahas/BparkingTriggerStudy_nomufilter/${line}" >> "KeeMC_Yuta.txt"
# done