
template <typename T>
int Image<T>::run(Task& v) override {
    return v.runner(*this);
}
