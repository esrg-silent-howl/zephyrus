/*
 * _attr.h
 *
 *  Created on: Dec 22, 2020
 *      Author: josep & Hugoc
 */

#ifndef Z_ATTRIBUTES_H
#define Z_ATTRIBUTES_H

/* Check for GNU compiler */
#if defined (__GNUC__)
    /**/
    #ifndef _weak_ 
        #define _weak_      __attribute__((weak))
    #endif

    #ifndef _unused_ 
        #define _unused_      __attribute__((unused))
    #endif

    #ifndef _packed_ 
        #define _packed_      __attribute__((packed))
    #endif

#endif