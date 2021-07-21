#include "windows_helper.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include <WinUser.h>
#include <iterator>
#include <algorithm>
#include <dwmapi.h>
#include <processthreadsapi.h>
#include <shlobj.h> // SHGetFolderPathW
#include <shlwapi.h>
#include <shellapi.h>
#include <wingdi.h>

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Dwmapi.lib")

std::wstring WindowsHelpers::m_strCurrentExe = L"";

//不要忘记使用完char*后delete[]释放内存
char *wideCharToMultiByte(wchar_t *pWStrSrc)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    int size = WideCharToMultiByte(CP_OEMCP, 0, pWStrSrc, wcslen(pWStrSrc), NULL, 0, NULL, NULL);
    char *pStrDest = new char[size + 1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWStrSrc, wcslen(pWStrSrc), pStrDest, size, NULL, NULL);
    pStrDest[size] = '\0';
    return pStrDest;

    //如果想要转换成string，直接赋值即可
    // string pKey = pCStrKey;
}

std::string wideCharToString(wchar_t *pWStrSrc)
{
    if (nullptr == pWStrSrc)
    {
        return "";
    }

    char *pStr = wideCharToMultiByte(pWStrSrc);
    std::string strTmp = pStr;

    delete[] pStr;
    return strTmp;
}

//不要忘记在使用完wchar_t*后delete[]释放内存
wchar_t *multiByteToWideChar(const std::string &strSrc)
{
    char *pStrDest = const_cast<char *>(strSrc.c_str());
    //第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
    int size = MultiByteToWideChar(CP_OEMCP, 0, pStrDest, strlen(pStrDest) + 1, NULL, 0);
    wchar_t *pWStrDest = new wchar_t[size];
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pStrDest, strlen(pStrDest) + 1, pWStrDest, size);
    return pWStrDest;
}

WindowsHelpers::WindowsHelpers()
{
    m_pPrintCaptureHelper = new PrintCaptureHelper();

    static bool bInit = false;
    if (!bInit)
    {
        bInit = true;

        HANDLE hToken;
        BOOL fOk = FALSE;
        DWORD dwRet = 0;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        {
            TOKEN_PRIVILEGES tp;
            tp.PrivilegeCount = 1;
            LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
            dwRet = GetLastError();
            fOk = (dwRet == ERROR_SUCCESS);
            CloseHandle(hToken);
        }
        if (!fOk)
        {
            // YXLOG(Info) << "OpenProcessToken failed: GetLastError = " << dwRet;
        }
    }
}

WindowsHelpers::~WindowsHelpers()
{
    if (m_pPrintCaptureHelper)
    {
        delete m_pPrintCaptureHelper;
        m_pPrintCaptureHelper = nullptr;
    }
}

