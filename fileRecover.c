#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char starting_cluster[4];
    unsigned int length_sectors;
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size;
    unsigned char sector_cluster;
    unsigned short reserved_sectors;
    unsigned char number_of_fats;
    unsigned short root_dir_entries;
    unsigned short sector_volumen;
    unsigned char descriptor;
    unsigned short fat_size_sectors;
    unsigned short sector_track;
    unsigned short headers;
    unsigned int sector_hidden;
    unsigned int sector_partition;
    unsigned char physical_device;
    unsigned char current_header;
    unsigned char firm;
    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8]; 
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {
    unsigned char filename[11];
    unsigned char atributos;
    unsigned char reserved;
    unsigned char created_time_seconds;
    unsigned char created_time_hours_minutes_seconds[2];
    unsigned char created_day[2];
    unsigned char accessed_day[2];
    unsigned char cluster_highbytes_address[2];
    unsigned char written_time[2];
    unsigned char written_day[2];
    unsigned short cluster_lowbytes_address;
    unsigned int size_of_file;
} __attribute((packed)) Fat12Entry;

void print_file(Fat12Entry *entry, FILE *in, char *parametro){
	switch((entry->filename[0])) {
	case 0x00:
		return;
	case 0xE5:
		if((strstr(entry->filename, parametro) != NULL)) {
			printf("Recuperando archivo: [?%.7s.%.3s]\n", &entry->filename[1], &entry->filename[8]);
			unsigned char letra = 'B';
			unsigned int posicionInicial = ftell(in);
			unsigned int posicionPrimerByte = posicionInicial - 32;
			
			fseek(in, posicionPrimerByte, SEEK_SET);
			if(fwrite(&letra, sizeof(letra), 1, in) != 1) {
				printf(stderr,"Error al escribir el archivo");
			}
			fflush(in);
			fseek(in, posicionInicial, SEEK_SET);
		
			break;
		}	
		else {
			printf("El archivo [?%.7s.%.3s] no contiene el string %s\n", &entry->filename[1], &entry->filename[0], parametro);
		}
	case 0x05:
		return;
	case 0x2E:
		return;
	default:
		if(entry->atributos == 0x10){
			return;
		}
	}
}

int main(int argc, char** argv){
   
    int length = 0;
    if(argc > 1 && argv[1] != NULL) {
    	length = strlen(argv[1]);
    }
    
    char parametro[length];
    printf("Ingrese el nombre del archivo a recuperar: ");
    scanf("%s", parametro);
     

    FILE *in = fopen("test.img", "rb+");
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
	//{...} Leo boot sector
    
    printf("En  0x%lX, sector size %d, FAT size %d sectors, %d FATs\n\n", 
           ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);
           
    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR);
    
    printf("Root dir_entries %d \n", bs.root_dir_entries);
    
    
    for(int i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
         print_file(&entry, in, parametro);
    }
   
    printf("\nLeido Root directory, ahora en 0x%lX\n", ftell(in));
    fclose(in);
    return 0;

}
