#define _CRT_SECURE_NO_WARNINGS
#include <archive.h>
#include <archive_entry.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {
	RPM,
	BIN,
	PIC,
	PDB,
	DBA,
	DBA2,
	TDA,
	ICO,
	THREEGP,
	Z,
	Z2,
	BAC,
	BZ2,
	GIF,
	TIF,
	TIF2,
	CR2,
	CIN,
	DPX,
	DPX2,
	EXR,
	BPG,
	JPG,
	ILBM,
	EIGHTSVX,
	ACBM,
	ANBM,
	ANIM,
	FAXX,
	FTXT,
	SMUS,
	CMUS,
	YUVN,
	IFF,
	AIFF,
	IDX,
	LZ,
	EXE,
	ZIP,
	ZIP2,
	RAR,
	RAR2,
	PNG,
	CLASS,
	PS,
	PDF,
	ASF,
	OGG,
	PSD,
	WAV,
	AVI,
	MP3,
	MP32,
	BMP,
	ISO,
	FITS,
	FLAC,
	MID,
	DOC,
	DEX,
	VMDK,
	CRX,
	FH8,
	CWK,
	CWK2,
	TOAST,
	DMG,
	XAR,
	DAT,
	NES,
	TAR,
	TOX,
	MLV,
	SEVENZ,
	GZ,
	XZ,
	LZ4,
	CAB,
	FLIF,
	MKV,
	STG,
	DJVU,
	DER,
	DCM,
	WOFF,
	WOFF2,
	XML,
	WASM,
	LEP,
	SWF,
	DEB,
	WEBP,
	RTF,
	TS,
	M2P,
	MPG,
	ZLIB,
	DAT2,
	LZFSE,
	ORC,
	AVRO,
	RC,
	P25,
	PCV,
	PBT,
	EZ2,
	EZ3,

} file_type_t;
typedef struct file_sigs_ {
	file_type_t type;
	const char *sig;
	const char *extension;
	bool(*FileTypeFoundCallback)(char*);
	unsigned int siglen;
} file_sigs_t;

bool ScanPDF(char*buf) {

	return 1;
}

