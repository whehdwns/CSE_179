objects= max.o
NVCC= nvcc              
opt= -O2 -g -G           
ARCH= -arch=sm_30        
LIBS=  
execname= max


#compile
$(execname): $(objects)
	$(NVCC) $(opt) -o $(execname) $(objects) $(LIBS) 

max.o: max.cu
	$(NVCC) $(opt) $(ARCH) -c max.cu

#clean Makefile
clean:
	rm -rf $(objects)
#end of Makefile

