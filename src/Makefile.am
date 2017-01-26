objects  = bmpLoader.o
objects += Camera.o
objects += Floor.o
objects += Hair.o
objects += Light.o
objects += Mannequin.o
objects += Object3D.o
objects += offLoader.o
objects += shaderLoader.o

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

cuda_objects = kernel.o

cuda_sources = kernel.cu

rapunzel: $(objects) $(cuda_objects)
	g++ -std=c++17 -O3 -o $@ $^ main.cpp -lglut -lGL -lGLU -lcuda -lcudart

$(objects): $(sources)
	g++ -std=c++17 -O3 -c $^ -lglut -lGL -lGLU -lcuda -lcudart

$(cuda_objects): $(cuda_sources)
	nvcc -c $^

all-local: rapunzel
	mkdir -p ${abs_top_srcdir}/build
	rm -rf ${abs_top_srcdir}/build/rapunzel
	ln -s ${abs_top_srcdir}/src/rapunzel ${abs_top_srcdir}/build/rapunzel

clean-local:
	rm -rf *.o rapunzel

.NOT_PARALLEL: rapunzel $(objects) $(sources) $(cuda_objects) $(cuda_sources)
