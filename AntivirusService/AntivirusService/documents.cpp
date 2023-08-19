/*
finds SWF and etc in a OLE/DOC

*/
#include <Windows.h>

#define OLE_MAGIC "\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1"

typedef struct _OLEHeader { 
        BYTE Sig[9];         
        BYTE clsid[9];   
        USHORT MinorVersion; 
        USHORT DllVersion;   
        USHORT ByteOrder;    
        USHORT SectorShift;  
        USHORT MiniSectorShift; 
        ULONG MiniSectorCutoff; 
        UCHAR FATSections[110]; 
} OLEHeader;

typedef enum{
	UnCompressed,
	Compressed

} FlashTypes;

typedef enum{
	EMTPY,
	NIL,
	I2,
	I4,
	R4,
	R8, 
	CY,
	DATE, 
	My_BSTR,
	DISPATCH,
	ERROR, 
	BOOL,
	VARIANT,
	UNKNOWN, 
	DECIMAL, 
	ONE_CHAR,
	UNSIGNED_CHAR,
	UNSIGNED_SHORT, 
	UNSIGNED_LONG, 
	LONG_LONG, 
	UNSIGNED_LONG_LONG, 
	INTEGER, 
	UNSIGNED_INTEGER,
	VOID,
	HRESULT,
	PTR,
	SAFEARRAY,
	CARRAY,
	USERDEFINED,
	My_LPSTR,
	My_LPWSTR,
	My_FILETIME=64,
	BLOB = 65,
	STREAM = 66, 
	STORAGE=67, 
	STREAMED_OBJECT = 68,
	STORED_OBJECT = 69, 
	BLOB_OBJECT = 70, 
	CF = 71, 
	CLSID = 72,
	VECTOR = 0x1000

} PropertyTypes;

void *memmem(const char* haystack, size_t hl, const void* needle, size_t nl) {
	int i;
	if (nl > hl) return 0;
	for (i = hl - nl + 1; i; --i) {
		if (!RtlCompareMemory(haystack, needle, nl))
			return (char*)haystack;
		++haystack;
	}
	return 0;
}

BOOL IsEmbeddedFlashPresent(const char *buf, size_t buflen, UINT FlashType) {
	if (memmem(buf, buflen, "FWS", 3) != 0) {
		FlashType = UnCompressed;
	}
	else if(memmem(buf, buflen, "CWS", 3) != 0){
		FlashType = Compressed;
	}
	else{
		return FALSE;
	}
	return TRUE;
}

BOOL CheckOleStream(const char*buf, size_t buflen) {
	if (memmem(buf, buflen, "Object Pool", 11)) {
		return TRUE;
	}
	return FALSE;
}
BOOL CheckViso(const char*buf, size_t buflen) {
	if (memmem(buf, buflen, "Viso Drawing", 12)) {
		return TRUE;
	}
	return FALSE;
}
BOOL HandlePowerPoint(const char *buf, size_t buflen, BOOL&ContainsViso) {
	ContainsViso = CheckViso(buf, buflen);

}


BOOL HandleWord(const char *buf, size_t buflen, BOOL &ContainsMacros) {
	if (memmem(buf, buflen, "Macros", 6) != 0) {
		ContainsMacros = TRUE;
	}
}

BOOL ExcelMacroCheck(const char* buf, size_t buflen, BOOL &ContainsMacros) {
		if (memmem(buf, buflen, "_VBA_PROJECT_CUR", 16)) {
			ContainsMacros = TRUE;
		}
}



typedef enum{

} SummaryInfo;





