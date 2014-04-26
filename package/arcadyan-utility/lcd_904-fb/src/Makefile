ifeq ($(LITE_FLAG),y)
	drv_lcd_904-objs := lcd_904_lite.o
else
	drv_lcd_904-objs := lcd_904.o sysfillrect.o syscopyarea.o sysimgblt.o
endif
obj-m	:= drv_lcd_904.o
