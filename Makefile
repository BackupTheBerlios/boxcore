all:
	$(MAKE) -s -C build32
	$(MAKE) -s -C build64

cmake:
	cmake build32
	cmake build64

doc:
#	$(MAKE) doc -C build32
	$(MAKE) doc -s -C build64

astyle:
	$(MAKE) astyle -s -C build64

package:
	$(MAKE) package -s -C build32
	$(MAKE) package -s -C build64

boxbar-package:
	mkdir -p packages
	cd packages; cpack --config ../build32/coreplugins/boxBar/CPackConfig.cmake
	cd packages; cpack --config ../build64/coreplugins/boxBar/CPackConfig.cmake

package_source:
#	$(MAKE) package_source -s -C build32
	$(MAKE) package_source -s -C build64

clean:
	$(MAKE) clean -s -C build32
	$(MAKE) clean -s -C build64
