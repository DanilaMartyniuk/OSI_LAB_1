#include <iostream>
#include <random>
#include <windows.h>
#include <cmath>
#include "thread"
#include "algorithm"
using namespace std;

struct Structure
{
private:
    int sleep_time;
    int* array;
    int arraySize;
    void DeepCopy(int* array)
    {
        int array_size = _msize(array) / sizeof(int);
        this->array = new int[array_size];
        for (int i = 0; i < array_size; i++)
        {
            this->array[i] = array[i];
        }
    }
public:
    Structure(int sleep_time, int* array, int arrSize)
    {
        this->arraySize = arrSize;
        this->sleep_time = sleep_time;
        DeepCopy(array);
    }
    int* GetArray()
    {
        return this->array;
    }
    int GetSleepTime()
    {
        return this->sleep_time;
    }
    int GetArraySize()
    {
        return this->arraySize;
    }
};

int* CreateArray(int size) {
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100;
    }
    return array;
}

DWORD WINAPI getOddCount(LPVOID  structure)
{
    Structure& p = *reinterpret_cast<Structure*>(structure);
    int array_size = p.GetArraySize();
    int oddCount = 0;
    for (int i = 0; i < array_size; i++)
    {
        if (p.GetArray()[i] % 2 != 0)
        {
            oddCount += 1;
        }
    }
    cout << oddCount;
    return 0;
}

DWORD WINAPI getFibonachi(LPVOID  structure)
{
    Structure& p = *reinterpret_cast<Structure*>(structure);
    int number = p.GetArraySize();
    unsigned long OldValue = 0;
    unsigned long Value = 1;
    unsigned long Hold;
    for (int i = 1; i < number; i++)
    {
        cout << Value << endl;
        Hold = Value;
        Value += OldValue;
        OldValue = Hold;
    }
    return 0;
}

int main()
{
    int size = 0, time = 0;
    cout << "Enter the size of array: ";
    cin >> size;
    cout << "Enter the time in milliseconds:";
    cin >> time;
    int* array = CreateArray(size);
    cout << "Array: ";
    for (int i = 0; i < size; i++)
    {
        cout << array[i] << " ";
    }
    cout << "\n";
    Structure object(time, array, size);
    DWORD workerid; //идентификатор потока
    HANDLE worker = CreateThread(NULL, 0, &getOddCount, &object, 0, &workerid);
    if (worker == NULL)
    {
        return GetLastError();
    }

    SuspendThread(worker);
    ResumeThread(worker);

    if (WaitForSingleObject(worker, INFINITE) != WAIT_OBJECT_0)
    {
        cout << "Wait for single object failed." << endl;
        cout << "Press any key to exit." << endl;
    }

    HANDLE fibonachi = CreateThread(NULL, 0, &getFibonachi, &object, CREATE_SUSPENDED, &workerid);

    ResumeThread(fibonachi);

    if (WaitForSingleObject(fibonachi, INFINITE) != WAIT_OBJECT_0)
    {
        cout << "Wait for single object failed." << endl;
        cout << "Press any key to exit." << endl;
    }
    CloseHandle(fibonachi);
    CloseHandle(worker);
    return 0;
}
#