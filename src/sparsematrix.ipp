#include <iterator>
#include <stdexcept>

#pragma region RowProxy

template<typename T>
SparseMatrix<T>::RowProxy::RowProxy(std::map<size_t, T>* row, size_t width) : width_(width), row_(row) {};

template<typename T>
T& SparseMatrix<T>::RowProxy::operator[](int index) { 
    if (index < 0 || (size_t) index >= width_)
        throw std::out_of_range("SparseMatrix column index out of range");
    return (*row_)[index];
};

#pragma endregion RowProxy


#pragma region Constructors

template<typename T>
SparseMatrix<T>::SparseMatrix() : SparseMatrix(0, 0) {};

template<typename T>
SparseMatrix<T>::SparseMatrix(int width, int height) : SparseMatrix((size_t) width, (size_t) height) {};

template<typename T>
SparseMatrix<T>::SparseMatrix(size_t width, size_t height) : width_(width), height_(height) {
    data_.resize(height_);
    for (typename std::vector<std::map<size_t, T>*>::iterator it = data_.begin(); it != data_.end(); ++it)
        *it = new std::map<size_t, T>();
};

#pragma endregion Constructors

template<typename T>
typename SparseMatrix<T>::RowProxy SparseMatrix<T>::operator[](int index) { 
    if (index < 0 || (size_t) index >= height_)
        throw std::out_of_range("SparseMatrix row index out of range");
    
    return RowProxy(data_[index], width_);
}

template<typename T>
const typename SparseMatrix<T>::RowProxy SparseMatrix<T>::operator[](int index) const {
    if (index < 0 || (size_t) index >= width_)
        throw std::out_of_range("SparseMatrix column index out of range");

    return RowProxy(data_[index], width_);
}
