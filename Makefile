OPTIONS = -lm -std=c++11

a.out : sofianeselaouiTP1.cpp
	g++ sofianeselaouiTP1.cpp $(OPTIONS)

.PHONY : clean 

clean :
	rm -f *.out 
