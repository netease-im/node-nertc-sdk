#include <windows.h>
#include <string>
#include <vector>

//遍历windows窗口
struct CaptureTargetInfo
{
    HWND id = 0;
    std::wstring title;
    RECT rc{0, 0, 0, 0};
    int type; //0:default 1:window 2:monitor
};
typedef std::vector<CaptureTargetInfo> CaptureTargetInfoList;
std::string wstring2string(std::wstring wstr);
CaptureTargetInfoList enumerateWindows();