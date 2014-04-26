#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

### ctc ###
TARGET_IMG_DIR=$(BIN_DIR)/$(SUBTARGET)
###########

JFFS2_BLOCKSIZE:=$(call qstrip,$(CONFIG_TARGET_ROOTFS_JFFS2_BLOCK_SIZE))
include $(INCLUDE_DIR)/image.mk

# add_image_padding <input_image_name> <block_size(bytes)> <size_to_less(bytes)>
# Result: <input_image_name>.padded
define add_image_padding
	len=`wc -c $(1) | awk '{ printf $$1 }'`; \
	pad=`expr $(2) - $$len % $(2)`; \
	pad=`expr $$pad % $(2)`; \
	pad=`expr $$pad - $(3)`; \
	cat $(1) > $(1).padded; \
	dd if=/dev/zero of=$(1).padded bs=1 count=$$pad seek=$$len
endef

define Image/BuildKernel
	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux $(KDIR)/vmlinux.lzma
	mkimage -A mips -O linux -T kernel -a 0x80002000 -C lzma -e \
		0x80002000 \
		-n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
		-d $(KDIR)/vmlinux.lzma $(KDIR)/uImage

	cp -f $(KDIR)/uImage $(BIN_DIR)/openwrt-$(BOARD)-uImage

	$$(call add_image_padding,$(KDIR)/vmlinux.lzma,\
		$(if $(JFFS2_BLOCKSIZE),$(shell expr $(patsubst %k,%,$(JFFS2_BLOCKSIZE)) \* 1024),16),\
		$(if $(JFFS2_BLOCKSIZE),64,0))
	mkimage -A mips -O linux -T kernel -a 0x80002000 -C lzma -e \
		0x$(shell grep kernel_entry $(KDIR)/linux-$(LINUX_VERSION)/System.map 2>/dev/null| awk '{ printf "%s", $$1 }') \
		-n 'MIPS LTQCPE Linux-$(LINUX_VERSION)' \
		-d $(KDIR)/vmlinux.lzma.padded $(KDIR)/uImage.padded

	mkdir -p $(TARGET_IMG_DIR)
	cp -f $(KDIR)/uImage.padded $(TARGET_IMG_DIR)/uImage
endef

define Image/Build/squashfs
	cat $(KDIR)/uImage $(KDIR)/root.$(1) > $(BIN_DIR)/openwrt-$(BOARD)-$(1).image
	$(call prepare_generic_squashfs,$(BIN_DIR)/openwrt-$(BOARD)-$(1).image)

	### ctc ###
#	$$(call add_image_padding,$(KDIR)/root.$(1),16,0)
	$$(call add_image_padding,$(KDIR)/root.$(1),4096,0)
	###########
	mkimage -A MIPS -O Linux -C lzma -T filesystem -e 0x00 -a 0x00 -n "LTQCPE RootFS" -d $(KDIR)/root.$(1).padded $(KDIR)/rootfs.img.padded
	mkdir -p $(TARGET_IMG_DIR)
	cp -f $(KDIR)/rootfs.img.padded $(TARGET_IMG_DIR)/rootfs.img
endef

define Image/Build/jffs2-$(JFFS2_BLOCKSIZE)
	mkdir -p $(TARGET_IMG_DIR)
	$(STAGING_DIR_HOST)/bin/mkfs.jffs2 -n -p $(JFFS2OPTS) \
		-e $(patsubst %k,%KiB,$(JFFS2_BLOCKSIZE)) \
		-o $(KDIR)/root.$(1) \
		-d $(TARGET_DIR)
	mkimage -A MIPS -O Linux -C gzip -T filesystem -e 0x00 -a 0x00 -n "LTQCPE RootFS" \
		-d $(KDIR)/root.$(1) \
		$(TARGET_IMG_DIR)/rootfs.img
endef

define Image/Build
	$(call Image/Build/$(1),$(1))
endef

