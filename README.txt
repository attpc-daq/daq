如果QA界面出现track中心部分空白或者看上去丢失部分像素则需要重新编译"/home/ATTPC/DAQ/src/EventConverter"和"/home/ATTPC/DAQ/src/EventQA"

随便在这两个目录下的cpp文件的convert和fill函数中加一个cout操作,再编译一切恢复正常,随后注释cout即可

目前该问题每次重新开机就会出现
