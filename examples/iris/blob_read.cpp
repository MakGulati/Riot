#include <iostream>
#include <vector>
#include <sstream>
// #include "blob/iris_aug_train.csv.h"
#include "blob/iris_aug_test.csv.h"

int main()
{
    constexpr size_t iris_aug_test_csv_size = sizeof(iris_aug_test_csv);


    std::istringstream iss_test(std::string(reinterpret_cast<const char *>(iris_aug_test_csv), iris_aug_test_csv_size));

    std::string line;
    std::vector<std::vector<float>> data;

    while (std::getline(iss_test, line, '\n'))
    {
        std::istringstream line_stream(line);
        std::string cell;
        std::vector<float> row;

        while (std::getline(line_stream, cell, ','))
        {
            float value = std::stof(cell);
            row.push_back(value);
        }

        data.push_back(row);
    }

    for (const auto &row : data)
    {
        for (float value : row)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
