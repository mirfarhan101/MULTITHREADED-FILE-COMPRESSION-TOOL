# MULTITHREADED-FILE-COMPRESSION-TOOL

**COMPANY NAME** : CODETECH IT SOLUTIONS

**NAME** : FARHAN MIR

**INTERN ID** : CT08JFI

**DOMAIN NAME** : C++

**BATCH DURATION** : JANUARY 5th,2025 to FEBUARY 5th,2025

**MENTOR NAME** : NEELA SANTHOSH

**TASK DESCRITION** :  In this task, I created a file compression and decompression utility using multithreading and the `zlib` library, a widely used tool for lossless data compression. The main tools I used for this project were the C++ programming language, `zlib` for the compression and decompression algorithms, and several parts of the C++ Standard Library, including `iostream`, `fstream`, `vector`, and `thread`. `zlib` is well known for its efficiency in compressing data, and I leveraged it to compress and decompress chunks of data in parallel to make the program more efficient when handling larger files. The program allows users to choose between compressing or decompressing files through a simple console interface. To achieve better performance, I divided the files into 16KB chunks and used multiple threads so that each chunk is processed independently, taking full advantage of modern multi-core processors. The `compress2` function from `zlib` is used to compress the chunks, and the `uncompress` function handles the decompression. The program provides a minimal user interface, asking for the input file, performing the requested task, and saving the processed data as a new file with the appropriate extension (`.comp` for compressed files and `.decomp` for decompressed ones). The program reads and writes files in binary mode, using vectors to store chunks of data before processing them. The `thread` library allows the program to process multiple chunks in parallel, significantly improving the speed of the task, especially for large files. Although I included the `mutex` library for potential thread synchronization, I didn't need to use it in this version of the code. The user interface is straightforward, offering a simple menu to either compress or decompress files, making it easy to use. The output is the compressed or decompressed file, stored in the same directory as the original file with a new extension. Overall, this task demonstrates how using multithreading and libraries like `zlib` in C++ can solve real-world problems by making file compression and decompression faster and more efficient, particularly for larger datasets.
