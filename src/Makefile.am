objects  = bmpLoader.o
objects += Camera.o
objects += Floor.o
objects += Hair.o
objects += Light.o
objects += Mannequin.o
objects += Object3D.o
objects += offLoader.o
objects += shaderLoader.o
objects += errors.o

sources  = bmpLoader.cpp
sources += Camera.cpp
sources += Floor.cpp
sources += Hair.cpp
sources += kernel.cu
sources += Light.cpp
sources += Mannequin.cpp
sources += Object3D.cpp
sources += offLoader.cpp
sources += shaderLoader.cpp
sources += errors.cpp

cuda_objects  = kernel.o
cuda_objects += errors.o

cuda_sources  = kernel.cu
cuda_sources += errors.cpp

cxx_flags=-std=c++17 -O3 -Wall -Wextra -Werror -pedantic
cuda_cxx_flags=-O3 -Werror cross-execution-space-call -Wno-deprecated-gpu-targets
ld_flags=-lglut -lGL -lGLU -lcuda -lcudart

rapunzel: $(objects) $(cuda_objects)
	g++ $(cxx_flags) -o $@ $^ main.cpp $(ld_flags)

$(objects): $(sources)
	g++ $(cxx_flags) -c $^ $(ld_flags)

$(cuda_objects): $(cuda_sources)
	nvcc $(cuda_cxx_flags) -c $^

all-local: rapunzel
	mkdir -p ${abs_top_srcdir}/build
	rm -rf ${abs_top_srcdir}/build/rapunzel
	ln -s ${abs_top_srcdir}/src/rapunzel ${abs_top_srcdir}/build/rapunzel

clean-local:
	rm -rf *.o rapunzel

.NOTPARALLEL: rapunzel $(objects) $(sources) $(cuda_objects) $(cuda_sources)
