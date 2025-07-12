OPTS=-g
SFML_OPTS=-lsfml-system -lsfml-window -lsfml-graphics

LIBDIR=lib
INCLUDEDIR=include

#_FILES=
FILES=$(patsubst %,$(LIBDIR)/%,$(_FILES))

tests: src/tests.cpp
	g++ src/tests.cpp -o bin/tests $(OPTS) $(SFML_OPTS)