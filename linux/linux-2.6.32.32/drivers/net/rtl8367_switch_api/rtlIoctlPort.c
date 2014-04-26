
#include <rtlIoctl.h>
#include <rtl8367b_asicdrv_port.h>
#include <rtl8367b_asicdrv_phy.h>


#define	RTL_PORT_ISSET( val, port )	( (0x1<<(port) & (val)) ? 1 : 0 )


int rtlIoctlPortGet( unsigned int cmd, stRtlIoctlPort* portBuf )
{
	int				cnt;
	int				ret;
	unsigned long	pbm;
	rtk_uint16		data16;

	pbm = RTL_PORT_OP_PBM( portBuf->op );

	for ( cnt=0; cnt<RTL_MAX_PORT; cnt++)
	{
		if ( RTL_PORT_ISSET( pbm, cnt ) == 0 )
			continue;
		switch ( RTL_PORT_OP_CMD( portBuf->op ) )
		{
		    case RTL_PORT_OP_CMD_STAT:
				ret = rtl8367b_getAsicPortStatus( cnt, (rtl8367b_port_status_t*)&data16 );
				if ( ret != RT_ERR_OK )
					return ret;
				portBuf->data[cnt] = (unsigned long)data16;
				break;
			case RTL_PORT_OP_CMD_FORCELINK:
				ret = rtl8367b_getAsicPortForceLink( cnt, (rtl8367b_port_ability_t*)&data16 );
				if ( ret != RT_ERR_OK )
					return ret;
				portBuf->data[cnt] = (unsigned long)data16;
				break;
		    default:
			    return RT_ERR_FAILED;
		}
	}

	return RT_ERR_OK;
}

int rtlIoctlPortSet( unsigned int cmd, stRtlIoctlPort* portData )
{
	int				cnt;
	int				ret;
	unsigned long	pbm;
	rtk_uint16		data16;
	rtk_uint32		data32;

	pbm = RTL_PORT_OP_PBM( portData->op );

	for ( cnt=0; cnt<RTL_MAX_PORT; cnt++)
	{
		if ( RTL_PORT_ISSET( pbm, cnt ) == 0 )
			continue;
		switch ( RTL_PORT_OP_CMD( portData->op ) )
		{
			case RTL_PORT_OP_CMD_FORCELINK:
				data16 = (rtk_uint16)portData->data[cnt];
				ret = rtl8367b_setAsicPortForceLink( cnt, (rtl8367b_port_ability_t*)&data16 );
				if ( ret != RT_ERR_OK )
					return ret;
				break;
			case RTL_PORT_OP_CMD_POWEROFF:
				ret = rtl8367b_getAsicPHYReg( cnt, 0, &data32 );
				if ( ret != RT_ERR_OK )
					return ret;
				ret = rtl8367b_setAsicPHYReg( (rtk_uint32)cnt, 0, data32|0x0800 );
				if ( ret != RT_ERR_OK )
					return ret;
				break;
			case RTL_PORT_OP_CMD_POWERON:
				ret = rtl8367b_getAsicPHYReg( cnt, 0, &data32 );
				if ( ret != RT_ERR_OK )
					return ret;
				ret = rtl8367b_setAsicPHYReg( (rtk_uint32)cnt, 0, data32&~0x0800 );
				if ( ret != RT_ERR_OK )
					return ret;
				break;
		    default:
			    return RT_ERR_FAILED;
		}
	}

	return RT_ERR_OK;
}

int rtlIoctlPortOp( unsigned int cmd, unsigned long opCode )
{
	switch ( opCode )
	{
		case RTL_PORT_OP_CMD_ENABLE_ALL:
			return rtl8367b_setAsicPortEnableAll( 1 );
		case RTL_PORT_OP_CMD_DISABLE_ALL:
			return rtl8367b_setAsicPortEnableAll( 0 );
		default:
		    return RT_ERR_FAILED;
	}
}
