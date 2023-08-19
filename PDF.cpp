#include <stdio.h>
#include <stdlib.h>

bool GetFromStream(const char *src, size_t sourcelen, char * dest, const char *needle, size_t needlelen){
    char *source = src;
    size_t index = 0;
    do{
        source = memmem(source, sourcelen, needle, needlelen);
        
        if(source !=  NULL){
          memcpy(dest, source, needlelen);
          printf("%s at index: ", dest);
          printf("from %d to %d\n",  source - src, source - src + needlelen);

          source++;
          sourcelen--;
        }

      
    } while(source != NULL);
}

bool GetPdfDate(char *UnformattedDate, const char* PDFBuf, size_t MaxSize){
	for(size_t i = 0; i <= (MaxSize - 10); ++i){
		if(PDFBuf[i] == '(' && PDFBuf[++i] == 'D'){
		    memcpy(UnformattedDate, PDFBuf+ i + 2, 8);
		    return true;
		}
	}
	return false;
}

bool ExtractPDFHeader(const char*PDFBuf, size_t PDFBuflen, char *Dest){
	if(PDFBuflen > 8){
	    if(PDFBuf[0] == '%' && PDFBuf[1] == 'P' && PDFBuf[2] == 'D' && PDFBuf[3] == 'F' && PDFBuf[4] == '-'){
    	        memcpy(Dest, PDFBuf, 8);
        	    return true;
		}
    }
  return false;
}


bool GetFromStream(const char *str1, char * Dest, const char *needle, size_t needlelen, size_t MaxSize){
    char *newptr = str1; 
    do{
        newptr = strstr(newptr, needle);
        if(newptr !=  NULL){
          char specificsize[10] = { 0 };
          printf("%s",  newptr);
          
          newptr += needlelen;
        
       	}

      
    } while(newptr != NULL);
}


 
bool GetFromStream(const char *src, size_t sourcelen, char * dest, const char *needle, size_t needlelen){
    char *source = src;
    size_t index = 0;
    do{
        source = memmem(source, sourcelen, needle, needlelen);
        
        if(source !=  NULL){
          memcpy(dest, source, needlelen);
          printf("%s at index: ", dest);
          printf("from %d to %d\n",  source - src, source - src + needlelen);
            source++;
        }

      
    } while(source != NULL);
}


 
int makehist(char *block,int *hist, int len){
	size_t histogram[256];
	size_t histogramlen = 0;
	
	for(size_t i= 0; i < 256; i++){
		histogram[i]=-1;
	}
	for(size_t i = 0; i < len; i++){
		if(histogram[(int)block[i]] == -1){
			histogram[(int)block[i]]=histogramlen;
			histogramlen++;
		}
		hist[histogram[(int)block[i]]]++;
	}
	return histogramlen;
}
 
double entropy(char * block, int *histogramindex, size_t len){
	size_t histogram[256] = { 0 };
	size_t histogramlen = 0;
	
	double H = 0;

	for(size_t i= 0; i < 256; i++){
		histogram[i]=-1;
	}
	for(size_t i = 0; i < blocklen; i++){
		if(histogram[(int)block[i]] == -1){
			histogram[(int)block[i]]=histogramlen;
			histogramlen++;
		}
		histogramindex[histogram[(int)block[i]]]++;
	}
	for(size_t i = 0; i < histogramlen; i++){
		if(histogramindex[i] != 0){
			// log of 0 yikes!
			H-=(double)histogramindex[i]/blocklen*log2((double)histogramindex[i]/blocklen);
		}
	}
	return H;
}
 
int CalculateEntropy(void){
	const char S[] = "lmaooo";
	int len,*hist,histlen;
	double H;
	len=strlen(S);
	hist=(int*)calloc(len,sizeof(int));

	H=entropy(S, hist,len);
	printf("%lf\n",H);
	return 0;
}


