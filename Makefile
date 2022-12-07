# Specify the kernel directory to use
KERNELDIR?=/lib/modules/$(shell uname -r)/build
# Need the absolute directory do the driver directory to build kernel modules
DRIVERDIR?=$(shell pwd)/src
# Where kernel drivers are going to be installed
MODULEDIR?=/lib/modules/$(shell uname -r)


# build and install driver
all: clean driver install

clean:
	@echo -e "\n\033[34m cleaning project\033[0m"
	@echo "========================================"
	$(MAKE) -C "$(KERNELDIR)" M="$(DRIVERDIR)" clean

# compile driver
driver:
	@echo -e "\n\033[34m build project\033[0m"
	@echo "========================================"
	$(MAKE) -C $(KERNELDIR) M=$(DRIVERDIR) modules

# install driver
install:
	@echo -e "\n\033[34m install driver\033[0m"
	@echo "====================================================="
	@cp -v $(DRIVERDIR)/*.ko $(MODULEDIR)
	@chown -v root:root $(MODULEDIR)/*.ko
	depmod

# uninstall driver
uninstall:
	@echo -e "\n\033[34m uninstall driver\033[0m"
	@echo "====================================================="
	@rm -fv $(MODULEDIR)/razer_nari_ultimate_driver.ko
