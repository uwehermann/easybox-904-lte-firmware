#! /bin/sh

SKIP_FLASH_WRITE=$1
EVENT_CLI=/usr/sbin/libmapi_event_cli
FW_UPGRADE_NOTE_FILE=/tmp/in_fw_upgrade
UE_FW_VER_FILE=/tmp/UEfwVer

#echo "upgradeURfw.sh sleeping ....."

#sleep 35s

echo "upgradeURfw.sh running ....." > /dev/console

# check if the existing UE firmware same as that of the downloaded UE firmware, two cases:
# case 1: /tmp/UEfwVer should already be set in /usr/sbin/MACverify.sh (UE firmware is downloaded and verified, but not yet written to flash)
# case 2: lte_fw_date@system was set in the previous UE firmware download and upgrade 
if [ -f "$UE_FW_VER_FILE" ]; then 
	FLASH_FWVERSION=`cat $UE_FW_VER_FILE`
else
	FLASH_FWVERSION=`ccfg_cli get lte_fw_date@system`
fi
if [ -z "$FLASH_FWVERSION" ]; then
	return 0
fi
DONGLE_FWVERSION=`grep deviceFirmware /proc/arcusb/status.lte | cut -d " " -f 2`
echo ${DONGLE_FWVERSION} | grep -q ${FLASH_FWVERSION}
if [ "$?" == "0" ]; then
	#same version, skip the upgrade
	echo "same LTE fw version, skip upgrade" > /dev/console
	return 0 
fi

/usr/sbin/libmapi_mon_cli start_upgrade

pwd > /dev/console

cd /tmp
ls -l

if [ ! -r UEfw.bin ] ; then
	/usr/sbin/read_img ltefw UEfw.bin
	if [ ! -r UEfw.bin ] ; then
		echo "read UEfw.bin from FLASH failed! exit!" > /dev/console
    	return 2
    else
    	echo "read UEfw.bin from FLASH OK!" > /dev/console
    fi
fi

#mv UEfw.bin UEfw.tar

#/usr/sbin/read_img ltefw UEfw.tar

[ -x "$EVENT_CLI" ] && $EVENT_CLI add upgradeUEfw.sh UEFWUPGRD 1
touch $FW_UPGRADE_NOTE_FILE

# write to flash
if [ "$SKIP_FLASH_WRITE" != "1" ]; then
    /usr/sbin/write_img UEfw.bin ltefw 0 0
    ccfg_cli set lte_fw_date@system=$FLASH_FWVERSION commitcfg
fi

# call UE upgrade procedure
mkdir UEfw
cd UEfw
# now we are in /tmp/UEfw
tar xvf ../UEfw.bin
if [ "$?" != "0" ] ; then
	echo "untar UEfw.bin failed! exit!" > /dev/console
    return 2
fi

# runUEupgrade.sh
echo lte_upg_start=on > /proc/arcusb/config

umts_firmwareupt upgrade /tmp/UEfw
ret=$?

if [ "$ret" -eq 0 ] ; then
    # success
    echo "upgrade LTE firmware success!" > /dev/console
    echo lte_upg_start=off > /proc/arcusb/config
else
    #fail
    echo "upgrade LTE firmware failed!" > /dev/console
	/usr/sbin/libmapi_mon_cli stop_upgrade
	echo lte_upg_start=off > /proc/arcusb/config
    return 1
fi

/usr/sbin/libmapi_mon_cli stop_upgrade

# if upgrade success, erase the partition
cd /tmp
rm -f UEfw.bin

#flash_eraseall /dev/mtd13
#mtd erase ltefw

##reboot IAD
#reboot.sh

