#include <iterator>
#include <stdexcept>

#pragma region RowProxy

template<typename T>
SparseMatrix<T>::RowProxy::RowProxy(std::map<size_t, T>* row, size_t width) : width_(width), row_(row) {};

template<typename T>
T& SparseMatrix<T>::RowProxy::operator[](int index) { 
    if (index < 0 || (size_t) index >= width_)
        throw std::out_of_range("SparseMatrix column index out of range: " + std::to_string(index));
    return (*row_)[index];
};

#pragma endregion RowProxy


#pragma region Constructors

template<typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix& other) {
    height_ = other.height_;
    width_ = other.width_;
    data_.resize(other.height_);
    for (size_t i = 0; i < other.height_; ++i) {
        data_[i] = new std::map<size_t, T>();
        data_[i] = other.data_[i];
    }    
}

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

template<typename T>
typename std::map<size_t, T>::iterator SparseMatrix<T>::RowProxy::begin() {
    return row_->begin();
}

template<typename T>
typename std::map<size_t, T>::iterator SparseMatrix<T>::RowProxy::end() {
    return row_->end();
}

template<typename T>
ostream& operator<<(ostream& os, const SparseMatrix<T>& sm) {
    os << "[";
    for (size_t y = 0; y < sm.height_; ++y) {
        os << "[ ";
        for (size_t x = 0; x < sm.width_; ++x) {
            typename std::map<size_t, T>::iterator lookup = sm.data_[y]->find(x);
            if (lookup != sm.data_[y]->end())
                os << lookup->second << " ";
            else
                os << 0 << " ";
        }
        os << "]";
        if (y != sm.height_ - 1) os << "\n ";
    }
    os << "]" << std::endl;
    return os;
}

template<typename T>
size_t SparseMatrix<T>::Width() {
    return width_;
}

template<typename T>
size_t SparseMatrix<T>::Height() {
    return height_;
}

template<typename T>
void SparseMatrix<T>::RowProxy::erase(size_t index) {
    row_->erase(index);
}

template<typename T>
size_t SparseMatrix<T>::RowProxy::size() {
    return row_->size();
}

template<typename T>
SparseMatrix<T>::~SparseMatrix() {
    for (size_t i = 0; i < data_.size(); ++i)
        delete data_[i];
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator=(const SparseMatrix<T>& rhs) {
    for (size_t i = 0; i < data_.size(); ++i)
        delete data_[i];


    for (size_t i = 0; i < height_; ++i)
        *data_[i] = *rhs.data_[i];
    
    return *this;
}

template<typename T>
void SparseMatrix<T>::Set(const SparseMatrix& other) {

    for (size_t i = other.height_; i < height_; ++i)
        delete data_[i];
    
    data_.reserve(other.height_);

    for (size_t i = height_; i < other.height_; ++i)
        data_[i] = new std::map<size_t, T>();

    for (size_t i = 0; i < other.height_; ++i)
        *data_[i] = *other.data_[i];

    height_ = other.height_;
    width_ = other.width_;

}

template<typename T>
void SparseMatrix<T>::SetWidth(size_t new_width) {
    if (width_ > new_width)
        throw std::runtime_error("Cannot resize to a smaller width");

    width_ = new_width;
}

template<typename T>
void SparseMatrix<T>::SetHeight(size_t new_height) {
    if (height_ > new_height)
        throw std::runtime_error("Cannot resize to smaller height");
    if (height_ == new_height)
        return;
    data_.resize(new_height, new std::map<size_t, double>());

    height_ = new_height;

}