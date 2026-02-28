My solution for parts 1-3 works correctly with elf_extra_credit. Though the relocations go through the global offset table, the type relocations are the same. So rather than relocating the symbols directly, they are relocated in the global offset table.

### For elf1
`
Relocation section '.rela.dyn' at offset 0x2b8 contains 3 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000309  000000000008 R_X86_64_RELATIVE                    201004
00000000031b  000000000008 R_X86_64_RELATIVE                    201000
000000000327  000000000008 R_X86_64_RELATIVE                    201008
`

### For elf_extra_credit
`
Relocation section '.rela.dyn' at offset 0x2b8 contains 3 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000200fe8  000000000008 R_X86_64_RELATIVE                    201018
000000200ff0  000000000008 R_X86_64_RELATIVE                    20101c
000000200ff8  000000000008 R_X86_64_RELATIVE                    201020
`