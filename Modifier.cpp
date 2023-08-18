#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>

using namespace std;

enum DataType {INT, FLOAT};

class ProcessMemorry {
public:
    void minitor(string name);
    ProcessMemorry() {
        PID = NULL;
        hProcess = NULL;
    };
    ~ProcessMemorry() {
        CloseHandle(hProcess);
    };
    bool GetValueInProcessMemory(int targetValue);
    void FilterValueInList(int targetValue);
    void Modify(int value);
    int getValidNum() {
        return validDatasOff.size();
    };
private:
    HANDLE hProcess;
    int64_t PID;
    vector<LPVOID> validDatasOff;

    DWORD getPidByName(const char* name);

};

//monitor a process by name, get it's PID by `getPidByName()`, and bind to the class
void ProcessMemorry::minitor(string name) {
    int pid = getPidByName(name.c_str());
    if (pid == -1) {
        cout << "   Process \"" << name << " \"dont's exist." << endl;
        return;
    }

    PID = pid;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL) {
        std::cerr << "  Failed to open process. Error code: " << GetLastError() << std::endl;
    }
}

//Traverse all processes to find processes with specific names.
DWORD ProcessMemorry::getPidByName(const char* name) {
    if (name == NULL) {
        printf("    The argument [name] is empty\n");
        exit(0);
    }
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (stricmp (entry.szExeFile, name) == 0) {
                return entry.th32ProcessID;
            }
        }
    }
    return -1;
}

//Find specific data and put it's address into the list named `validDatas`
bool ProcessMemorry::GetValueInProcessMemory(int targetValue) {
    validDatasOff.clear();
    MEMORY_BASIC_INFORMATION memInfo;
    LPVOID address = NULL;

    while (VirtualQueryEx(hProcess, address, &memInfo, sizeof(memInfo))) {
        if (memInfo.State == MEM_COMMIT && memInfo.Protect == PAGE_READWRITE) {
            BYTE* buffer = new BYTE[memInfo.RegionSize];
            SIZE_T bytesRead;
            if (ReadProcessMemory(hProcess, address, buffer, memInfo.RegionSize, &bytesRead)) {
                for (SIZE_T i = 0; i < bytesRead - sizeof(targetValue); i++) {
                    if (*(int*)(buffer + i) == targetValue) {
                        //Put the address to list
                        validDatasOff.push_back((LPVOID)((uintptr_t)address + i));
                        // Found the target value, modify it
                    }
                }
            }
            delete[] buffer;
        }
        address = (LPVOID)((uintptr_t)address + memInfo.RegionSize);
    }
    return true;
}


//Filter specific value from the list `validDatas`
void ProcessMemorry::FilterValueInList(int targetValue) {
    MEMORY_BASIC_INFORMATION memInfo;
    LPVOID address = NULL;

    int value;
    int cnt = 0;
    for (auto iter = validDatasOff.begin(); iter != validDatasOff.end();) {
        ReadProcessMemory(hProcess, (*iter), &value, sizeof(value), NULL);
        if (value != targetValue) {
            iter = validDatasOff.erase(iter);
        }
        else iter++;
        cnt++;

        cout << "\r" << cnt*1.0/validDatasOff.size() * 100 << "%   ";
    }
}


//Modify all data recorded in list `validDatas`
void ProcessMemorry::Modify(int newValue) {    
    LPVOID base = NULL;
    for (auto iter = validDatasOff.begin(); iter != validDatasOff.end();iter++) {
        WriteProcessMemory(hProcess, (*iter), &newValue, sizeof(newValue), NULL);
        std::cout << "  Modified value at address: " << std::hex << *iter << std::endl;
    }
}

vector<string> args;
void readCommand() {
    string input;
    args.clear();
    while (cin >> input) {
        args.emplace_back(input);
        if(getchar()=='\n') break;
    }
}

int main() {
    ProcessMemorry procMem;
    string command;
    while (1) {
        cout << "Modifier > ";
        readCommand();

        //requerry pid value
        if (args[0] == "monitor") {
            if (args.size() != 2) {
                cout << "   The argument is invalid." << endl;
                continue;
            }
            procMem.minitor(args[1]);
        }
        else if (args[0] == "requerry"){
            if (args.size() != 2) {
                cout << "   The argument is invalid." << endl;
                continue;
            }
            procMem.GetValueInProcessMemory(atoi(args[1].c_str()));
            cout << "Total " << procMem.getValidNum() << " result." << endl;
        }
        //querry value
        else if (args[0] == "querry") {
            if (args.size() != 2) {
                cout << "   The argument is invalid." << endl;
                continue;
            }
            procMem.FilterValueInList(atoi(args[1].c_str()));
            cout << "   Total " << procMem.getValidNum() << " result." << endl;
        }
        else if (args[0] == "modify") {
            if (args.size() != 2) {
                cout << "   The argument is invalid." << endl;
                continue;
            }
            procMem.Modify(atoi(args[1].c_str()));
            cout << "   Modified finished." << endl;
        }
        else if (args[0] == "exit") {
            break;
        }
        else {
            cout << "   This isn't a valid command." << endl;
        }
        
    }
    return 0;
}