#include <Windows.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>

std::ofstream logFile("log.txt");

const int nScreenWidth = 120;
const int nScreenHeight = 40;
const int FPS = 10;
const int nFrameDuration = 1000 / FPS; // Duration of each frame in milliseconds


class Snake
{
private:
    std::vector<std::pair<int, int>> parts;
    std::pair<int, int> direction;
public:
    Snake()
    {
        parts.push_back({ nScreenWidth / 2, nScreenHeight / 2 });
        direction = { 1, 0 };
    }

    std::pair<int, int> getHead()
    {
        return this->parts.back();
    }

    std::pair<int, int> getTail()
    {
        return this->parts.front();
    }

    void setDirection(std::pair<int, int> newDir) 
    {
        if (this->direction.first == -1 * newDir.first && this->direction.second == -1 * newDir.second)
        {
            return;
        }

        this->direction = newDir;
    }

    void move()
    {
        int newX = this->parts.back().first + this->direction.first;
        int newY = this->parts.back().second + this->direction.second;

        parts.erase(parts.begin());
        parts.push_back({ newX, newY });
    }

    std::vector<std::pair<int, int>> getParts()
    {
        return this->parts;
    }

    void insertPart()
    {
        std::pair<int, int> tailPos = this->getTail();
        std::pair<int, int> insertDir = { -1 * this->direction.first, -1 * this->direction.second };
        this->parts.insert(this->parts.begin(), { tailPos.first + insertDir.first, tailPos.second + insertDir.second });
    }

    // Returns true if point is part of a snake
    bool pointBelongs(std::pair<int, int> point)
    {
        return std::find(this->parts.begin(), this->parts.end(), point) != this->parts.end();
    }
};

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    WORD* attributes = new WORD[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Set up random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distr(0, nScreenWidth * nScreenHeight - 2);

    // Set up time
    auto time1 = std::chrono::system_clock::now();
    auto time2 = std::chrono::system_clock::now();

    Snake snake = Snake();

    // Generate apple
    int nRandIndex = distr(gen);
    std::pair<int, int> applePos = { nRandIndex / nScreenWidth, nRandIndex % nScreenHeight };
    while (snake.pointBelongs(applePos)) 
    {
        nRandIndex = distr(gen);
        applePos = { nRandIndex / nScreenWidth, nRandIndex % nScreenWidth };
    }

    while (true)    
    {   
        // Control frame rate 
        time2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = time2 - time1;
        time1 = time2;
        float fElapsedTime = elapsedTime.count();
        int nSleepTime = nFrameDuration - static_cast<int>(fElapsedTime);
        if (nSleepTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
        }

        // read

        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) 
        {
            snake.setDirection({0, -1});
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) 
        {
            snake.setDirection({0, 1});
        }
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) 
        {
            snake.setDirection({-1, 0});
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) 
        {
            snake.setDirection({1, 0});
        }

        // update
        snake.move();
        std::pair<int, int> snakeHead = snake.getHead();
        if (snakeHead.first < 0 || snakeHead.first >= nScreenWidth || snakeHead.second < 0 || snakeHead.second >= nScreenHeight)
        {
            break;
        }

        if (screen[snakeHead.second * nScreenWidth + snakeHead.first] == '#')
        {
            break;
        }

        if (screen[snakeHead.second * nScreenWidth + snakeHead.first] == 'A')
        {
            snake.insertPart();

            // Generate new apple
            nRandIndex = distr(gen);
            applePos = { nRandIndex / nScreenWidth, nRandIndex % nScreenHeight };
            while (snake.pointBelongs(applePos)) 
            {
                nRandIndex = distr(gen);
                applePos = { nRandIndex / nScreenWidth, nRandIndex % nScreenWidth };
            }
        } 

        // render
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        {
            screen[i] = ' ';
            attributes[i] = 0; // Black
        }

        std::vector<std::pair<int, int>> snakePos = snake.getParts();
        for(auto [x, y]: snakePos)
        {
            screen[y * nScreenWidth + x] = '#';
            attributes[y * nScreenWidth + x] = FOREGROUND_GREEN;
        }
        attributes[snakeHead.second * nScreenWidth + snakeHead.first] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        
        screen[applePos.second * nScreenWidth + applePos.first] = 'A';
        attributes[applePos.second * nScreenWidth + applePos.first] = FOREGROUND_RED | FOREGROUND_INTENSITY;

        // Draw
        DWORD dwBytesWritten;
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
        WriteConsoleOutputAttribute(hConsole, attributes, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }

    // Show Game Over message
    std::wstring endMessage = L"You lost. Snake length: %d. Press E to exit.";
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        screen[i] = ' ';
        attributes[i] = 0; // Black
    }

    swprintf_s(screen, endMessage.size(), L"You lost. Snake length: %d. Press E to exit.", snake.getParts().size());

    for (int i = 0; i < endMessage.size(); i++) {
        attributes[i] = FOREGROUND_GREEN;
    }

    DWORD dwBytesWritten;
    while (!(GetAsyncKeyState((unsigned short)'E') & 0x8000))
    {
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
        WriteConsoleOutputAttribute(hConsole, attributes, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }
}