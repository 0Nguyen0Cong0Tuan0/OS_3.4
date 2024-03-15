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
    char osName[4];
    short startDataSector;
    char startClusterTable;
    char password[4];
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
struct Entry {
    char fileName[31];       // Tên tập tin
    char fileExtension[3];   // Tên mở rộng
    char createHour, createMinute, createSecond;    // Giờ tạo
    char createDay, createMonth, createYear;    // Ngày tạo
    uint32_t size;           // Kích thước
    uint32_t startCluster;   // Cluster bắt đầu
    char password[16];       // Password

    Entry(const string& fileName, const string& fileExtension, const string& createHour, const string& createMinute, const string& createSecond, const string& createDay, const string& createMonth, const string& createYear, uint32_t size, uint32_t startCluster, const string& password) {
        strcpy(this->fileName, fileName.c_str());
        strcpy(this->fileExtension, fileExtension.c_str());
        this->createHour = stoi(createHour.substr(0, 2));
        this->createMinute = stoi(createMinute.substr(3, 2));
        this->createSecond = stoi(createSecond.substr(6, 2));
        this->createDay = stoi(createDay.substr(0, 2));
        this->createMonth = stoi(createMonth.substr(3, 2));
        this->createYear = stoi(createYear.substr(6, 4));
        this->size = size;
        this->startCluster = startCluster;
        strcpy(this->password, password.c_str());
    }
};

// Hàm tạo / định dạng volume MyFS.Dat
void createVolume(const string& volumeName, int maxVolumeSize, int clusterSize) {
    ofstream outFile(volumeName, ios::binary);
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
}

//Format Boot Sector
void formatBootSector(const string& volumeName, string password){

    BootSector bootSector;
    strcpy(bootSector.osName, "TEST");
    bootSector.startDataSector = 4091;
    bootSector.startClusterTable = 1;
    strcpy(bootSector.password, password.c_str());
    bootSector.sectorPerCluster = 4;
    bootSector.sectorBootSize = 1;
    bootSector.sizeClusterTable = 2045;
    bootSector.numClusterTables = 2;
    bootSector.sizeDataArea = 2093061;
    bootSector.sizeVolume = 2097152;
    bootSector.sizeSector = 512;
    bootSector.sizeVolumeBytes = 1073741824;
    bootSector.endSignature = 0xAA55;

    std::ofstream file(volumeName, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
    }

    // Offset where you want to write the data
    std::streampos offset = 0;

    // Data to be written to the file

    // Move the put pointer to the desired offset
    file.seekp(offset, std::ios::beg);

    // Write data to the file
    file.write(reinterpret_cast<char*>(&bootSector), sizeof(BootSector));

    // Close the file
    file.close();
}

// Hàm đọc dữ liệu từ volume theo từng cụm 512 byte (block)
void readBlock(const string& volumeName, int blockNumber) {
    ifstream inFile(volumeName, ios::binary);
    if (!inFile) {
        cerr << "Error: Unable to open volume!" << endl;
        return;
    }
    inFile.seekg(blockNumber * BLOCK_SIZE);
    inFile.close();
}

// Hàm ghi dữ liệu vào volume theo từng cụm 512 byte (block)
void writeBlock(const string& volumeName, int blockNumber) {
    ofstream outFile(volumeName, ios::binary | ios::in);
    if (!outFile) {
        cerr << "Error: Unable to open volume!" << endl;
        return;
    }
    outFile.seekp(blockNumber * BLOCK_SIZE);
    outFile.close();
}

//
void formatVungData(const string& volumnName){
    //Tao random cac file luu vao vung data
    ofstream ofs(volumnName, ios::binary);
    writeBlock(volumnName, 4091);

    Entry file1("example", "txt", "12", "30", "00", "01", "01", "2021", 0, 4091, "1234");
    Entry file2("example2", "txt", "12", "30", "00", "01", "01", "2021", 0, 4091, "1234");
    Entry file3("example3", "txt", "12", "30", "00", "01", "01", "2021", 0, 4091, "1234");
    Entry file4("example4", "txt", "12", "30", "00", "01", "01", "2021", 0, 4091, "1234");

    ofs.write(reinterpret_cast<char*>(&file1), sizeof(Entry));
    ofs.write(reinterpret_cast<char*>(&file2), sizeof(Entry));
    ofs.write(reinterpret_cast<char*>(&file3), sizeof(Entry));
    ofs.write(reinterpret_cast<char*>(&file4), sizeof(Entry));

    ofs.close();
}

void formatBangQuanLyCluster(const string& volumnName){
    int count = 0;
    for (int i = 2; i <= 523265; i++){
        readBlock(volumnName, 4091 + count);
        char buffer;
        ifstream ifs(volumnName, ios::binary);
        ifs.read(&buffer, 1);
        if (buffer == 0){
            writeBlock(volumnName, 1);
            ofstream ofs(volumnName, ios::binary);
            ofs.seekp(512 + i, ios::beg);
            short cluster = 0;
            ofs.write(reinterpret_cast<char*>(&cluster), sizeof(char));
            ofs.close();
        }
        else{
            writeBlock(volumnName, 1);
            ofstream ofs(volumnName, ios::binary);
            ofs.seekp(512 + i, ios::beg);
            short cluster = 0xFFEF;
            ofs.write(reinterpret_cast<char*>(&cluster), sizeof(char));
            ofs.close();
        }
        ifs.close();
        count += 4;
    }
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
    
    //Tao/Dinh dang volume MyFS.Dat
    cout << "Tao/Dinh dang volume MyFS.Dat" << endl;
    cout << "Nhap mat khau volume (4 ki tu): " << endl;
    string password;
    getline(cin, password);
    const string volumeName = "MyFS.Dat";
    cout << "Tao volume MyFS.Dat co kich thuoc 1GiB";
    createVolume(volumeName, 1024 * 1024 * 1024, 4096);
    formatBootSector(volumeName, password);
    cout << "Tao ngau nhien 4 tap tin trong vung data" << endl;
    formatVungData(volumeName);
    cout << "Da format vung data" << endl;
    formatBangQuanLyCluster(volumeName);
    cout << "Da format bang quan ly cluster" << endl;
    
    //Thiet lap/Doi/Kiem tra mat khau truy xuat MyFS




    // setPassword("MyFS.Dat", "newPassword");
    // listFiles("MyFS.Dat");
    // setFilePassword("MyFS.Dat", "example.txt");
    // importFile("MyFS.Dat", "externalFile.txt", "internalFile.txt");
    // exportFile("MyFS.Dat", "internal);
}