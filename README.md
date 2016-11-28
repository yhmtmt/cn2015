#Communication Network Suppliments

The source codes are prepared for the lecture "Communication Network" in 2015. You can use them for any purpose if you agree that any problem caused with them is not responsible for me. 

## simple_client_and_server_communication_with_udp
This directory conatins two source codes "server.cpp" and "client.cpp". Build them and execute the server and then the client. In the server's console, you will see the message written in the source code. Of course you need to setup network according to the program codes, or change the source codes for your network setting.


## simple_file_transfer_with_udp
This directory also contains two source codes "sndf.cpp" and "rcvf.cpp" "sndf.cpp" sends a file, and "rcvf.cpp" recieves it. In executing "sndf.cpp", you should specify file name, destination IP address, port number, and packet size. For "rcvf.cpp", port number and packet size should be specified. Here, the packet size specified in both programs should be the same. 


## thread_sample
"thread.cpp" is an example creating two threads with a mutex. It should be covered before implementing the bidirectional communication.


## bidirectional_communication_with_udp
"sndrcv.cpp" is an example implementing both udp client and server. Two threads are created for the client and the server respectively. 


## transfer_data_structure_with_udp
"sndrcvd.cpp" is an example of data transfer using data structure. An data object, the type is "struct data", is passed directry to send/recv interface. Note that the code can only be used in the systems with same byte-endian.
 