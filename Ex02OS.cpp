#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <cstring>

using namespace std;

const int BLOCK_SIZE = 512; // Kích thước của mỗi cụm dữ liệu (block)
const int SIZE = 128;
const string KEY = "Operating System";

// Cấu trúc một tập tin trong MyFS
class File {
    public:
        string ten;
        string gioTao;
        string ngayTao;
        string matkhau;
        unsigned int kichThuoc;
        unsigned short clusterBatDau;
    // Các thuộc tính khác của tập tin
};

struct BootSector {
    char osName[6];
    short startDataSector;
    char startClusterTable;
    int password;
    char sectorPerCluster;
    char sectorBootSize;
    short sizeClusterTable;
    char numClusterTables;
    int sizeDataArea;
    int sizeVolume;
    short sizeSector;
    int sizeVolumeBytes;
    short endSignature;
};

// Hàm tạo / định dạng volume MyFS.Dat
void createVolume(const string& volumeName, int maxVolumeSize, int clusterSize) {
    ofstream outFile(volumeName, ios::binary | ios::out);
    if (!outFile) {
        cerr << "Error: Unable to create volume!" << endl;
        return;
    }

    const long long int FILE_SIZE = 1LL * 1024 * 1024 * 1024; // 1 GiB
    
    char zero = 0;
    long long int bytesWritten = 0;
    while (bytesWritten < FILE_SIZE) {
        outFile.write(&zero, sizeof(char));
        bytesWritten++;
    }
    outFile.close();

    // // Tạo vùng System và Data với kích thước tương ứng
    // char* systemArea = new char[BLOCK_SIZE * clusterSize];
    // char* dataArea = new char[maxVolumeSize - BLOCK_SIZE * clusterSize];

    // // Khởi tạo vùng System và Data với các giá trị mặc định

    // // Ghi các thông tin vào volume
    // outFile.write(systemArea, BLOCK_SIZE * clusterSize);
    // outFile.write(dataArea, maxVolumeSize - BLOCK_SIZE * clusterSize);

    // outFile.close();
    // delete[] systemArea;
    // delete[] dataArea;
}

//Format Boot Sector
void formatBootSector(const string& volumeName){

    BootSector bootSector;
    strcpy(bootSector.osName, "CUSTOM");
    bootSector.startDataSector = 4091;
    bootSector.startClusterTable = 1;
    bootSector.password = 1234;
    bootSector.sectorPerCluster = 4;
    bootSector.sectorBootSize = 1;
    bootSector.sizeClusterTable = 2045;
    bootSector.numClusterTables = 2;
    bootSector.sizeDataArea = 2093061;
    bootSector.sizeVolume = 2097152;
    bootSector.sizeSector = 512;
    bootSector.sizeVolumeBytes = 1073741824;
    bootSector.endSignature = 0xAA55;
    
    std::fstream file(volumeName, std::ios::binary | std::ios::in | std::ios::out);

    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
    }

    // Offset where you want to write the data
    std::streampos offset = 0;

    // Data to be written to the file
    const char data[] = "Hello, Binary World!";

    // Move the put pointer to the desired offset
    file.seekp(offset, std::ios::beg);

    // Write data to the file
    file.write(reinterpret_cast<char*>(&bootSector), sizeof(BootSector));

    // Close the file
    file.close();
    // // Open MyFS.Dat file
    // ofstream outFile(volumeName, ios::binary);
     
    // std::streampos offset = 0;
    // outFile.seekp(offset, std::ios::beg);

    // // Write boot sector to file

    // // Close file
    // outFile.close();
}



// Hàm đọc dữ liệu từ volume theo từng cụm 512 byte (block)
void readBlock(const string& volumeName, int blockNumber, char* buffer) {
    ifstream inFile(volumeName, ios::binary);
    if (!inFile) {
        cerr << "Error: Unable to open volume!" << endl;
        return;
    }
    inFile.seekg(blockNumber * BLOCK_SIZE);
    inFile.read(buffer, BLOCK_SIZE);
    inFile.close();
}

// Hàm ghi dữ liệu vào volume theo từng cụm 512 byte (block)
void writeBlock(const string& volumeName, int blockNumber, const char* buffer) {
    ofstream outFile(volumeName, ios::binary | ios::in);
    if (!outFile) {
        cerr << "Error: Unable to open volume!" << endl;
        return;
    }
    outFile.seekp(blockNumber * BLOCK_SIZE);
    outFile.write(buffer, BLOCK_SIZE);
    outFile.close();
}

