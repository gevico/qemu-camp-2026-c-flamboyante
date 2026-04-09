#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    // TODO: 在这里添加你的代码
    case ET_NONE:  type_str = "Unknown"; break;
    case ET_REL:   type_str = "Relocatable"; break;
    case ET_EXEC:  type_str = "Executable"; break;
    case ET_DYN:   type_str = "Shared Object/PIE"; break;
    case ET_CORE:  type_str = "Core"; break;
    default:       type_str = "Unknown"; break;    
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  // TODO: 在这里添加你的代码
  for (int i = 0; i < 2; i++) 
  { 
    fd = open(filepath[i], O_RDONLY);
    if (fd < 0) {
      perror("open");
      exit(1);
    }
    if (read(fd, &ehdr, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
      perror("read");
      close(fd);
      exit(1);
    }
    // flam: 先校验 ELF 魔数，避免把非 ELF 内容当成 ELF 头解析。
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "%s 不是 ELF 文件\n", filepath[i]);
      close(fd);
      exit(1);
    }
    close(fd);
    print_elf_type(ehdr.e_type);
  }
  
  return 0;
}
