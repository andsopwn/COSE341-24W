cmd_/home/pwn/a/jiffies_module.ko := ld -r -m elf_x86_64 -z noexecstack --build-id=sha1  -T scripts/module.lds -o /home/pwn/a/jiffies_module.ko /home/pwn/a/jiffies_module.o /home/pwn/a/jiffies_module.mod.o;  true