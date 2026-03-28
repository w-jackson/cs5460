.section .multiboot_header
.p2align 3 # 8 bytes alignment
header_start:
    .long 0xe85250d6 # magic number
    .long 0 # architecture (0 for i386)
    .long header_end - header_start # total header length

    .long -(0xe85250d6 + 0 + (header_end - header_start)) # checksum

    # required end tag
    .short 0 # type
    .short 0 # flags
    .short 8 # size
header_end:
