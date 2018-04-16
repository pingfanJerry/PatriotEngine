﻿#include <sys/mman.h>
#ifdef PTPOSIXXCB
#include <dirent.h>
#elif defined (PTPOSIXANDROID)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#else
#error 未知的平台
#endif

static uint32_t const s_Page_Size = 1024U * 4U;

static inline void *PTS_MemoryMap_Alloc(uint32_t Size)
{
	//在Linux中仅对齐到页大小（1024*4），并不保证对齐到块大小（1024*4*4）
	//先用mmap分配（SizeAligned/PageSize+3）个页，再用munmap释放多余的3个页（前0后3/前1后2/前3后0）

	assert(s_Page_Size == (1U << 12U));
	assert(s_Block_Size == (s_Page_Size * 4U));

	uint32_t PageNumber = ((Size - 1U) >> 12U) + 1U;
	uintptr_t AddressWholeBegin = reinterpret_cast<uintptr_t>(::mmap(NULL, s_Page_Size*(PageNumber + 3U), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0U));
	assert(::PTS_Size_IsAligned(AddressWholeBegin, s_Page_Size));

	uintptr_t PageWholeBegin = AddressWholeBegin >> 12U;
	uintptr_t PageNeededBegin = ::PTS_Size_AlignUpFrom(PageWholeBegin, static_cast<uintptr_t>(4U));
	uint32_t PageToFreeBeginNumber = static_cast<uint32_t>(PageNeededBegin - PageWholeBegin);

	int iResult;
	if (PageToFreeBeginNumber != 0U)
	{
		iResult = ::munmap(reinterpret_cast<void*>(AddressWholeBegin), s_Page_Size*PageToFreeBeginNumber);
		assert(iResult == 0);
	}
	if (PageToFreeBeginNumber != 3U)
	{
		iResult = ::munmap(reinterpret_cast<void*>(s_Page_Size*(PageNeededBegin + PageNumber)), s_Page_Size*(3U - PageToFreeBeginNumber));
		assert(iResult == 0);
	}
	return reinterpret_cast<void*>(s_Page_Size*PageNeededBegin);
}

static inline bool PTS_Number_CharToUI(char C, uint32_t *pI)
{
	switch (C)
	{
	case '0':
		(*pI) = 0U;
		return true;
	case '1':
		(*pI) = 1U;
		return true;
	case '2':
		(*pI) = 2U;
		return true;
	case '3':
		(*pI) = 3U;
		return true;
	case '4':
		(*pI) = 4U;
		return true;
	case '5':
		(*pI) = 5U;
		return true;
	case '6':
		(*pI) = 6U;
		return true;
	case '7':
		(*pI) = 7U;
		return true;
	case '8':
		(*pI) = 8U;
		return true;
	case '9':
		(*pI) = 9U;
		return true;
	case 'a':
		(*pI) = 10U;
		return true;
	case 'b':
		(*pI) = 11U;
		return true;
	case 'c':
		(*pI) = 12U;
		return true;
	case 'd':
		(*pI) = 13U;
		return true;
	case 'e':
		(*pI) = 14U;
		return true;
	case 'f':
		(*pI) = 15U;
		return true;
	default:
		return false;
	}
}