bool Check() {

	file_sigs_t sigs[] = {
		{ RPM, "edabeed", "rpm", ScanPDF, 8 },
	{ BIN, "5350303", "bin", ScanPDF, 8 },
	{ PIC, "0", "PIC", ScanPDF, 2 },
	{ PDB, "00000000000000000000000000000000000000000000000", "PDB", ScanPDF, 48 },
	{ DBA, "BEBAFEC", "DBA", ScanPDF, 8 },
	{ DBA2, "0001424", "DBA", ScanPDF, 8 },
	{ TDA, "0001445", "TDA", ScanPDF, 8 },
	{ ICO, "0000010", "ico", ScanPDF, 8 },
	{ THREEGP, "66747970336", "THREEGP", ScanPDF, 12 },
	{ Z, "1F9", "z", ScanPDF, 4 },
	{ Z2, "1FA", "z", ScanPDF, 4 },
	{ BAC, "4241434B4D494B454449534", "bac", ScanPDF, 24 },
	{ BZ2, "425A6", "bz2", ScanPDF, 6 },
	{ GIF, "47494638376147494638396", "gif", ScanPDF, 24 },
	{ TIF, "49492A00", "tif", ScanPDF, 53 },
	{ TIF2, "4D4D002A", "tif", ScanPDF, 53 },
	{ CR2, "49492A0010000000435", "cr2", ScanPDF, 20 },
	{ CIN, "802A5FD", "cin", ScanPDF, 8 },
	{ DPX, "53445058", "dpx", ScanPDF, 53 },
	{ DPX2, "58504453", "dpx", ScanPDF, 53 },
	{ EXR, "762F310", "exr", ScanPDF, 8 },
	{ BPG, "425047F", "bpg", ScanPDF, 8 },
	{ JPG, "FFD8FFDBFFD8FFE0????4A4649460001FFD8FFE1????45786966000", "jpg", ScanPDF, 56 },
	{ ILBM, "464F524D????????494C424", "ilbm", ScanPDF, 24 },
	{ EIGHTSVX, "464F524D????????3853565", "EIGHTSVX", ScanPDF, 24 },
	{ ACBM, "464F524D????????4143424", "acbm", ScanPDF, 24 },
	{ ANBM, "464F524D????????414E424", "anbm", ScanPDF, 24 },
	{ ANIM, "464F524D????????414E494", "anim", ScanPDF, 24 },
	{ FAXX, "464F524D????????4641585", "faxx", ScanPDF, 24 },
	{ FTXT, "464F524D????????4654585", "ftxt", ScanPDF, 24 },
	{ SMUS, "464F524D????????534D555", "smus", ScanPDF, 24 },
	{ CMUS, "464F524D????????434D555", "cmus", ScanPDF, 24 },
	{ YUVN, "464F524D????????5955564", "yuvn", ScanPDF, 24 },
	{ IFF, "464F524D????????46414E5", "iff", ScanPDF, 24 },
	{ AIFF, "464F524D????????4149464", "aiff", ScanPDF, 24 },
	{ IDX, "494E445", "idx", ScanPDF, 8 },
	{ LZ, "4C5A495", "lz", ScanPDF, 8 },
	{ EXE, "4D5", "exe", ScanPDF, 4 },
	{ ZIP, "504B0304504B0506", "zip", ScanPDF, 54 },
	{ ZIP2, "504B0708", "zip", ScanPDF, 54 },
	{ RAR, "526172211A070", "rar", ScanPDF, 14 },
	{ RAR2, "526172211A07010", "rar", ScanPDF, 16 },
	{ PNG, "89504E470D0A1A0", "png", ScanPDF, 16 },
	{ CLASS, "CAFEBAB", "class", ScanPDF, 8 },
	{ PS, "2521505", "ps", ScanPDF, 8 },
	{ PDF, "2550444", "pdf", ScanPDF, 8 },
	{ ASF, "3026B2758E66CF11A6D900AA0062CE6", "asf", ScanPDF, 32 },
	{ OGG, "4F67675", "ogg", ScanPDF, 8 },
	{ PSD, "3842505", "psd", ScanPDF, 8 },
	{ WAV, "52494646????????5741564", "wav", ScanPDF, 24 },
	{ AVI, "52494646????????4156492", "avi", ScanPDF, 24 },
	{ MP3, "FFF", "mp3", ScanPDF, 4 },
	{ MP32, "49443", "mp3", ScanPDF, 6 },
	{ BMP, "424", "bmp", ScanPDF, 4 },
	{ ISO, "434430303", "iso", ScanPDF, 10 },
	{ FITS, "53494D504C4520203D20202020202020202020202020202020202020205", "fits", ScanPDF, 60 },
	{ FLAC, "664C614", "flac", ScanPDF, 8 },
	{ MID, "4D54686", "mid", ScanPDF, 8 },
	{ DOC, "D0CF11E0A1B11AE", "doc", ScanPDF, 16 },
	{ DEX, "6465780A3033350", "dex", ScanPDF, 16 },
	{ VMDK, "4B444", "vmdk", ScanPDF, 6 },
	{ CRX, "4372323", "crx", ScanPDF, 8 },
	{ FH8, "4147443", "fh8", ScanPDF, 8 },
	{ CWK, "05070000424F424F050700000000000000000000000", "cwk", ScanPDF, 44 },
	{ CWK2, "0607E100424F424F0607E1000000000000000000000", "cwk", ScanPDF, 44 },
	{ TOAST, "4552020000008B45520200000", "toast", ScanPDF, 26 },
	{ DMG, "7801730D62626", "dmg", ScanPDF, 14 },
	{ XAR, "7861722", "xar", ScanPDF, 8 },
	{ DAT, "504D4F43434D4F4", "dat", ScanPDF, 16 },
	{ NES, "4E45531", "nes", ScanPDF, 8 },
	{ TAR, "7573746172003030757374617220200", "tar", ScanPDF, 32 },
	{ TOX, "746F783", "tox", ScanPDF, 8 },
	{ MLV, "4D4C564", "MLV", ScanPDF, 8 },
	{ SEVENZ, "377ABCAF271", "SEVENZ", ScanPDF, 12 },
	{ GZ, "1F8", "gz", ScanPDF, 4 },
	{ XZ, "FD377A585A000", "xz", ScanPDF, 14 },
	{ LZ4, "04224D1", "lz4", ScanPDF, 8 },
	{ CAB, "4D53434", "cab", ScanPDF, 8 },
	{ FLIF, "464C494", "flif", ScanPDF, 8 },
	{ MKV, "1A45DFA", "mkv", ScanPDF, 8 },
	{ STG, "4D494C2", "stg", ScanPDF, 8 },
	{ DJVU, "41542654464F524D????????444A5", "djvu", ScanPDF, 30 },
	{ DER, "308", "der", ScanPDF, 4 },
	{ DCM, "4449434", "dcm", ScanPDF, 8 },
	{ WOFF, "774F464", "woff", ScanPDF, 8 },
	{ WOFF2, "774F463", "woff2", ScanPDF, 8 },
	{ XML, "3c3f786d6c2", "XML", ScanPDF, 12 },
	{ WASM, "0061736", "wasm", ScanPDF, 8 },
	{ LEP, "cf840", "lep", ScanPDF, 6 },
	{ SWF, "43575346575", "swf", ScanPDF, 12 },
	{ DEB, "213C617263683", "deb", ScanPDF, 14 },
	{ WEBP, "52494646????????5745425", "webp", ScanPDF, 24 },
	{ RTF, "7B5C7274663", "rtf", ScanPDF, 12 },
	{ TS, "4", "ts", ScanPDF, 2 },
	{ M2P, "000001B", "m2p", ScanPDF, 8 },
	{ MPG, "000001BA47000001B", "mpg", ScanPDF, 18 },
	{ ZLIB, "7801789C78D", "zlib", ScanPDF, 12 },
	{ DAT2, "1F8B080", "dat", ScanPDF, 8 },
	{ LZFSE, "6276783", "lzfse", ScanPDF, 8 },
	{ ORC, "4F524", "orc", ScanPDF, 6 },
	{ AVRO, "4F626A0", "avro", ScanPDF, 8 },
	{ RC, "5345513", "rc", ScanPDF, 8 },
	{ P25, "6587785", "p25", ScanPDF, 8 },
	{ PCV, "5555aaa", "pcv", ScanPDF, 8 },
	{ PBT, "78563", "pbt", ScanPDF, 6 },
	{ EZ2, "6977885", "ez2", ScanPDF, 8 },
	{ EZ3, "6977855", "ez3", ScanPDF, 8 },
		

	};
	for (unsigned int i = 0; sigs[i].sig != NULL; i++) {
		printf("%s", sigs[i].sig);
	}

	return TRUE;
}


