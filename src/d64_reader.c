#include "d64_reader.h"

long	calculate_offset(int track, int sector) {
	int sectors_before = (track - 1) * SECTORS_PER_TRACK;
	return (sectors_before + sector) * SECTOR_SIZE;
}

int	dump_prg(FILE *d64file, int track, int sector, char *prg_file) {
	unsigned char buffer[SECTOR_SIZE];
	FILE *prg = fopen(prg_file, "w+b");
	if (!prg) {
		printf("failed to create PRG file at \"%s\"\n", prg_file);
		return 0;
	}
	printf("PRG file created at \"%s\"\n", prg_file);
	while (track != 0) {
		long offset = calculate_offset(track, sector);
		fseek(d64file, offset, SEEK_SET);
		memset(buffer, 0, sizeof(buffer));
		fread(buffer, 1, SECTOR_SIZE, d64file);

		unsigned char next_track = buffer[0];
		unsigned char next_sector = buffer[1];

		for (int i = 2; i < SECTOR_SIZE; i++)
			fwrite(&buffer[i], 1, 1, prg);

		track = next_track;
		sector = next_sector;
	}
	fclose(prg);
	return 1;
}

void	gen_name(char *name, char *pre_name, unsigned size) {
	unsigned pre_size = strlen(pre_name);
	if (pre_size + sizeof(name) + 4 > 0x400) return;
	memset(name, 0, 0x400);
	memcpy(name, pre_name, pre_size);
	for (unsigned i = 0; i < size - 4; i++)
		name[pre_size + i] = (rand() % (90 - 65 + 1)) + 65;
	memcpy(name + (size - 4), ".prg", 4);
}

void	read_d64file(FILE *d64file) {
	unsigned char dir_sector[SECTOR_SIZE];
	long dir_offset = calculate_offset(DIR_TRACK, DIR_SECTOR);
	unsigned found = 0;

	if (fseek(d64file, dir_offset, SEEK_SET) < 0) {
		fclose(d64file);
		printf("invalid D64 disk image\n");
		return;
	}
	memset(dir_sector, 0, sizeof(dir_sector));
	unsigned chars_read = fread(dir_sector, 1, SECTOR_SIZE, d64file);
	if (!chars_read || chars_read != SECTOR_SIZE) {
		printf("invalid D64 disk image\n");
		fclose(d64file);
		return;
	}

	for (int i = 0; i < 8; i++) {
		unsigned entry_offset = i * 32;
		unsigned char file_type = dir_sector[entry_offset + 2];
		// type == 0x82 || 0x02 -> PRG file
		if ((file_type & 0x7) == 0x2) {
			int start_track = dir_sector[entry_offset + 3];
			int start_sector = dir_sector[entry_offset + 4];

			char prg_name[0x400];
			gen_name(prg_name, EXTRACT_LOCATION, 15);
			found += dump_prg(d64file, start_track, start_sector, prg_name);
		}
	}
	if (!found)
		printf("no PRG files found\n");
	fclose(d64file);
}

int	main(int c, char **v) {
	if (c != 2) {
		printf("usage: %s <d64 file>\n", v[0]);
		return 1;
	}
	srand(time(NULL));

	FILE* d64file = fopen(v[1], "rb");
	if (!d64file) {
		printf("failed to open file \"%s\"\n", v[1]);
		return 1;
	}

	read_d64file(d64file);
	return 0;
}
