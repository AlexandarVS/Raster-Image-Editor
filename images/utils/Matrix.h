#pragma once
#include <vector>

template <typename T>
class Matrix {
public:
    class Proxy {
    public:
        Proxy(std::vector<T>& data, size_t rowIndex, size_t cols) : data(data), row(rowIndex), cols(cols) {}

        T& operator[](size_t col) {
            if (col >= cols) throw std::out_of_range("Column out of range");
            return data[row * cols + col];
        }

        const T& operator[](size_t col) const {
            if (col >= cols) throw std::out_of_range("Column out of range");
            return data[row * cols + col];
        }

    private:
        std::vector<T>& data;
        size_t row;
        size_t cols;
    };

    Matrix() : rows(0), cols(0), data(rows* cols) {}
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows* cols) {}

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    Proxy operator[](size_t row) {
        if (row >= rows) throw std::out_of_range("Row out of range");
        return Proxy(data, row, cols);
    }

    const Proxy operator[](size_t row) const {
        if (row >= rows) throw std::out_of_range("Row out of range");
        return Proxy(const_cast<std::vector<T>&>(data), row, cols);
    }

private:
    size_t rows;
    size_t cols;
    std::vector<T> data;
};
