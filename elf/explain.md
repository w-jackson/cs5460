Which instruction fails (or accesses wrong region of memory)?
- `mov    eax,DWORD PTR [rax]`

What address it is trying to access?
- The address of the global variable c (`0x201004`).

Why that address is invalid?
- This address assumes the program was loaded at `min_vaddress` and doesn't take into account the actual base address (`load_base`). If we don't relocate it'll either be unmapped or point to the wrong location in memory. 

How this relates to virtual addresses and loading location?
- Loading location is `load_base` and everything else needs to be offset from it. Relocation happens in virtual memory and is translated to physical after. Before relocation global variables have virtual addresses that assume the program was loaded at min_vaddress.
