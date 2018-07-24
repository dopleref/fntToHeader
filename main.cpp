#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    system("chcp 1251");

    cout << "<fntToHeader converter>" << endl;

    ifstream fin("font.fnt");
    ofstream fout("font.h", ios_base::trunc);

    if (!fin.is_open()) {
        cout << "Не найден входной файл!\n";
    }

    // формирование объявления типов
    fout << "typedef struct TCommonFontSize {\n";
    fout << "    uint8_t lineHeight;\n";
    fout << "    uint8_t base;\n";
    fout << "    uint16_t scaleW;\n";
    fout << "    uint16_t scaleH;\n";
    fout << "} CommonFontSize;\n\n";

    fout << "typedef struct TCharSize {\n";
    fout << "    uint16_t id;\n";
    fout << "    uint16_t x;\n";
    fout << "    uint16_t y;\n";
    fout << "    uint16_t height;\n";
    fout << "    uint16_t xoffset;\n";
    fout << "    uint16_t yoffset;\n";
    fout << "    uint16_t xadvance;\n";
    fout << "} CharSize;\n\n";

    fout << "typedef struct TKerning {\n";
    fout << "    uint16_t first;\n";
    fout << "    uint16_t second;\n";
    fout << "    uint16_t amount;\n";
    fout << "} Kerning;\n\n";

    string str;
    vector<string> commonGoals {
        "common",
        "lineHeight",
        "base",
        "scaleW",
        "scaleH"
    };
    size_t i = 0;
    size_t length;
    // заполнение переменной commonFontSize
    fout << "CommonFontSize commonFontSize = {";
    while (!fin.eof() && i < 5) {
        fin >> str;
        if (str.find(commonGoals[i]) != std::string::npos) {
            //cout << str << endl;
            //current = goal;
            if (i > 0) {
                length = commonGoals[i].length() + 1;
                str.erase(0, length);
                //cout << str << endl;
                fout << str;
                if (i < 4) {
                    fout << ", ";
                }
            }
            i++;
        }
    }
    fout << "};\n\n";

    // заполнение переменной charSize
    fout << "CharSize charSize[";
    while (!fin.eof()) {
        fin >> str;
        if (str.find("chars") != std::string::npos) {
            fin >> str;
            if (str.find("count") != std::string::npos) {
                str.erase(0, 6);
                fout << str;
            }
            break;
        }
    }
    fout << "] = {\n";
    i = 0;
    vector<string> charGoals {
        "char",
        "id",
        "x",
        "y",
        "width",
        "height",
        "xoffset",
        "yoffset",
        "xadvance"
    };
    int value;
    while (!fin.eof() && str != "kernings") {
        fin >> str;
        if (str.find(charGoals[i]) != std::string::npos) {
            //cout << str << endl;
            //current = goal;
            if (i == 0) {
                fout << "    {";
            }
            if (i > 0) {
                length = charGoals[i].length() + 1;
                str.erase(0, length);
                //cout << str << endl;
                if (i == 1) {
                    value = std::stoi(str);
                    if (value > 1000) {
                        value = value - 0x410 + 0xC0;
                    }
                    fout << value;
                }
                else {
                    fout << str;
                }

                if (i < 8) {
                    fout << ", ";
                }
            }
            i++;
            if (i == 9) {
                i = 0;
                fout << "},\n";
            }
        }
    }
    fout << "};\n\n";

    // заполнение переменной kerning
    fout << "Kerning kerning[";
    while (!fin.eof()) {
        fin >> str;
        if (str.find("count") != std::string::npos) {
            str.erase(0, 6);
            fout << str;
            break;
        }
    }
    fout << "] = {\n";
    i = 0;
    vector<string> kerningGoals {
        "kerning",
        "first",
        "second",
        "amount"
    };
    while (!fin.eof()) {
        fin >> str;
        if (str.find(kerningGoals[i]) != std::string::npos) {
            //cout << str << endl;
            //current = goal;
            if (i == 0) {
                fout << "    {";
            }
            if (i > 0) {
                length = kerningGoals[i].length() + 1;
                str.erase(0, length);
                //cout << str << endl;
                //fout << str;
                if (i == 1 || i == 2) {
                    value = std::stoi(str);
                    if (value > 1000) {
                        value = value - 0x410 + 0xC0;
                    }
                    fout << value;
                }
                else {
                    fout << str;
                }
                if (i < 3) {
                    fout << ", ";
                }
            }
            i++;
            if (i == 4) {
                i = 0;
                fout << "},\n";
            }
        }
    }
    fout << "};\n\n";

    // закрытие файлов
    fin.close();
    fout.close();

    return 0;
}