bool WindowsHelpers::getFileVersion(const wchar_t *file_path, WORD *major_version, WORD *minor_version, WORD *build_number, WORD *revision_number)
{
    DWORD handle = 0, len = 0;
    UINT buf_len = 0;
    LPTSTR buf_data = nullptr;
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

int WindowsHelpers::getNTDLLVersion()
{
    static int ret = 0;
    if (ret == 0)
    {
        wchar_t buf_dll_name[MAX_PATH] = {0};
        HRESULT hr = ::SHGetFolderPathW(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, buf_dll_name);
        if (SUCCEEDED(hr) && ::PathAppendW(buf_dll_name, L"ntdll.dll"))
        {
            WORD major_version, minor_version, build_number, revision_number;
            getFileVersion(buf_dll_name, &major_version, &minor_version, &build_number, &revision_number);
            ret = major_version * 100 + minor_version;
        }
    }
    return ret;
}

BOOL CALLBACK WindowsEnumerationHandler(HWND hwnd, LPARAM param)
{
    WindowsHelpers::CaptureTargetInfoList *list = reinterpret_cast<WindowsHelpers::CaptureTargetInfoList *>(param);
    if (nullptr == list)
    {
        // YXLOG(Info) << "list is nullptr" << YXLOGEnd;;
        return TRUE;
    }

    const size_t kTitleLength = 500;
    WCHAR window_title[kTitleLength] = {0};

    const size_t kClassLength = 256;
    WCHAR class_name[kClassLength] = {0};
    if (::IsWindow(hwnd) && ::IsWindowVisible(hwnd) && (::GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW)
    {
        int class_name_length = GetClassNameW(hwnd, class_name, kClassLength);
        if (0 != class_name_length)
        {
            GetWindowTextW(hwnd, window_title, kTitleLength);
            if (wcscmp(class_name, L"TXGuiFoundation") == 0 && window_title == L"腾讯视频")
            {
            }
            else
            {
#ifndef _WIN64
                if (::GetWindowLongW(hwnd, GWL_HWNDPARENT) != 0)
                    return TRUE;
#endif
            }
        }
    }
    else
    {
        return TRUE;
    }

    if (wcscmp(class_name, L"Progman") == 0 || wcscmp(class_name, L"Button") == 0)
        return TRUE;

    if (WindowsHelpers::getNTDLLVersion() >= 602 &&
        (wcscmp(class_name, L"ApplicationFrameWindow") == 0 || wcscmp(class_name, L"Windows.UI.Core.CoreWindow") == 0))
    {
        // YXLOG(Info) << "WindowsHelpers::getNTDLLVersion()" << YXLOGEnd;
        return TRUE;
    }

    DWORD dwPID; //保存进程标识符
    GetWindowThreadProcessId(hwnd,
                             &dwPID);                                //接受一个窗口句柄。dwPID保存窗口的创建者的进程标识符，GetWindowThreadProcessId返回值是该创建者的线程标识符
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //打开一个已存在的进程对象,并返回进程的句柄，这就是我们要的进程句柄了
    // YXLOG(Info) << "hProcess: " << hProcess << YXLOGEnd;
    WCHAR exePath[256] = {0};
    if (NULL != hProcess)
    {
        //获取程序的path，并保存到exePath
        DWORD dwPathNameSize = sizeof(exePath);
        if (TRUE != QueryFullProcessImageNameW(hProcess, 0, exePath, &dwPathNameSize))
        {
            CloseHandle(hProcess);
            // YXLOG(Info) << "QueryFullProcessImageName GetLastError: " << GetLastError() << YXLOGEnd;
            return TRUE;
        }

        CloseHandle(hProcess);
        std::wstring strTemp = exePath;
        // YXLOG(Info) << "exePath: " << wideCharToString(exePath) << YXLOGEnd;
        // 过滤当前的exe
        if (0 == _wcsicmp(strTemp.c_str(), WindowsHelpers::getCurrentExe().c_str()))
        {
            return TRUE;
        }
    }
    else
    {
        // YXLOG(Info) << "OpenProcess GetLastError: " << GetLastError() << YXLOGEnd;
    }

    WindowsHelpers::CaptureTargetInfo window;
    window.id = hwnd;
    window.title = window_title;
    window.isMinimizeWindow = IsIconic(hwnd);
    window.type = 2;

    if (!(wcscmp(exePath, L"") == 0))
    {
        std::string strExePath = wideCharToString(exePath);
        window.app = strrchr(strExePath.c_str(), '\\') + 1;
    }

    // YXLOG(Info) << "exe: " << window.app << ", window_title: " << wideCharToString(window_title) << YXLOGEnd;
    // Skip windows when we failed to convert the title or it is empty.
    if (window.title.empty())
    {
        // LOG(INFO) << "window.title.empty";
        return TRUE;
    }

    list->push_back(window);

    return TRUE;
}

std::wstring WindowsHelpers::getCurrentExe()
{
    if (m_strCurrentExe == L"")
    {
        WCHAR szBuffer[MAX_PATH] = {0};
        GetModuleFileNameW(NULL, szBuffer, MAX_PATH);
        m_strCurrentExe = szBuffer;
    }

    return m_strCurrentExe;
}

bool WindowsHelpers::isWindow(HWND hWnd) const
{
    return TRUE == IsWindow(hWnd);
}

bool WindowsHelpers::isMinimized(HWND hWnd) const
{
    return TRUE == IsIconic(hWnd);
}

bool WindowsHelpers::isZoomed(HWND hWnd) const
{
    return TRUE == IsZoomed(hWnd);
}

bool WindowsHelpers::isHide(HWND hWnd) const
{
    return FALSE == IsWindowVisible(hWnd);
}

RECT WindowsHelpers::getWindowRect(HWND hWnd) const
{
    RECT rect = {0, 0, 0, 0};
    BOOL bRet = GetWindowRect(hWnd, &rect);
    if (TRUE != bRet)
    {
        // YXLOG(Info) << "getWindowRect failed. GetLastError: " << GetLastError();
    }
    return rect;
}

HBITMAP WindowsHelpers::getWindowImage(HWND hWnd) const
{
    if (!m_pPrintCaptureHelper)
    {
        m_pPrintCaptureHelper->Cleanup();
        m_pPrintCaptureHelper->Init(hWnd);
        HBITMAP hBitmap = m_pPrintCaptureHelper->GetBitmap();
        return hBitmap;
    }

    return HBITMAP();
}

HICON WindowsHelpers::getWindowIcon(HWND hWnd) const
{
    HICON pixmap = nullptr;
    if (TRUE != IsWindow(hWnd))
    {
        return pixmap;
    }
    //获取程序的path，并保存到exePath
    DWORD dwPID; //保存进程标识符
    GetWindowThreadProcessId(hWnd,
                             &dwPID);                                //接受一个窗口句柄。dwPID保存窗口的创建者的进程标识符，GetWindowThreadProcessId返回值是该创建者的线程标识符
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //打开一个已存在的进程对象,并返回进程的句柄，这就是我们要的进程句柄了
    if (NULL == hProcess)
    {
        return pixmap;
    }
    WCHAR exePath[256];
    memset(exePath, 0, 256);
    DWORD dwPathNameSize = sizeof(exePath);
    if (TRUE != QueryFullProcessImageNameW(hProcess, 0, exePath, &dwPathNameSize))
    {
        CloseHandle(hProcess);
        // YXLOG(Info) << "GetLastError: " << GetLastError();
        return pixmap;
    }
    CloseHandle(hProcess);
    std::wstring strTemp = exePath;

    SHFILEINFOW sfiTemp;
    ZeroMemory(&sfiTemp, sizeof(sfiTemp));
    SHGetFileInfoW(strTemp.c_str(), FILE_ATTRIBUTE_NORMAL, &sfiTemp, sizeof(SHFILEINFOW), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_LARGEICON);
    if (NULL != sfiTemp.hIcon)
    {
        pixmap = sfiTemp.hIcon;
        DestroyIcon(sfiTemp.hIcon);
    }

    return pixmap;
}

bool WindowsHelpers::getCaptureWindowList(CaptureTargetInfoList *windows, int type)
{
    if (nullptr == windows)
    {
        return false;
    }
    CaptureTargetInfoList result;

    if (type == 0 || type == 1)
    {
        bool ret = EnumDisplayMonitors(
            nullptr, nullptr, [](HMONITOR hmon, HDC hdc, LPRECT pRC, LPARAM lparam)
            {
                auto &monitors = *reinterpret_cast<CaptureTargetInfoList *>(lparam);
                wchar_t buf[100] = {0};
                // int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
                // int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
                // int cx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                // int cy = GetSystemMetrics(SM_CYVIRTUALSCREEN);
                // MONITORINFOEXW infoEx;
                // memset(&infoEx, 0, sizeof(infoEx));
                // infoEx.cbSize = sizeof(infoEx);
                // if (GetMonitorInfoW(hmon, &infoEx))
                // {
                // swprintf_s(buf, L"screen:%s-(%d,%d,%d,%d)-(%d,%d,%d,%d)", infoEx.szDevice, pRC->left, pRC->top, pRC->right, pRC->bottom, x, y, cx, cy);
                swprintf_s(buf, L"screen: %d", monitors.size() + 1);
                // }
                monitors.push_back({(HWND)hmon, buf, "", 1, false, *pRC});

                return TRUE;
            },
            reinterpret_cast<LPARAM>(&result));
        if (type == 1)
        {
            if (ret)
                std::copy(result.begin(), result.end(), std::back_inserter(*windows));
            return ret;
        }
    }

    if (type == 0 || type == 2)
    {
        LPARAM param = reinterpret_cast<LPARAM>(&result);
        if (EnumWindows(WindowsEnumerationHandler, param))
        {
            std::copy(result.begin(), result.end(), std::back_inserter(*windows));
            return true;
        }
    }

    return false;
}

void WindowsHelpers::setForegroundWindow(HWND hWnd) const
{
    if (GetForegroundWindow() == hWnd)
    {
        //  YXLOG(Info) << "GetForegroundWindow() == hWnd.";
        //return;
    }

    if (::IsIconic(hWnd))
    {
        BOOL bRet = ::ShowWindow(hWnd, SW_RESTORE);
        if (TRUE != bRet)
        {
            // YXLOG(Info) << "ShowWindow failed(SW_RESTORE). GetLastError: " << GetLastError();
        }
    }
    else
    {
        if (!::IsWindowVisible(hWnd))
        {
            BOOL bRet = ::ShowWindow(hWnd, SW_SHOW);
            if (TRUE != bRet)
            {
                // YXLOG(Info) << "ShowWindow failed(SW_SHOW). GetLastError: " << GetLastError();
            }
        }
    }

    BOOL bRet = SetForegroundWindow(hWnd);
    if (TRUE != bRet)
    {
        // YXLOG(Info) << "SetForegroundWindow failed. GetLastError: " << GetLastError();
    }
}

void WindowsHelpers::sharedOutsideWindow(HWND wid, HWND hWnd, bool bFullScreen)
{
    if (bFullScreen)
    {
        BOOL bRet = SetWindowPos(wid, hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
        if (TRUE != bRet)
        {
            // YXLOG(Info) << "SetWindowPos failed. GetLastError: " << GetLastError();
        }
        bRet = SetWindowPos(hWnd, wid, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
        if (TRUE != bRet)
        {
            // YXLOG(Info) << "SetWindowPos failed. GetLastError: " << GetLastError();
        }
    }
    else
    {
        BOOL bRet = SetWindowPos(wid, hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
        if (TRUE != bRet)
        {
            // YXLOG(Info) << "SetWindowPos failed. GetLastError: " << GetLastError();
        }
    }
}

void WindowsHelpers::setWindowTop(HWND wid)
{
    BOOL bRet = SetWindowPos(wid, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    if (TRUE != bRet)
    {
        // YXLOG(Info) << "SetWindowPos failed. GetLastError: " << GetLastError();
    }
}

RECT WindowsHelpers::getWindowFrameRect(HWND hWnd) const
{
    RECT rect;
    DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));
    return rect;
}

void WindowsHelpers::getTaskbarInfo(int &width,
                                    int &height,
                                    int &pos)
{ //获取系统任务栏状态位置有四种情况：左、上、右、下，此外要考虑任务栏自动隐藏的情况
    int wx = GetSystemMetrics(SM_CXSCREEN);
    int wy = GetSystemMetrics(SM_CYSCREEN);
    RECT rtWorkArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);

    int cx = rtWorkArea.right - rtWorkArea.left;
    int cy = rtWorkArea.bottom - rtWorkArea.top;

    // 1.任务栏停靠在左边情况
    if (0 != rtWorkArea.left)
    {
        width = wx - cx;
        height = wy;
        pos = 0;
        return;
    }
    // 2.任务栏停靠在上边情况
    if (0 != rtWorkArea.top)
    {
        width = wx;
        height = wy - cy;
        pos = 1;
        return;
    }
    // 3.任务栏停靠在右边情况
    if (0 == rtWorkArea.left && wx != cx)
    {
        width = wx - cx;
        height = wy;
        pos = 2;
        return;
    }
    // 4.任务栏停靠在下边情况
    if (0 == rtWorkArea.top && wy != cy)
    {
        width = wx;
        height = wy - cy;
        pos = 3;
        return;
    }
    // 5.任务栏自动隐藏的情况，这样其宽高都是0
    if (0 == rtWorkArea.left && 0 == rtWorkArea.top && wx == cx && wy == cy)
    {
        width = 0;
        height = 0;
        pos = 4;
        return;
    }
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    (void)hdcMonitor;
    (void)lprcMonitor;
    static BOOL first = TRUE; //标志
    std::vector<RECT> *pRect = (std::vector<RECT> *)dwData;
    //保存显示器信息
    MONITORINFO monitorinfo;
    monitorinfo.cbSize = sizeof(MONITORINFO);

    //获得显示器信息，将信息保存到monitorinfo中
    GetMonitorInfoW(hMonitor, &monitorinfo);
    //若检测到主屏
    if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
    {
        if (first) //第一次检测到主屏
        {
            first = FALSE;
            pRect->push_back(monitorinfo.rcMonitor);
        }
        else //第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
        {
            first = TRUE; //标志复位
            return FALSE; //结束检测
        }
    }
    else
    {
        pRect->push_back(monitorinfo.rcMonitor);
    }
    first = TRUE; // 恢复主屏标记为初始状态
    return TRUE;
}

//根据屏幕上指定的坐标点，获取坐标点窗口对应的pid
DWORD WindowsHelpers::getPidByPoint(int nX, int nY)
{
    POINT lpt = {nX, nY};
    HWND hwnd = (HWND)WindowFromPoint(lpt);
    HWND lHdesktop = (HWND)GetDesktopWindow();

    while (1)
    {
        // 查找窗口的主窗口
        HWND lHparent = ::GetParent(hwnd);
        if (lHparent == lHdesktop || lHparent == 0)
            break;
        hwnd = lHparent;
    }

    DWORD dwPid = 0;
    GetWindowThreadProcessId(hwnd, &dwPid);
    return dwPid;
}

//根据屏幕上指定的坐标点，获取坐标点窗口对应的HWND
HWND WindowsHelpers::getHwndByPoint(int nX, int nY)
{
    POINT lpt = {nX, nY};
    HWND hwnd = (HWND)WindowFromPoint(lpt);
    HWND lHdesktop = (HWND)GetDesktopWindow();

    while (1)
    {
        // 查找窗口的主窗口
        HWND lHparent = ::GetParent(hwnd);
        if (lHparent == lHdesktop || lHparent == 0)
            break;
        hwnd = lHparent;
    }

    return hwnd;
}

//查找全屏的应用窗口
BOOL WindowsHelpers::findFullScreenWindow(DWORD &dwProcessID, std::string &strProcessName, HWND &hWnd, bool &bPowerpnt)
{
    bPowerpnt = false;
    // 检测显示器数量
    std::vector<RECT> vRect;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&vRect); // 枚举所有显示器的Rect

    /*
   这个函数获取屏幕4角的窗口的进程句柄，判断与激活句柄是否相等的方式来判断是否全屏程序。
   特别的，对 IE 之类的多标签多进程程序，子窗口的进程会和主窗口不同。需要获取窗口的主窗口来对比才可以
   */
    // bool lbRet = false;
    // HWND lHforewnd = (HWND)0x430a4a;// ::GetForegroundWindow();
    // DWORD lWDProcessID;
    // GetWindowThreadProcessId(lHforewnd, &lWDProcessID);

    std::vector<RECT>::iterator itor = vRect.begin();
    for (; itor != vRect.end(); ++itor)
    {
        int nLeftPoint = itor->left;
        int nTopPoint = itor->top;
        int nRightPoint = itor->right;
        int nBottomPoint = itor->bottom;

        // 左上
        HWND hLTPid = getHwndByPoint(nLeftPoint, nTopPoint);
        // 右下
        HWND hRBPid = getHwndByPoint(nRightPoint - 70, nBottomPoint - 70);
        // YXLOG(Info) << "hLTPid, hRBPid: " << hLTPid << ", " << hRBPid << YXLOGEnd;
        // 找到全屏应用
        if (hLTPid == hRBPid)
        {
            hWnd = hRBPid;
            GetWindowThreadProcessId(hWnd, &dwProcessID);
            strProcessName = getModuleNameByPid(dwProcessID);

            std::string strDst;
            //转换成小写
            std::transform(strProcessName.begin(), strProcessName.end(), std::back_inserter(strDst), ::tolower);
            if (strDst == "wpp.exe" || strDst == "powerpnt.exe")
            {
                // YXLOG(Info) << strDst.c_str() << "%s is playing ..." << YXLOGEnd;
                if (strDst == "powerpnt.exe")
                {
                    bPowerpnt = true;
                }
                return true;
            }
        }

        /*
        // 左上
        DWORD dwLTPid = getPidByPoint(nLeftPoint, nTopPoint);
        // 右下
        DWORD dwRBPid = getPidByPoint(nRightPoint - 70, nBottomPoint - 70);
        // YXLOG(Info) << "dwLTPid, dwRBPid: " << dwLTPid << ", " << dwRBPid << YXLOGEnd;
        //找到全屏应用
        if (dwLTPid == dwRBPid)
        {
            dwProcessID = dwLTPid;
            strProcessName = getModuleNameByPid(dwProcessID);
            POINT lpt = { nLeftPoint, nTopPoint };
            hWnd = (HWND)WindowFromPoint(lpt);
            return true;
        }
        */
    }

    return false;
}

std::string WindowsHelpers::getModuleName(HWND hWnd)
{
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hWnd, &dwPid);
    return getModuleNameByPid(dwPid);
}

bool WindowsHelpers::getForegroundWindow(HWND hWnd) const
{
    return GetForegroundWindow() == hWnd;
}

bool WindowsHelpers::getActiveWindow(HWND hWnd) const
{
    return GetActiveWindow() == hWnd;
}

bool WindowsHelpers::getFocusWindow(HWND hWnd) const
{
    return GetFocus() == hWnd;
}

// bool WindowsHelpers::getDisplayRect(HWND hWnd, QRectF& rect, QRectF& availableRect, bool bPpt) const {
//     QPointF point = getWindowRect(hWnd).center();
//     QList<QScreen*> screens = QGuiApplication::screens();
//     if (bPpt && screens.size() > 1) {
//         rect = screens.at(1)->geometry();
//         availableRect = screens.at(1)->availableGeometry();
//         return true;
//     }

//     for (int i = 0; i < screens.size(); i++) {
//         auto it = screens.at(i);
//         if (it->geometry().contains(point.toPoint())) {
//             rect = it->geometry();
//             availableRect = it->availableGeometry();
//             return true;
//         }
//     }

//     return false;

//     //    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
//     //    MONITORINFO monitorInfo;
//     //    monitorInfo.cbSize = sizeof(MONITORINFO);
//     //    BOOL bRet = GetMonitorInfo(hMonitor, &monitorInfo);
//     //    if (TRUE != bRet)
//     //    {
//     //        YXLOG(Info) << "GetMonitorInfo failed. GetLastError: " << GetLastError() << YXLOGEnd;
//     //    }
//     //    auto rectTmp = monitorInfo.rcMonitor;
//     //    rect = QRectF((qreal)rectTmp.left,  (qreal)rectTmp.right, (qreal)(rectTmp.right - rectTmp.left), (qreal)(rectTmp.bottom - rectTmp.top));
//     //    rectTmp = monitorInfo.rcWork;
//     //    availableRect = QRectF((qreal)rectTmp.left,  (qreal)rectTmp.right, (qreal)(rectTmp.right - rectTmp.left), (qreal)(rectTmp.bottom -
//     //    rectTmp.top)); return true;
// }

bool WindowsHelpers::isPptPlaying(HWND &hWnd, bool &bPowerpnt)
{
    DWORD dwProcessID = 0;
    std::string strProcessName;
    hWnd = NULL;
    return TRUE == findFullScreenWindow(dwProcessID, strProcessName, hWnd, bPowerpnt);
}

//根据进程id获得进程名
std::string WindowsHelpers::getModuleNameByPid(DWORD dwPid)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPid);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return "";
    }
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);        //存放进程快照信息的结构体
    BOOL ret = Process32FirstW(hSnapshot, &pe); //获得第一个进程的信息
    //遍历
    while (ret)
    {
        if (dwPid == pe.th32ProcessID)
        {
            CloseHandle(hSnapshot);
            return wideCharToString(pe.szExeFile);
        }
        ret = Process32NextW(hSnapshot, &pe); //接着往下遍历
    }

    CloseHandle(hSnapshot);
    return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief PrintCaptureHelper::PrintCaptureHelper
