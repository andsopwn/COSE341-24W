cmd_/home/pwn/a/Module.symvers := sed 's/\.ko$$/\.o/' /home/pwn/a/modules.order | scripts/mod/modpost -m -a  -o /home/pwn/a/Module.symvers -e -i Module.symvers   -T -
