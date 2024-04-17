#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char start_sector[4];
    char length_sectors[4];
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size; // 2 bytes
	// {...}  COMPLETAR
	unsigned char cluster_size;
	unsigned short reserved_sectors;
	unsigned char num_fats;
	unsigned short num_root_entries;
	unsigned short num_sectors;
	unsigned char media_type;
	unsigned short fat_size_sectors;
	unsigned short sectors_per_track;
	unsigned short num_heads;
	unsigned int hidden_sectors;
	unsigned int total_sectors;
	unsigned char drive_number;
	unsigned char current_head;
	unsigned char signature;

	//
    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8]; // Type en ascii
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    
    fseek(in, 0x1BE , SEEK_SET); // Ir al inicio de la tabla de particiones. Completar ...
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas 
    
    for(i=0; i<4; i++) {        
        printf("Partition type: %d\n", pt[i].partition_type);
        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No se encontró filesystem FAT12, saliendo ...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
    
    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  Bytes per sector: %d\n", bs.sector_size);
	// {...} COMPLETAR
	printf("  Sector per cluster (cluster_size): %d\n", bs.cluster_size);
	printf("  Size of reserved area, in sectors(number_of_reserved_sectors): %d\n", bs.reserved_sectors);
	printf("  Number of fats: %d\n", bs.num_fats);
	printf("  Maximun number of files in the root directory (number_of_root_entries): %d\n", bs.num_root_entries);
	printf("  Number of sectors in the file system: %d\n", bs.num_sectors);
	printf("  Media type: %d\n", bs.media_type);
	printf("  Size of each FAT, in sector (fat_size_sectors): %d\n", bs.fat_size_sectors);
	printf("  Sector per track in storage device(sectors_per_track): %d\n", bs.sectors_per_track);
	printf("  Number of heads in storage device (num_heads): %d\n", bs.num_heads);
	printf("  Number of serctors before the star partition (hidden_sectors): %d\n", bs.hidden_sectors);
	printf("  Number of the sectors in the file system (total_sectors): %d\n", bs.total_sectors);
	printf("  line-36 drive_number: %d\n", bs.drive_number);
	printf("  current_head: %d\n", bs.current_head);
	printf("  signature: %d\n", bs.signature);

	//
    printf("  volumen serial number (volume_id): 0x%08X\n", bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Signature value (Boot sector signature): 0x%04X\n", bs.boot_sector_signature);
    
    fclose(in);
    return 0;
}
