OPTS=-g
SFML_OPTS=-lsfml-system -lsfml-window -lsfml-graphics

LIBDIR=lib
INCLUDEDIR=include

_FILES=Ray.o Camera.o
FILES=$(patsubst %,$(LIBDIR)/%,$(_FILES))


main: src/main.cpp $(FILES)
	g++ src/main.cpp -I $(INCLUDEDIR) $(FILES) $(OPTS) $(SFML_OPTS) -o bin/main

$(LIBDIR)/%.o : $(LIBDIR)/%.cpp $(INCLUDEDIR)/%.hpp
	g++ $(OPTS) $(SFML_OPTS) -c -I $(INCLUDEDIR) -o $@ $<

tests: src/tests.cpp
	g++ src/tests.cpp -o bin/tests $(OPTS) $(SFML_OPTS)