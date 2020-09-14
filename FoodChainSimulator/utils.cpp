#include <Windows.h>
#include <string>
namespace EcoSim {
	std::wstring GetExeDir()
	{
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(NULL, path, MAX_PATH);

		std::wstring path_str(path);
		auto pos = path_str.find_last_of(L"\\");
		path_str = std::wstring(path_str.begin(), path_str.begin() + pos);
		return path_str;
	}
}