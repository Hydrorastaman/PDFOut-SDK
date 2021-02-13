#pragma once

#include <string>
#include <unordered_map>

namespace emf2pdf{ namespace impl{
  bool getFontFileMapping(std::unordered_map<std::wstring, std::wstring> &fontMapping);
}}
