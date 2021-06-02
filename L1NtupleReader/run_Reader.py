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
    
    if mode is 10:
        os.system(r"root -l -q -b -x Frequency_plot_0.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") #standard plot
    if mode is 11:
        for MuEtaCut in [2,1.5,1]:
            for EgEtaCut in [2.5,1.1]:
                os.system(r"root -l -q -b -x Frequency_plot_1.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown events on mu and eg cuts
    if mode is 12:
        for MuEtThr in range(2,10,1):
            os.system(r"root -l -q -b -x Frequency_plot_2.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtThr) + "\)") #plot shown events on leading eg and sub-lead eg cuts with fixed muon threshold
    if mode is 13:
        os.system(r"root -l -q -b -x Frequency_plot_3.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") #plot frequncy of single muon trigger as a function of eta and pt cut 
    if mode is 14:
        for MuEtCut in [5]:
            for MuEtaCut in [1.5,1.0]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_4.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown dR of leading muon and Eg with fixed muon  pt threshold 
    if mode is 15:
        for MuEtCut in [5]:
            for MuEtaCut in [1.0,1.5]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_5.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown dR of leading EG and sL Eg with fixed muon  pt threshold 
    if mode is 16:
        os.system(r"root -l -q -b -x Frequency_plot_6.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") #plot the second muon pt
    if mode is 17:
        for MuEtCut in [4]:
            for MuEtaCut in [1.0]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_7.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown mass of leading EG and sL Eg with fixed muon pt and eta threshold
    if mode is 18:
        os.system(r"root -l -q -b -x Frequency_plot_8.C+\(\"" + data_list + r"\"," + str(Nevt) + "\)") #plot shown events on leading mu and lead eg cuts with fixed subleading eg threshold
        
    if mode is 24:
        for MuEtCut in [5]:
            for MuEtaCut in [1.0,1.5]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_4MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown dR of leading muon and Eg with fixed muon  pt threshold 
                    
    if mode is 25:
        for MuEtCut in [5]:
            for MuEtaCut in [1.0,1.5]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_5MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown dR of leading EG and sL Eg with fixed muon  pt threshold 
                    
    if mode is 27:
        for MuEtCut in [4]:
            for MuEtaCut in [1.0]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_7MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown mass of leading EG and sL Eg with fixed muon pt and eta threshold
                    
    if mode is 29:
        for MuEtCut in [5]:
            for MuEtaCut in [1.5]:
                for EgEtaCut in [1.1]:
                    os.system(r"root -l -q -b -x Frequency_plot_9MC.C+\(\"" + data_list + r"\"," + str(Nevt) + "," + str(MuEtCut) + "," + str(MuEtaCut) + "," + str(EgEtaCut) + "\)") #plot shown dR of leading EG and sL Eg with fixed muon  pt threshold 
    
    os.system("rm *.d")
    os.system("rm *.so")
    os.system("rm *.pcm")