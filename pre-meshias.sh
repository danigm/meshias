#!/bin/bash

iptables -t filter -A OUTPUT -o $1 -p udp --dport 8765 -j DROP
iptables -t filter -A INPUT -i $1 -p udp --dport 8765 -j DROP
iptables -t filter -A OUTPUT -o $1 -p udp --sport 654 -j ACCEPT
iptables -t filter -A INPUT -i $1 -p udp --dport 654 -j ACCEPT
iptables -t filter -A OUTPUT -o $1 -j NFQUEUE --queue-num 0
iptables -t filter -A INPUT -i $1 -j NFQUEUE --queue-num 0
iptables -t filter -A FORWARD -i $1 -j NFQUEUE --queue-num 0
