#include <iostream>
#include "color_console.hpp" //https://github.com/memoryInject/color-console
#include <vector>
#include <fstream>
class app
{
private:
    std::vector<std::string> tasks;
    std::ifstream fin;
    std::ofstream fout;

public:
    void start()
    {
        fin.open("tasks.txt");
        std::string row;
        while (fin >> row)
            tasks.push_back(row);
        fin.close();
    }
    void add_task(std::string task)
    {
        tasks.push_back(task);
        fout.open("tasks.txt");
        fout << task << '\n';
        fout.close();
    }
    app()
    {
        start();
        system("clear");
        
        std::cout << "hi";
    }
};
int main()
{
    app a;
    return 0;
}