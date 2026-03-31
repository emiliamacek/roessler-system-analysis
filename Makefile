cxx = g++

# sciezka do capd
capdconfig ?= capd-config

capdflags = $(shell ${capdconfig} --cflags)
capdlibs = $(shell ${capdconfig} --libs)

cxxflags += ${capdflags} -O2 -std=c++17 -Isrc -Iheaders

objdir = .obj
srcdir = src

# programy
progs = poincare-bifurcation-diagram upper-bound lower-bound
# utils
utils_obj = ${objdir}/utils.o

# regula dla wszystkich prog
all: $(progs)

# kazdy prog osobno z wlasnym cpp + utils, zeby nie bylo multiple def of main
$(progs): %: ${objdir}/%.o ${utils_obj}
	$(cxx) ${cxxflags} -o $@ $^ ${capdlibs}

# kompilacja .o z kazdego cpp
${objdir}/%.o: ${srcdir}/%.cpp
	@mkdir -p ${objdir}
	$(cxx) ${cxxflags} -c -o $@ $<

clean:
	rm -rf ${objdir} $(progs)
