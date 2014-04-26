
include $(TOPDIR)/rules.mk

PKG_NAME:=ffmpeg
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ffmpeg
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=FFmpeg vedio and audio decoder library
  DEPENDS:=
endef

define Package/ffmpeg/description
 This package contains a library for vedio and audio decoding
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	mkdir -p $(STAGING_DIR)/usr/include/ffmpeg
	mkdir -p $(STAGING_DIR)/usr/include/ffmpeg/libavformat
	mkdir -p $(STAGING_DIR)/usr/include/ffmpeg/libavcodec
	mkdir -p $(STAGING_DIR)/usr/include/ffmpeg/libavutil
	$(CP) $(PKG_BUILD_DIR)/libavformat/*.h  $(STAGING_DIR)/usr/include/ffmpeg/libavformat
	$(CP) $(PKG_BUILD_DIR)/libavcodec/*.h   $(STAGING_DIR)/usr/include/ffmpeg/libavcodec
	$(CP) $(PKG_BUILD_DIR)/libavutil/*.h    $(STAGING_DIR)/usr/include/ffmpeg/libavutil
	ln -sf $(STAGING_DIR)/usr/include/ffmpeg/libavformat/avformat.h  $(STAGING_DIR)/usr/include/ffmpeg/avformat.h
	ln -sf $(STAGING_DIR)/usr/include/ffmpeg/libavformat/avio.h      $(STAGING_DIR)/usr/include/ffmpeg/avio.h
	ln -sf $(STAGING_DIR)/usr/include/ffmpeg/libavcodec/avcodec.h    $(STAGING_DIR)/usr/include/ffmpeg/avcodec.h
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) BLD_DIR=$(PKG_BUILD_DIR)
endef

define Package/ffmpeg/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libavformat/libavformat.so* $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libavcodec/libavcodec.so*   $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libavutil/libavutil.so*     $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libavformat/libavformat.so* $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libavcodec/libavcodec.so*   $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libavutil/libavutil.so*     $(1)/usr/lib/
endef

$(eval $(call BuildPackage,ffmpeg))