///
PrintCaptureHelper::PrintCaptureHelper()
    : hwnd_(nullptr), scrDc_(nullptr), memDc_(nullptr), bitmap_(nullptr), oldBitmap_(nullptr), bitsPtr_(nullptr), windowRect_{0, 0, 0, 0}, clientRect_{0, 0, 0, 0}, bmpDataSize_(0) {}

PrintCaptureHelper::~PrintCaptureHelper()
{
    Cleanup();
}

bool PrintCaptureHelper::Init(const std::string &windowName)
{
    const auto handle = ::FindWindowA(nullptr, windowName.c_str());
    if (handle == nullptr)
    {
        return false;
    }

    return Init(handle);
}

bool PrintCaptureHelper::Init(HWND hwnd)
{
    if (hwnd != hwnd_)
    {
        Cleanup();
    }
    hwnd_ = hwnd;

    //获取窗口大小
    if (!::GetWindowRect(hwnd_, &windowRect_) || !::GetClientRect(hwnd_, &clientRect_))
    {
        return false;
    }

    bmpWidth_ = clientRect_.right - clientRect_.left;
    bmpHeight_ = clientRect_.bottom - clientRect_.top;


    const int bitcount = 32;
    bmpDataSize_ = (bmpWidth_ * bitcount + 31) / 32 * 4 * bmpHeight_;

    //位图信息
    BITMAPINFO bitmapInfo;
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo);
    bitmapInfo.bmiHeader.biWidth = bmpWidth_;
    bitmapInfo.bmiHeader.biHeight = -bmpHeight_;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = bitcount;
    bitmapInfo.bmiHeader.biSizeImage = bmpDataSize_;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    scrDc_ = ::GetWindowDC(hwnd_);
    memDc_ = ::CreateCompatibleDC(scrDc_);
    bitmap_ = ::CreateDIBSection(scrDc_, &bitmapInfo, DIB_RGB_COLORS, &bitsPtr_, nullptr, 0);
    if (bitmap_ == nullptr)
    {
        ::DeleteDC(memDc_);
        ::ReleaseDC(hwnd_, scrDc_);
        return false;
    }

    oldBitmap_ = static_cast<HBITMAP>(::SelectObject(memDc_, bitmap_));
    return true;
}