int xaw(int argc, char *argv[])
{

	if (argc != 2) {
		printf("Error: Invalid command line argument ... \n");
		printf("Done ...\n");
		return FALSE;
	}

	char *arch_name = argv[1];

	FILE *file;
	void *buffer;
	unsigned long f_len;
	struct archive *arch;
	struct archive_entry *arch_entry;
	bool rv;



	file = fopen(arch_name, "rb");
	if (!file) {
		printf("Unable to open file %s", arch_name);
		return FALSE;
	}
	fseek(file, 0, SEEK_END);
	f_len = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char *)malloc(f_len + 1);
	if (!buffer)
	{
		printf("Unable to allocate memory for achive buffer ... \n");
		fclose(file);
		return FALSE;
	}

	if (!fread(buffer, f_len, 1, file)) {
		printf("fread was 0 ... \n");
		fclose(file);
		return FALSE;
	}
	fclose(file);




	arch = archive_read_new(); archive_read_support_filter_all(arch);
	archive_read_support_format_all(arch);

	rv = archive_read_open_memory(arch, buffer, 10240);

	if (rv != ARCHIVE_OK) {
		printf("Archive is not ok \n");
		free(buffer);
		return FALSE;
	}

	while (archive_read_next_header(arch, &arch_entry) == ARCHIVE_OK) {
		__int64 entrysize;
		char *entrydata;
		entrysize = archive_entry_size(arch_entry);
		printf("File name: %s\n", archive_entry_pathname(arch_entry));
		printf("File size: %llu\n", entrysize);
		entrydata = (char *)malloc(entrysize + 1);
		if (!entrydata) {
			printf("Failed to allocate memory for entry data\n");
			free(buffer);
			rv = archive_read_free(arch);
			return FALSE;
		}
		archive_read_data(arch, entrydata, entrysize);
		printf("Data: \n%s\n", entrydata);
		free(entrydata);
		archive_read_data_skip(arch);
	}

	free(buffer);
	rv = archive_read_free(arch);
	if (rv != ARCHIVE_OK) {
		return FALSE;
	}

	return TRUE;
}
