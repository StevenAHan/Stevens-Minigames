all: twentyFourSolver sudokuSolver spellcastSolver

twentyFourSolver: twentyFourSolver.cpp
	g++ -o twentyFourSolver twentyFourSolver.cpp

sudokuSolver: sudokuSolver.cpp
	g++ -o sudokuSolver sudokuSolver.cpp

spellcastSolver: spellcastSolver.cpp
	g++ -o spellcastSolver spellcastSolver.cpp

clean:
	rm *.o