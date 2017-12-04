Homework After-report

We need to multithread the nested for loops
that is the kernel of the program
So, copy the whole thing into a function 
that can be passed to pthread_create()
However, we need to make sure the output remains the same
so we need to first store the outputs into a 3D array
before printing. 
If we print in the function the output will be out of order
since the threads printing will run concurrently.

but now that that part of the main function is now
in its own function
nthreads and scene_t scene need to be declared outside the
main function so that he kernel function render_()
can access them

In order to later join the threads,
we need to keep the pthread_t in an array
We also need to pass into the multhreaded function
the thread number, so that we know which pixels
for this particular thread to process.

I split the threads to work on contiguous pieces with
  int start = threadnum*width/nthreads;
  int end = (threadnum+1)*width/nthreads;
so that there is spatial locality with the 3D array access


I didn't magically think up all these things
I just put the big chunk of code into a function
and did the pthread stuff
then looked at the errors like
scene isn't defined
nthreads isn't defined
and fixed them up

Also the output was fuzzed up so I realised
I needed to store each threads output in an
array first
then print

Makefile also needs an additional
-lpthread flag to link pthread.h

did 
make clean check &> make-log.txt
to record its output