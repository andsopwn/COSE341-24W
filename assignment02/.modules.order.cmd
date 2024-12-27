cmd_/home/pwn/a/modules.order := {   echo /home/pwn/a/jiffies_module.ko;   echo /home/pwn/a/seconds_module.ko; :; } | awk '!x[$$0]++' - > /home/pwn/a/modules.order
