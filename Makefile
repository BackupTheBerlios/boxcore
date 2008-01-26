VERSION = cross_x64

all: 
	$(MAKE) -C blackbox VERSION=$(VERSION)
	
clean:
	$(MAKE) clean -C blackbox VERSION=$(VERSION)