/* file_creator_sys.c */
#include <fcntl.h>    // open(), O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // write(), close()
#include <string.h>   // strlen()

int main() {
    // YOUR CODE HERE
    // 1. Open/create "output.txt" using open()
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if( fd < 0){
        write(1, "Error opening file\n", 19);
        return 1;
    }
    // 2. Write "Hello from Operating Systems class!\n" using write()
    const char *message = "Hello from Operating Systems class!\n";
    write(fd,message,30);
    // 3. Close the file using close()
    close(fd);
    // 4. Print "File created successfully!\n" to the terminal using write()
    const char *success_msg = "File created successfully!\n";
    write(1, success_msg, 20);
    return 0;
}