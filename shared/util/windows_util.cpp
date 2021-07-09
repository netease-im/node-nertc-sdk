#include "windows_util.h"
#include <shlwapi.h>
#include <ShlObj.h>
#include <stdint.h>
#include <stdio.h>
// #pragma comment(lib, "shlwapi.lib")

static bool GetFileVersion(const wchar_t *file_path, WORD *major_version, WORD *minor_version, WORD *build_number, WORD *revision_number)
{
    DWORD handle, len;
    UINT buf_len;
    LPTSTR buf_data;
    VS_FIXEDFILEINFO *file_info;
    len = GetFileVersionInfoSizeW(file_path, &handle);
    if (0 == len)
        return false;

    buf_data = (LPTSTR)malloc(len);
    if (!buf_data)
        return false;

    if (!GetFileVersionInfoW(file_path, handle, len, buf_data))
    {
        free(buf_data);
        return false;
    }
    if (VerQueryValueW(buf_data, L"\\", (LPVOID *)&file_info, (PUINT)&buf_len))
    {
        *major_version = HIWORD(file_info->dwFileVersionMS);
        *minor_version = LOWORD(file_info->dwFileVersionMS);
        *build_number = HIWORD(file_info->dwFileVersionLS);
        *revision_number = LOWORD(file_info->dwFileVersionLS);
        free(buf_data);
        return true;
    }
    free(buf_data);
    return false;
}

static int GetNTDLLVersion()
{
    static int ret = 0;
    if (ret == 0)
    {
        wchar_t buf_dll_name[MAX_PATH] = {0};
        HRESULT hr = ::SHGetFolderPathW(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, buf_dll_name);
        if (SUCCEEDED(hr) && ::PathAppendW(buf_dll_name, L"ntdll.dll"))
        {
            WORD major_version, minor_version, build_number, revision_number;
            GetFileVersion(buf_dll_name, &major_version, &minor_version, &build_number, &revision_number);
            ret = major_version * 100 + minor_version;
        }
    }
    return ret;
}

static BOOL CALLBACK WindowsEnumerationHandler(HWND hwnd, LPARAM param)
{
    CaptureTargetInfoList *list =
        reinterpret_cast<CaptureTargetInfoList *>(param);

    // Skip windows that are invisible, minimized, have no title, or are owned,
    // unless they have the app window style set.
    HWND owner = GetWindow(hwnd, GW_OWNER);
    LONG exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (IsIconic(hwnd) || !IsWindowVisible(hwnd) ||
        (owner && !(exstyle & WS_EX_APPWINDOW)) || (exstyle & WS_EX_LAYERED))
    {
        return TRUE;
    }
    int len = GetWindowTextLength(hwnd);
    if (len == 0)
    {
        return TRUE;
    }

    // Skip the Program Manager window and the Start button.
    const size_t kClassLength = 256;
    WCHAR class_name[kClassLength];
    int class_name_length = GetClassNameW(hwnd, class_name, kClassLength);

    // Skip Program Manager window and the Start button. This is the same logic
    // that's used in Win32WindowPicker in libjingle. Consider filtering other
    // windows as well (e.g. toolbars).
    if (wcscmp(class_name, L"Progman") == 0 || wcscmp(class_name, L"Button") == 0)
        return TRUE;

    if (GetNTDLLVersion() >= 602 &&
        (wcscmp(class_name, L"ApplicationFrameWindow") == 0 ||
         wcscmp(class_name, L"Windows.UI.Core.CoreWindow") == 0))
    {
        return TRUE;
    }

    CaptureTargetInfo window;
    window.id = hwnd;

    const size_t kTitleLength = 500;
    WCHAR window_title[kTitleLength];
    // Truncate the title if it's longer than kTitleLength.
    GetWindowTextW(hwnd, window_title, kTitleLength);
    window.title = window_title;

    // Skip windows when we failed to convert the title or it is empty.
    if (window.title.empty())
        return TRUE;

    window.type = 1;
    list->push_back(window);

    return TRUE;
}

static bool GetCaptureWindowList(CaptureTargetInfoList *windows)
{
    CaptureTargetInfoList result;
    LPARAM param = reinterpret_cast<LPARAM>(&result);
    if (!EnumWindows(&WindowsEnumerationHandler, param))
        return false;
    for (auto e : result)
        windows->push_back(e);
    return true;
}

std::string wstring2string(std::wstring wstr)
{
    std::string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char *buffer = new char[len + 1];
    //宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值
    result.append(buffer);
    delete[] buffer;
    return result;
}

// Napi::Array enumerateWindows(const Napi::CallbackInfo &info)
CaptureTargetInfoList enumerateWindows()
{
    // Napi::Env env = info.Env();
    // Napi::Array ar = Napi::Array::New(env);
    CaptureTargetInfoList windows;
    /// for desktop
    windows.push_back({0, L"Desktop", RECT{0, 0, 0, 0}});
    /// for monitors
    EnumDisplayMonitors(
        nullptr, nullptr, [](HMONITOR hmon, HDC, LPRECT pRC, LPARAM lparam) {
            auto &monitors = *reinterpret_cast<CaptureTargetInfoList *>(lparam);
            wchar_t buf[100] = {0};
            swprintf_s(buf, L"Monitor:(%d,%d,%d,%d)", pRC->left, pRC->top, pRC->right, pRC->bottom);
            monitors.push_back({(HWND)hmon, buf, *pRC, 2});

            return TRUE;
        },
        reinterpret_cast<LPARAM>(&windows));
    /// for windows
    GetCaptureWindowList(&windows);
    return windows;
    // uint32_t i = 0;
    // for (auto w : windows)
    // {
    //     Napi::Object obj = Napi::Object::New(env);
    //     obj.Set("id", reinterpret_cast<int32_t>(w.id));
    //     obj.Set("title", wstring2string(w.title).c_str());
    //     obj.Set("left", w.rc.left);
    //     obj.Set("top", w.rc.top);
    //     obj.Set("right", w.rc.right);
    //     obj.Set("bottom", w.rc.bottom);
    //     ar[i++] = obj;
    // }
    // return ar;
}