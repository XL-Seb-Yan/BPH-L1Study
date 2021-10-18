#! /usr/bin/env python
from __future__ import print_function, division
from collections import defaultdict, OrderedDict
import os
import sys
import math
import concurrent.futures
import gzip
import pickle
import json
import time
#import numexpr
import array
from functools import partial
import re
import numpy as np
from IPython.display import display
import ROOT

if __name__ == "__main__":

    import argparse
    parser = argparse.ArgumentParser(description="Make histograms for Zerobias data")
    parser.add_argument("--Data", "-d", type=str, help="name of data list")
    parser.add_argument("--MaxEvt", "-n", type=int, help="Maximum number of event to process")
    parser.add_argument("--Mode", "-m", type=int, help="Mode to process")
    args = parser.parse_args()
    
    data_list = args.Data
    Nevt = args.MaxEvt
    mode = args.Mode
    
    if mode is 11:
        os.system(r"root -l -q -b -x Frequency_plot_mu.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") # rate of singleMu trigger 
    if mode is 12:
        os.system(r"root -l -q -b -x Frequency_plot_mu_MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") # eff of singleMu trigger 
                
                
    if mode is 21:
         for EgEtaCut in [1.2]:
            for dRCut in [0.7]:
                os.system(r"root -l -q -b -x Frequency_plot_elel.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtaCut) + "," + str(dRCut) +"\)") # rate of double_el_Etax.x_dRx.x trigger
    if mode is 22:
        for EgEtaCut in [1.2]:
            for dRCut in [0.7]:
                os.system(r"root -l -q -b -x Frequency_plot_elel_MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtaCut) + "," + str(dRCut) +"\)") # eff of double_el_Etax.x_dRx.x trigger

    if mode is 31:
        for EgEtCut in [7]:
            for EgEtaCut in [1.2]:
                for dRCut in [0.7]:
                    os.system(r"root -l -q -b -x Frequency_plot_elelj.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtCut) + "," + str(EgEtaCut) + "," + str(dRCut) +"\)") # rate of elel+jet trigger
    if mode is 32:
        for EgEtCut in [7]:
            for EgEtaCut in [1.2]:
                for dRCut in [0.7]:
                    os.system(r"root -l -q -b -x Frequency_plot_elelj_MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtCut) + "," + str(EgEtaCut) + "," + str(dRCut) +"\)") # rate of elel+jet trigger
                    
    if mode is 99:
        os.system(r"root -l -q -b -x MCuts.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") # MC study
    
    
    
    os.system("rm *.d")
    os.system("rm *.so")
    os.system("rm *.pcm")