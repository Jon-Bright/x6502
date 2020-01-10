#include "cpu.h"
#include "emu.h"
#include "functions.h"
#include "io.h"
#include "gui.h"
#include "opcodes.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() {
    printf("x6502: a simple 65C02/65C22 emulator\n");
    printf("usage: x6502 [OPTION]... FILE\n");
    printf("options:\n");
    printf("\t-b addr\t\tthe base address at which code will be loaded\n");
}

int main(int argc, char *argv[]) {
    int base_addr = 0;

    int c;
    while ((c = getopt(argc, argv, "hb:")) != -1) {
        switch (c) {
        case 'b':
            base_addr = atoi(optarg);
            break;

        case 'h':
            usage();
            return 0;

        case '?':
            if (optopt == 'b') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            usage();
            return -1;
        }
    }

    if (optind == argc) {
        printf("no input file specified.\n");
        usage();
        return -1;
    }

    FILE *in_f = fopen(argv[optind], "r");
    int b;
    int i = base_addr;
    cpu *m = new_cpu();
    while ((b = fgetc(in_f)) != EOF) {
        m->mem[i++] = (uint8_t) b;
    }
    fclose(in_f);
    set_pc(m, (m->mem[0xFFFD] << 8) | m->mem[0xFFFC]);
    main_loop(m);
    destroy_cpu(m);
    return 0;
}
