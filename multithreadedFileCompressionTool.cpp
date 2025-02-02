#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <zlib.h>

using namespace std;

const int CHUNK_SIZE = 16384;  // 16KB chunk size for multithreading
mutex mtx;  // Mutex for thread synchronization

// Function Declarations
void compressChunk(const vector<char>& input, vector<char>& output, int level);
void decompressChunk(const vector<char>& input, vector<char>& output, uLong originalSize);
void compressFile(const string& inputFile, const string& outputFile);
void decompressFile(const string& inputFile, const string& outputFile);

int main() {
    int choice;
    string filename;

    do {
        cout << "\n1. Compress File\n2. Decompress File\n3. Exit\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter filename to compress: ";
                getline(cin, filename);
                compressFile(filename, filename + ".comp");
                break;
            case 2:
                cout << "Enter filename to decompress: ";
                getline(cin, filename);
                decompressFile(filename, filename + ".decomp");
                break;
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice, try again!" << endl;
        }
    } while (choice != 3);

    return 0;
}

// Function Definitions

// Function to compress a chunk of data
void compressChunk(const vector<char>& input, vector<char>& output, int level) {
    uLongf compressedSize = compressBound(input.size());  
    output.resize(compressedSize);  

    if (compress2(reinterpret_cast<Bytef*>(output.data()), &compressedSize,
                  reinterpret_cast<const Bytef*>(input.data()), input.size(), level) != Z_OK) {
        cerr << "Compression failed!" << endl;
        return;
    }
    output.resize(compressedSize);
}

// Function to decompress a chunk of data
void decompressChunk(const vector<char>& input, vector<char>& output, uLong originalSize) {
    output.resize(originalSize);

    if (uncompress(reinterpret_cast<Bytef*>(output.data()), &originalSize,
                   reinterpret_cast<const Bytef*>(input.data()), input.size()) != Z_OK) {
        cerr << "Decompression failed!" << endl;
        return;
    }
    output.resize(originalSize);
}

// Function to compress a file using multithreading
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    if (!in || !out) {
        cerr << "Error opening file!" << endl;
        return;
    }

    vector<thread> threads;
    vector<vector<char>> compressedChunks;
    
    while (!in.eof()) {
        vector<char> buffer(CHUNK_SIZE);
        in.read(buffer.data(), CHUNK_SIZE);
        size_t bytesRead = in.gcount();

        if (bytesRead == 0) break;

        buffer.resize(bytesRead);
        compressedChunks.emplace_back();

        // Launch compression thread
        threads.emplace_back(compressChunk, buffer, ref(compressedChunks.back()), Z_BEST_COMPRESSION);
    }

    for (auto& th : threads) th.join();  // Wait for all threads to finish

    // Write compressed data to file
    for (const auto& chunk : compressedChunks) {
        uLong chunkSize = chunk.size();
        out.write(reinterpret_cast<const char*>(&chunkSize), sizeof(chunkSize));
        out.write(chunk.data(), chunk.size());
    }

    in.close();
    out.close();
    cout << "Compression complete: " << outputFile << endl;
}

// Function to decompress a file using multithreading
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    if (!in || !out) {
        cerr << "Error opening file!" << endl;
        return;
    }

    vector<thread> threads;
    vector<vector<char>> decompressedChunks;

    while (!in.eof()) {
        uLong chunkSize;
        in.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));

        if (in.eof()) break;

        vector<char> compressedData(chunkSize);
        in.read(compressedData.data(), chunkSize);

        uLong originalSize = CHUNK_SIZE;  
        decompressedChunks.emplace_back();
        
        // Launch decompression thread
        threads.emplace_back(decompressChunk, compressedData, ref(decompressedChunks.back()), originalSize);
    }

    for (auto& th : threads) th.join();  // Wait for all threads to finish

    // Write decompressed data to file
    for (const auto& chunk : decompressedChunks) {
        out.write(chunk.data(), chunk.size());
    }

    in.close();
    out.close();
    cout << "Decompression complete: " << outputFile << endl;
}
