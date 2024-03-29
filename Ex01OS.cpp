#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int sector_size = 512;

void createFiles(string volumePath, int sectorsPerCluster) 
{
    // Loop to create 100 files
    for (int i = 0; i < 100; i++) 
    {
        string filename = volumePath + "F" + to_string(i) + ".Dat";

        ofstream file(filename, ios::binary);

        if (file.is_open()) 
        {
            // Calculate the number of clusters needed for this file
            int fileSizeInBytes = (4 - (i % 4)) * sectorsPerCluster * sector_size;

            while (file.tellp() < fileSizeInBytes) 
            { 
                file << 2020 + i << "\n";
            }
            file.close();
        } 
        else 
        {
            cout << "Unable to create file: " << filename << endl;
        }
    }
}

int main() {
    string volumePaths[] = {"G:\\", "H:\\", "I:\\"}; // Path to FAT, NTFS and FAT32 volumes
    int clusterSizes[] = {8, 4, 2}; // Cluster sizes for FAT, NTFS, and FAT32 volumes
    for (int i = 0; i < 3; i++) {
        createFiles(volumePaths[i] , clusterSizes[i]);
        cout << "Files created in volume " << volumePaths[i] << endl;
    }
    return 0;
}
