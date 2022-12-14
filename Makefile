all: twentyFourSolver sudokuSolver

twentyFourSolver: twentyFourSolver.cpp
	g++ -o twentyFourSolver twentyFourSolver.cpp

sudokuSolver: sudokuSolver.cpp
	g++ -o sudokuSolver sudokuSolver.cpp

clean:
	rm *.o