#!/usr/bin/env python
import os, re, sys, commands, math, time, calendar

print '\nSTART\n'
ts = calendar.timegm(time.gmtime())

fileName = "L1Ntuple"
jobName = "ZeroBiasPrompt2018D"
jobCfg = "run-2018D.py"
jobScript = "cmsRun.sh"
rel = "CMSSW_10_2_1"
eosDir = "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/" + os.environ["USER"] + "/condor/" + jobName + "_" + str(ts) + "/"
rootDir = os.environ["CMSSW_BASE"] + "/src/BPH-L1Study/L1NtupleProducer/"
jobDir = rootDir + jobName + "_" + str(ts) + "/"
ret = 0

fileList = rootDir + "ZeroBiasPrompt2018D_L1p15.list" 

while ret == 0:
   ret = os.system("mkdir -p " + jobDir)
   ret = os.system("mkdir " + jobDir + "out/")
   ret = os.system("mkdir " + jobDir + "err/")
   ret = os.system("mkdir " + jobDir + "log/")
   ret = os.system("mkdir -p " + eosDir)
   ret = os.chdir(os.environ["CMSSW_BASE"]+"/../")
   print('Tarballing ' + rel + "/ into " + jobName + ".tgz...")
   ret = os.system("tar --exclude='L1Ntuple.root' --exclude='ignore' --exclude='.git' " + 
                   "-zcf " + jobName + ".tgz " + rel)
   print 'Done!'
   ret = os.system("mv " + jobName + ".tgz " + eosDir) 
   ret = os.chdir(rootDir)
   ret = os.system("cp /tmp/x509up_u93529 /afs/cern.ch/user/x/xuyan/private/x509up/x509up_u93529")
   proxy_path = "/afs/cern.ch/user/x/xuyan/private/x509up/x509up_u93529"
   
   file1 = open(fileList, 'r')
   file_content = file1.readlines()
   chunks = len(file_content) / 30 + 1
   count=0
   for ichunk in range(chunks):
      filelist_tmp = open("filelist_tmp.list", 'w')
      for file in file_content[ichunk*30:(ichunk+1)*30]:
         filelist_tmp.write(file)
      filelist_tmp.close()
      with open(jobDir + jobName + str(count) + '.jdl', 'w') as jdl:
         jdl.write("universe = vanilla\n")
         jdl.write("Executable = " + jobScript + "\n")
         jdl.write("Should_Transfer_Files = YES\n")
         jdl.write("WhenToTransferOutput = ON_EXIT\n")
         jdl.write("Transfer_Input_Files = " + jobScript + ", " + jobCfg + ", filelist_tmp.list" + "\n")
         jdl.write("Output = "    + jobDir + "out/$(ProcId).out\n")
         jdl.write("Error = "     + jobDir + "err/$(ProcId).err\n")
         jdl.write("Log = "       + jobDir + "log/$(ProcId).log\n")
         jdl.write("Arguments = " + eosDir + " " + jobName + " " + rel + " " + str(ichunk) + " " + jobCfg + " " + "filelist_tmp.list" + " " + fileName + " " + proxy_path + "\n")
         # jdl.write("+MaxRuntime = 28800\n")
         jdl.write("on_exit_remove = (ExitBySignal == False) && (ExitCode == 0)\n")
         jdl.write("max_retries = 3\n")
         jdl.write("requirements = Machine =!= LastRemoteHost\n")
         jdl.write("Queue " + str(1) + "\n")      

      os.system("condor_submit " + jobDir + jobName + str(ichunk) + ".jdl")
      count+=1
      os.system("rm filelist_tmp.list")
   print str(count) + " out of " + str(chunks) + " jobs submitted."
   os.system("condor_q")
   sys.exit(0)

print("Submission failed.")
sys.exit(1)