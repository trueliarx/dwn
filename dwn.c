#pragma comment(linker, "/ENTRY:MyStart")
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(lib, "kernel32.lib")

#include <windows.h>
#include <wininet.h>

typedef HINTERNET (WINAPI *InternetOpenA_t)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
typedef HINTERNET (WINAPI *InternetOpenUrlA_t)(HINTERNET, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR);
typedef BOOL (WINAPI *InternetReadFile_t)(HINTERNET, LPVOID, DWORD, LPDWORD);
typedef BOOL (WINAPI *InternetCloseHandle_t)(HINTERNET);

void Log(const char* msg) {
    DWORD written;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, lstrlenA(msg), &written, NULL);
}

char* NextToken(char* src, char* dest, int destSize) {
    while (*src == ' ') src++;
    if (*src == '\0') return NULL;
    int i = 0;
    if (*src == '"') {
        src++;
        while (*src && *src != '"' && i < destSize - 1) dest[i++] = *src++;
        if (*src == '"') src++;
    } else {
        while (*src && *src != ' ' && i < destSize - 1) dest[i++] = *src++;
    }
    dest[i] = '\0';
    return src;
}

void MyStart(void) {
    char url[2048];
    char outFile[512];
    char skip[512];

    char* cmdLine = GetCommandLineA();
    char* p = NextToken(cmdLine, skip, sizeof(skip));
    if (!p) { Log("Usage: dwn.exe <URL> <OutputFile>\n"); ExitProcess(1); }
    p = NextToken(p, url, sizeof(url));
    if (!p) { Log("Usage: dwn.exe <URL> <OutputFile>\n"); ExitProcess(1); }
    p = NextToken(p, outFile, sizeof(outFile));
    if (!p) { Log("Usage: dwn.exe <URL> <OutputFile>\n"); ExitProcess(1); }

    HMODULE hWininet = LoadLibraryA("wininet.dll");
    if (!hWininet) { Log("Failed to load wininet.dll\n"); ExitProcess(1); }

    InternetOpenA_t pInternetOpenA = (InternetOpenA_t)GetProcAddress(hWininet, "InternetOpenA");
    InternetOpenUrlA_t pInternetOpenUrlA = (InternetOpenUrlA_t)GetProcAddress(hWininet, "InternetOpenUrlA");
    InternetReadFile_t pInternetReadFile = (InternetReadFile_t)GetProcAddress(hWininet, "InternetReadFile");
    InternetCloseHandle_t pInternetCloseHandle = (InternetCloseHandle_t)GetProcAddress(hWininet, "InternetCloseHandle");

    HINTERNET hInternet = pInternetOpenA("TinyDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hUrl = pInternetOpenUrlA(hInternet, url, NULL, 0,
        INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD |
        INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP, 0);

    if (hUrl) {
        HANDLE hFile = CreateFileA(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            char buffer[4096];
            DWORD bytesRead, bytesWritten;
            while (pInternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
            }
            CloseHandle(hFile);
            Log("Download complete.\n");
        }
        pInternetCloseHandle(hUrl);
    } else {
        Log("Download failed.\n");
    }
    pInternetCloseHandle(hInternet);
    ExitProcess(0);
}