void PrintCaptureHelper::Cleanup()
{
    if (bitmap_ == nullptr)
    {
        return;
    }

    //删除用过的对象
    ::SelectObject(memDc_, oldBitmap_);
    ::DeleteObject(bitmap_);
    ::DeleteDC(memDc_);
    ::ReleaseDC(hwnd_, scrDc_);

    hwnd_ = nullptr;
    scrDc_ = nullptr;
    memDc_ = nullptr;
    bitmap_ = nullptr;
    oldBitmap_ = nullptr;
}

bool PrintCaptureHelper::RefreshWindow()
{
    const auto hwnd = hwnd_;
    Cleanup();
    return Init(hwnd);
}

bool PrintCaptureHelper::ChangeWindowHandle(const std::string &windowName)
{
    Cleanup();
    return Init(windowName);
}

bool PrintCaptureHelper::ChangeWindowHandle(HWND hwnd)
{
    Cleanup();
    return Init(hwnd);
}

bool PrintCaptureHelper::Capture() const
{
    if (bitmap_ == nullptr || memDc_ == nullptr || scrDc_ == nullptr)
    {
        return false;
    }

    bool ret = false;
    ret = ::PrintWindow(hwnd_, memDc_, PW_CLIENTONLY | PW_RENDERFULLCONTENT);
    if (ret)
    {
        ret = ::BitBlt(memDc_, 0, 0, clientRect_.right - clientRect_.left, clientRect_.bottom - clientRect_.top, scrDc_, 0, 0, SRCCOPY /*| CAPTUREBLT*/);
    }

    return TRUE == ret;
}

