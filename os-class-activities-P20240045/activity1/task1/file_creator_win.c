#include <windows.h>
#include <string.h>

int main() {
    const char *text = "Hello from Operating Systems class!\n";
    const char *confirm = "File created successfully!\n";
    DWORD bytesWritten;

    // 1. Open/create "output.txt" using CreateFile()
    HANDLE hFile = CreateFile(
        "output.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        const char *err = "Error creating file\n";
        WriteFile(GetStdHandle(STD_ERROR_HANDLE), err, strlen(err), &bytesWritten, NULL);
        return 1;
    }

    // 2. Write "Hello from Operating Systems class!\n" using WriteFile()
    WriteFile(hFile, text, strlen(text), &bytesWritten, NULL);

    // 3. Close the file using CloseHandle()
    CloseHandle(hFile);

    // 4. Print "File created successfully!\n" to the terminal using WriteFile()
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), confirm, strlen(confirm), &bytesWritten, NULL);

    return 0;
}