#include <windows.h>
#include <string>
#include <vector>

//遍历windows窗口
struct CaptureTargetInfo
{
    HWND id = 0;
    std::wstring title;
    RECT rc{0, 0, 0, 0};
    int type; //0:none 1:screen 2:window 3:webcontents(non-support) 4:currenttab(non-support)
};
typedef std::vector<CaptureTargetInfo> CaptureTargetInfoList;
std::wstring UTF8ToUTF16(const std::string& utf8);
std::string UTF16ToUTF8(const std::wstring utf16);
CaptureTargetInfoList enumerateWindows();