void *PrintCaptureHelper::Crop(int x, int y, int width, int height, int zoomWidth, int zoomHeight)
{
    if (bitmap_ == nullptr || memDc_ == nullptr || scrDc_ == nullptr)
    {
        return nullptr;
    }
    const int bitcount = 32;
    int size = (zoomWidth * bitcount + 31) / 32 * 4 * zoomHeight;
    
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = bitcount;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biWidth = zoomWidth;
    bmi.bmiHeader.biHeight = -zoomHeight;
    bmi.bmiHeader.biSizeImage = size;

    void *pdata;
    HDC hdc = ::GetWindowDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP bitmap = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pdata, nullptr, 0);
    if (bitmap == nullptr)
    {
        ::DeleteDC(hdcMem);
        ::ReleaseDC(NULL, hdc);
        return false;
    }

    HBITMAP oldBitmap = static_cast<HBITMAP>(::SelectObject(hdcMem, bitmap));

    StretchBlt(hdcMem, 0, 0, zoomWidth, zoomHeight, memDc_, x, y, width, height, SRCCOPY);

    bmpWidth_ = zoomWidth;
    bmpHeight_ = zoomHeight;
    bmpDataSize_ = size;
    void* buffer = malloc(bmpDataSize_);
    memcpy(buffer, pdata, bmpDataSize_);

    ::SelectObject(hdcMem, oldBitmap);
    ::DeleteObject(bitmap);
    ::DeleteDC(hdcMem);
    ::ReleaseDC(NULL, hdc);
    return buffer;
}

