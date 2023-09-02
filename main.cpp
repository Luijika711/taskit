#include <iostream>
#include "color_console.hpp" //https://github.com/memoryInject/color-console
#include <vector>
#include <fstream>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define ENTER_KEY 10
#define BACKSPACE_KEY 127
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
    int selected_index = 0;

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
        if (selected_index == 0)
            console.print("tasks:\n", {console.bg_dark_gray});
        else
            std::cout << "tasks:\n";
        for (int i = 0; i < tasks.size(); ++i)
        {
            std::string str = std::to_string(i + 1) + "." + tasks[i] + "\n";
            if (selected_index == i + 1)
                console.print(str, {console.green, console.bg_dark_gray});
            else
                console.print(str, {console.green});
        }
        printf(hide);
        for (int i = tasks.size() + 1; i <= w.ws_row - 3; ++i)
        {
            if (i == w.ws_row - 3)
                for (int j = 0; j < w.ws_col; ++j)
                    console.print(" ", {console.bg_dark_gray});
            else
                std::cout << "\n";
        }
        std::cout << "press w/s to go up/down | press enter to edit selected task | press backspace to delete selected task";
    }
    void edit_mode_render(std::string &str)
    {
        system("clear");
        std::cout << str;
        while (char ch = in::getch())
        {

            if (ch == BACKSPACE_KEY && str.size())
            {
                str.pop_back();
                system("clear");
                std::cout << str;
                continue;
            }

            if (ch == ENTER_KEY)
            {
                fout.open("tasks.txt", std::ofstream::out | std::ofstream::trunc);
                fout.close();
                fout.open("tasks.txt");
                for (auto &it : tasks)
                    fout << it << '\n';
                fout.close();
                return;
            }
            str.push_back(ch);
            system("clear");
            std::cout << str;
        }
    }
    app()
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        start();
        render_screen();
        while (char ch = in::getch())
        {
            std::cout << (int)ch << '\n';
            if (ch == 's' && selected_index <= tasks.size() - 1)
            {
                selected_index++;
            }
            if (ch == 'w' && selected_index > 0)
            {
                selected_index--;
            }
            if (ch == ENTER_KEY && selected_index != 0)
            {
                edit_mode_render(tasks[selected_index - 1]);
            }

            render_screen();
        }
    }
};
int main()
{
    app a;
    return 0;
}