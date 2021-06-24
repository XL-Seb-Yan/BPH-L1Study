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
        os.system(r"root -l -q -b -x Frequency_plot_1.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") # rate of singleMu trigger 
    if mode is 21:
        os.system(r"root -l -q -b -x Frequency_plot_1MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") # eff of singleMu trigger 
        
    if mode is 12:
        for MuEtaCut in [1.0]:
            for EgEtaCut in [2.5]:
                os.system(r"root -l -q -b -x Frequency_plot_2.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") # rate of e+mu trigger
    if mode is 22:
        for MuEtaCut in [1.0]:
            for EgEtaCut in [2.5]:
                os.system(r"root -l -q -b -x Frequency_plot_2MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") # eff of e+mu trigger
                
                
    if mode is 13:
        for EgEtaCut in [1.0]:
            os.system(r"root -l -q -b -x Frequency_plot_3.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtaCut) + "\)") # rate of double_el_Etax.x_dRx.x trigger
    if mode is 23:
        for EgEtaCut in [1.0]:
            os.system(r"root -l -q -b -x Frequency_plot_3MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtaCut) + "\)") # eff of double_el_Etax.x_dRx.x trigger
    if mode is 231:
        for EgEtaCut in [1.0]:
            os.system(r"root -l -q -b -x Frequency_plot_31MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(EgEtaCut) + "\)") # eff of double_el_Etax.x_dRx.x trigger

            
    if mode is 14:
        for MuEtCut in [2.0]:
            for MuEtaCut in [2.5]:
                for EgEtaCut in [1.0]:
                    os.system(r"root -l -q -b -x Frequency_plot_4.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") # rate of mu+double_el_Etax.x_dRx.x trigger
    if mode is 24:
        for MuEtCut in [2.0]:
            for MuEtaCut in [2.5]:
                for EgEtaCut in [1.0]:
                    os.system(r"root -l -q -b -x Frequency_plot_4MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") # rate of mu+double_el_Etax.x_dRx.x trigger
    
    
    
    os.system("rm *.d")
    os.system("rm *.so")
    os.system("rm *.pcm")