void* PrintCaptureHelper::Zoom(int width, int height, int type)
{
    if (bitmap_ == nullptr || memDc_ == nullptr || scrDc_ == nullptr)
    {
        return nullptr;
    }
    if (type == 2)
    {
        if (!::PrintWindow(hwnd_, memDc_, PW_CLIENTONLY | PW_RENDERFULLCONTENT))
        {
            return nullptr;
        }
    }

    const int bitcount = 32;
    int size = (width * bitcount + 31) / 32 * 4 * height;
    
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = bitcount;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biSizeImage = size;

    void *pdata;
    HDC hdc = ::GetWindowDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP bitmap = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pdata, nullptr, 0);
    if (bitmap == nullptr)
    {
        ::DeleteDC(hdcMem);
        ::ReleaseDC(NULL, hdc);
        return false;
    }

    HBITMAP oldBitmap = static_cast<HBITMAP>(::SelectObject(hdcMem, bitmap));

    int oldMode = SetStretchBltMode(hdcMem, COLORONCOLOR);
    StretchBlt(hdcMem, 0, 0, width, height, memDc_, 0, 0, GetWidth(), GetHeight(), SRCCOPY);
    SetStretchBltMode(hdcMem, COLORONCOLOR);

    bmpWidth_ = width;
    bmpHeight_ = height;
    bmpDataSize_ = size;
    void* buffer = malloc(bmpDataSize_);
    memcpy(buffer, pdata, bmpDataSize_);

    ::SelectObject(hdcMem, oldBitmap);
    ::DeleteObject(bitmap);
    ::DeleteDC(hdcMem);
    ::ReleaseDC(NULL, hdc);
    return buffer;
}

