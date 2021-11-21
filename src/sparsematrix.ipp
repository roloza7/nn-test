#include <iterator>
#include <stdexcept>

template<typename T>
SparseMatrix<T>::SparseMatrix() : width_(0), height_(0) {};

template<typename T>
SparseMatrix<T>::SparseMatrix(int width, int height) : width_(width), height_(height) {
    data_.resize(height_);
}

template<typename T>
SparseMatrix<T>::SparseMatrix(size_t width, size_t height) : width_(width), height_(height) {
    data_.resize(height);
};

template<typename T>
T& SparseMatrix<T>::operator()(int x, int y) {
    std::map<size_t, T>& row_map = data_[y];
    if (x < 0 || x >= int(width_)) throw std::out_of_range("Width index out of range.");
    return row_map[x];
} 

template<typename T>
ostream& operator<<(ostream& os, const SparseMatrix<T>& sm) {
    os << "[ ";
    for (unsigned y = 0; y < sm.data_.size(); y++) {
        typename std::map<size_t, T>::const_iterator row_it = sm.data_[y].begin();
        for (unsigned x = 0; x < sm.width_; x++) {
            if (row_it == sm.data_[y].end() || x < row_it->first)
                os << "0";
            else {
                os << row_it->second;
                ++row_it;
            }  
            os << " ";
        }
        if (y != sm.data_.size() - 1)
            os << "\n  ";
    }
    os << "]" << std::endl;
    return os;
}

template<typename T>
void SparseMatrix<T>::Multiply(std::vector<T>*& values, std::vector<T>*& out) const {
    for (unsigned y = 0; y < data_.size(); y++) {
        typename std::map<size_t, T>::const_iterator it = data_[y].begin();
        for(; it != data_[y].end(); ++it) {
            (*out)[it->first] += (*values)[y] * it->second;
        }
    }
}