define Image/Build/firmware
	FIRMWARE_DIR_NAME="firmware"; \
	GET_FIRMWARE_PATH="$(STAGING_DIR)"; \
	PUT_FIRMWARE_PATH="$(TARGET_IMG_DIR)"; \
	OUT_FIRMWARE_NAME="firmware.img"; \
	\
	PLATFORM=`echo $(SUBTARGET)|cut -d_ -f2-|awk '{ print toupper($$1) }'`; \
	if [ -z "$(CONFIG_UBOOT_CONFIG_FIRMWARE_IN_ROOTFS)" ]; then \
	   if [ -d $$GET_FIRMWARE_PATH/$$FIRMWARE_DIR_NAME ]; then \
		rm -rf $$PUT_FIRMWARE_PATH/$$OUT_FIRMWARE_NAME; \
		cd $$GET_FIRMWARE_PATH; \
		$(MKSQUASHFS_CMD) $$FIRMWARE_DIR_NAME tmp_firmware.img $(SQUASHFS_OPTS); \
		mkimage -A MIPS -O Linux -C lzma -T $$FIRMWARE_DIR_NAME -e 0x00 -a 0x00 -n \
			"$$PLATFORM Firmware" -d tmp_firmware.img $$OUT_FIRMWARE_NAME; \
		mkdir -p $$PUT_FIRMWARE_PATH; \
		mv $$OUT_FIRMWARE_NAME $$PUT_FIRMWARE_PATH/; \
		chmod 644 $$PUT_FIRMWARE_PATH/$$OUT_FIRMWARE_NAME; \
		rm -rf tmp_firmware.img; \
	   else \
		echo "$$GET_FIRMWARE_PATH/$$FIRMWARE_DIR_NAME not found..!"; \
		exit 1; \
	   fi; \
	fi;
endef

define Image/Build/fullimage
	IMAGE_LIST="$(TARGET_IMG_DIR)/rootfs.img \
		$(TARGET_IMG_DIR)/uImage"; \
	if [ -z "$(CONFIG_UBOOT_CONFIG_FIRMWARE_IN_ROOTFS)" ]; then \
		IMAGE_LIST="$$IMAGE_LIST $(TARGET_IMG_DIR)/firmware.img"; \
	fi; \
	ONEIMAGE="$(TARGET_IMG_DIR)/fullimage.img"; \
	PLATFORM=`echo $(SUBTARGET)|cut -d_ -f2-|awk '{ print toupper($$1) }'`; \
	rm -f $$ONEIMAGE; \
	for i in $$IMAGE_LIST; do \
		if [ -e $$i  ] ; then \
			len=`wc -c $$i | awk '{ printf $$1 }'`; \
			pad=`expr 16 - $$len % 16`; \
			pad=`expr $$pad % 16`; \
			if [ -e $$ONEIMAGE.tmp ] ; then \
				cat $$i >> $$ONEIMAGE.tmp; \
			else \
				cat $$i > $$ONEIMAGE.tmp; \
			fi; \
			while [ $$pad -ne 0 ]; do \
				echo -n "0" >> $$ONEIMAGE.tmp; \
				pad=`expr $$pad - 1`; \
			done; \
		else \
			echo "$$i not found!"; \
			rm -f $$ONEIMAGE.tmp; \
			exit 1; \
		fi; \
	done; \
	mkimage -A MIPS -O Linux -C none -T multi -e 0x00 -a 0x00 -n \
		"$$PLATFORM Fullimage" -d $$ONEIMAGE.tmp $$ONEIMAGE; \
	rm -f $$ONEIMAGE.tmp; \
	chmod 644 $$ONEIMAGE;
endef

### ctc ###
define Image/Build/fullimage_arcadyan
	###
	echo [htf] GOAL:[$(MAKECMDGOALS)]
	echo [htf] [$@]:[$^]
	ONEIMAGE="$(TARGET_IMG_DIR)/fullimage.$(1)" ; \
	ONEUEIMAGE="$(TARGET_IMG_DIR)/UEfullimage.$(1)" ; \
	rm -f $$ONEIMAGE ; \
	IMAGE_LIST="$(TARGET_IMG_DIR)/$(1) $(TARGET_IMG_DIR)/uImage" ; \
	for i in $$IMAGE_LIST; do \
		if [ -e $$i  ] ; then \
			len=`wc -c $$i | awk '{ printf $$1 }'`; \
			pad=`expr 16 - $$len % 16`; \
			pad=`expr $$pad % 16`; \
			if [ -e $$ONEIMAGE ] ; then \
				cat $$i >> $$ONEIMAGE; \
				KERNEL_SZ=`expr $$len + $$pad`;\
				echo "$$i = $$KERNEL_SZ bytes, padding = $$pad"; \
			else \
				cat $$i > $$ONEIMAGE; \
				ROOTFS_SZ=`expr $$len + $$pad`;\
				echo "$$i = $$ROOTFS_SZ bytes, padding = $$pad"; \
			fi; \
			while [ $$pad -ne 0 ]; do \
				echo -n "0" >> $$ONEIMAGE; \
				pad=`expr $$pad - 1`; \
			done; \
		else \
			echo "$$i not found!"; \
			rm -f $$ONEIMAGE; \
			exit 1; \
		fi; \
	done ; \
	echo -n "arcadyan" >> $$ONEIMAGE ; \
	printf "%08x%08x" $$ROOTFS_SZ $$KERNEL_SZ >> $$ONEIMAGE ; \
	echo -n "ARCADYAN" >> $$ONEIMAGE ; \
	chmod 644 $$ONEIMAGE ; \
	mv $$ONEIMAGE $$ONEIMAGE.nosigned
	###
	# pad LTE_UE_FW.tar to multiple of 2048