bool PrintCaptureHelper::CaptureScreen()
{
    scrDc_ = GetDC(GetDesktopWindow());
    // bmpWidth_ = GetDeviceCaps(scrDc_, HORZRES);
    // bmpHeight_ = GetDeviceCaps(scrDc_, VERTRES);
    bmpWidth_ = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    bmpHeight_ = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    memDc_ = CreateCompatibleDC(scrDc_);

    const int bitcount = 32;
    int size = (bmpWidth_ * bitcount + 31) / 32 * 4 * bmpHeight_;
    
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = bitcount;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biWidth = bmpWidth_;
    bmi.bmiHeader.biHeight = -bmpHeight_;
    bmi.bmiHeader.biSizeImage = size;

    bitmap_ = ::CreateDIBSection(scrDc_, &bmi, DIB_RGB_COLORS, &bitsPtr_, nullptr, 0);
    if (bitmap_ == nullptr)
    {
        ::DeleteDC(memDc_);
        ::ReleaseDC(NULL, scrDc_);
        return false;
    }

    oldBitmap_ = static_cast<HBITMAP>(::SelectObject(memDc_, bitmap_));

    return BitBlt(memDc_, 0, 0, bmpWidth_, bmpHeight_, scrDc_, GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN), SRCCOPY);
}

uint8_t *RGBAToBGRA(void *src, int size)
{
    char *pixeslRGBA = (char *)src;
    void *Pixels = malloc(size);
    const int bytesPerPixel = 4;
    int count = size / bytesPerPixel;
    char *pixelsBGRA = (char *)Pixels;
    for (size_t i = 0; i < count; i++)
    {
        pixelsBGRA[0] = pixeslRGBA[2];
        pixelsBGRA[1] = pixeslRGBA[1];
        pixelsBGRA[2] = pixeslRGBA[0];
        pixelsBGRA[3] = pixeslRGBA[3];
        pixelsBGRA += bytesPerPixel;
        pixeslRGBA += bytesPerPixel;
    }
    return reinterpret_cast<uint8_t *>(Pixels);
}

