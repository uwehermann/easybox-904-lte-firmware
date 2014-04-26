#!/bin/sh
# Copyright (C) 2010 Arcadyan

DROPBEAR_PROG="/etc/init.d/dropbear"
DROPBEAR_CFG="/etc/config/dropbear"

WAN_ETH="eth1"
NAS_ETH="nas"
PPP_ETH="ppp"

# $1 - interface name
export_if_ip() {
	local	WAN_IP

	if [ -z "$1" ] ; then
		return
	fi

	WAN_IP=`ifconfig $1 2>&- | grep "inet addr:" | awk '{ print $2 }' | awk 'BEGIN { FS=":" } { print $2 }'`
	if [ -n "$WAN_IP" ] ; then
		echo "	option Port '$WAN_IP:22'"	>> $DROPBEAR_CFG
	fi
}

gen_cfg() {
	local	CNT
	local	MAX_WAN_IF=8

	echo "config dropbear"				 > $DROPBEAR_CFG
	echo "	option PasswordAuth 'on'"	>> $DROPBEAR_CFG

	export_if_ip $WAN_ETH

	CNT=0
	while [ $CNT -lt $MAX_WAN_IF ] ; do
		export_if_ip $NAS_ETH$CNT
		export_if_ip $PPP_ETH$CNT
		let CNT=$CNT+1
	done
}

start() {
	gen_cfg

	$DROPBEAR_PROG start
}

stop() {
	$DROPBEAR_PROG stop
}

restart() {
	stop
	start
}


case $1 in
  "start")
		start
		;;
  "stop")
		stop
		;;
  "restart")
		restart
		;;
  *)
		echo "$0 start    - start   DropBear/SSH2 server"
		echo "$0 stop     - stop    DropBear/SSH2 server"
		echo "$0 restart  - restart DropBear/SSH2 server"
		;;
esac
