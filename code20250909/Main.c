#include "MyFile.h"

#include <stdio.h>

int main () {

    MyFile* file = _fopen("log.txt" , "a");
    
    // char* msg = "hello world\n";
    // _fwrite(file , msg , strlen(msg));

    int cnt = 10;
    while(cnt--)
    {
        char *msg = (char*)"hello myfile!!!";
        _fwrite(file, msg, strlen(msg));
        // MyFFlush(file);
        printf("buffer: %s\n", file->write_buffer);
        sleep(1);
    }
    _fclose(file);
    
    return 0;
}