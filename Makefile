all:compile

clean:
	rm -f c.out

compile: 
	gcc task3.c -o c.out 
