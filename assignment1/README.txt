CMPUT 379 
Assignment 1
Chongyang YE
1264608

The goal of the program is trying to scan the memory
and find the accessability of each part of memory
set the memory can only read be 0, read/write be 1 and 
can not access be -1.

In order to run the program, user have to type 
"make" in terminal within the correct directionary
after this, user may type"make clean " to clean the 
executable file called memtest

The example out put should be following:


cye2@ug13:~/Desktop/379/assignment1>./memtest 
Chunk # 1 is not accessable from (nil)
length: 134512640
Chunk # 2 is readable only from 0x8048000
length: 8192
Chunk # 3 is readable and writable from 0x804a000
length: 4096
Chunk # 4 is not accessable from 0x804b000
length: 13815808
Chunk # 5 is readable and writable from 0x8d78000
length: 135168
Chunk # 6 is not accessable from 0x8d99000
length: 4001210368
Chunk # 7 is readable and writable from 0xf7573000
length: 4096
Chunk # 8 is readable only from 0xf7574000
length: 1720320
Chunk # 9 is not accessable from 0xf7718000
length: 4096
Chunk # 10 is readable only from 0xf7719000
length: 8192
Chunk # 11 is readable and writable from 0xf771b000
length: 16384
Chunk # 12 is not accessable from 0xf771f000
length: 212992
Chunk # 13 is readable and writable from 0xf7753000
length: 8192
Chunk # 14 is readable only from 0xf7755000
length: 139264
Chunk # 15 is readable and writable from 0xf7777000
length: 4096
Chunk # 16 is not accessable from 0xf7778000
length: 141799424
Chunk # 17 is readable and writable from 0xffeb3000
length: 139264
Chunk # 18 is not accessable from 0xffed5000
length: 1220608
There is/are 18 chunks in total

