all: WTF.c WTFserver.c
        gcc -pthread -lcrypto -O -g -o WTFserver WTFserver.c
        gcc -lcrypto -O -g -o WTF WTF.c

clean:
        rm ./WTFserver ./WTF ./*.o
           echo Clean done
