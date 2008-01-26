VERSION = cross_x64

all: 
	$(MAKE) -n -C blackbox VERSION=$(VERSION)
	
clean:
	$(MAKE) -n clean -C blackbox VERSION=$(VERSION)