#	if [ -e $(TARGET_IMG_DIR)/LTE_UE_FW.tar  ] ; then \
#	    len=`wc -c $(TARGET_IMG_DIR)/LTE_UE_FW.tar | awk '{ printf $$1 }'`; \
#	    padLen=$$(( (2048 - $$len % 2048) % 2048 )); \
#	    echo "LTE_UE_FW.tar = $$len bytes, padding = $$padLen"; \
#	    dd if=/dev/zero of=$(TARGET_IMG_DIR)/LTE_UE_FW.tar bs=1 count=$$padLen  seek=$$len; \
#	else \
#            echo "LTE_UE_FW.tar not found!"; \
#            exit 1; \
#	fi;
#	./signImage.sh vf904Key.pem vf904Cert.pem $(TARGET_IMG_DIR)/LTE_UE_FW.tar 0 ;
	###
	ONEIMAGE="$(TARGET_IMG_DIR)/fullimage.$(1)" ; \
	ONEUEIMAGE="$(TARGET_IMG_DIR)/UEfullimage.$(1)" ; \
	rm -f $$ONEIMAGE ; \
#	./signImage.sh vf904Key.pem vf904Cert.pem $(TARGET_IMG_DIR)/$(1) 64 ; \
#	./signImage.sh vf904Key.pem vf904Cert.pem $(TARGET_IMG_DIR)/uImage 64 ; \
	IMAGE_LIST="$(TARGET_IMG_DIR)/$(1).signed $(TARGET_IMG_DIR)/uImage.signed" ; \
	for i in $$IMAGE_LIST; do \
		if [ -e $$i  ] ; then \
			len=`wc -c $$i | awk '{ printf $$1 }'`; \
			pad=`expr 16 - $$len % 16`; \
			pad=`expr $$pad % 16`; \
			if [ -e $$ONEIMAGE ] ; then \
				cat $$i >> $$ONEIMAGE; \
				KERNEL_SZ=`expr $$len + $$pad`;\
				echo "$$i = $$KERNEL_SZ bytes, padding = $$pad"; \
			else \
				cat $$i > $$ONEIMAGE; \
				ROOTFS_SZ=`expr $$len + $$pad`;\
				echo "$$i = $$ROOTFS_SZ bytes, padding = $$pad"; \
			fi; \
			while [ $$pad -ne 0 ]; do \
				echo -n "0" >> $$ONEIMAGE; \
				pad=`expr $$pad - 1`; \
			done; \
		else \
			echo "$$i not found!"; \
			rm -f $$ONEIMAGE; \
			exit 1; \
		fi; \
	done ; \
	echo -n "arcadyan" >> $$ONEIMAGE ; \
	printf "%08x%08x" $$ROOTFS_SZ $$KERNEL_SZ >> $$ONEIMAGE ; \
	echo -n "ARCADYAN" >> $$ONEIMAGE ; \
	len=`wc -c $$ONEIMAGE | awk '{ printf $$1 }'`; \
	printf "%08x" $$len ; \
	cat $$ONEIMAGE $(TARGET_IMG_DIR)/LTE_UE_FW.tar.signed >> $$ONEUEIMAGE ; \
	cat $(TARGET_IMG_DIR)/UEfwVer >> $$ONEUEIMAGE ; \
	printf "%08x" $$len >> $$ONEUEIMAGE ; \
	echo -n UEfullFW >> $$ONEUEIMAGE ; \
	wc -c $(TARGET_IMG_DIR)/UEfwVer | awk '{ printf "%8d", $$1 }' >>  $$ONEUEIMAGE ; \
	chmod 644 $$ONEIMAGE ; \
	mv $$ONEIMAGE $$ONEIMAGE.signed ; \
	mv $$ONEUEIMAGE $$ONEUEIMAGE.signed ;
	#
	ONEIMAGE="$(TARGET_IMG_DIR)/fullimage.$(1)" ; \
	mv $$ONEIMAGE.nosigned $$ONEIMAGE
	echo [htf] `wc -c $(TARGET_IMG_DIR)/UEfwVer | awk '{ printf "%8d", $$1 }'`

# Tail of UEfull:
#       | UEfwVer(variable Len) | Part1Len(%08x) | "UEfullFW" | UEfwVer_Len(%8d) |
#
# The related files:
#    package/arcadyan-utility/httpd-brn/httpd-brn-904LTE/cgi-src/cgi_system.c
#    package/base-files/files/usr/sbin/MACverify.sh
#    package/base-files/files/usr/sbin/upgrade.sh ---> image_type()
endef
###########

$(eval $(call BuildImage))
