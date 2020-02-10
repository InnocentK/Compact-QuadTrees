CXX = g++
FLAGS = -gstabs+

all: proj3.out t1.out t2.out

t1.out: QTree.o QTNode.o BBox.o Point.o p3test01.cpp
	$(CXX) $(FLAGS)  QTree.o QTNode.o BBox.o Point.o p3test01.cpp -o t1.out

t2.out: QTree.o QTNode.o BBox.o Point.o p3test02.cpp
	$(CXX) $(FLAGS)  QTree.o QTNode.o BBox.o Point.o p3test02.cpp -o t2.out

proj3.out: QTree.o QTNode.o BBox.o Point.o driver.cpp
	$(CXX) $(FLAGS) QTree.o QTNode.o BBox.o Point.o driver.cpp -o proj3.out

QTree.o: QTree.cpp QTree.h QTNode.o BBox.o Point.o
	$(CXX) $(FLAGS) -c QTree.cpp

QTNode.o: QTNode.cpp QTNode.h BBox.o Point.o
	$(CXX) $(FLAGS) -c QTNode.cpp

BBox.o: BBox.cpp BBox.h Point.o
	$(CXX) $(FLAGS) -c BBox.cpp

Point.o: Point.cpp Point.h
	$(CXX) $(FLAGS) -c Point.cpp

run:
	./proj3.out

test1:
	./t1.out

test2:
	./t2.out

val:
	valgrind ./proj3.out

val1:
	valgrind ./t1.out

val2:
	valgrind ./t2.out

clean:
	rm *~

cleaner:
	rm *.o

cleanest:
	rm *~
	rm *.o