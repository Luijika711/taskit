#include <iostream>
#include "color_console.hpp" //https://github.com/memoryInject/color-console
#include <vector>
#include <fstream>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
namespace in
{
    static struct termios old, current;
    void initTermios(int echo)
    {
        tcgetattr(0, &old);
        current = old;
        current.c_lflag &= ~ICANON;
        if (echo)
        {
            current.c_lflag |= ECHO;
        }
        else
        {
            current.c_lflag &= ~ECHO;
        }
        tcsetattr(0, TCSANOW, &current);
    }
    void resetTermios(void)
    {
        tcsetattr(0, TCSANOW, &old);
    }
    char getch_(int echo)
    {
        char ch;
        initTermios(echo);
        ch = getchar();
        resetTermios();
        return ch;
    }
    char getch(void)
    {
        return getch_(0);
    }
    char getche(void)
    {
        return getch_(1);
    }
}
class app
{
private:
    std::vector<std::string> tasks;
    std::ifstream fin;
    std::ofstream fout;
    const char *hide = "\e[?25l";
    struct winsize w;

public:
    void start()
    {
        fin.open("tasks.txt");
        std::string row;
        while (std::getline(fin, row))
        {
            tasks.push_back(row);
        }
        fin.close();
    }
    void add_task(std::string task)
    {
        tasks.push_back(task);
        fout.open("tasks.txt");
        fout << task << '\n';
        fout.close();
    }
    void render_screen()
    {
        system("clear");
        std::cout << "tasks : \n";
        for (int i = 0; i < tasks.size(); ++i)
        {
            std::cout << i + 1 << "." + tasks[i] << '\n';
        }
        printf(hide);
        for (int i = tasks.size() + 1; i <= w.ws_row - 3; ++i)
        {
            if (i == w.ws_row - 3)
                for (int j = 0; j < w.ws_col; ++j)
                    std::cout << '-';
            else
                std::cout << "\n";
        }
        std::cout << "asdf";
    }
    app()
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        start();
        render_screen();
        while (char ch = in::getch())
        {
            render_screen();
        }
    }
};
int main()
{
    app a;
    return 0;
}