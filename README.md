# disable-aslr-on-mac
Small tool to run applications not compatible with ASLR mechanism (for MacOS)

## Building
You need to install X-Code to build this tool

To build aslr-wrapper:

`% cc -o aslr-wrapper main.c`

To build test application:

`% cc -o test-app main.c`

## Using

1. Check if ASLR is used:
    ```
    % ./test
    0x16f7537cc
    % ./test
    0x16f65f7cc
    ```
2. If the output is different then ASLR is used
3. Rename the original app an make the symlink:
    ```
    % mv test test.orig
    % ln -s aslr-wrapper test
    ```
4. Check again:
    ```
    % ./test
    0x16fdff7cc
    % ./test
    0x16fdff7cc
    ```
5. If the output is the same then ASLR is disabled
