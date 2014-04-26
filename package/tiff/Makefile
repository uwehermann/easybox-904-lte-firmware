#
# Copyright (C) 2009 Arcadyan Corporation
# All Rights Reserved.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tiff
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=util
  CATEGORY:=Utilities
  TITLE:=FAX, TIFF and PDF/PS converting utilities
  DEPENDS:=
endef

define Package/$(PKG_NAME)/description
 This package contains FAX, TIFF and PDF/PS converting utilities.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)	CC="$(TARGET_CC)"
endef

define Package/$(PKG_NAME)/install
	# tools
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/fax2tiff	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/fax2ps		$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiff2bw		$(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiff2pdf	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiff2ps		$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffcp		$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffdither	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffdump	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffinfo	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffset		$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffmedian	$(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/.libs/tiffsplit	$(1)/usr/bin/
	# libs
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libtiff/.libs/libtiff.so.3.8.2 $(1)/usr/lib/libtiff.so.3
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
