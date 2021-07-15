#ifndef _WINDOWS_HELPERS_H_
#define _WINDOWS_HELPERS_H_

#include <windows.h>
#include <string>
#include <vector>

class PrintCaptureHelper;
class WindowsHelpers
{
public:
    struct CaptureTargetInfo {
        HWND              id = 0;
        std::wstring      title;
        std::string app;
        RECT rc{0, 0, 0, 0};
        int type; //0:none 1:screen 2:window 3:webcontents(non-support) 4:currenttab(non-support)
        bool isMinimizeWindow=false;
    };
    typedef std::vector<CaptureTargetInfo> CaptureTargetInfoList;

    WindowsHelpers();
    ~WindowsHelpers();

    bool isWindow(HWND hWnd) const;
    bool isMinimized(HWND hWnd) const;
    bool isZoomed(HWND hWnd) const;
    bool isHide(HWND hWnd) const;
    RECT getWindowRect(HWND hWnd) const;
    HBITMAP getWindowImage(HWND hWnd) const;
    HICON getWindowIcon(HWND hWnd) const;
    bool static getCaptureWindowList(CaptureTargetInfoList* windows);
    void setForegroundWindow(HWND hWnd) const;
    void sharedOutsideWindow(HWND wid, HWND hWnd, bool bFullScreen);
    void setWindowTop(HWND wid);
    RECT getWindowFrameRect(HWND hWnd) const;
    // pos：任务栏位置1左边，2上边，3右边，4下边
    void getTaskbarInfo(int &width, int &height, int &pos);
    std::string getModuleName(HWND hWnd);
    bool isPptPlaying(HWND& hWnd, bool& bPowerpnt);
    bool getForegroundWindow(HWND hWnd) const;
    bool getActiveWindow(HWND hWnd) const;
    bool getFocusWindow(HWND hWnd) const;
    // bool getDisplayRect(HWND hWnd, RECT& rect, RECT& availableRect, bool bPpt = false) const;

    bool static getFileVersion(const wchar_t *file_path, WORD *major_version, WORD *minor_version, WORD *build_number, WORD *revision_number);
    int static getNTDLLVersion();
    std::wstring static getCurrentExe();

private:
    //根据屏幕上指定的坐标点，获取坐标点窗口对应的pid
    DWORD getPidByPoint(int nX, int nY);
    //根据屏幕上指定的坐标点，获取坐标点窗口对应的HWND
    HWND getHwndByPoint(int nX, int nY);
    //查找全屏的应用窗口
    BOOL findFullScreenWindow(DWORD &dwProcessID, std::string &strProcessName, HWND &hWnd, bool& bPowerpnt);
    //根据进程id获得进程名
    std::string getModuleNameByPid(DWORD dwPid);

private:
    std::wstring static m_strCurrentExe;
    PrintCaptureHelper* m_pPrintCaptureHelper = nullptr;
};

class PrintCaptureHelper
{
public:
    PrintCaptureHelper();
    virtual ~PrintCaptureHelper();

    bool Init(const std::string& windowName);
    bool Init(HWND hwnd);
    void Cleanup();
    bool RefreshWindow();
    bool ChangeWindowHandle(const std::string& windowName);
    bool ChangeWindowHandle(HWND hwnd);
    bool Capture() const;

    const RECT& GetWindowRect() const { return windowRect_; }
    const RECT& GetClientRect() const { return clientRect_; }
    int GetBitmapDataSize() const { return bmpDataSize_; }
    HBITMAP GetBitmap() const { return bitmap_; }
    void* GetBitmapAddress() const { return bitsPtr_; }
    int GetWidth() const { return bmpWidth_; }
    int GetHeight() const { return bmpHeight_; }

private:
    HWND hwnd_ = nullptr;
    HDC scrDc_ = nullptr;
    HDC memDc_ = nullptr;
    HBITMAP bitmap_ = nullptr;
    HBITMAP oldBitmap_ = nullptr;
    void* bitsPtr_ = nullptr;

    RECT windowRect_ = {0, 0, 0, 0};
    RECT clientRect_ = {0, 0, 0, 0};
    int bmpDataSize_ = 0;
    int bmpWidth_ = 0;
    int bmpHeight_ = 0;
};

uint8_t *GetWindowsIconRGBA(HWND hWnd, int *width, int *height, int *size);
uint8_t *RGBAToBGRA(void *src, int size);

#endif // _WINDOWS_HELPERS_H_
