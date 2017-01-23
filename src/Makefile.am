bin_PROGRAMS = rapunzel

rapunzel_SOURCES  = bmpLoader.cpp
rapunzel_SOURCES += bmpLoader.h
rapunzel_SOURCES += Camera.cpp
rapunzel_SOURCES += Camera.h
rapunzel_SOURCES += Flor.cpp
rapunzel_SOURCES += Floor.h
rapunzel_SOURCES += Hair.cpp
rapunzel_SOURCES += Hair.h
rapunzel_SOURCES += HeadersAndDefines.h
rapunzel_SOURCES += kernel.cu
rapunzel_SOURCES += Light.cpp
rapunzel_SOURCES += Light.h
rapunzel_SOURCES += main.cpp
rapunzel_SOURCES += Mannequin.cpp
rapunzel_SOURCES += Mannequin.h
rapunzel_SOURCES += Object3D.cpp
rapunzel_SOURCES += Object3D.h
rapunzel_SOURCES += offLoader.cpp
rapunzel_SOURCES += offLoader.h
rapunzel_SOURCES += shaderLoader.cpp
rapunzel_SOURCES += shaderLoader.h
rapunzel_SOURCES += Vector2D.h
rapunzel_SOURCES += Vector3D.h
rapunzel_SOURCES += Vector4D.h
rapunzel_SOURCES += wglew.h

rapunzel_CXXFLAGS  = -O3 -g -Wall -Werror -Wpedantic -fpic

all-local: rapunzel
	mkdir -p ${abs_top_srcdir}/build
	rm -rf ${abs_top_srcdir}/build/rapunzel
	ln -s ${abs_top_srcdir}/src/rapunzel ${abs_top_srcdir}/build/rapunzel

clean-local:
	rm -rf *.o *.lo rapunzel
