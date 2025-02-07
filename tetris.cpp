#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>
#include <ctime>

using namespace std;

int which_style_of_game = 3; // 1. 1 = # 0 = 0 // 2. 1 = 1 0 = 0 // 3. ascii full blocks
const int height = 20;
const int width = 10;
int main_array[height][width];
int current_x = 0;
int current_y = 0;
int speed_of_falling_ms = 800;
int speed_of_falling_ms_help_variable = 800;
int colour = 3;
int degrees = 0;
int counter_to_fill_array = 1;

int** current_figure;



int f1[1][5] = { 1, 3, 1, 1, 1 }; // pozioma rurka 3
int f2[5][1] = { {3}, {1}, {1}, {1}, {1} }; // pionowa rurka 3
int f3[2][5] = { {2, 3, 1, 1, 1}, {0, 0, 1, 0, 0} }; // jakies gowno
int f4[2][4] = { {2, 2, 1, 1}, {0, 0, 1, 1} }; // kwadrat 2x2
int f5[3][5] = { {3, 3, 1, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 1, 1, 1} };
int f6[2][5] = { {2, 3, 0, 1, 0}, {0, 0, 1, 1, 1} }; // jakies gowno
int f7[2][4] = { {2, 2, 0, 1}, {0, 0, 1, 1, } };
int f8[2][4] = { {2, 2, 1, 0}, {0, 0, 1, 1, } };
int f9[2][4] = { {2, 2, 1, 1}, {0, 0, 0, 1, } };
int f10[2][4] = { {2, 2, 1, 1}, {0, 0, 1, 0, } };
int f11[3][4] = { {3, 2, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1} };
int f12[3][4] = { {3, 2, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 1} };
int f13[2][5] = { {2, 3, 1, 1, 0}, {0, 0, 0, 1, 1} };


vector <int*> pointers = { &f1[0][0], &f2[0][0], &f3[0][0], &f4[0][0], &f5[0][0], &f6[0][0],
    &f7[0][0], &f8[0][0], &f9[0][0], &f10[0][0], &f11[0][0], &f12[0][0], &f13[0][0] };

vector <int> last_indexes;
vector <int> colour_for_each_number;

int rows = 0;
int cols = 0;
int* p = pointers[0];
int* copy_of_p = p;
int points = 80;
bool is_game_fullscreen = false;
bool chuj = true;


bool if_it_felt = true;
bool if_lose = false;
bool if_play_again = true;

