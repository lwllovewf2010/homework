#include <stdio.h>
#include <stddef.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

void *read_all (int *filesize) {
  void *data = NULL;
  int offset = 0;
  int size = 0;

  while ((size = fread (buffer, 1, sizeof (buffer), stdin)) > 0) {
    if ((data = realloc (data, offset + size)) == NULL)
      exit (-1);
    memcpy (data + offset, buffer, size);
    offset += size;
  }
  *filesize = offset;
  return data;
}


#define LOAD_ADDRESS 0x8048000

struct Binary {
  Elf32_Ehdr ehdr;
  Elf32_Phdr phdr;
  char code[];
};

int main (int argc, char *argv[]) {

  void *code;
  int code_size;

  struct Binary binary = {
    /* ELF HEADER */
    .ehdr = {
      /* general */
      .e_ident   = {
        ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3,
        ELFCLASS32, 
        ELFDATA2LSB,
        EV_CURRENT,
        ELFOSABI_LINUX,
      },
      .e_type    = ET_EXEC,
      .e_machine = EM_386,
      .e_version = EV_CURRENT,
      .e_entry   = LOAD_ADDRESS + (offsetof (struct Binary, code)),
      .e_phoff   = offsetof (struct Binary, phdr),
      .e_shoff   = 0,
      .e_flags   = 0,
      .e_ehsize   = sizeof (Elf32_Ehdr),
      /* program header */
      .e_phentsize = sizeof (Elf32_Phdr),
      .e_phnum     = 1,
      /* section header */
      .e_shentsize = sizeof (Elf32_Shdr),
      .e_shnum     = 0,
      .e_shstrndx  = 0
    },

    /* PROGRAM HEADER */
    .phdr = {
      .p_type   = PT_LOAD,
      .p_offset = 0,
      .p_vaddr = LOAD_ADDRESS,
      .p_paddr = LOAD_ADDRESS,
      .p_filesz = 0,
      .p_memsz = 0,
      .p_flags = PF_R | PF_X,
      .p_align = 0x1000
    }
  };

  if ((code = read_all (&code_size)) == NULL)
    return -1;

  /* fix program header */
  binary.phdr.p_filesz = sizeof (struct Binary) + code_size;
  binary.phdr.p_memsz = sizeof (struct Binary) + code_size;

  /* write binary */
  fwrite (&binary, sizeof (struct Binary), 1, stdout);
  fwrite (code, 1, code_size, stdout);

  free (code);

  return 0;
}

