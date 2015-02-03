/*
   SVC -- the SVC (Systems Validation Centre) file format library

   Copyright (C) 2000  Stichting Mathematisch Centrum, Amsterdam,
                       The  Netherlands

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   $Id: huffman.h,v 1.2 2008/09/30 08:22:51 bertl Exp $ */

#ifndef __HUFFMANHEADER
#define __HUFFMANHEADER


#include "hashtable.h"
#include "blocklist.h"
#include "lz.h"


  struct HFnode
  {
    struct HFnode* high, *low, *parent, *next, *previous;
    tBlock*  block;
    unsigned long frequency;
    atermpp::aterm        term;
  };

  typedef struct
  {
    struct HFnode* codes, *top;
    HTable* terms;
    BList blockList;
    LZbuffer buffer;
  } HFtree;



  int HFinit(HFtree*, HTable*);
  void HFfree(HFtree*);

  int HFencodeATerm(BitStream*, HFtree*, atermpp::aterm);
  int HFencodeIndex(BitStream*, HFtree*, long);
  int HFdecodeATerm(BitStream*, HFtree*, atermpp::aterm*);
  int HFdecodeIndex(BitStream*, HFtree*, long*);


#endif