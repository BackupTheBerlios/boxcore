ifndef VERSION
VERSION = cross_x64
endif

all: 
	$(MAKE) -C blackbox VERSION=$(VERSION)
	$(MAKE) -C plugins VERSION=$(VERSION)
	
clean:
	$(MAKE) clean -C blackbox VERSION=$(VERSION)
	$(MAKE) clean -C plugins VERSION=$(VERSION)