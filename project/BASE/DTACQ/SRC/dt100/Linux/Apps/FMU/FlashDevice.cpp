/*****************************************************************************
 *
 * File: FlashDevice.cpp: universal flash manager for ACQ32, EBSA. 
 *             extendible to other flash types
 *
 * $RCSfile: FlashDevice.cpp,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FlashDevice.cpp,v 1.14 2002/07/30 02:28:00 pgm Exp $
 * $Log: FlashDevice.cpp,v $
 * Revision 1.14  2002/07/30 02:28:00  pgm
 * don't advertise first retry
 *
 * Revision 1.13  2002/07/30 11:00:52  pgm
 * with retry on write
 *
 * Revision 1.12  2001/10/14 08:59:39  pgm
 * works on Alpha
 *
 * Revision 1.11  2001/08/12 08:33:08  pgm
 * works on Lewis
 *
 * Revision 1.10  2000/09/30 09:50:08  pgm
 * MakeArmSpin call added for boot66 op
 *
 * Revision 1.9  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.8  1999/10/31 20:22:43  pgm
 * help and ident opts added
 *
 * Revision 1.7  1999/10/31 19:41:11  pgm
 * works again with acq32 device
 *
 * Revision 1.6  1999/10/27 22:07:20  pgm
 * acq32-drv.o replaces ebsa-drv
 *
 * Revision 1.5  1999/10/20 19:56:43  pgm
 * mod to suit acq32 driver (untested)
 *
 * Revision 1.4  1999/10/11 10:34:56  pgm
 * Works at DTACQ
 *
 * Revision 1.3  1999/10/01 21:47:05  pgm
 * WORD rwrite fixup/frigs for 21285
 *
 * Revision 1.2  1999/10/01 09:18:06  pgm
 * fix leader instance bug
 *
 * Revision 1.1  1999/09/30 10:14:33  pgm
 * pfmu - enhanced fmu utility
 *
 *
\*****************************************************************************/


#include "local.h"

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "FlashDevice.h"
#include "ebsa_app_layer.h"

#include "platform.h"
/*
 * implement Board
 */


#include "acq32ioctl.h"

static int _fd;


class EbsaMapping
// singleton factory
{
    enum { ROM_LEN = 0x400000, CSR_LEN = 0x80 };

private:
    
    unsigned* m_p_csr;
    unsigned* m_p_mem;

    EbsaMapping( const char* devname )
    {
        int fd;

        if ( (fd = open( devname, O_RDWR )) < 0 ){
            perror( devname );
            exit( 1 );
         }    
         _fd = fd;
         m_p_mem = (unsigned*)mmap( NULL, ROM_LEN, O_RDWR, MAP_SHARED, fd, 0 );
         ASSERT( m_p_mem != (unsigned*)-1 );

         m_p_csr = (unsigned*)mmap( NULL, CSR_LEN, O_RDWR, MAP_SHARED, fd, 0 );
         ASSERT( m_p_csr != (unsigned*)-1 );
    }

public:
    static EbsaMapping& Instance( const char* devname )
    {
        static struct EbsaInstances {
           char devname[80];
           EbsaMapping* mapping;
        }
            _instances[4];

	for ( int ii = 0; ii != 4; ++ii ){
	    if ( STREQ( _instances[ii].devname, devname ) ){
                return *_instances[ii].mapping;
            }else if ( !_instances[ii].mapping ){
                strcpy( _instances[ii].devname, devname );
                _instances[ii].mapping = new EbsaMapping( devname );
                return *_instances[ii].mapping;
            }   
        }

        ASSERT( false );
        return *_instances[0].mapping;  // won't happen
    }

    unsigned* GetCsr() const { return m_p_csr; }
    unsigned* GetMem() const { return m_p_mem; }
};

FlashDevice* Board::m_known_devices[Board::MAXDEVS];

Board::Board( const char* device_name ) :
    m_p_flash( EbsaMapping::Instance( device_name ).GetMem() ),
    m_p_csr(   EbsaMapping::Instance( device_name ).GetCsr() )
{
    ASSERT( m_p_flash );
}

int Board::GetFileId() 
{
    return _fd;
}

FlashDevice* Board::GetFlashDevice() const
{
    for ( int idev = 0; 
          idev != MAXDEVS && m_known_devices[idev]; ++idev ){

        FlashDevice* fdclone = 
            m_known_devices[idev]->Clone( m_p_flash, m_p_csr );

        if ( fdclone->InitIfPresent() ){
            return fdclone;
        }else{
            fdclone->Recycle();
        }
    }
    fprintf( stderr, "ERROR: flash Device NOT identified\n" );
    exit( 1 );
    return 0;
}

