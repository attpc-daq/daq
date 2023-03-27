import os
import time
import ROOT
import numpy as np
import math

def Generate_threshold_file(filename):
    # 打开ROOT文件
    root_file = ROOT.TFile(filename, 'read')

    # 获取树(Tree)
    tree = root_file.Get("RawData")

    # 获取分支(Branch)
    # event_id_branch = tree.GetBranch("event_id")
    # timestamp_branch = tree.GetBranch("timestamp")
    FE_id_branch = tree.GetBranch("FE_id")
    channel_index_branch = tree.GetBranch("channel_index")
    waveform_branch = tree.GetBranch("waveform")
    
    fp = open("threshold.txt", "w")

    # 读取分支数据
    for event in tree:
        # event_id = event_id_branch.GetLeaf("event_id").GetValue()
        # timestamp = timestamp_branch.GetLeaf("timestamp").GetValue()
        FE_id = FE_id_branch.GetLeaf("FE_id").GetValue()
        channel_index = channel_index_branch.GetLeaf("channel_index").GetValue()
        waveform = waveform_branch.GetLeaf("waveform").GetValue()
        # 处理读取到的数据
        if(FE_id == 0):
            continue
        if(FE_id > 1):
            break
        if(FE_id == 1):
            mean = np.mean(np.ndarray(1024, dtype=np.float32, buffer=waveform))
            std  = np.std(np.ndarray(1024, dtype=np.float32, buffer=waveform))
            threshold = mean + 15 * std
            print("mean = ", mean, "std = ", std, "threshold = ", threshold)
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
