
没有更新singularity文件

支持的命令（通过gui网页操作）

start 

stop

run

shutdown

set	SiTCP  address  <x.x.x.x>

set	SiTCP  data  port  <x>

set	SiTCP  command  port  <x>
  
set	SiTCP  register  <x>
  
set	rootfile  events  <x>
  
set	rootfile  prefix <x>
  
set   output  directory <x>
  
get  SiTCP register <x>
  
需要实现的代码：
  
  1. SiTCP数据的解码和存储为root，需要修改daq/Data2ROOT.py
  2. SiTCP寄存器的控制命令
  3. 编写一个具有线程安全的websocket类，要求socket.send 是多线程安全的。

  test my vscode
