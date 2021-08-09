#ifndef _WINDOWS_HELPERS_H_
#define _WINDOWS_HELPERS_H_

#include <windows.h>
#include <string>
#include <vector>
#include <map>

uint32_t SuperFastHash(const char* data, int len);

namespace nertc_electron_util
{

class WindowCaptureHelper;
class WindowsHelpers
{
public:
    struct CaptureTargetInfo {
        HWND            id = 0;
        std::wstring    title;
        std::string     app;
        std::string     display_id;
        int type; // 0:none 1:screen 2:window 3:webcontents(non-support) 4:currenttab(non-support)
        bool isMinimizeWindow = false;
        RECT rc{ 0, 0, 0, 0 };
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
    bool static getCaptureWindowList(CaptureTargetInfoList* windows, int type = 0);
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
    void updateCachedInfos(int64_t id, RECT rect) { cached_infos[id] = rect; }
    RECT getCachedRect(int64_t id) { return cached_infos[id]; }

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
    std::map<int64_t, RECT> cached_infos;
};

class ScreenCaptureHelper
{
public:
    // DesktopFrame objects always hold RGBA data.
    static const int kBytesPerPixel = 4;

public:
    ScreenCaptureHelper() {}
    virtual ~ScreenCaptureHelper() { UnintScreen(); }

public:
    bool InitScreen();
    void UnintScreen();
    bool CaptureScreenRect(int x, int y, int width, int height, int zoomWidth, int zoomHeight);
    int GetSize(int width, int height);
    void *GetData() {return outPtr_;}

private:
    void ClearOutPut();

private:
    HWND hwnd_ = nullptr;
    HDC scrDc_ = nullptr;
    HDC memDc_ = nullptr;
    HBITMAP bitmap_ = nullptr;
    HBITMAP oldBitmap_ = nullptr;
    void* bitsPtr_ = nullptr;
    void* outPtr_ = nullptr;
};

class WindowCaptureHelper
{
public:
    // DesktopFrame objects always hold RGBA data.
    static const int kBytesPerPixel = 4;
public:
    WindowCaptureHelper();
    virtual ~WindowCaptureHelper();

public:
    bool CaptureWindow(HWND hwnd);
    void Cleanup();

    bool Zoom(int width, int height);
    void *GetData() {return outPtr_;}
    const RECT& GetWindowRect() const { return windowRect_; }
    const RECT& GetClientRect() const { return clientRect_; }
    int GetBitmapDataSize() const { return bmpDataSize_; }
    HBITMAP GetBitmap() const { return bitmap_; }
    void* GetBitmapAddress() const { return bitsPtr_; }
    int GetWidth() const { return bmpWidth_; }
    int GetHeight() const { return bmpHeight_; }

private:
    bool Init(const std::string& windowName);
    bool Init(HWND hwnd);
    bool RefreshWindow();
    bool ChangeWindowHandle(const std::string& windowName);
    bool ChangeWindowHandle(HWND hwnd);
    int GetSize(int width, int height);

private:
    void ClearOutPut();

private:
    HWND hwnd_ = nullptr;
    HDC scrDc_ = nullptr;
    HDC memDc_ = nullptr;
    HBITMAP bitmap_ = nullptr;
    HBITMAP oldBitmap_ = nullptr;
    void* bitsPtr_ = nullptr;
    void* outPtr_ = nullptr;

    RECT windowRect_ = {0, 0, 0, 0};
    RECT clientRect_ = {0, 0, 0, 0};
    int bmpDataSize_ = 0;
    int bmpWidth_ = 0;
    int bmpHeight_ = 0;
};

int GetRGBASize(int width, int height);
uint8_t *GetWindowsIconRGBA(HWND hWnd, int *width, int *height, int *size);
uint8_t *RGBAToBGRA(void *src, int size);
// void *CaptureScreen(int x, int y, int width, int height);

}
#endif // _WINDOWS_HELPERS_H_
