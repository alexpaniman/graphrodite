
template <typename T>
class my_class {
	my_class<T> xyz();
};

template <typename T>
my_class<T> my_class<T>::xyz() {
	return *this;
}
