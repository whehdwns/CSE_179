objects= game.o
NVCC= nvcc              
opt= -O2 -g -G           
ARCH= -arch=sm_30        
LIBS=  
execname= game


#compile
$(execname): $(objects)
	$(NVCC) $(opt) -o $(execname) $(objects) $(LIBS) 

game.o: game.cu
	$(NVCC) $(opt) $(ARCH) -c game.cu

#clean Makefile
clean:
	rm -rf $(objects)
#end of Makefile

