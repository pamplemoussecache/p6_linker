#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>

#include "../MoarVM/src/gen/config.h"

typedef struct {
    int8_t magic_num[9];
    uint32_t version;
    uint32_t sc_offset;
    uint32_t num_scs;
    uint32_t op_offset;
    uint32_t num_ops;
    uint32_t frame_offset;
    uint32_t num_frames;
    uint32_t callsite_offset;
    uint32_t num_callsites;
    uint32_t str_heap_offset;
    uint32_t num_str_entries;
    uint32_t sc_data_offset;
    uint32_t sc_data_length;
    uint32_t bytecode_offset;
    uint32_t bytecode_length;
    uint32_t annotation_offset;
    uint32_t annotation_length;
    uint32_t hll_name;
    uint32_t main_frame;
    uint32_t library_load_frame;
    uint32_t deserial_frame;
} mbc_header;

typedef struct {

} str_heap;

int read_mbc_header(FILE* mbc_file, mbc_header* mbc) {
    fread(&(mbc->magic_num), sizeof(uint8_t), 8, mbc_file);
    mbc->magic_num[8] = '\0';
    char magic_test[] = "MOARVM\r\n\0";
    if (strcmp(mbc->magic_num,magic_test))
    {
        printf("Provided file does not contain an MBC binary\n");
        fclose(mbc_file);
        exit(0);
    }

    fread(&(mbc->version), sizeof(mbc->version), 1, mbc_file);
    printf("Version: %d\n", mbc->version);
    fread(&(mbc->sc_offset), sizeof(mbc->sc_offset), 1, mbc_file);
    printf("SC Dependency Offset: %d\n", mbc->sc_offset);
    fread(&(mbc->num_scs), sizeof(mbc->num_scs), 1, mbc_file);
    printf("Number of SC Entries: %d\n", mbc->num_scs);
    fread(&(mbc->op_offset), sizeof(mbc->op_offset), 1, mbc_file);
    printf("Extension OPs table offset: %d\n", mbc->op_offset);
    fread(&(mbc->num_ops), sizeof(mbc->num_ops), 1, mbc_file);
    printf("Number of Extension OP table entries: %d\n", mbc->num_ops);
    fread(&(mbc->frame_offset), sizeof(mbc->frame_offset), 1, mbc_file);
    printf("Frames data segment offset: %d\n", mbc->frame_offset);
    fread(&(mbc->num_frames), sizeof(mbc->num_frames), 1, mbc_file);
    printf("Number of frames in frames data segment: %d\n", mbc->num_frames);
    fread(&(mbc->callsite_offset), sizeof(mbc->callsite_offset), 1, mbc_file);
    printf("Callsites data segment offset: %d\n", mbc->callsite_offset);
    fread(&(mbc->num_callsites), sizeof(mbc->num_callsites), 1, mbc_file);
    printf("Number of callsites: %d\n", mbc->num_callsites);
    fread(&(mbc->str_heap_offset), sizeof(mbc->str_heap_offset), 1, mbc_file);
    printf("String heap offset: %d\n", mbc->str_heap_offset);
    fread(&(mbc->num_str_entries), sizeof(mbc->num_str_entries), 1, mbc_file);
    printf("Number of string heap entries: %d\n", mbc->num_str_entries);
    fread(&(mbc->sc_data_offset), sizeof(mbc->sc_data_offset), 1, mbc_file);
    printf("SC data segment offset: %d\n", mbc->sc_data_offset);
    fread(&(mbc->sc_data_length), sizeof(mbc->sc_data_length), 1, mbc_file);
    printf("Length of SC data segment: %d\n", mbc->sc_data_length);
    fread(&(mbc->bytecode_offset), sizeof(mbc->bytecode_offset), 1, mbc_file);
    printf("Bytecode segment offset: %d\n", mbc->bytecode_offset);
    fread(&(mbc->bytecode_length), sizeof(mbc->bytecode_length), 1, mbc_file);
    printf("Length of bytecode segment: %d\n", mbc->bytecode_length);
    fread(&(mbc->annotation_offset), sizeof(mbc->annotation_offset), 1, mbc_file);
    printf("Annotation segment offset: %d\n", mbc->annotation_offset);
    fread(&(mbc->annotation_length), sizeof(mbc->annotation_length), 1, mbc_file);
    printf("Length of annotation segment: %d\n", mbc->annotation_length);
    fread(&(mbc->hll_name), sizeof(mbc->hll_name), 1, mbc_file);
    printf("Index into string heap to get HLL name: %d\n", mbc->hll_name);
    fread(&(mbc->main_frame), sizeof(mbc->main_frame), 1, mbc_file);
    printf("Main entry point frame index + 1: %d\n", mbc->main_frame);
    fread(&(mbc->library_load_frame), sizeof(mbc->library_load_frame), 1, mbc_file);
    printf("Library load frame index + 1: %d\n", mbc->library_load_frame);
    fread(&(mbc->deserial_frame), sizeof(mbc->deserial_frame), 1, mbc_file);
    printf("Deserialization frame index + 1: %d\n", mbc->deserial_frame);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Input is of format 'linker <MBC file>'\n");
        exit(0);
    }
    FILE* mbc_file = fopen(argv[1], "r");
    if (NULL == mbc_file)
    {
        printf("Error opening MBC file\n");
        exit(0);
    }

    mbc_header mbc_head;
    if (read_mbc_header(mbc_file, &mbc_head)) {
        printf("Error reading content of MBC file\n");
        exit(0);
    }
    fclose(mbc_file);

    FILE* elf_file = fopen("hello_world", "w+");
    if (NULL == elf_file) {
        printf("Error opening ELF file\n");
        exit(0);
    }

    Elf64_Ehdr elf_head;
    elf_head.e_ident[EI_MAG0] = (unsigned char)ELFMAG0;
    elf_head.e_ident[EI_MAG1] = (unsigned char)ELFMAG1;
    elf_head.e_ident[EI_MAG2] = (unsigned char)ELFMAG2;
    elf_head.e_ident[EI_MAG3] = (unsigned char)ELFMAG3;
    elf_head.e_ident[EI_CLASS] = (unsigned char)ELFCLASS64; /* Always 64 bit ELF FIXME*/
    #ifdef MVM_BIGENDIAN
    elf_head.e_ident[EI_DATA] = (unsigned char)ELFDATA2MSB;
    #else
    elf_head.e_ident[EI_DATA] = (unsigned char)ELFDATA2LSB;
    #endif
    elf_head.e_ident[EI_VERSION] = (unsigned char)EV_CURRENT;
    elf_head.e_ident[EI_OSABI] = (unsigned char)ELFOSABI_NONE; /* Always UNIX System V ABI FIXME */
    elf_head.e_type = (Elf64_Half)ET_EXEC; /* Object file type */
    elf_head.e_machine = EM_X86_64; /* Architecture */
    elf_head.e_version = 0x1; /* Object file version */
    elf_head.e_entry = sizeof(elf_head) + 3*sizeof(Elf64_Shdr) + 41; /* Entry point virtual address */
    elf_head.e_phoff = 0; /*Program header table file offset */
    elf_head.e_shoff = sizeof(elf_head);    /* Section header table file offset */
    elf_head.e_flags = 0x0;		/* Processor-specific flags */
    elf_head.e_ehsize = sizeof(elf_head);		/* ELF header size in bytes */
    elf_head.e_phentsize = sizeof(Elf64_Phdr);		/* Program header table entry size */
    elf_head.e_phnum = 0;		/* Program header table entry count */
    elf_head.e_shentsize = sizeof(Elf64_Shdr);		/* Section header table entry size */
    elf_head.e_shnum = 3;		/* Section header table entry count */
    elf_head.e_shstrndx = 2;		/* Section header string table index */

    char* interp_sgmt = "/home/madeleine/sandbox/perl6_2/bin/perl6";

    Elf64_Shdr interp_hdr;
    interp_hdr.sh_name = 1;		/* Section name (string tbl index) */
    interp_hdr.sh_type = SHT_PROGBITS;		/* Section type */
    interp_hdr.sh_flags = SHF_ALLOC;		/* Section flags */
    interp_hdr.sh_addr = sizeof(elf_head) + 3*sizeof(Elf64_Shdr);		/* Section virtual addr at execution */
    interp_hdr.sh_offset = interp_hdr.sh_addr;		/* Section file offset */
    interp_hdr.sh_size = strlen(interp_sgmt);		/* Section size in bytes */
    interp_hdr.sh_link = 0;		/* Link to another section */
    interp_hdr.sh_info = 0;		/* Additional section information */
    interp_hdr.sh_addralign = 1;		/* Section alignment */
    interp_hdr.sh_entsize = 00;		/* Entry size if section holds table */

    char* text_sgmt =  "-e 'say \"Hello World!\";'";

    Elf64_Shdr text_hdr;
    text_hdr.sh_name = 9;		/* Section name (string tbl index) */
    text_hdr.sh_type = SHT_PROGBITS;		/* Section type */
    text_hdr.sh_flags = SHF_ALLOC+SHF_EXECINSTR;		/* Section flags */
    text_hdr.sh_addr = sizeof(elf_head) + 3*sizeof(Elf64_Shdr) + strlen(interp_sgmt);		/* Section virtual addr at execution */
    text_hdr.sh_offset = text_hdr.sh_addr;		/* Section file offset */
    text_hdr.sh_size = strlen(text_sgmt);		/* Section size in bytes */
    text_hdr.sh_link = 0;		/* Link to another section */
    text_hdr.sh_info = 0;		/* Additional section information */
    text_hdr.sh_addralign = 16;		/* Section alignment */
    text_hdr.sh_entsize = 0;		/* Entry size if section holds table */

    char* str_tab = "\0.interp\0.text\0.shstrtab\0";
    printf("%s\n", &str_tab[16]);

    Elf64_Shdr strtab_hdr;
    strtab_hdr.sh_name = 15;		/* Section name (string tbl index) */
    strtab_hdr.sh_type = SHT_STRTAB;		/* Section type */
    strtab_hdr.sh_flags = 0;		/* Section flags */
    strtab_hdr.sh_addr = text_hdr.sh_addr + strlen(text_sgmt);		/* Section virtual addr at execution */
    strtab_hdr.sh_offset = strtab_hdr.sh_addr;		/* Section file offset */
    strtab_hdr.sh_size = 25;		/* Section size in bytes */
    strtab_hdr.sh_link = 0;		/* Link to another section */
    strtab_hdr.sh_info = 0;		/* Additional section information */
    strtab_hdr.sh_addralign  = 1;		/* Section alignment */
    strtab_hdr.sh_entsize = 0;		/* Entry size if section holds table */

    
    int err= fwrite(&elf_head, 1, sizeof(elf_head),elf_file);
    printf("%d\n", err);
    err = fwrite(&interp_hdr, 1, sizeof(interp_hdr), elf_file);
    printf("%d\n", err);
    err = fwrite(&text_hdr, 1, sizeof(text_hdr), elf_file);
    printf("%d\n", err);
    err = fwrite(&strtab_hdr, 1, sizeof(strtab_hdr), elf_file);
    printf("%d\n", err);
    err = fwrite(interp_sgmt, 1, strlen(interp_sgmt), elf_file);
    printf("%d\n", err);
    err = fwrite(text_sgmt, 1, strlen(text_sgmt), elf_file);
    printf("%d\n", err);
    err = fwrite(str_tab, 1, 25, elf_file);
    printf("%d\n", err);
    printf("size: %d\n", sizeof(*elf_file));
    fclose(elf_file);
}