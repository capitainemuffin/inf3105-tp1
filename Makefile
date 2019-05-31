OPTIONS = -lm -std=c++11

a.out : SELS28049204.cpp
	g++ SELS28049204.cpp $(OPTIONS)

.PHONY : clean 

clean :
	rm -f *.out 
