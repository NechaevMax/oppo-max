#pragma once

// Минимальная заглушка windows.h для сборки проекта в Linux/macOS в рамках анализа.
// В исходной (PR-3) версии использовались функции SetConsoleCP/SetConsoleOutputCP.
// На Windows подключается настоящий <windows.h>. Здесь они не требуются.

using UINT = unsigned int;

inline void SetConsoleCP(UINT) {}
inline void SetConsoleOutputCP(UINT) {}
