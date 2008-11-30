all:
	$(MAKE) -C build32
	$(MAKE) -C build64

cmake:
	cmake build32
	cmake build64

doc:
#	$(MAKE) doc -C build32
	$(MAKE) doc -C build64

package:
	$(MAKE) package -C build32
	$(MAKE) package -C build64

package_source:
#	$(MAKE) package_source -C build32
	$(MAKE) package_source -C build64

clean:
	$(MAKE) clean -C build32
	$(MAKE) clean -C build64