uint8_t *GetWindowsIconRGBA(HWND hWnd, int *width, int *height, int *size)
{
    if (TRUE != IsWindow(hWnd))
    {
        return nullptr;
    }
    //获取程序的path，并保存到exePath
    DWORD dwPID; //保存进程标识符
    GetWindowThreadProcessId(hWnd,
                             &dwPID);                                //接受一个窗口句柄。dwPID保存窗口的创建者的进程标识符，GetWindowThreadProcessId返回值是该创建者的线程标识符
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); //打开一个已存在的进程对象,并返回进程的句柄，这就是我们要的进程句柄了
    if (NULL == hProcess)
    {
        return nullptr;
    }
    WCHAR exePath[256];
    memset(exePath, 0, 256);
    DWORD dwPathNameSize = sizeof(exePath);
    if (TRUE != QueryFullProcessImageNameW(hProcess, 0, exePath, &dwPathNameSize))
    {
        CloseHandle(hProcess);
        // YXLOG(Info) << "GetLastError: " << GetLastError();
        return nullptr;
    }
    CloseHandle(hProcess);
    std::wstring strTemp = exePath;

    SHFILEINFOW sfiTemp;
    ZeroMemory(&sfiTemp, sizeof(sfiTemp));
    SHGetFileInfoW(strTemp.c_str(), FILE_ATTRIBUTE_NORMAL, &sfiTemp, sizeof(SHFILEINFOW), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_LARGEICON);
    if (NULL == sfiTemp.hIcon)
    {
        return nullptr;
    }

    ICONINFO info;
    if (!GetIconInfo(sfiTemp.hIcon, &info) || !info.fIcon)
    {
        *width = 1;
        *height = 1;
        *size = 1;
        DestroyIcon(sfiTemp.hIcon);
        return nullptr;
    }

    INT nWidth = 0;
    INT nHeight = 0;
    if (width != NULL && height != NULL && (*width) > 0 && (*height) > 0)
    {
        nWidth = *width;
        nHeight = *height;
    }
    else
    {
        if (info.hbmColor != NULL)
        {
            BITMAP bmp = {0};
            GetObject(info.hbmColor, sizeof(bmp), &bmp);

            *width = nWidth = bmp.bmWidth;
            *height = nHeight = bmp.bmHeight;
        }
    }

    if (info.hbmColor != NULL)
    {
        DeleteObject(info.hbmColor);
        info.hbmColor = NULL;
    }

    if (info.hbmMask != NULL)
    {
        DeleteObject(info.hbmMask);
        info.hbmMask = NULL;
    }

    if (nWidth <= 0 || nHeight <= 0)
    {
        DestroyIcon(sfiTemp.hIcon);
        return NULL;
    }

    INT nPixelCount = nWidth * nHeight;
    *size = nPixelCount * 4;

    HDC dc = GetDC(NULL);
    INT *pData = NULL;
    void *pOut = NULL;
    HDC dcMem = NULL;
    HBITMAP hBmpOld = NULL;
    bool *pOpaque = NULL;
    HBITMAP dib = NULL;
    BOOL bSuccess = FALSE;

    do
    {
        BITMAPINFOHEADER bi = {0};
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = nWidth;
        bi.biHeight = -nHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        dib = CreateDIBSection(dc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (VOID **)&pData, NULL, 0);
        if (dib == NULL)
            break;

        memset(pData, 0, nPixelCount * 4);

        dcMem = CreateCompatibleDC(dc);
        if (dcMem == NULL)
            break;

        hBmpOld = (HBITMAP)SelectObject(dcMem, dib);
        ::DrawIconEx(dcMem, 0, 0, sfiTemp.hIcon, nWidth, nHeight, 0, NULL, DI_MASK);

        pOpaque = new (std::nothrow) bool[nPixelCount];
        if (pOpaque == NULL)
            break;
        for (INT i = 0; i < nPixelCount; ++i)
        {
            pOpaque[i] = !pData[i];
        }

        memset(pData, 0, nPixelCount * 4);
        ::DrawIconEx(dcMem, 0, 0, sfiTemp.hIcon, nWidth, nHeight, 0, NULL, DI_NORMAL);

        BOOL bPixelHasAlpha = FALSE;
        UINT *pPixel = (UINT *)pData;
        for (INT i = 0; i < nPixelCount; ++i, ++pPixel)
        {
            if ((*pPixel & 0xff000000) != 0)
            {
                bPixelHasAlpha = TRUE;
                break;
            }
        }

        if (!bPixelHasAlpha)
        {
            pPixel = (UINT *)pData;
            for (INT i = 0; i < nPixelCount; ++i, ++pPixel)
            {
                if (pOpaque[i])
                {
                    *pPixel |= 0xFF000000;
                }
                else
                {
                    *pPixel &= 0x00FFFFFF;
                }
            }
        }

        bSuccess = TRUE;

        pOut = malloc(nPixelCount * 4);
        memcpy(pOut, pData, nPixelCount * 4);

    } while (FALSE);

    if (pOpaque != NULL)
    {
        delete[] pOpaque;
        pOpaque = NULL;
    }

    if (dcMem != NULL)
    {
        SelectObject(dcMem, hBmpOld);
        DeleteDC(dcMem);
    }

    ReleaseDC(NULL, dc);

    if (!bSuccess)
    {
        if (dib != NULL)
        {
            DeleteObject(dib);
            dib = NULL;
        }
    }

    DestroyIcon(sfiTemp.hIcon);
    return reinterpret_cast<uint8_t*>(pOut);
}