#include <ctime>
#include <Windows.h>
#include <filesystem>
#include "io.h"
#include "loadLibraryA.h"
#include "unlinkDll.h"
#include "auth.hpp"
#include <string>
#include "utils.hpp"
#include "skStr.h"

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);
#define PROCESS_REQUIRED_ACCESS (PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_CREATE_THREAD)

using namespace KeyAuth;
std::string name = skCrypt("UPasta").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = skCrypt("8XinNh6izS").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = skCrypt("adbb53143018cee602895134edf129107257a1c1481370b156d328b8c5de6171").decrypt(); // app secret, the blurred text on licenses tab and other tabs
std::string version = skCrypt("1.0").decrypt(); // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);

static void takeInjectionAction(io::action curAction, io::launchMethod curLaunchMethod, io::handleCreation curHandleCreation, const std::string* pProcName, const std::string* pDllName, const std::string* pDllDir);
static void takeUnlinkAction(io::handleCreation curHandleCreation, const std::string* pProcName, const std::string* pDllName);
std::string gen_string(const int length)
{
	std::string GeneratedString;
	static const char Alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	srand((unsigned)time(NULL) * 5);
	for (int i = 0; i < length; i++)
		GeneratedString += Alphabet[rand() % (sizeof(Alphabet) - 1)];
	return GeneratedString;
}

int main(int argc, const char* argv[]) {
	SetConsoleTitleA(gen_string(25).c_str());

	std::string procName;
	std::string dllName;
	std::string dllDir;

	io::init();
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	io::printHeader();

	procName = "League Of Legends.exe";
	dllName = "Spaghetti.dll";
	dllDir = "C:\\UPorn";

	// defaults
	io::action curAction = io::action::LOAD_LIB;
	io::handleCreation curHandleCreation = io::handleCreation::OPEN_PROCESS;
	io::launchMethod curLaunchMethod = io::launchMethod::CREATE_THREAD;

	while (curAction != io::action::EXIT) {
		io::printHeader();
		io::printTargetInfo(procName, dllName, dllDir);

		std::cout << skCrypt("\n\n [1] Login\n [2] Register\n\n\n Choose option: ");

		int option;
		std::string username;
		std::string password;
		std::string key;

		std::cin >> option;
		switch (option)
		{
		case 1:
			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			KeyAuthApp.login(username, password);
			break;
		case 2:
			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			std::cout << skCrypt("\n Enter license: ");
			std::cin >> key;
			KeyAuthApp.regstr(username, password, key);
			break;
		default:
			std::cout << skCrypt("\n\n Status: Failure: Invalid Selection");
			Sleep(3000);
			exit(0);
		}

		if (!KeyAuthApp.data.success)
		{
			std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
			Sleep(1500);
			exit(0);
		}
		if (username.empty() || password.empty())
		{
			WriteToJson("test.json", "license", key, false, "", "");
			std::cout << skCrypt("Successfully Created File For Auto Login");
		}
		else
		{
			WriteToJson("test.json", "username", username, true, "password", password);
			std::cout << skCrypt("Successfully Created File For Auto Login");
		}

		//
		io::printMainMenu(curAction);
		io::selectAction(&curAction);




		switch (curAction) {
		case io::action::LOAD_LIB:
			io::selectLaunchMethod(&curLaunchMethod);
		default:
			break;
		}

		switch (curAction) {
		case io::action::LOAD_LIB:
			io::initLog();
			takeInjectionAction(curAction, curLaunchMethod, curHandleCreation, &procName, &dllName, &dllDir);
			takeUnlinkAction(curHandleCreation, &procName, &dllName);
			curAction = io::action::EXIT;
			break;
		default:
			break;
		}

	}

	return 0;
}


static HANDLE getProcessHandle(const std::string* pProcName, io::handleCreation curHandleCreation);
static hax::launch::tLaunchFunc getLaunchFunction(io::launchMethod launchMethod, BOOL isWow64);

