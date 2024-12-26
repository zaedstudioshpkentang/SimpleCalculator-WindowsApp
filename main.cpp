#include <windows.h>
#include <string>
#include <sstream>
#include <cstdlib>

// Deklarasi global
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
char szClassName[] = "SimpleCalculator";
HWND hEdit;
std::string currentInput = "";
std::string displayText = "";
char currentOperator = '\0';
double previousValue = 0;
bool operatorPressed = false;

// Fungsi untuk konversi string ke double
double StringToDouble(const std::string& str) {
    return atof(str.c_str());
}

// Fungsi untuk konversi double ke string
std::string DoubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Fungsi untuk memperbarui tampilan
void UpdateDisplay(const std::string& text) {
    SetWindowText(hEdit, text.c_str());
}

// Fungsi untuk melakukan operasi matematika
void PerformCalculation(double value) {
    if (currentOperator == '+') previousValue += value;
    else if (currentOperator == '-') previousValue -= value;
    else if (currentOperator == '*') previousValue *= value;
    else if (currentOperator == '/') previousValue = value == 0 ? 0 : previousValue / value;
    else previousValue = value; // Jika tidak ada operator, set nilai awal
}

// Fungsi untuk menangani tombol "="
void Calculate() {
    if (!currentInput.empty()) {
        double currentValue = StringToDouble(currentInput);
        PerformCalculation(currentValue);
        currentInput = DoubleToString(previousValue);
        displayText = currentInput; // Tampilkan hasil
        currentOperator = '\0';
        operatorPressed = false;
        UpdateDisplay(displayText);
    }
}

// Fungsi untuk menangani input dari tombol
void HandleButtonPress(const std::string& button) {
    if (isdigit(button[0])) {
        // Tambahkan angka ke input saat ini
        if (operatorPressed) {
            currentInput = ""; // Reset input untuk angka kedua
            operatorPressed = false;
        }
        currentInput += button;
        displayText += button;
        UpdateDisplay(displayText);
    } else if (button == "C") {
        // Reset kalkulator
        currentInput = "";
        displayText = "";
        currentOperator = '\0';
        previousValue = 0;
        operatorPressed = false;
        UpdateDisplay("0");
    } else if (button == "=") {
        // Hitung hasil
        Calculate();
    } else {
        // Jika operator ditekan
        if (!currentInput.empty()) {
            double currentValue = StringToDouble(currentInput);
            PerformCalculation(currentValue);
            currentInput = ""; // Reset input setelah operator ditekan
        }
        currentOperator = button[0];
        operatorPressed = true; // Menandai bahwa operator telah ditekan
        displayText += " " + button + " ";
        UpdateDisplay(displayText);
    }
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
    WNDCLASSEX wincl;
    HWND hwnd;
    MSG messages;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!RegisterClassEx(&wincl)) return 0;

    hwnd = CreateWindowEx(
        0, szClassName, "Simple Calculator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
        HWND_DESKTOP, NULL, hThisInstance, NULL
    );

    hEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
        10, 10, 260, 40, hwnd, NULL, hThisInstance, NULL
    );

    const char* buttons[] = {
        "7", "8", "9", "/", 
        "4", "5", "6", "*", 
        "1", "2", "3", "-", 
        "0", "C", "=", "+"
    };

    for (int i = 0; i < 16; ++i) {
        CreateWindow(
            "BUTTON", buttons[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10 + (i % 4) * 65, 60 + (i / 4) * 65, 60, 60,
            hwnd, (HMENU)(i + 1), hThisInstance, NULL
        );
    }

    ShowWindow(hwnd, nFunsterStil);

    while (GetMessage(&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            if (wmId >= 1 && wmId <= 16) {
                const char* buttons[] = {
                    "7", "8", "9", "/", 
                    "4", "5", "6", "*", 
                    "1", "2", "3", "-", 
                    "0", "C", "=", "+"
                };

                std::string button = buttons[wmId - 1];
                HandleButtonPress(button);
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}