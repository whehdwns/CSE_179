EXEC: collective
MPICC = mpicc

all :${EXEC}

collective: collective.c
	${MPICC} -o collective collective.c
clean:
	rm -rf *.exe