static void takeInjectionAction(io::action curAction, io::launchMethod curLaunchMethod, io::handleCreation curHandleCreation, const std::string* pProcName, const std::string* pDllName, const std::string* pDllDir) {
	const HANDLE hProc = getProcessHandle(pProcName, curHandleCreation);

	if (!hProc) return;

	BOOL isWow64 = FALSE;
	IsWow64Process(hProc, &isWow64);

	const hax::launch::tLaunchFunc pLaunchFunc = getLaunchFunction(curLaunchMethod, isWow64);

	if (!pLaunchFunc) {
		CloseHandle(hProc);

		return;
	}

	const std::string dllFullPath = (*pDllDir + "\\" + *pDllName);
	bool success = false;

	switch (curAction) {
	case io::action::LOAD_LIB:
		success = loadLib::inject(hProc, dllFullPath.c_str(), pLaunchFunc);
		break;
	default:
		break;
	}

	CloseHandle(hProc);

	io::printInjectionResult(curAction, curLaunchMethod, success);
	io::printSpace(" ");
	io::printSpace("------------------------------------------------------");
	io::printSpace(" ");

	return;
}


static DWORD searchProcId(const std::string* pProcName);

static HANDLE getProcessHandle(const std::string* pProcName, io::handleCreation curHandleCreation) {
	const DWORD procId = searchProcId(pProcName);

	if (!procId) return nullptr;

	HANDLE hProc = nullptr;
	switch (curHandleCreation) {
	case io::OPEN_PROCESS:
		hProc = OpenProcess(PROCESS_REQUIRED_ACCESS, FALSE, procId);
		break;
	case io::DUPLICATE_HANDLE:
		hProc = hax::proc::getDuplicateProcessHandle(PROCESS_REQUIRED_ACCESS, FALSE, procId);
		break;
	default:
		break;
	}

	if (!hProc || hProc == INVALID_HANDLE_VALUE) {
		io::printWinError("Failed to get process handle.");

		// common error when trying to access a process run by higher priviliged user
		if (GetLastError() == ERROR_ACCESS_DENIED) {
			io::printInfo("Try running as administrator.");
		}

		return nullptr;
	}

	return hProc;
}


static DWORD searchProcId(const std::string* pProcName) {
	io::printInfo("Looking for process '" + *pProcName + "'...");
	DWORD procId = 0;

	// search for the process for five seconds
	for (int i = 0; i < 50; i++) {
		procId = hax::proc::getProcessId(pProcName->c_str());

		if (procId) break;

		Sleep(100);
	}

	if (!procId) {
		io::printWinError("Could not find target process.");

		return 0;
	}

	io::printInfo("Found target process. Process ID: " + std::to_string(procId));

	return procId;
}


static hax::launch::tLaunchFunc getLaunchFunction(io::launchMethod launchMethod, BOOL isWow64) {
	hax::launch::tLaunchFunc pLaunchFunc = nullptr;

	switch (launchMethod) {
	case io::launchMethod::CREATE_THREAD:
		pLaunchFunc = hax::launch::createThread;
		break;
	case io::launchMethod::HIJACK_THREAD:
		pLaunchFunc = hax::launch::hijackThread;
		break;
	case io::launchMethod::SET_WINDOWS_HOOK:
		
		#ifdef _WIN64

		if (isWow64) {
			io::printPlainError("\"Set windows hook\" on x86 targets only possible from x86 binary.");
			
			return nullptr;
		}
		
		#else

		if (!isWow64) {
			io::printPlainError("\"Set windows hook\" on x64 targets only possible from x64 binary.");

			return nullptr;
		}

		#endif // _WIN64

		pLaunchFunc = hax::launch::setWindowsHook;
		break;
	case io::launchMethod::HOOK_BEGIN_PAINT:
		pLaunchFunc = hax::launch::hookBeginPaint;
		break;
	case io::launchMethod::QUEUE_USER_APC:
		pLaunchFunc = hax::launch::queueUserApc;
		break;
	}

	return pLaunchFunc;
}


static void takeUnlinkAction(io::handleCreation curHandleCreation, const std::string* pProcName, const std::string* pDllName) {
	const HANDLE hProc = getProcessHandle(pProcName, curHandleCreation);

	if (!hProc) return;

	const bool success = ldr::unlinkModule(hProc, pDllName->c_str());

	io::printUnlinkResult(success);

	CloseHandle(hProc);

	return;
}