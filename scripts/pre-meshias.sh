#!/bin/bash

# Use a rule similar to the following one to filter out packets from a given computer interface:

iptables -A INPUT -m mac --mac-source 00:1B:77:1F:DF:12 -j DROP # danigm big one
iptables -A INPUT -m mac --mac-source 00:15:AF:EF:EE:3D -j DROP # danigm netbook

iptables -t filter -A OUTPUT -o $1 -p udp --dport 8765 -j DROP
iptables -t filter -A INPUT -i $1 -p udp --dport 8765 -j DROP
iptables -t filter -A OUTPUT -o $1 -p udp --sport 654 -j ACCEPT
iptables -t filter -A INPUT -i $1 -p udp --dport 654 -j ACCEPT
iptables -t filter -A OUTPUT -o $1 -j NFQUEUE --queue-num 0
iptables -t filter -A INPUT -i $1 -j NFQUEUE --queue-num 0

iptables -t filter -A FORWARD -i $1 -j NFQUEUE --queue-num 0
iptables -t nat -A POSTROUTING -o $1 -j MASQUERADE
