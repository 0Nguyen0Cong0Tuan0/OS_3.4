#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

const int SIZE = 128;
const int BLOCK_SIZE = 1024;
const string KEY = "Operating System";

// vector<string> ReadBlock(string filename) 
// {
//     ifstream file(filename, ios::binary);
//     vector<string> blocks;

//     if (!file.is_open()) 
//     {
//         cout << "Failed to open file: " << filename << endl;
//         return blocks;
//     }

//     char buffer[BLOCK_SIZE];
//     while (file.read(buffer, BLOCK_SIZE)) 
//         blocks.emplace_back(buffer, BLOCK_SIZE);
    

//     if (file.gcount() > 0) 
//         blocks.emplace_back(buffer, file.gcount());
    
//     file.close();
//     return blocks;
// }

// void listFile(string filename) 
// {
//     vector<string> blocks = ReadBlock(filename);
//     int fileNumber = 1;

//     for (int i = 0; i < blocks.size(); i++)
//     {
//         int count = 0;
//         int tmp = blocks[i].size() / SIZE;
//         int number = 0;
//         while (number != tmp)
//         {
//             string entry = blocks[i].substr(count, SIZE);
//             string data = entry.substr(0, 60);
//             cout << "File #" << fileNumber << " name: ";
//             for (int i = 0; i < 60; i+=2)
//             {
//                 string temp = data.substr(i, 2);
//                 int value = stoul(temp, nullptr, 16);
//                 cout << char(value);
//             }
//             cout << endl;

//             count += SIZE;
//             number += 1;
//             fileNumber += 1;
//         }
//     }
// }

// void readPassword(string filename) 
// {
//     vector<string> blocks = ReadBlock(filename);
//     int fileNumber = 1;
//     for (int i = 0; i < blocks.size(); i++)
//     {
//         int count = 0;
//         int tmp = blocks[i].size() / SIZE;
//         int number = 0;
//         while (number != tmp)
//         {
//             string entry = blocks[i].substr(count, SIZE);
//             string data = entry.substr(SIZE - 32, 32);
//             cout << "File #" << fileNumber << " password: ";

//             string password = "";
        
//             for (int i = 0; i < 32; i+=2)
//             {
//                 string temp = data.substr(i, 2);
//                 int value = stoul(temp, nullptr, 16);
//                 password += char(value);
//             }
//             cout << decrypt(password) << endl;

//             count += SIZE;
//             number += 1;
//             fileNumber += 1;
//         }
//     }
// }

void changeVolumePassword(string filename)
{
    ifstream fin(filename, ios::binary); // Open the file with the given filename

    fin.seekg(18); // Move the file pointer to the 19th byte (index 18)

    std::string pass; // String to store the password
    pass.resize(8); // Resize the string to accommodate 8 characters

    fin.read(&pass[0], 8); // Read 8 characters (the password) from the file

    std::cout << "Volume Password: " << pass << std::endl; // Output the password

    fin.close(); // Close the file
}

int main() {
    string filename = "input.bin";
    changeVolumePassword(filename);
    // listFile(filename);
    // readPassword(filename);
    //changePassword(filename);
    //readPassword(filename);
    return 0;
}
