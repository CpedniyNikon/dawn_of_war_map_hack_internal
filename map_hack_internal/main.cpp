#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <thread>

namespace offset {
	constexpr ::std::ptrdiff_t first = 0x0083F24C;
	constexpr ::std::ptrdiff_t second = 0x24;
	constexpr ::std::ptrdiff_t third = 0xC58;
}



void map_hack(const HMODULE instance) noexcept {
	const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandle(L"soulstorm.exe"));
	const auto first = *reinterpret_cast<std::uintptr_t*>(base + offset::first);
	const auto second = *reinterpret_cast<std::uintptr_t*>(first + offset::second);
	bool isOn = false;

	while (!GetAsyncKeyState(VK_END)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (!GetAsyncKeyState(VK_INSERT))
			continue;

		isOn ^= 1;

		if (isOn) {
			*reinterpret_cast<std::uintptr_t*>(second + offset::third) = 1;
		}
		else {
			*reinterpret_cast<std::uintptr_t*>(second + offset::third) = 0;
		}
	}
	FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(
	const HMODULE instance,
	const std::uint32_t reason,
	const void* reserved) {

	if (reason == 1) {
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(map_hack),
			instance,
			0,
			nullptr);

		if (thread) {
			CloseHandle(thread);
		}

	}
	return 1;
}