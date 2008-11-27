ifndef VERSION
VERSION = cross_x64
endif

all: 
	$(MAKE) -C bbKeys VERSION=$(VERSION)
	$(MAKE) -C bbLeanBar VERSION=$(VERSION)
	$(MAKE) -C bbLeanSkin VERSION=$(VERSION)
	
clean:
	$(MAKE) clean -C bbKeys VERSION=$(VERSION)
	$(MAKE) clean -C bbLeanBar VERSION=$(VERSION)
	$(MAKE) clean -C bbLeanSkin VERSION=$(VERSION)