#!/bin/sh
#
# Copyright (C) 2009 Arcadyan
# All Rights Reserved.
#
# FTP configuration shell program
#

START=99

FTP_PWD_FILE="/tmp/pure-ftpd/pure-ftpd.pwd"
FTP_FW_PREFIX="ftp-upgrade-"
UPGRADE_PROG="/usr/sbin/upgrade.sh"


boot()
{
	sleep 120

	start
}

start()
{
	while [ 1 ] ; do

		sleep 1

		if ! [ -f $FTP_PWD_FILE ] ; then
			exit
		fi

		ALL_ROOT_DIR=`cut -d ':' -f 3,4,6 $FTP_PWD_FILE | grep "^0:0:" | cut -d ':' -f 3`
		FTP_FW_PREFIX_LEN=${#FTP_FW_PREFIX}

		for ROOT_DIR in $ALL_ROOT_DIR ; do
			let POS=${#ROOT_DIR}-1
			if [ "${ROOT_DIR:$POS:1}" == "/" ] ; then
				ROOT_DIR=${ROOT_DIR:0:$POS}
			fi
			ALL_FW=`cd $ROOT_DIR ; ls -1 ${FTP_FW_PREFIX}* 2>&-`
			for FW in $ALL_FW ; do
				if ! [ -f "$ROOT_DIR/$FW" ] ; then
					continue
				fi
				FW_TYPE=${FW:$FTP_FW_PREFIX_LEN}
				case "$FW_TYPE" in
					kernel-*)
							$UPGRADE_PROG kernel "$ROOT_DIR/$FW" reboot	;;
					rootfs-*)
							$UPGRADE_PROG rootfs "$ROOT_DIR/$FW" reboot	;;
					module-*)
							$UPGRADE_PROG module "$ROOT_DIR/$FW"	;;
				esac
			done
		done

	done
}


stop()
{
	PIDS=`pgrep -f ftp_fw_upgrade.sh | grep -v grep`
	for PID in $PIDS ; do
		if [ $PID != $$ ] && [ $PID != $PPID ] ; then
			kill -9 $PID 2>&-
		fi
	done
}


case "$1" in
	"boot")			boot &	;;
	"start")		start &	;;
	"stop")			stop	;;
	"restart")		stop
					start & ;;
	*)
			echo $0 'boot     - setup and start FTP FW upgrade service'
			echo $0 'start    - start FTP FW upgrade service'
			echo $0 'stop     - stop FTP FW upgrade service'
			echo $0 'restart  - restart FTP FW upgrade service'
			;;
esac

