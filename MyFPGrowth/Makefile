CC = g++ -lm -std=c++11

#SRCS = ../*.cpp
RO_SRCS = ./*.cpp

main:
	$(CC) -o $@ $(RO_SRCS)


#RO: $(RO_SRCS)
#	$(CC) -c $(RO_SRCS)

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	del main.exe *.o
else
	rm main *.o
endif