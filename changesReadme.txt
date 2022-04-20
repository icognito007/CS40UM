In this implementation of the code
we have changed the underlying architecture of the memory.

The memeory sequence no longer pointers to a segment struct
which in turn contains pointers to the array of instructions.
Instead the memeory sequence, contains pointers directly to 
the array instructions. In order to keep a record of the 
length of the array, the size of the array is stored in the
first slot of the newly created array. 

It has been observed that this has resulted in substantial
time savings. 