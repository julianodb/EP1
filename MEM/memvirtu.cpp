/* 
  PROJETO USPDesigner
  MODULO: MEM (Administrador de Memoria)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: memvirtu.cpp
   Coded by Marcos Tsuzuki

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   email: mtsuzuki at usp.br (remove space)
*/
/* Virtual Array Routines */
#include <stdio.h>
#include <stdlib.h>
#ifndef __UNIX_
 #include <malloc.h>
#endif
#include "memvirtu.h"
#define header    7

int init_v_array(char *filename, int rec_size, char filchar)
{
   long size;
   FILE *f;

   if ((f = fopen(filename, "wb")) != (FILE *)NULL)
   {
      size = 0;
      fwrite(&size, sizeof(long), 1, f);               /* write array size of 0 */
      fwrite(&rec_size, sizeof(int), 1, f);            /* and array element size */
      fwrite(&filchar, sizeof(char), 1, f);            /* and fill char */
      fclose(f);
      return(1);
   }
   return(0);
}

VACB *open_v_array(char *filename, int buffer_size)
{
   VACB *v_array;
   char *buf_ptr;
   int  i;
   char filchar;

   /* allocate virtual array control block */
   v_array = (VACB *)malloc(sizeof(VACB));
   if (v_array == NULL)
   {
      fprintf(stderr, "sem memoria\n");
      exit(1);
   }

   /* open virtual array file */
   v_array->file = fopen(filename, "r+b");
   if (v_array->file == (FILE *)NULL)
   {
      free(v_array);
      return((VACB *)NULL);
   }

   /* get array size and element size for control block */
   fread(&v_array->size, sizeof(long), 1, v_array->file);
   fread(&v_array->elsize, sizeof(int), 1, v_array->file);
   fread(&filchar, sizeof(char), 1, v_array->file);
   v_array->buf_elsize = v_array->elsize + sizeof(long);

   /* allocate buffer */
   v_array->buffer = (char *)malloc(v_array->buf_elsize *
                                    (buffer_size + 1));
   if (v_array->buffer == (char *)NULL)
   {
      fclose(v_array->file);
      free(v_array);
      fprintf(stderr, "sem memoria\n");
      exit(1);
      return((VACB *)NULL);
   }
   v_array->buf_size = buffer_size;

   /* set up blank_rec using the fill character in array header */
   /* for initializing new array elements                       */
   buf_ptr            = v_array->buffer + v_array->buf_elsize * v_array->buf_size;
   v_array->blank_rec = buf_ptr + sizeof(long);
   for (i = 0; i < v_array->buf_elsize; ++i)
   {
      *buf_ptr++ = filchar;
   }

   /* set record index negative for all buffer elements */
   buf_ptr = v_array->buffer;
   for (i = 0; i < v_array->buf_size; ++i)
   {
      *((long *)buf_ptr) = -1L;
      buf_ptr           += v_array->buf_elsize;
   }
   return(v_array);
}

void close_v_array(VACB *v_array)
{
   int  i;
   char *buf_ptr;
   long rec_index, file_offset;

   buf_ptr = v_array->buffer;
   /* flush buffer */
   for (i = 0; i < v_array->buf_size; ++i)
   {
      /* check each element index */
      /* if element present write it to disk */
      rec_index = *((long *)buf_ptr);
      if (rec_index >= 0)
      {
         file_offset = header + rec_index * v_array->elsize;
         fseek(v_array->file, file_offset, 0);
         fwrite(buf_ptr + sizeof(long), v_array->elsize, 1, v_array->file);
      }
      buf_ptr += v_array->buf_elsize;
   }
   free(v_array->buffer);                          /* de-allocate buffer */
   fclose(v_array->file);                          /* close array file */
   free(v_array);                                  /* de-allocate VACB */
}

char *access_v_rec(VACB *v_array, long index)
{
   char        *buf_ptr;
   int         buf_index;
   long        rec_index, temp_index;
   extern VACB *SET08VirtualArray;


   /* calculate buffer address of referenced element */
   buf_index = (int)(index % v_array->buf_size);
   buf_ptr   = v_array->buffer + buf_index * v_array->buf_elsize;
   rec_index = *(long *)buf_ptr;

   /* if element present, return its buffer address */
   if (rec_index == index)
   {
      return(buf_ptr + sizeof(long));
   }

   /* if element doesn't exist, extend the file */
   if (index >= v_array->size)
   {
      fseek(v_array->file, 0, 2);
      for (temp_index = v_array->size; temp_index++ <= index;)
      {
         fwrite(v_array->blank_rec, v_array->elsize, 1, v_array->file);
      }
      v_array->size = index + 1;
      fseek(v_array->file, 0, 0);
      fwrite(&v_array->size, sizeof(long), 1, v_array->file);
   }

   /* if buffer slot is occupied by another element */
   /* save it to disk                               */
   if (rec_index >= 0)
   {
      fseek(v_array->file, rec_index * v_array->elsize + header, 0);
      fwrite(buf_ptr + sizeof(long), v_array->elsize, 1, v_array->file);
   }

   /* read referenced element into buffer slot */
   fseek(v_array->file, index * v_array->elsize + header, 0);
   fread(buf_ptr + sizeof(long), v_array->elsize, 1, v_array->file);
   *((long *)buf_ptr) = index;

   /* return address of element in the buffer */
   return(buf_ptr + sizeof(long));
}
