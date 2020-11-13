#pragma once

#ifdef MERGESORTDLIB_EXPORTS
#define sort_API extern "C" __declspec(dllexport)
#else
#define sort_API extern "C" __declspec(dllimport)
#endif

sort_API void MergeSortDll(int a[], int l);
