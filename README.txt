Project 1 CS485G
Name: Christopher Will

Files being submitted:
combineFiles.c, Makefile, README.txt

Program Description:
This program is meant to read in any number of SPEC-FILE pairs from the command line, combine the files based on the SPEC, and send the output
to stdout. Valid specs are -a, -sN, and -eN (where N is some positive integer). Files may be binary or text.If the user enters -a they wish
for the entire file to be sent to stdout, where as -sN corresponds to the first N bytes of their file, and -eN is the last N bytes. The 
user may enter any number of SPEC-FILE pairs (including 0).The program will print to stderr and continue to examine the next SPEC-FILE pair if
an invalid SPEC or FILE is provided. If - is given as the file name then the program will pause and allow the user to enter no more than 100KB
of text to stdin, resuming after the user hits ctrl + D. This inputfrom stdin will then be sent to stdout based on the SPEC the user
provided(I implemented the -sN and -eN options as well). An example of how to run the program follows:
./combineFiles -a combineFiles -s10 someText.txt -e500 example.txt -a - > out.txt
If the user enters the special file - more than once only the input from the first time will be sent to stdout. If the user gives the last SPEC
but doesn't include a file then the program will give an appropriate error. 

Note about use of fseeko and ftello:
My program began to receive Segmentation fault errors when I tested it with 500 MB files. I found the following page:
http://stackoverflow.com/questions/14533836/large-file-support-not-working-in-c-programming
which recommended the use of fseeko and ftello instead of fseek and ftell, and also advised to use _FILE_OFFSET_BITS 64
so that I should be able to handle more than 32 bits from a file. And after making these changes I was able to handle larger
files

Bugs:
The max file size I gave with the -a option was 1 GB. And my program was able to copy all of this 1GB file successfully. 
However, at around 1.5 GB I start to receive Segmentation faults.

Testing:
I tested the following SPEC options:
-a, -s100, -s100000, -e100, -e10000, -a123, -s100s, -e100e, -, -abc
These tests gave valid options when the options were valid, else I received the expected error message.

I tested the following files:
combineFiles, example.txt, someText.txt. Which allowed me to test both .txt files and binary files
Additionally I used "fallocate -l 1G gentoo_root.img" from the command line to generate a 1 GB file
that I could use to test how my program responds to large files. I received the command from the following:
http://stackoverflow.com/questions/257844/quickly-create-a-large-file-on-a-linux-system