void Board::Register( FlashDevice* flash_device )
{
    for ( int idev = 0; idev != MAXDEVS; ++idev ){
        if ( m_known_devices[idev] == flash_device ){
            return;                                    // already registered
        }else if ( m_known_devices[idev] == 0 ){

            printf( "Board::Register() registered %s at index %d\n",
                        flash_device->GetIdent(), idev );

            m_known_devices[idev] = flash_device;
            return;
        }
    }

    fprintf( stderr, "ERROR: too many devices\n" );
    exit( 1 );
}

/*
 * implementation of base flash device class
 */
FlashDevice::FlashDevice( const BlockDef *const pBlockDef ) :
    m_p_base( 0 ),
    m_p_csr( 0 ),
    is_class_leader( true ),
    m_pBlockDef( pBlockDef ),
    main_id( 0 )
{

}

FlashDevice::FlashDevice( 
    const BlockDef *const pBlockDef,
    unsigned *const p_base,
    unsigned* const p_csr,
    unsigned _main_id
) :

    m_p_base( p_base ),
    m_p_csr( p_csr ),
    is_class_leader( false ),
    m_pBlockDef( pBlockDef ),
    main_id( _main_id )
{

}

void FlashDevice::Register()
{
  Board::Register( this );
}

void FlashDevice::Recycle()
{
   if ( !is_class_leader ){
       delete this;
   }
}

bool FlashDevice::InitIfPresent()
{
    ASSERT( !is_class_leader );

    return OnInitIfPresent();
}

int FlashDevice::write( int offset, unsigned data )
{
    for ( int itry = 0; itry != MAXRETRY; ++itry ){
	if ( onWrite( offset, data ) ){
	    return 1;
	}else{
	  if ( itry > 0 ){
	    fprintf( stderr, "%d:write 0x%08x 0x%04x failed\n",
		     itry, offset, data );
	  }
	}
    }

    return 0;
}
int FlashDevice::onWrite( int offset, unsigned data )
{
    ASSERT( !is_class_leader );

    m_p_base[offset] = data;   // no checking

    return 1;
}

unsigned FlashDevice::read( int offset )
{
    ASSERT( !is_class_leader );

    return m_p_base[offset];
}

int FlashDevice::onWrite( int offset, const unsigned* client_buf, int nints )
{
    int nwrite;

    for ( nwrite = 0; nwrite != nints; ++nwrite ){
        if ( !write( offset+nwrite, client_buf[nwrite]  ) ){
            return nwrite;
        }
    }

    return nwrite;
}

int FlashDevice::read( int offset, unsigned* client_buf, int nints )
{
    int nread;

    for ( nread = 0; nread != nints; ++nread ){
        client_buf[nread] = read( offset+nread );
    }

    return nread;
}

int  FlashDevice::GetNumBlocks() const
{
    int iblock;
    for ( iblock = 0; m_pBlockDef[iblock].nbytes; ++iblock ){
        ;
    }

    return iblock;
}
int  FlashDevice::GetBlockStart( int iblock ) const
{
    ASSERT( IN_RANGE( iblock, 0, GetNumBlocks() ) );

    return m_pBlockDef[iblock].startoff;
}
int  FlashDevice::GetBlockLen( int iblock ) const
{
    ASSERT( IN_RANGE( iblock, 0, GetNumBlocks() ) );

    return m_pBlockDef[iblock].nbytes/sizeof(__u32);
}


void FlashDevice::D21285WordFix( int woffset )
{
    unsigned a01 =  (woffset<<1)&0x02;

    if ( ioctl( _fd, ACQ32_IOSROM_WORD_A01, a01 ) == ERR ){
 // if ( ioctl( _fd, EBSA_IOSMBX( 3 ),  a01 )     == ERR    ){
        perror( "FlashDevice::D21285WordFix failed" );
        exit( 1 );
    }
}
void FlashDevice::D21285OffsetFixup( int& offset )
// frig copie dfrom DEC code. remember, our offset is in longs
{
   if ( IN_RANGE( offset, 0, OFFSET(0x1f) ) ){
       offset += OFFSET(0x20);
   }else if ( IN_RANGE( offset, OFFSET(0x20), OFFSET(0x3f) ) ){
       offset -= OFFSET(0x20);
   }else{
       ;
   }
}

void FlashDevice::D21285WordOffsetFixup( int& woffset )
// frig copie dfrom DEC code. remember, our offset is in longs
{
   if ( IN_RANGE( woffset, 0, WOFFSET(0x1f) ) ){
       woffset += WOFFSET(0x20);
   }else if ( IN_RANGE( woffset, WOFFSET(0x20), WOFFSET(0x3f) ) ){
       woffset -= WOFFSET(0x20);
   }else{
       ;
   }
}


void FlashDevice::MakeArmSpin()
  // disable ARM to ensure programming works
{
  if ( ioctl( _fd, ACQ32_IOARMSPIN ) == ERR ){
    perror( "FlashDevice::MakeArmSpin() failed\n" );
  }
}

unsigned FlashDevice::alternate_id;
bool     FlashDevice::alternate_id_valid;






