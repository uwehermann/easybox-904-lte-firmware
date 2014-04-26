#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include "rtlIoctl.h"
#include <rtk_error.h>


stRtlIoctlCbData stRtlIoctlCb[] = { RTL_IOC_MODULES };

int  iRtlIoctlCbNum = sizeof( stRtlIoctlCb ) / sizeof( stRtlIoctlCb[0] );

stRtlIoctlCbData* rtlIoctlCb( void )
{
	return stRtlIoctlCb;
}

int rtlIoctlCbNum( void )
{
	return iRtlIoctlCbNum;
}


int rtlIoctl ( unsigned int cmd, unsigned long arg )
{
	stRtlIoctlCbData*	pCb;
	unsigned char		reqType;
	unsigned short		reqSize;
	int					cnt;
	int					rv;
	char*				usrData;
	char				buf[ 512 ];
	char*				pNew;
	char*				pBuf;

	if (_IOC_TYPE( cmd ) != RTL_IOC_DEVTYPE)
	{
		printk("rtlIoctl: _IOC_TYPE( 0x%08x ) = 0x%x\n", cmd, _IOC_TYPE( cmd ));
		return RT_ERR_FAILED;
    }

	reqType = (unsigned char) _IOC_NR( cmd );
	reqSize = (unsigned short)_IOC_SIZE( cmd );

	for (cnt=0, pCb=rtlIoctlCb(); cnt<rtlIoctlCbNum(); cnt++, pCb++)
	{
		if ( pCb->type == reqType && ( pCb->dataSize == reqSize || _IOC_DIR( cmd ) == _IOC_NONE ) )
		{
			break;
		}
	}

	if ( cnt >= rtlIoctlCbNum() )
    {
        printk("rtlIoctl: cnt = %d\n", cnt);
        printk("rtlIoctl: rtlIoctlCbNum() = %d\n", rtlIoctlCbNum());
		return RT_ERR_FAILED;
    }

	if ( reqSize > sizeof( buf ) )
	{
		pBuf = buf;
		pNew = NULL;
	}
	else
	{
		pBuf = pNew = kmalloc( reqSize, GFP_KERNEL );
		if (pNew == NULL)
        {
            printk("rtlIoctl: pNew == NULL\n");
			return RT_ERR_FAILED;
        }
	}

	usrData = (char*) arg;

	switch (_IOC_DIR( cmd ) )
	{
	  case _IOC_READ:
		{
    		if (arg == 0)
			{
				if (pNew != NULL)
					kfree( pNew );

                printk("rtlIoctl: [_IOC_WRITE]\n");
				return RT_ERR_FAILED;
			}
			if (pCb->fnR == NULL)
			{
				if (pNew != NULL)
					kfree( pNew );
				return RT_ERR_FAILED;
			}
			/* retrieve data */
			if ( (rv = pCb->fnR( cmd, pBuf ) ) >= 0 )
			{
    			/* return data */
    			if ( copy_to_user( usrData, pBuf, reqSize ) )
    			{
    				if (pNew != NULL)
    					kfree( pNew );

                    printk("rtlIoctl: copy_to_user\n");
    				return RT_ERR_FAILED;
    			}
			}
			/*  */
			if (pNew != NULL)
				kfree( pNew );
			return rv;
		}
	  case _IOC_WRITE:
		{
			if (arg == 0)
			{
				if (pNew != NULL)
					kfree( pNew );

                printk("rtlIoctl: [_IOC_WRITE]\n");
				return RT_ERR_FAILED;
			}
			if (pCb->fnW == NULL)
			{
				if (pNew != NULL)
					kfree( pNew );
				return RT_ERR_FAILED;
			}
			/* copy data */
			if ( copy_from_user( pBuf, usrData, reqSize ) )
			{
				if (pNew != NULL)
					kfree( pNew );
                printk("rtlIoctl: [_IOC_WRITE]copy_from_user\n");
				return RT_ERR_FAILED;
			}
			/* set data */
			rv = pCb->fnW( cmd, pBuf );
			/*  */
			if (pNew != NULL)
				kfree( pNew );
			return rv;
		}
	  case _IOC_READ | _IOC_WRITE:
		{
			if (arg == 0)
			{
				if (pNew != NULL)
					kfree( pNew );
                printk("rtlIoctl: [_IOC_READ | _IOC_WRITE]\n");
				return RT_ERR_FAILED;
			}
			if (pCb->fnRW == NULL)
			{
				if (pNew != NULL)
					kfree( pNew );
				return RT_ERR_FAILED;
			}
			/* copy data */
			if ( copy_from_user( pBuf, usrData, reqSize ) )
			{
				if (pNew != NULL)
					kfree( pNew );
                printk("rtlIoctl: [_IOC_READ | _IOC_WRITE]copy_from_user\n");
				return RT_ERR_FAILED;
			}
			/* exchange data */
			if ( (rv = pCb->fnRW( cmd, pBuf ) ) >= 0 )
			{
    			/* return data */
    			if ( copy_to_user( usrData, pBuf, reqSize ) )
    			{
    				if (pNew != NULL)
    					kfree( pNew );

                    printk("rtlIoctl: [_IOC_READ | _IOC_WRITE]copy_to_user\n");
    				return RT_ERR_FAILED;
    			}
			}
			/*  */
			if (pNew != NULL)
				kfree( pNew );
			return rv;
		}
	  case _IOC_NONE:
		{
			if (pCb->fnN == NULL)
			{
				if (pNew != NULL)
					kfree( pNew );
				return RT_ERR_FAILED;
			}
			/* call handler */
			rv = pCb->fnN( cmd, arg );
			/*  */
			if (pNew != NULL)
				kfree( pNew );
			return rv;
		}
	  default:
		{
			if (pNew != NULL)
				kfree( pNew );

            printk("rtlIoctl: [default]\n");
			return RT_ERR_FAILED;
		}
	}
}

