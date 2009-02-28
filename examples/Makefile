all: libnl alarm recvmsg sendmsg

libnl: libnl-routes.c
	gcc -o libnl-routes -lnl libnl-routes.c
alarm: alarm.c
	gcc -o alarm ../src/alarm/alarm.c ../src/alarm/rbtree.c alarm.c
recvmsg: recvmsg.c
	gcc -o recvmsg recvmsg.c
sendmsg: sendmsg.c
	gcc -o sendmsg sendmsg.c
