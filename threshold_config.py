import os
import time
import ROOT
import numpy as np
import sys
import math

import ROOT

def Generate_threshold_file(filename):
    # 打开ROOT文件
    root_file = ROOT.TFile(filename, 'read')

    # 获取树(Tree)
    tree = root_file.Get("RawData")
    
    # 获取TTree的entries
    n_entries = tree.GetEntries()
    
    fp = open("threshold.txt", "w")

    # 读取分支数据
    # 遍历TTree的每个entry
    for jentry in range(n_entries):
        tree.GetEntry(jentry)
        event_id = tree.event_id
        timestamp = tree.timestamp
        FE_id = tree.FE_id
        channel_index = tree.channel_index
        waveform = tree.waveform
        
        # 处理读取到的数据
        if(event_id == 0):
            continue
        if(event_id > 1):
            break
        if(event_id == 1):
            waveform_mean = ROOT.TMath.Mean(len(waveform), waveform)
            waveform_rms = ROOT.TMath.RMS(len(waveform), waveform)
            threshold = waveform_mean + 15 * waveform_rms
            print("waveform_mean = {:.3f}; waveform_rms = {:.3f}; threshold = {:.3f}".format(waveform_mean,waveform_rms,threshold))
            # 1806 FE ID
            cmd1 = 'send_to_SiTCP '+'06102831'+ '4' + str(hex(int(FE_id)%16))[2] + '5' + str(hex(math.floor(int(FE_id)/16)))[2] + '607083\n'
            fp.write(cmd1)   
            fp.write('#sleep 100\n')
            # 1808 channel ID
            cmd2 = 'send_to_SiTCP '+'08102831'+ '4' + str(hex(int(channel_index)%16))[2] + '5' + str(hex(math.floor(int(channel_index)/16)))[2] + '607083\n'
            fp.write(cmd2)
            fp.write('#sleep 100\n')
            # 180a 压缩阈值
            cmd3 = 'send_to_SiTCP '+'0A102831'+ '4' + str(hex(int(threshold)%16))[2] + '5' + str(hex((int(threshold)//16)%16))[2] + '6' + str(hex(((int(threshold)//16)//16)%16))[2] + '7' + str(hex((((int(threshold)//16)//16)//16)%16))[2] +'83\n'
            fp.write(cmd3)
            fp.write('#sleep 100\n')
            # 180c 确认命令 
            cmd4 = 'send_to_SiTCP '+'0C1028314150607083\n'
            fp.write(cmd4)
            fp.write('#sleep 100\n') 
    fp.close()

if __name__=='__main__':
    Generate_threshold_file(sys.argv[1])