int consoleWidthFromChatGPT()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi; //zmienna jak cos xd
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return -1;
}
void fillinArray()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            main_array[i][j] = 0;
        }
    }

}
void coutWidtColourFromChatGPT(int colour, string text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colour);
    std::cout << text;
    SetConsoleTextAttribute(hConsole, 7);
}
void coutWidtColourFromChatGPT(int colour, char text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colour);
    std::cout << text;
    SetConsoleTextAttribute(hConsole, 7);
}
void justColourChange(int colour)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colour);
}
void changeWritingLinePosition(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}
void mainTitle()
{
    int console_width = consoleWidthFromChatGPT();
    string title = "tetris czy cos xd";
    for (int i = 0; i < (console_width / 2) - (title.length() / 2); i++)
    {
        std::cout << " ";
    }
    coutWidtColourFromChatGPT(5, title);
}
void writeFigure(int i, int j, bool temp = false)
{
    int console_width = consoleWidthFromChatGPT();
    if (which_style_of_game == 1 || which_style_of_game == 2)
    {
        changeWritingLinePosition((console_width / 2) - (width - 1) + (j * 2) + (current_x * 2), current_y + i + 2 + temp);
        if (main_array[current_y + i + temp][current_x + j]) (which_style_of_game == 1) ? std::cout << "#" : std::cout << "1";
        else std::cout << "0";

    }
    else
    {
        for (int c = 0; c < (is_game_fullscreen ? 2 : 1); c++) //c++ mentioned
        {
            changeWritingLinePosition(is_game_fullscreen + (console_width / 2) - (width - 1) + (j * (2 + (is_game_fullscreen * 2))) + (current_x * 2), (current_y * (is_game_fullscreen ? 2 : 1)) + i + 2 + temp);
            for (int k = 0; k < 2 + (is_game_fullscreen * 2); k++)
            {
                coutWidtColourFromChatGPT((main_array[current_y + i + temp][current_x + j]) ? colour : 7, 219);

            }
        }
    }


}
void writeAnArray()
{
    int console_width = consoleWidthFromChatGPT();

    if (which_style_of_game == 1 || which_style_of_game == 2)
    {
        for (int i = 0; i < height; i++)
        {
            for (int i = 0; i < (console_width / 2) - (width - 1); i++)
            {
                std::cout << " ";
            }
            for (int j = 0; j < width; j++)
            {
                
                (main_array[i][j] >= 1) ? which_style_of_game == 1 ? std::cout << "#" : std::cout << "1" : std::cout << "0";
                std::cout << " ";
            }
            std::cout << endl;
        }
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            for(int g = 0; g < is_game_fullscreen * 2 + !is_game_fullscreen; g++)
            {
                for (int i = 0; i < (console_width / 2) - ((width - 1) * !is_game_fullscreen) - (width * is_game_fullscreen * 2); i++)
                {
                    std::cout << " ";
                }
                for (int j = 0; j < width; j++)
                {

                    if (main_array[i][j] >= 1)
                    {
                        for (int k = 0; k < (!is_game_fullscreen ? 2 : 4); k++)
                        {
                            coutWidtColourFromChatGPT(colour_for_each_number[(main_array[i][j]) - 1], (char)(219));
                        }
                    }
                    else
                    {
                        for (int k = 0; k < (!is_game_fullscreen ? 2 : 4); k++)
                        {
                            std::cout << (char)219;
                        }

                    }

                }
                std::cout << endl;
            }
        }
    }
    changeWritingLinePosition(0, 0);
    justColourChange(3);
    std::cout << "liczba punktow to: " << points;
    justColourChange(7);
}
bool couldBeRotate()
{
    //int rotate figure cols = rows and rows == cols


    if (current_x + rows > width) return false;
    return true;

}
void move90Degrees(char key)
{
    if (!couldBeRotate()) return;
    swap(rows, cols);
    int** temp_array = new int* [rows];

    for (int i = 0; i < rows; i++)
    {
        temp_array[i] = new int[cols];
    }

    if (key == (char)75) //lewo
    {

        int counter = 0;
        for (int i = rows - 1; i >= 0; i--)
        {
            for (int j = 0; j < cols; j++)
            {
                temp_array[counter / cols][counter % cols] = current_figure[j][i];
                counter++;

            }
        }
    }
    else if (key == (char)77) //prawo
    {
        int counter = 0;

        for (int i = 0; i < rows; i++)
        {
            for (int j = cols - 1; j >= 0; j--)
            {
                temp_array[counter / cols][counter % cols] = current_figure[j][i];
                counter++;

            }
        }

    }
    int console_width = consoleWidthFromChatGPT();
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            main_array[current_y + i][current_x + j] = 0;
            if (which_style_of_game == 1 || which_style_of_game == 2)
            {
                changeWritingLinePosition((console_width / 2) + (current_x * 2) - (width - 1) + (j * 2), current_y + i + 2);
                std::cout << "0";
            }
            else if (which_style_of_game == 3)
            {
                changeWritingLinePosition((console_width / 2) + (current_x * 2) - (width - 1) + (j * 2), current_y + i + 2);
                coutWidtColourFromChatGPT(7, 219);
                changeWritingLinePosition((console_width / 2) + (current_x * 2) - (width - 1) + (j * 2) + 1, current_y + i + 2);
                coutWidtColourFromChatGPT(7, 219);
            }

        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!main_array[current_y + i][current_x + j])main_array[current_y + i][current_x + j] = temp_array[i][j] ? counter_to_fill_array : 0;
            writeFigure(i, j);
        }

    }
    for (int i = 0; i < cols; i++)
    {
        delete[] current_figure[i];

    }
    delete[]current_figure;

    current_figure = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        current_figure[i] = new int[cols];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            current_figure[i][j] = temp_array[i][j];
        }

    }
    last_indexes.clear();
    for (int i = 0; i < cols; i++)
    {
        for (int j = rows - 1; j >= 0; j--)
        {
            if (current_figure[j][i])
            {
                last_indexes.push_back(j);
                break;
            }

        }
    }


    for (int i = 0; i < rows; i++)
    {
        delete[] temp_array[i];
    }
    delete[] temp_array;

}
void destroyRow()
{
    int console_width = consoleWidthFromChatGPT();
    vector <int> rows_to_destroy;
    rows_to_destroy.clear();
    for (int i = 0; i < height; i++)
    {
        bool if_destroy = true;
        for (int j = 0; j < width && if_destroy; j++)
        {
            if (!main_array[i][j]) if_destroy = false;
        }
        if (if_destroy)
        {

            for (int j = 0; j < width; j++)
            {
                main_array[i][j] = 0;
            }
            rows_to_destroy.push_back(i);
        }
    }

    if (rows_to_destroy.size() > 0)
    {
        int lowest_to_destroy = rows_to_destroy[0];
        for (int i = 0; i < rows_to_destroy.size(); i++)
        {
            if (rows_to_destroy[i] > lowest_to_destroy) lowest_to_destroy = rows_to_destroy[i];
        }
        for (int i = lowest_to_destroy; i >= 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                for (int k = rows_to_destroy.size() - 1; k >= 0; k--)
                {
                    swap(main_array[i - k][j], main_array[i - k - 1][j]);
                }

            }

        }
        points += rows_to_destroy.size() * 10;

        if(which_style_of_game == 3)
        {
            
            for(int g = 0; g < is_game_fullscreen * 2 + !is_game_fullscreen; g++)
            {
                    
                for(int l = 0; l < 5; l++)
                {
                    
                    int colour_for_migawka_xd;
                    do
                    {
                        colour_for_migawka_xd = rand() % 16;
                    } while (colour_for_migawka_xd == 7);
                    for (int j = 0; j < width; j++)
                    {
                        for(int i = 0; i < rows_to_destroy.size(); i++)
                        {
                            changeWritingLinePosition(console_width / 2 - width + 1 + j*2 , rows_to_destroy[i] * (is_game_fullscreen ? 2 : 1) + g + 2);
                            for (int k = 0; k < (!is_game_fullscreen ? 2 : 4); k++)
                            {
                                coutWidtColourFromChatGPT(colour_for_migawka_xd, (char)219);

                            }
                        }
                    }
                    Sleep(110);
                    for (int j = 0; j < width; j++)
                    {
                        for (int i = 0; i < rows_to_destroy.size(); i++)
                        {
                            changeWritingLinePosition(console_width / 2 - width + 1 + j * 2, rows_to_destroy[i] * (is_game_fullscreen ? 2 : 1) + g + 2);
                            for (int k = 0; k < (!is_game_fullscreen ? 2 : 4); k++)
                            {
                                coutWidtColourFromChatGPT(7, (char)219);

                            }
                        }
                    }
                    Sleep(110);
                }
            }

            
        }
        system("cls");
        mainTitle();
        std::cout << endl << endl;
        writeAnArray();

        changeWritingLinePosition(0, 0);
        justColourChange(3);
        std::cout << "liczba punktow to: " << points;
        justColourChange(7);


    }


}
bool couldfall()
{
    for (int i = 0; i < cols; i++)
    {

        if (main_array[current_y + 1 + last_indexes[i]][current_x + i] != 0 || current_y + rows >= height) return false;

    }
    return true;

}
void takeItDown()
{
    int console_width = consoleWidthFromChatGPT();
    changeWritingLinePosition(console_width / 2 - (width - 1) + current_x, current_y + 2);
    changeWritingLinePosition(0, 0);

    if (couldfall())
    {

        for (int i = 0; i < cols; i++)
        {
            for (int j = rows - 1; j >= 0; j--)
            {
                if (main_array[current_y + j][current_x + i])
                {
                    swap(main_array[current_y + j][current_x + i], main_array[current_y + j + 1][current_x + i]);
                }
            }
        }

        for (int i = 0; i < cols; i++)
        { 
            for (int j = rows - 1; j >= 0; j--)
            {
                if (main_array[current_y + j + 1][current_x + i] == current_figure[j][i])
                {
                    writeFigure(j, i, 1);
    
                }

            }

        }
        for (int i = 0; i < cols; i++)
        {
            if (which_style_of_game == 1 || which_style_of_game == 2)
            {
                changeWritingLinePosition(console_width / 2 - (width - 1) + (current_x * 2) + (i * 2), current_y + 2);
                std::cout << "0";
            }
            else if (which_style_of_game == 3)
            {
                for (int k = 0; k < 2 + (is_game_fullscreen * 2); k++)
                {
                    changeWritingLinePosition(console_width / 2 - (width - 1) + (current_x * 2) + (i * 2) + k, current_y + 2);
                    coutWidtColourFromChatGPT(7, 219);

                }
                

            }

        }


    }
    changeWritingLinePosition(0, 0);

}
void allFallsDown()
{
    static int counter = 0;

    if (counter >= speed_of_falling_ms / 10)
    {
        if (speed_of_falling_ms >= 300)
        {
            speed_of_falling_ms = 800 - (points / 2);
            speed_of_falling_ms_help_variable = speed_of_falling_ms;
        }
        takeItDown();
        current_y++;
        counter = 0;
        if (!couldfall())
        {
            takeItDown();
            destroyRow();
            current_y = 0;
            current_x = 0;
            if_it_felt = true;
        }

    }
    counter++;
}
bool ifCouldBeTakenLeftOrRight(char key)
{
    switch (key)
    {
    case 'd':

        if (width <= current_x + cols) return false;
        for (int i = 0; i < rows; i++)
        {
            int last_one_index = -1;
            for (int j = cols - 1; j >= 0; j--)
            {
                if (main_array[current_y + i][current_x + j] >= 1)
                {
                    last_one_index = j;
                    break;
                }
            }
            if (last_one_index != -1)
            {
                if (main_array[current_y + i][current_x + last_one_index + 1] >= 1) return false;
            }

        }
        return true;
        break;
    case 'a':
        if (current_x == 0) return  false;
        for (int i = 0; i < rows; i++)
        {
            int first_one_index = -1;
            for (int j = 0; j < cols; j++)
            {
                if (main_array[current_y + i][current_x + j] >= 1)
                {
                    first_one_index = j;
                    break;
                }
            }
            if (first_one_index != -1)
            {
                if (main_array[current_y + i][current_x + first_one_index - 1] >= 1) return false;
            }

        }

    }


}
void takeItLeftOrRight(char key)
{
    int console_width = consoleWidthFromChatGPT();
    if (ifCouldBeTakenLeftOrRight(key))
    {
        switch (key)
        {
        case 'd':

            for (int i = 0; i < rows; i++)
            {
                for (int j = cols - 1; j >= 0; j--)
                {
                    if (main_array[current_y + i][current_x + j])
                    {
                        swap(main_array[current_y + i][current_x + j], main_array[current_y + i][current_x + j + 1]);
                    }
                }

            }
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    switch (which_style_of_game)
                    {
                    case 1:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + ((current_x * 2) + (j * 2) + 2), current_y + i + 2);
                        if (main_array[current_y + i][current_x + j + 1]) std::cout << "#";
                        else std::cout << "0";
                        break;

                    case 2:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + ((current_x * 2) + (j * 2) + 2), current_y + i + 2);
                        if (main_array[current_y + i][current_x + j + 1]) std::cout << "1";
                        else std::cout << "0";
                        break;
                    case 3:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + (j * 2) + 2, current_y + i + 2);
                        coutWidtColourFromChatGPT(main_array[current_y + i][current_x + j + 1] ? colour_for_each_number[(main_array[current_y + i][current_x + j + 1]) - 1] : 7, 219);
                        changeWritingLinePosition((console_width / 2) - (width - 1) + current_x * 2 + (j * 2) + 3, current_y + i + 2);
                        coutWidtColourFromChatGPT(main_array[current_y + i][current_x + j + 1] ? colour_for_each_number[main_array[current_y + i][current_x + j + 1] - 1] : 7, 219);
                        break;

                    }

                }
            }

            for (int i = 0; i < rows; i++)
            {

                if (which_style_of_game == 1 || which_style_of_game == 2)
                {
                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2), current_y + i + 2);
                    std::cout << "0";
                }
                else if (which_style_of_game == 3)
                {
                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2), current_y + i + 2);
                    coutWidtColourFromChatGPT(7, 219);
                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + 1, current_y + i + 2);
                    coutWidtColourFromChatGPT(7, 219);
                }

            }
            current_x++;
            break;
        case 'a':
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    if (main_array[current_y + i][current_x + j])
                    {
                        swap(main_array[current_y + i][current_x + j], main_array[current_y + i][current_x + j - 1]);
                    }
                }

            }
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    switch (which_style_of_game)
                    {
                    case 1:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + ((current_x * 2) + (j * 2) - 2), current_y + i + 2);
                        if (main_array[current_y + i][current_x + j - 1]) std::cout << "#";
                        else std::cout << "0";
                        break;

                    case 2:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + ((current_x * 2) + (j * 2) - 2), current_y + i + 2);
                        if (main_array[current_y + i][current_x + j - 1]) std::cout << "1";
                        else std::cout << "0";
                        break;
                    case 3:
                        changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + (j * 2) - 1, current_y + i + 2);
                        coutWidtColourFromChatGPT(main_array[current_y + i][current_x + j - 1] ? colour_for_each_number[(main_array[current_y + i][current_x + j - 1]) - 1] : 7, 219);
                        changeWritingLinePosition((console_width / 2) - (width - 1) + current_x * 2 + (j * 2) - 2, current_y + i + 2);
                        coutWidtColourFromChatGPT(main_array[current_y + i][current_x + j - 1] ? colour_for_each_number[main_array[current_y + i][current_x + j - 1] - 1] : 7, 219);
                        break;

                    }

                }
            }
            for (int i = 0; i < rows; i++)
            {

                if (which_style_of_game == 1 || which_style_of_game == 2)
                {
                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + cols * 2 - 2, current_y + i + 2);
                    std::cout << "0";
                }
                else if (which_style_of_game == 3)
                {
                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + cols * 2 - 1, current_y + i + 2);
                    coutWidtColourFromChatGPT(7, 219);

                    changeWritingLinePosition((console_width / 2) - (width - 1) + (current_x * 2) + cols * 2 - 2, current_y + i + 2);
                    coutWidtColourFromChatGPT(7, 219);

                }

            }
            current_x--;
            break;


        }
        changeWritingLinePosition(0, 0);

    }

}
void mainInFunc()
{
    if (if_it_felt)
    {
        counter_to_fill_array++;


        srand(time(NULL));
        p = pointers[rand() % pointers.size()];
        rows = *p;
        cols = *(p + 1);
        copy_of_p = p;
        do
        {
            colour = rand() % 16;
        } while (colour == 7);
        colour_for_each_number.push_back(colour);
        current_figure = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            current_figure[i] = new int[cols];
        }

        copy_of_p += 2;
        for (int i = 0; i < rows; i++)
        {

            for (int j = 0; j < cols; j++)
            {
                current_figure[i][j] = (*copy_of_p) ? counter_to_fill_array : 0;
                if (!main_array[i][j])
                {
                    main_array[i][j] = (*copy_of_p) ? counter_to_fill_array : 0;
                    copy_of_p++;
                }
                else
                {
                    if_lose = true;
                }
                if (main_array[i][j]) points++;

            }
            copy_of_p += (2 * (cols != 1));


        }
        copy_of_p = p;

        last_indexes.clear();
        for (int i = 0; i < cols; i++)
        {
            for (int j = rows - 1; j >= 0; j--)
            {
                if (current_figure[j][i])
                {
                    last_indexes.push_back(j);
                    break;
                }

            }
        }
        int console_width = consoleWidthFromChatGPT();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                writeFigure(i, j);

            }

        }

        if_it_felt = false;
        changeWritingLinePosition(0, 0);
        justColourChange(3);
        std::cout << "liczba punktow to: " << points;
        justColourChange(7);
    }

    allFallsDown();


}


