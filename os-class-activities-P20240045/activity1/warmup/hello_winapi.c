/* hello_winapi.c */
#include <windows.h>

int main() {
    // 1. Get the Handle for Standard Output
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    const char *message = "Hello, my friend!\n";
    DWORD bytesToWrite = 19;
    DWORD bytesWritten = 0;

    // 2. Use WriteFile instead of write()
    // Parameters: Handle, Buffer, Bytes to Write, Pointer to receive count, Overlapped (NULL)
    WriteFile(hStdout, message, bytesToWrite, &bytesWritten, NULL);

    return 0;
}