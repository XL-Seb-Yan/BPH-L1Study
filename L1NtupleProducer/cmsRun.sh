#!/bin/bash
echo "Starting job on " `date` #Date/time of start of job
echo "Running on: `uname -a`" #Condor job is running on this node
echo "System software: `cat /etc/redhat-release`" #Operating System on that node
source /cvmfs/cms.cern.ch/cmsset_default.sh  ## if a bash script, use .sh instead of .csh
export X509_USER_PROXY=${8}
voms-proxy-info -all
voms-proxy-info -all -file ${8}
### for case 1. EOS have the following line, otherwise remove this line in case 2.
#Arguments 1)eosDir 2)jobName 3)rel 4)iJob 5)jobCfg 6)inputFile 7)fileName

# You need to move transfered files to the working area
cp ${1}${2}.tgz .
tar -xf ${2}.tgz
rm ${2}.tgz
mv ${5} ${3}/src/BPHL1Study/L1NtupleProducer
mv JSON_dataNtuples.txt ${3}/src/BPHL1Study/L1NtupleProducer
mv filelist.tgz ${3}/src/BPHL1Study/L1NtupleProducer

export $SCRAM_ARCH=slc7_amd64_gcc700
cd ${3}/src/
scramv1 b ProjectRename
eval `scramv1 runtime -sh` # cmsenv is an alias not on the workers
cd BPHL1Study/L1NtupleProducer
tar -xf filelist.tgz
ls ./
touch log.txt
cmsRun ${5} inputFiles_load=${6} 2>&1 | tee log.txt
cp log.txt ${1}/log_${4}.txt
cp ${7}.root ${1}/${7}_${4}.root
rm ${7}.root
cd ${_CONDOR_SCRATCH_DIR}
rm -rf ${3}