VOID ExtractProperties(const char *buf, unsigned int offset){
	unsigned short type, padding;
	memcpy(&type, buf + offset, sizeof(unsigned short));
	offset += sizeof(type);
	type = le16_to_host(type);
	switch(type){
		case EMTPY:
		case NIL:
		{
			break;
		}
		case I2: // signed 16 bit integer
 		{
 			int16_t out;
 			memcpy(&out, buf + offset + 4, sizeof(out)); //sizeof(out) should be 2!!!
 			if(out >= 32768){
 				out -= 65536;
 			}
 			break;
		}
		case I4:
		{
			int16_t out;
			memcpy(&out, buf + offset + 4, sizeof(out));

			break;
		}
		case R4:
		case INTEGER:
		case ERROR:
		{
			int out;
			memcpy(&out, buf + offset + 4, sizeof(out));
			break;
		}
		case UNSIGNED_INTEGER:
		case UNSIGED_LONG:{
			unsigned int out;
			memcpy(&out, buf + offset + 4, sizeof(out));
		}
		case My_BTSTR:
		case My_LPSTR:
		{
			LPSTR out = NULL;
			memcpy(&out, buf + offset + 4, sizeof(LPSTR));
			// remove nnull chars
			break;
		}
		case My_LPWSTR:
		{

			break;
		}
		case BLOB:
		{
			int out;
			int pos;
			memcpy(&pos, buf + offset + 4, sizeof(pos));
			memcpy(&out, buf + offset + 8, offset + 8 + pos);
			break;

		}
		case My_FILETIME:
		{
			unsigned int t1, t2;
			memcpy(&t1, buf, offset + 4, sizeof(t1));
			memcpy(&t2, buf, offset + 8, sizeof(t2));
			SYSTEMTIME SysTime;
			FILETIME FileTime;
			FileTime.dwLowDateTime = t1;
			FileTime.dwHighDateTime = t2;
			FileTimeToSystemTime(&FileTime, &SysTime);	
		}
		case UNSIGNED_CHAR:
		{
			int8_t out;
			memcpy(&out, buf, offset + 4. sizeof(out));
			
		}
	}

}	

BOOL GetMetaData(const char*buf, size_t buflen, SummaryInfo Info){
	if(Info == SummaryInfo){

	}
	else if(Info == DocumentSummaryInfo){

	}
	else{
		return FALSE;
	}
}

BOOL CheckProperties(const char *buf, size_t buflen) {
	if (memmem(buf, buflen, "\x05SummaryInformation", 19)) {
		GetMetaData()


	}
	if(memmem(buf, buflen, "\x05DocumentSummaryInformation", 27)){

	}
	if(memmem(buf, buflen, "\x05HwpSummaryInformation", )){

	}
}


VOID DocumentHandler(const char*buf, size_t buflen){
	OLEHeader OLEH;
	if(!memcmp(buf, OLE_MAGIC, 8)){
		memcpy(OLEH.Sig, OLE_MAGIC, 8);
		memcpy(OLEH.clsid, buf + 8, 8);
		OLEH.MinorVersion = buf + 18;
		OLEH.DllVersion = buf + 26;
		OLEH.ByteOrder = buf + 28;
		OLEH.SectorShift = buf + 30;
		OLEH.MiniSectorShift = buf + 32;
		memcpy(OLEH.FATSections, buf + 76, 109);


	}
}



































VOID HandleWord2003XML(const char*data){



}

BOOL CheckMSOBase64(const char* data){
	if(strncmp(data, "QWN0aXZlTWltZQ", 14)){
		return TRUE;
	}
	return FALSE;
}
// CHCEK MSO FILE
BOOL IsMso(const char *data){

	if(strncmp(data, "ActiveMime", 10)){
		return TRUE;
	}
	return FALSE;
}


BOOL MSO_EXTRACTION(const char*data, size_t datalen){
	if(IsMso(data)){
		char *first = strtok(data, "=");

	}


}






void decompress_stream(const char *data, size_t sLen){

}

bool _extract_vba(){
	
}




#define RTF_MAGIC "\x7b\x5c\x72\x74"


//http://www.biblioscape.com/rtf15_spec.htm#Heading7
void PraseRTF(const char* data, size_t datalen){
	char *codepage = data + 7;
	if(*codepage == '')
}







