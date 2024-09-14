import argparse
import numpy as np
import matplotlib.pyplot as plt

def parse_data_file(file_path):
    waveform = np.zeros(1024, dtype=int)
    event_ids = []  # 存储event_id
    with open(file_path, 'rb') as f:
        datas = f.read()
        length_byte = len(datas)
        count = 0
        number_of_chs = 0
        while(count < length_byte):
            if(datas[count] == 0x5a and (datas[count+1] & 0xE0) == 0x40): 
                timestamp = (datas[count+4] << 40) + (datas[count+5] << 32) + (datas[count+6] << 24) + (datas[count+7] << 16) + (datas[count+8] << 8) + datas[count+9]
                event_id  = (datas[count+10] << 24) + (datas[count+11] << 16) + (datas[count+12] << 8) + datas[count+13]
                # event_ids.append(event_id)  # 添加event_id到列表
                print(event_id)
                count += 20   
                break
            else:
                count += 1
                
        while(count < length_byte):
            if(datas[count] == 0x5a):
                if((datas[count+1] & 0xE0) == 0x40):
                    if(((datas[count+1]&0b00011111)<<8)+datas[count+2] != 20):
                        print("Error: the length of the header is not 20 bytes")
                    if(count + 13 > length_byte):
                        count = count + 20
                    else:
                        timestamp = (datas[count+4] << 40) + (datas[count+5] << 32) + (datas[count+6] << 24) + (datas[count+7] << 16) + (datas[count+8] << 8) + datas[count+9]
                        event_id  = (datas[count+10] << 24) + (datas[count+11] << 16) + (datas[count+12] << 8) + datas[count+13]
                        # event_ids.append(event_id)  # 添加event_id到列表
                        hit = datas[count+14]&0b111111
                        print("number_of_chs: ", number_of_chs)
                        print("this is head, event_id " + str(event_id))
                        #print(event_id)
                        print("hits "+str(hit))
                        number_of_chs = 0
                        count = count + 20                         
                elif((datas[count+1] & 0xE0) == 0x20):
                    print("this is end")
                    if(((datas[count+1]&0b00011111)<<8)+datas[count+2] != 12):
                        print("Error: the length of the end is not 12 bytes")
                    count = count + 12                            
                elif((datas[count+1] & 0xE0) == 0x00):
                    print("this is body")
                    if(((datas[count+1]&0b00011111)<<8)+datas[count+2] != 2060):
                        print("Error: the length of the bode is not 2060 bytes")
                    if(count + 7 + 2048 > length_byte):
                        count = count + 2060
                    else:
                        FE_ID = datas[count+3]
                        channel_index = datas[count+4]
                        print(FE_ID, channel_index, timestamp, event_id)
                        number_of_chs += 1
                        for i in range(0, 1024):
                            waveform[i] = ((datas[count+6+i*2] << 8) + datas[count+7+i*2]) & 0b0000111111111111
                        count = count + 2060                              
                else:
                    count = count + 1
            else:
                count = count + 1

    # event_ids = np.array(event_ids)
    # # 画event_id的柱状图，横轴起始和终止以及bins由event_ids最大最小值决定
    # plt.hist(event_ids, bins=np.linspace(min(event_ids), max(event_ids), max(event_ids)-min(event_ids)+1), alpha=0.75)
    # # plt.title('Event ID Distribution')
    # plt.xlabel('Event ID')
    # plt.ylabel('Frequency')
    # # plt.show()
    # plt.savefig('/home/ATTPC/DAQ/output/raw/2/2.png')

if __name__ == '__main__':
    #parser = argparse.ArgumentParser()
    #parser.add_argument('--file')
    #args = parser.parse_args()
    #parse_data_file(args.file)
    #file_path = '/home/ATTPC/DAQ/output/buffers1/0.b'
    #parse_data_file(file_path)
    parse_data_file('0.a')
    parse_data_file('1.a')
    parse_data_file('2.a')
    parse_data_file('3.a')
    
