#
# Copyright (C) 2010 Arcadyan Corporation
# All Rights Reserved.
#

ARC_MOD_NAME=ccfgsal
ARC_MOD_DESCR=common configuration Software Abstraction Layer
PKG_VERSION_MAJOR:=0
PKG_VERSION_MINOR:=1
PKG_RELEASE:=1
PKG_TEST_CLI:=$(ARC_MOD_NAME)_cli
PKG_DEPENDS:=+arc-middle-framework +arc-middle-mod-ccfg +lib-arc-com

#####################################################################

PKG_NAME:=arc-middle-mod-$(ARC_MOD_NAME)
PKG_MAPI_LDFLAGS:=-lmapi_ccfg -lArcCom
PKG_HDL_LDFLAGS:=
PKG_TEST_CLI_LDFLAGS:="-lpthread -lmapi_ccfg -lArcCom"
DEBUG:=0
MAPFILE:=1

#define Package/$(PKG_NAME)/pre-install
#endef

define Package/$(PKG_NAME)/post-install
endef

include ../mod_package.mk

$(eval $(call BuildPackage,$(PKG_NAME)))
