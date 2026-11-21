/*****************************************************************************
 *
 * File: arm_modules.h
 *
 * $RCSfile: arm_modules.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: arm module info
 *
 * $Id: arm_modules.h,v 1.2 2002/03/14 12:03:48 pgm Exp $
 * $Log: arm_modules.h,v $
 * Revision 1.2  2002/03/14 12:03:48  pgm
 * compiled on benbecula
 *
 * Revision 1.1  2000/04/15 08:36:08  pgm
 * special from Harris
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/






struct image_header {
    int            Type ;
    unsigned char  Number ;
    unsigned char  Sig[3] ;
    int            Map ;
    int            Checksum ;
    int            Length ;
    unsigned char  Name[16]  ;
    unsigned int   Bootflags ; 
    unsigned char  Reserved[24] ;
} ;
typedef struct image_header image_header_t ;
