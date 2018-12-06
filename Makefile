src = ./weighted_chain/main.cpp
obj = ./main.o
target = ./chain
deps = ./weighted_chain/Chain.h \
	./weighted_chain/KDTree.h \
	./weighted_chain/Node.h \
	./weighted_chain/Pair.h \
	./weighted_chain/Position.h

$(target): $(src) $(deps)
	g++ -c $(src)
	g++ -o $(target) $(obj)
	rm -f $(obj)

clean:
	rm -f $(target)