static inline void PTS_MemoryMap_Free(void *pVoid)
{
	//在Linux中
	//在x86或ARM架构下，用户空间最多不超过4GB（0XFFFFFFFF 8个字符）
	//在x86_64（https://www.kernel.org/doc/Documentation/x86/x86_64/mm.txt）架构下，用户空间只有128TB（0X7FFFFFFFFFFF 12个字符）
	//在ARM64（https://www.kernel.org/doc/Documentation/arm64/memory.txt）架构下，用户空间只有512GB(0X7FFFFFFFFF 10个字符）

#if defined(PTX86) || defined(PTARM)
	char Str_AddressStart[8 + 1];
	char *pStr_AddressStart;
	int Str_AddressStart_Length;
	{
		Str_AddressStart[8] = '\0';
		char const *pStr_Number = "0123456789abcdef";
		int index = 8;
		uintptr_t AddressStart = reinterpret_cast<uintptr_t>(pVoid);
		while (AddressStart != 0U)
		{
			--index;
			assert(index >= 0U);
			Str_AddressStart[index] = pStr_Number[(AddressStart & 0XF)];
			AddressStart >>= 4U;
		}
		pStr_AddressStart = Str_AddressStart + index;
		Str_AddressStart_Length = 8 - index;
	}
#elif defined(PTX64)
	char Str_AddressStart[12 + 1];
	char *pStr_AddressStart;
	int Str_AddressStart_Length;
	{
		Str_AddressStart[12] = '\0';
		char const *pStr_Number = "0123456789abcdef";
		int index = 12;
		uintptr_t AddressStart = reinterpret_cast<uintptr_t>(pVoid);
		while (AddressStart != 0U)
		{
			--index;
			assert(index >= 0U);
			Str_AddressStart[index] = pStr_Number[(AddressStart & 0XF)];
			AddressStart >>= 4U;
		}
		pStr_AddressStart = Str_AddressStart + index;
		Str_AddressStart_Length = 12 - index;
	}
#elif defined(PTARM64)
	char Str_AddressStart[10 + 1];
	char *pStr_AddressStart;
	int Str_AddressStart_Length;
	{
		Str_AddressStart[10] = '\0';
		char const *pStr_Number = "0123456789abcdef";
		int index = 10;
		uintptr_t AddressStart = reinterpret_cast<uintptr_t>(pVoid);
		while (AddressStart != 0U)
		{
			--index;
			assert(index >= 0U);
			Str_AddressStart[index] = pStr_Number[(AddressStart & 0XF)];
			AddressStart >>= 4U;
		}
		pStr_AddressStart = Str_AddressStart + index;
		Str_AddressStart_Length = 10 - index;
	}
#else
#error 未知的架构
#endif

#ifdef PTPOSIXXCB
	//proc/self/map_files

	//mmap MAP_SHARED|MAP_ANONYMOUS
	//在proc/self/map_files 链接到 /dev/zero
	//不同的mmap之间 即使连续照样不会合并
	dirent DirEnt_mmap;
	DIR *pDir_mmap = ::opendir("/proc/self/map_files");
	assert(pDir_mmap != NULL);
	dirent *pDirEnt_mmap;
	bool bIsFound = false;
	while ((::readdir_r(pDir_mmap, &DirEnt_mmap, &pDirEnt_mmap) == 0) && (pDirEnt_mmap != NULL))
	{
		bool bIsEqual = true;
		for (int i = 0; i < Str_AddressStart_Length; ++i)
		{
			if (pDirEnt_mmap->d_name[i] == '\0' || pDirEnt_mmap->d_name[i] != pStr_AddressStart[i])
			{
				bIsEqual = false;
				break;
			}
		}

		if (bIsEqual)
		{
			bIsFound = true;
			break;
		}
	}
	assert(bIsFound);

	int iResult = ::closedir(pDir_mmap);
	assert(iResult == 0);

	assert(pDirEnt_mmap->d_name[Str_AddressStart_Length] == '-');
	char *pStr_AddressEnd = pDirEnt_mmap->d_name + (Str_AddressStart_Length + 1);
#elif defined (PTPOSIXANDROID)
	//proc/self/maps

	int FD_maps = ::open("/proc/self/maps",O_RDONLY);
	assert(FD_maps != -1);
	char Str_maps[4096U];
	assert(s_Page_Size == 4096U);
	ssize_t ssResult;
	char *pStr_AddressEnd = NULL;
	while ((ssResult = ::read(FD_maps, Str_maps, 4096U)) != 0)
	{
		assert(ssResult != -1);
		assert(ssResult < 4096);
		Str_maps[ssResult] = '\0';
		pStr_AddressEnd = ::strstr(Str_maps, pStr_AddressStart);
		if (pStr_AddressEnd != NULL)
		{
			break;
		}
	}
	assert(pStr_AddressEnd != NULL);

	int iResult = ::close(FD_maps);
	assert(iResult == 0);

	assert(pStr_AddressEnd[Str_AddressStart_Length] == '-');
	pStr_AddressEnd += (Str_AddressStart_Length + 1);
#else
#error 未知的平台
#endif

	uintptr_t AddressEnd = 0U;
	uint32_t ValueToAdd;
	while (::PTS_Number_CharToUI(*pStr_AddressEnd, &ValueToAdd))
	{
		AddressEnd <<= 4U;
		AddressEnd += ValueToAdd;
		++pStr_AddressEnd;
	}

	iResult = ::munmap(pVoid, AddressEnd - reinterpret_cast<uintptr_t>(pVoid));
	assert(iResult == 0);
}

static inline uint32_t PTS_Size_BitScanReverse(uint32_t Value)
{
	return 31 - ::__builtin_clz(233);
}