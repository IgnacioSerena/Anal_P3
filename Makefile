#-----------------------
# IMPORTANTE: The values of the parameters for exercise?_test have to be conveniently adjusted.
# It is assumed that the name of the source files is exercise1.c, exercise2.c,...,exercise.h
#-----------------------

CC = gcc -ansi -pedantic
CFLAGS = -Wall
EXE = exercise1 exercise2 exercise3 exercise4 exercise5

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o sorting.o times.o permutations.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -o $@ $@.o sorting.o times.o permutations.o

permutations.o : permutations.c permutations.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

sorting.o : sorting.c sorting.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

 times.o : times.c times.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<
	
exercise1_test:
	@echo Running exercise1
	@valgrind --leak-check=full --track-origins=yes ./exercise1 -limInf 1 -limSup 5 -numN 10

exercise2_test:
	@echo Running exercise2
	@valgrind --leak-check=full --track-origins=yes ./exercise2 -size 5 -numP 5

exercise3_test:
	@echo Running exercise3
	@valgrind --leak-check=full --track-origins=yes ./exercise3 -size 5 -numP 5

exercise4_test:
	@echo Running exercise4
	@valgrind --leak-check=full --show-leak-kinds=all ./exercise4 -size 10 

exercise5_test:
	@echo Running exercise5
	./exercise5 -num_min 1 -num_max 10000 -incr 100  -numP 100 -outputFile exercise5.log 