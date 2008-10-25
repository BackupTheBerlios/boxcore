all:
	$(MAKE) -C build32
	$(MAKE) -C build64

clean:
	$(MAKE) clean -C build32
	$(MAKE) clean -C build64
