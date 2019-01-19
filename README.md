# udpeer
make a tunnel with two port

# 测试&使用
服务端：
udpeer 8000 8001 &
设备1:
nc -u server.com 8001
设备2:
nc -u server.com 8002

# 传输文件
  服务端：
  ./Step1_server_init.sh 8000 8001
  设备1:
    ./Step2_sender_init.sh server.com 8000
  设备2:
    ./Step3_reciver_run.sh server.com 8001 myfile.txt
  设备1:
    ./Step4_sender_send.sh server.com 8000 myfile.txt
  即可看到设备2上的myfile.txt文件


# 设计理念 
简单易用
不会设计加密，加密应该在两个peer端做
用作少量数据的快速通信（PS：tcp太慢了）
用户可以使用java/php等语言在http server上设计调度服务器：比如1台数据通信server支撑1000对会话，一台httpserver调度100台/1000台设备实现服务
因此，本设计理论上可以支撑大规模集群服务。

# 如何防范攻击
第一层：调度服务器应该实现设备认证——只有授权设备才能访问http接口申请服务，否则不会暴露转发服务器IP
第二层：即使攻击者在知道部分转发服务器IP后，对部分服务器的攻击也会导致部分用户服务不可用
      因此，转发服务器应该向http接口服务器上报状况，如果端口全部被占用，应当不再使用本服务器
第三层：可以设定规则：相同服务器对于同一个IP最多只接受N个请求，增加黑客攻击成本
第四层：单个服务器有6万个udp端口，如果我们使用随机的2000个端口，黑客将增加成本遍历端口，而在其遍历端口时，应当采用对策封杀其IP


这是一个非常简单的设计，希望被用于IPC/门铃/实时通信类产品，希望可以慢慢改进，让它拥有更多好用的功能。

