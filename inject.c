#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

// 定义一个函数指针类型 MY_EXPORT_PROC是一个函数指针，返回值为DWORD，
typedef DWORD (*MY_EXPORT_PROC)();



int main(int argc, char *argv)
{
    int i;
    MY_EXPORT_PROC lpProcAdd,lpProcAdd1; 

    // 获得Dll模块句柄
    HMODULE hdll = LoadLibrary("dllmain.dll");

    if(hdll != NULL){
        // 获得指定导出函数的地址
        lpProcAdd = (MY_EXPORT_PROC) GetProcAddress(hdll, "test");
        //HOOKPROC hproc = (HOOKPROC)GetProcAddress(hdll, "KeyboardProc");
        // 判断是否成功
        if (NULL != lpProcAdd) 
        {
            //如果加载成功，则运行函数test
            (lpProcAdd) (); 
            //printf("Get KeyboardProc handle!\n");
            //MessageBox(NULL, TEXT("Get KeyboardProc handle!"), TEXT("Tips"), MB_OK);

        } else MessageBox(NULL, TEXT("Get KeyboardProc handle error!"), TEXT("Tips"), MB_OK);


        //unhook
        for (i = 0; i < 30; i++)
        {
            Sleep(2000);
        }

        //释放DLL之前先UnstallHook       
        //hproc = (HOOKPROC)GetProcAddress(hdll, "UnstallHook");
        lpProcAdd1 = (MY_EXPORT_PROC) GetProcAddress(hdll, "UnstallHook");
        // 判断是否成功
        if (NULL != lpProcAdd1) 
        {
            //如果加载成功，则运行函数Unstallhook
            (lpProcAdd1) (); 
        }

        // 释放DLL
        FreeLibrary(hdll); 


    }else MessageBox(NULL, TEXT("LoadLibrary dllmain.dll error!"), TEXT("Tips"), MB_OK);

    return 0;

}



