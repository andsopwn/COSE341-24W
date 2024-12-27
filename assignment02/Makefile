# Makefile for building jiffies_module and seconds_module

# 빌드할 모듈 이름을 지정합니다.
obj-m += jiffies_module.o
obj-m += seconds_module.o

# 현재 사용 중인 커널의 빌드 디렉토리를 지정합니다.
KDIR := /lib/modules/$(shell uname -r)/build

# 현재 작업 디렉토리를 지정합니다.
PWD := $(shell pwd)

# 기본 타겟: 모든 모듈을 빌드합니다.
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# clean 타겟: 생성된 파일들을 정리합니다.
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
