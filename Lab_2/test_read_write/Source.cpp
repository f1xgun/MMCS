#include <string>
#include <iostream>

#define BUFFER_SIZE 128

void read_file(const std::string& file_path)
{
    std::cout << "Try to read_file with path: " << file_path << std::endl;

    FILE* file;
    if (fopen_s(&file, file_path.c_str(), "r") != 0)
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != nullptr)
    {
        size_t bytesRead = strlen(buffer);
        std::cout.write(buffer, bytesRead);
    }

    if (ferror(file)) {
        std::cout << "Error while reading file" << std::endl;
    }

    std::cout << std::endl;

    fclose(file);
}

void write_to_file(const std::string& file_path, const std::string& text)
{
    std::cout << "Write to file with path: " << file_path << std::endl;

    FILE* file;
    if (fopen_s(&file, file_path.c_str(), "w") != 0)
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    size_t total_size = text.length();
    size_t offset = 0;

    while (offset < total_size)
    {
        size_t remaining_size = total_size - offset;
        size_t current_batch_size = std::min(size_t(BUFFER_SIZE), remaining_size);

        fwrite(text.c_str() + offset, sizeof(text[0]), current_batch_size, file);
        offset += current_batch_size;
    }

    fclose(file);
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        std::string file_path = argv[1];
        read_file(file_path);
    }
    else if (argc == 3)
    {
        std::string file_path = argv[1];
        std::string text = argv[2];
        write_to_file(file_path, text);
    }
    else
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "Usage: test_read_write <file_path> [text]" << std::endl;
        return 1;
    }

    system("PAUSE");
    return 0;
}