int main()
{
    writeAnArray();
    while (if_play_again)
    {

        srand(time(NULL));
        p = pointers[rand() % pointers.size()];
        rows = *p;
        cols = *(p + 1);
        do
        {
            colour = rand() % 16;
        } while (colour == 7);
        colour_for_each_number.push_back(colour);
        current_figure = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            current_figure[i] = new int[cols];
        }

        p += 2;
        for (int i = 0; i < rows; i++)
        {

            for (int j = 0; j < cols; j++)
            {
                current_figure[i][j] = *p;
                if (!main_array[i][j])
                {
                    main_array[i][j] = (*p) ? counter_to_fill_array : 0;
                    p++;
                }
                else
                {
                    if_lose = true;
                }
                if (main_array[i][j]) points++;


            }
            p += (2 * (cols != 1));

        }
        p = 0;

        last_indexes.clear();
        for (int i = 0; i < cols; i++)
        {
            for (int j = rows - 1; j >= 0; j--)
            {
                if (current_figure[j][i])
                {
                    last_indexes.push_back(j);
                    break;
                }

            }
        }

        system("cls");
        mainTitle();
        std::cout << endl << endl;
        writeAnArray();
        changeWritingLinePosition(0, 0);
        justColourChange(3);
        std::cout << "liczba punktow to: " << points;
        justColourChange(7);

        if_it_felt = false;


        while (!if_lose)
        {
            mainInFunc();

            char key;


            while (_kbhit())
            {

                key = _getch();
                if (key == '1' || key == '2' || key == '3')
                {
                    which_style_of_game = (int)key - 48;
                    system("cls");
                    mainTitle();
                    std::cout << endl << endl;
                    writeAnArray();

                }
                if (key == 's')
                {
                    speed_of_falling_ms = 37;
                    mainInFunc();
                    allFallsDown();
                    Sleep(5);


                }
                if (key == 'f')
                {
                    system("cls");
                    int console_width = consoleWidthFromChatGPT();
                    changeWritingLinePosition(console_width / 2 - 30, 0);
                    cout << "upewnij sie ze kliknales f11 bo nie umiem tego sprawdzic xd";
                    changeWritingLinePosition(console_width / 2 - 28, 1);
                    cout << "kliknij 't' jesli chcesz wyjsc lub wejsc do fullscreena";
                    char key1;
                    while (true)
                    {
                        if(_kbhit())
                        {
                            key1 = _getch();
                            if (key1 == 't')
                            {
                                system("cls");
                                changeWritingLinePosition(console_width / 2 - 6, 0);
                                cout << "przelaczam";
                                is_game_fullscreen = !is_game_fullscreen;

                            }
                            else
                            {
                                system("cls");
                                changeWritingLinePosition(console_width / 2 - 8, 0);
                                cout << "nie przelaczam";
                            }

                            Sleep(1000);
                            system("cls");
                            mainTitle();
                            std::cout << endl << endl;
                            writeAnArray();
                            
                            break;
                        }

                    }

                }
                if (key == 'r')
                {
                    string text = "dostosowywuje wielkosc";
                    int console_width = consoleWidthFromChatGPT() / 2 - (text.length() / 2);
                    system("cls");
                    changeWritingLinePosition(console_width, 0);
                    std::cout << text;
                    Sleep(1500);
                    system("cls");
                    mainTitle();
                    std::cout << endl << endl;
                    writeAnArray();

                }
                if (key == 'c')
                {
                    bool is_already_written = false;
                    string comand;
                    changeWritingLinePosition(0, 0);
                    std::cout << "poodaj komende: \nhelp - pomoc";
                    cin >> comand;
                    while (comand == "help")
                    {
                        if (!is_already_written)
                        {
                            system("cls");
                            int console_width = consoleWidthFromChatGPT() / 2;
                            changeWritingLinePosition(console_width - 9, 0);
                            std::cout << "lista komend to: ";
                            changeWritingLinePosition(console_width - 16, 1);
                            std::cout << "1, 2, 3 - rozne tryby graficzne ";
                            changeWritingLinePosition(console_width - 16, 2);
                            std::cout << "r - reset polozenia konsoli gry";
                            changeWritingLinePosition(console_width - 18, 3);
                            std::cout << "a, d - przesuniecie w prawo i w lewo";
                            changeWritingLinePosition(console_width - 10, 4);
                            std::cout << "s - szybsze spadanie";
                            changeWritingLinePosition(console_width - 13, 5);
                            std::cout << "c - konsola chyba wiesz xd";
                            changeWritingLinePosition(console_width - 14, 6);
                            std::cout << "kliknij cokolwiek zeby wyjsc";
                            is_already_written = true;
                        }
                        if (_kbhit())
                        {
                            system("cls");
                            mainTitle();
                            std::cout << endl << endl;
                            writeAnArray();
                        }


                    }



                }
                if (key == 'p')
                {
                    int chuj;
                    std::cin >> chuj;
                }
                if (key == 'a' || key == 'd')
                {

                    takeItLeftOrRight(key);


                }
                if (key == (char)224)
                {
                    key = _getch();
                    if (key == 75 || key == 77)move90Degrees(key);


                }



            }

            speed_of_falling_ms = speed_of_falling_ms_help_variable;


            Sleep(10);
        }

        system("cls");
        int console_wdith = consoleWidthFromChatGPT();
        changeWritingLinePosition((console_wdith / 2) - (10 / 2) - 1, 0);
        std::cout << "no i co przegrales";
        changeWritingLinePosition((console_wdith / 2) - (22 / 2) - 1, 1);
        std::cout << "jesli chcesz grac dalej nacisnij cokolwiek";
        changeWritingLinePosition((console_wdith / 2) - (10 / 2) - 1, 2);
        std::cout << "jesli nie nacisnij n";


        char key_1;

        key_1 = _getch();
        if (key_1 == 'n')
        {
            system("cls");
            changeWritingLinePosition((console_wdith / 2) - (8 / 2) - 1, 0);
            std::cout << "dzieki za gre";
            if_play_again = false;
            Sleep(1500);
        }
        else if (key_1 != 'n')
        {
            system("cls");
            changeWritingLinePosition((console_wdith / 2) - (16 / 2) - 1, 0);
            std::cout << "odpalam gre";
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    changeWritingLinePosition(console_wdith / 2 + 16 / 2 - 6 + j * 2, 0);
                    std::cout << ". ";
                    Sleep(700);
                }
                for (int j = 0; j < 6; j++)
                {
                    changeWritingLinePosition(console_wdith / 2 + 16 / 2 - 6 + j, 0);
                    std::cout << (char)32;
                }
                changeWritingLinePosition(console_wdith / 2 + 16 / 2 - 6, 0);
                Sleep(700);
            }
            system("cls");
            Sleep(100);
            fillinArray();

            counter_to_fill_array = 1;
            colour_for_each_number.clear();
            current_x = 0;
            current_y = 0;
            speed_of_falling_ms = speed_of_falling_ms_help_variable;
            points = 0;
            if_lose = false;


        }


    }




    return 0;
}

