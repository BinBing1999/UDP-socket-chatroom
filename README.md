# UDP-socket-chatroom
This is a cpp file which creates a client in a client-only chatroom. It can be used in Linux socket programming.
Use the mininet_build.py to create a mininet network, use xterm h1~4 to open the host terminal.
This udp socket support four clients(h1~h4) to communicate with each other. It is a multithread programme, so remember to compile it with -lpthread command!

This is my first socket program, hope it can help you!
Btw, when using the programme, you may find that each client in the chatroom receive the message sent by itself, this is a problem I cannot fix at the moment, hope you guys can find your own way to cope with it.
