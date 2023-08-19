#include <stdlib.h>
#include <stdio.h>
#define MAX_PARTITIONS 4

typedef enum __Types{
    NOTHING = 0x00,
    FAT12 = 0x01,
    XENIX_ROOT = 0x02,
    XENIX_USR = 0x03,
    FAT16_old = 0x04,
    Extended_DOS = 0x05,
    FAT16 = 0x06,
    FAT32 = 0x0b,
    FAT32_LBA = 0x0c,
    NTFS = 0x07,
    LINUX_SWAP = 0x82,
    LINUX_NATIVE = 0x83,
    PROTECTIVE_MBR = 0xee
} Types;

typedef struct _PARTITION{
    unsigned char *bootstrap_code[440];
	unsigned char Offset; 
	unsigned char CylinderHeadsStart[3];
	unsigned char CylinderHeadsEnd[3];
	unsigned char DiskStart[4];
    unsigned char DiskSig[4];
	Types type;

} PARTITION;

#define MBR_INFECTED true
#define MBR_CLEAN false


void Disassembly(unsigned char *buf, size_t buflen, unsigned long long address, BOOL CheckJmp){
    
    if(!OpenDisAsm(CS_ARCH_X86, &hCapStone, CS_MODE_16)){
        return false;
    }
    cs_insn *insn = cs_malloc(hCapstone);
    while(cs_disasm_iter(hCapStone, lpszSection, &len, &address, insn)){
        if(CheckJmp){
            if((insn.cs_detail->groups == CS_GRP_CALL) != NULL || (insn.cs_detail->groups == CS_GRP_RET)){
                printf("suspicious as fuck!\n");
            }
        }

        if(insn.mnemonic == "int"){
            switch(insn.bytes[1]){
                case 0x10:
                case 0x13:
                case 0x18:
                case 0x1a:

                    break;

                default:
                    printf("unknown intterupt 0x%x", insn.bytes[1]);
            }
        }


    }
    cs_free(insn, 1);
    
}



void ScanVBR(const char *buf, size_t buflen){
    unsigned int StartOfCode = 84;
    unsigned int stringoffset = 0;
    unsigned int EndOfCode = 0;
    char *LoadType = NULL;


    switch(stringoffset){
        case 131:
        {
            EndOfCode = 256 + stringoffset;
            LoadType = "NT 5.1/5.2 Virtual BootRecord";

            break;
        }

        case 128:
        {
            EndOfCode = 256 + stringoffset;
            LoadType = "NT 6.0 Virtual BootRecord";

            break;
        }

        case 140:
        {

            EndOfCode = 256 + stringoffset;
            LoadType = "NT 6.1 Virtual BootRecord";

            break;
        }
        
        default:
            EndOfCode = 0;
            break;
    
    }
    if(stringoffset == 394){
        EndOfCode = stringoffset;
        LoadType = "NT 6.2 and above Virtual BootRecord";
    }

    Disassembly(buf, buflen, 0, FALSE);

    
}
void ScanMbr(){
    size_t dwBuf = 0;
    FILE * lpszFile = 0;
    unsigned char  buf[512] = { 0 };

    lpszFile = fopen("/dev/sda", "rb");
    if (lpszFile == NULL)
    {
        printf("fopen() failed!\n");
            return;
    }

    fseek (lpszFile, 0, SEEK_CUR);
    if (fread(buf, 1, sizeof(buf), lpszFile) != 512)
    {
        printf("fread() failed\n");
        fclose(lpszFile);
        return;
    }

  
    if (fclose(lpszFile) ==  EOF)
    {
        printf("fclose() failed\n");
        return;
    }

    for (size_t i = 0; i < 512; i++)
    {
        printf("%02x", ((char *)buf)[i]& 0xFF);               
    }

    printf("Size of buf = %d - and number of bytes read are %d\n", sizeof(buf), dwBuf);

    for (size_t i = 0 ; i < MAX_PARTITIONS ; i++)
    {
        unsigned char *partition = buf + 446 + (16 * i);  
        if(partition){
            printf("\\x%02x\n", partition);
        }  
    }
}