int findFileIndex(const string& volumeName, const string& fileName) {
    ifstream file(volumeName, ios::binary);

    // Find the position of the file name in the volume file
    size_t fileNamePos = string::npos;
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(fileName);
        if (pos != string::npos) {
            fileNamePos = pos;
            break;
        }
    }

    // If file name is not found, return -1
    if (fileNamePos == string::npos) {
        return -1;
    }

    // Calculate the index of the file in terms of SIZE blocks
    int index = fileNamePos / SIZE;
    return index;
}

string vigenereCipher(string message, int direction = 1) 
{
    int key_index = 0;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string final_message = "";

    for (char c : message) 
    {
        if (!isalpha(c)) 
        {
            final_message += c;
        } 
        else 
        {
            char key_char = KEY[key_index % KEY.length()];
            key_index++;

            int offset = alphabet.find(tolower(key_char));
            int index = alphabet.find(tolower(c));
            int new_index = (index + offset * direction + alphabet.length()) % alphabet.length();
            final_message += alphabet[new_index];
        }
    }
    return final_message;
}

string encrypt(string message) 
{
    return vigenereCipher(message);
}

string decrypt(string message) 
{
    return vigenereCipher(message, -1);
}

// Hàm thiết lập / Đổi / Kiểm tra mật khẩu truy xuất MyFS
void setPassword(const string& volumeName, File file, const string& newPassword) 
{
    
    file.matkhau = newPassword;
    
}

// Hàm liệt kê danh sách các tập tin trong MyFS
void listFiles(const string& volumeName) {

}

string stringToHex(string input) 
{
    stringstream result;
    for (char c : input) 
    {
        result << hex << static_cast<int>(c);
    }
    return result.str();
}

string inputString() 
{
    string input;
    getline(cin, input);

    // If input length is more than 16, truncate to 16 characters
    if (input.length() > 16) 
        input = input.substr(0, 16);
    
    // If input length is less than 16, append spaces until length is 16
    while (input.length() < 16) 
        input += ' ';

    return input;
}

string makeEnough16char(string userInput)
{
    while (userInput.length() < 16) 
        userInput += ' ';
    return userInput;
}

// Hàm đặt / Đổi mật khẩu truy xuất cho 1 tập tin trong MyFS
void setFilePassword(const string& volumeName, const string& fileName) {
    // Open the volume file in binary mode for reading
    ifstream file(volumeName, ios::binary);

    // Find the position of the file name in the volume
    int dotPos = fileName.find('.');
    string name = stringToHex(fileName.substr(0, dotPos));
    int namePos = findFileIndex(volumeName, name);

    if (namePos == -1) {
        cout << "File not found in the volume." << endl;
        return;
    }

    // Seek to the position of the file name in the volume
    file.seekg(namePos * SIZE);

    char entry[SIZE];
    file.read(entry, SIZE);
    string currentPassword(entry + SIZE - 32, 32); 

    string userInput;
    do {
        cout << "Enter the current password for file: ";
        userInput = stringToHex(encrypt(inputString()));

        if (userInput != currentPassword) 
            cout << "Try again\n";
    } while (userInput != currentPassword);

    // Prompt the user to enter the new password
    cout << "Enter the new password: ";
    userInput = encrypt(inputString());

    for (int i = userInput.length(); i < 16; i++) {
        userInput += " ";
    }

    userInput = stringToHex(userInput);

    ofstream outFile(volumeName, ios::binary | ios::in | ios::out);
    outFile.seekp(namePos * SIZE + SIZE - 32, ios::beg);
    outFile.write(userInput.c_str(), 32);
    outFile.close();

    cout << "Password updated successfully." << endl;
}

// Hàm chép (Import) 1 tập tin từ bên ngoài vào MyFS
void importFile(const string& volumeName, const string& externalFileName, const string& internalFileName, const string& password = "") {
    // Code xử lý chép (import) 1 tập tin từ bên ngoài vào MyFS
}

// Hàm chép (Outport) 1 tập tin trong MyFS ra ngoài
void exportFile(const string& volumeName, const string& internalFileName, const string& externalFileName, const string& password = "") {
    // Code xử lý chép (export) 1 tập tin từ MyFS ra bên ngoài
}

// Hàm xóa 1 tập tin trong MyFS
void deleteFile(const string& volumeName, const string& fileName, const string& password = "") {
    // Code xử lý xóa 1 tập tin trong MyFS
}

int main() {
    // Sử dụng các hàm để thực hiện các chức năng tương ứng
    // Ví dụ:
    const string volumeName = "MyFS.Dat";
    // createVolume(volumeName, 1024 * 1024 * 1024, 4096);
    formatBootSector(volumeName);
    // setPassword("MyFS.Dat", "newPassword");
    // listFiles("MyFS.Dat");
    // setFilePassword("MyFS.Dat", "example.txt");
    // importFile("MyFS.Dat", "externalFile.txt", "internalFile.txt");
    // exportFile("MyFS.Dat", "internal);
}