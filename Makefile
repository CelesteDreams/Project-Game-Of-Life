all:compile

clean:
	rm -f a.out

compile: 
	gcc gameoflife.c -o a.out 


