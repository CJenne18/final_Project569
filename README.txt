To compile code:
  -MPI:
  mpicc -g -Wall -o mpiOut MPI_Cracker.c
  mpiexec -n<proccesses> ./mpiOut
  Enter password in whitespace

  -Pthred
  gcc -g -Wall -o ptOut pthread_Cracker.c -lpthread
  ./ptOut -n<threads> 
  Enter password

 -Standard PWCracker
  gcc -Wall PwCracker.c
  ./aout
  Enter password

