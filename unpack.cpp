#include <stdio.h>



void LZMADecompression(unsigned const char*src, size_t srclen, char* uncompressed, size_t uncompressedlen){
    size_t destlen = 0;
    size_t scratchlen = 0;
    if(LzmaGetInfo(src, srclen, destlen, scratchlen) == EFI_SUCCESS && uncompressedlen && scratchlen){
      char *dest = HeapAlloc(GetProcessHeap(), 0, destlen + 1);
      if(!dest){
        return FALSE;
      }
      char *Scratch = HeapAlloc(GetProcessHeap(), 0, scratchlen);
      if(!Scratch){
        HeapFree(GetProcessHeap(), 0, Scratch);
      }
      
      if(LzmaDecompress(src, srclen, dest, destlen, Scratch, scratchlen) == EFI_SUCCESS){
         uncompressed = dest;
         uncompressedlen = destlen;
         
         HeapFree(GetProcessHeap(), 0, Scratch);
         HeapFree(GetProcessHeap(), 0, dest);
         return TRUE;
      }
  }
  return FALSE;

}


https://github.com/OpenGG/swfzip/blob/master/swfzip.py
https://www.adlice.com/making-an-antivirus-engine-the-guidelines/
https://en.wikipedia.org/wiki/Executable_compression



/*
* UPX
*/





