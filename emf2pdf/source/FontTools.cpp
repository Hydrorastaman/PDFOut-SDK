#include <FontTools.h>

#include <algorithm>
#include <Windows.h>
#include <GenericHandle.h>

namespace emf2pdf{ namespace impl{

  bool getFontFileMapping(std::unordered_map<std::wstring, std::wstring> &fontMapping){
    HKey hkey(nullptr);
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", NULL, KEY_READ, &hkey) != ERROR_SUCCESS)
      return false;

    DWORD index = 0;
    WCHAR valueName[MAX_PATH] = {0};
    DWORD valueNameSize = MAX_PATH;
    DWORD valueType = 0;
    BYTE valueData[MAX_PATH] = {0};
    DWORD valueDataSize = MAX_PATH;

    std::wstring key;
    std::wstring value;
    while (RegEnumValueW(hkey, index, valueName, &valueNameSize, nullptr, &valueType, valueData, &valueDataSize) == ERROR_SUCCESS){
      ++index;

      key = valueName;
      value = (WCHAR *) valueData;
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      fontMapping[key] = value;

      valueNameSize = MAX_PATH;
      valueDataSize = MAX_PATH;
    }

    return true;
  }

}}
