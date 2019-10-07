/*
* Alexander Ocheretyany
*
* First homework on C++
*
* Charles University in Prague, 2018
*/

#include <iostream>

template <class T> class Array {

private:

    T* arr = nullptr; // Pointer to the array
    int arraySize = 0; // Size of the array
    size_t arrayCapacity = 0; // Capacity of the array

public:

    typedef T * iterator;
    typedef const T * const_iterator;
    iterator begin() {
        return &arr[0];
    }
    iterator end() {
        return &arr[arraySize];
    }

    // Default constructor
    Array() {
        // Set everything to NULL
        arr = nullptr;
        arraySize = 0;
        arrayCapacity = 0;
    }

    // Fill constructor
    Array(size_t n) {
        allocate(n); // Allocate an array with n elements
        setToZero(); // Initialize the array
        arraySize = n; // Set size to n
    }

    // Copy constructor
    Array(const Array<T>& arr_2) {

        arraySize = arr_2.arraySize; // Copy size
        arrayCapacity = arr_2.arrayCapacity; // Copy capacity

        // Create an array
        allocate(arr_2.arrayCapacity); // Allocate memory for an array
        for (size_t i = 0; i < arraySize; i++) {
            arr[i] = arr_2[i];
        }
    }

    // Move constructor
    Array(Array<T>&& arr_2) {
        arr = std::move(arr_2.arr); // Move array
        arr_2.arr = nullptr;
        arraySize = arr_2.arraySize; // Move array size
        arr_2.arraySize = 0;
        arrayCapacity = arr_2.arrayCapacity; // Move capacity
        arr_2.arrayCapacity = 0;
    }

    // Operator =
    Array<T>& operator=(const Array<T>& arr_2) {

        deallocate(); // Deallocate the array
        arraySize = arr_2.arraySize; // Set array size
        arrayCapacity = arr_2.arrayCapacity; // Set array capacity
        allocate(arr_2.arrayCapacity); // Create a new array
        // Fill it in
        for (size_t i = 0; i < arraySize; i++) {
            arr[i] = arr_2[i];
        }
        return *this;
    }

    // Operator =
    Array<T>& operator=(Array<T>&& arr_2) {
        swap(arr_2);
        return *this;
    }

    // Destructor
    ~Array() {
        deallocate(); // Delete allocated array and deallocate memory
    }

    // Definition of [] operator with const
    const T& operator[](size_t i) const {
        return arr[i];
    };

    // Definition of [] operator
    T& operator[](size_t i) {
        return arr[i];
    };

    // const T& at(size_t i) - returns an element at the i-th position for const
    const T& at(size_t i) const {
        if (i >= arraySize) throw std::out_of_range("Illegal index"); // Throw an exception if out of range
        return arr[i];
    }

    // T& at(size_t i) - returns an element at the i-th position
    T& at(size_t i) {
        if (i >= arraySize) throw std::out_of_range("Illegal index"); // Throw an exception if out of range
        return arr[i];
    }

    // void reserve(size_t n) - reserves memory for n elements of type "T"
    void reserve(size_t n) {
        if (n > arrayCapacity) {
            // If new reservation is bigger than the previous
            if (arraySize == 0) {
                // If no array exists
                arrayCapacity = n; // Set array capacity
                deallocate(); // Deallocate the array
                allocate(n); // Allocate new memory
            }
            else {
                // If there is an array
                T* tmp = new T[arraySize]; // Temporary array
                size_t tmpSize = arraySize; // Array size
                for (size_t i = 0; i < arraySize; i++) {
                    tmp[i] = arr[i];
                }
                deallocate(); // Dealloate the previous array
                allocate(n); // Allocate new array
                setToZero(); // Set everything to zero
                for (size_t i = 0; i < arraySize; i++) {
                    arr[i] = tmp[i];
                }
                arraySize = tmpSize; // Set size of the array
                arrayCapacity = n; // Set new capacity
                delete[] tmp; // Delete the temporary array
            }
        }
    }

    // void resize(size_t n) - resizes the array
    void resize(size_t n) {
        if (n < arraySize) {
            // If decreasing
            size_t diff = arraySize - n; // Difference of sizes
            for (size_t i = 0; i < diff; i++) {
                pop_back(); // Delete the lastest element
            }
        }
        else if(n > arraySize) {
            // If increasing
            if (arraySize > 0) {
                T* tmp = new T[arraySize];
                for (size_t i = 0; i < arraySize; i++) {
                    tmp[i] = arr[i];
                }
                deallocate(); // Deallocate array
                allocate(n); // Allocate new array
                setToZero(); // Set everything to zero
                for (size_t i = 0; i < arraySize; i++) {
                    arr[i] = tmp[i];
                }
                delete[](tmp);
            }
            else {
                // If there is no array
                allocate(n); // Create new array
                setToZero(); // Set everything to zero
            }
        }
        arraySize = n; // Set new size
    }

    // size_t size() - returns the size of the array
    size_t size() const noexcept {
        return arraySize;
    }

    // size_t capacity() - returns the array capacity
    size_t capacity() const noexcept {
        return arrayCapacity;
    }

    // void push_back(const T& el) - adds the element "el"to the end of the array
    void push_back(const T& el) {

        // If the array does not exist
        if (arrayCapacity == 0) {
            arraySize = 0;
            arrayCapacity = 1;
            allocate(arrayCapacity);
            setToZero();
        }

        // If by adding an element we violate the size of the array - resize the array
        if (arraySize == arrayCapacity) {
            size_t newCapacity = arrayCapacity * 2; // New capacity (double capacity)
            size_t tmpSize = arraySize; // Temporary size

            T* new_arr = new T[sizeof(T) * arraySize];
            for (size_t i = 0; i < arraySize; i++) {
                new_arr[i] = arr[i];
            }

            deallocate(); // Free memory of the old array
            allocate(newCapacity); // Set new array
            setToZero(); // Set everything to zero
            for (size_t i = 0; i < arraySize; i++) {
                arr[i] = new_arr[i];
            }
            arraySize = tmpSize; // Set size
            arrayCapacity = newCapacity; // Set capacity
            delete[] new_arr; // Delete temporary array pointer
            new_arr = nullptr;
        }

        arr[arraySize] = el; // Add the element "el" to the array
        arraySize++; // Increase the size of the array

    }

    // void push_back(const Array<T>& arr_2) - adds the elements of the "arr_2"to the end of the array
    void push_back(const Array<T>& arr_2) {
        if (arr_2.arr != arr) {
            // If we are appending a different array
            for (size_t i = 0; i < arr_2.arraySize; i++) {
                push_back(arr_2[i]);
            }
        }
        else {
            // To avoid an infinite loop, since we are appending the same array
            T* arr_3 = new T[arr_2.arraySize]; // Create a copy of the array
            for (size_t i = 0; i < arr_2.arraySize; i++) {
                arr_3[i] = arr_2[i];
            }
            // Append the copy
            for (size_t i = 0; i < arr_2.arraySize; i++) {
                push_back(arr_3[i]);
            }
            delete[] arr_3;
        }
    }

    // void push_back(T&& el) - adds the element el to the end and destroyes el
    void push_back(T&& el) {
        emplace_back(std::move(el));
    }

    // void pop_back() - removes the last element of the array
    void pop_back() {

        if (arraySize > 1) {
            // If the array has at least two elements
            back().~T();
            arraySize--;
        }
        else if (arraySize == 1) {
            back().~T();
            clear();
        }
    }

    // void emplace_back(Args&& ...args) - takes variable number of arguments and add them to the array
    template <class... Args>
    void emplace_back(Args&&... args) {

        // If the array does not exist
        if (arrayCapacity == 0) {
            arraySize = 0;
            arrayCapacity = 1;
            allocate(arrayCapacity);
            setToZero();
        }

        // If by adding an element we violate the size of the array - resize the array
        if (arraySize >= arrayCapacity) {
            size_t newCapacity = arrayCapacity * 2; // New capacity (double capacity)
            size_t tmpSize = arraySize; // Temporary size

            T* new_arr = (T*)malloc(sizeof(T) * newCapacity);
            for (size_t i = 0; i < arraySize; i++) {
                new_arr[i] = arr[i];
            }

            deallocate(); // Free memory of the old array
            arraySize = tmpSize; // Set size
            arrayCapacity = newCapacity; // Set capacity
            arr = new_arr; // Set array
            new_arr = nullptr; // Delete temporary array pointer
        }

        new (arr + arraySize) T(std::forward<Args>(args)...); // Add a new element to the array
        arraySize++; // Increase the size of the array
    }

    // T front() - returns the reference to the first element of the array
    T front() const {
        return *arr;
    }

    // T back() - returns the reference to the last element of the array
    T back() const {
        if (arraySize == 0) return *arr; // Will throw an error since arr == nullptr in this case
        else return *(arr + arraySize - 1);
    }

    // void append(const Array<T>& arr_2) - appends the elements of the "arr_2"to the end of the array
    void append(const Array<T>& arr_2) {
        push_back(arr_2);
    }

    // void swap(Array<T>& arr_2) - swaps two arrays
    void swap(Array<T>& arr_2) {
        T *tmp_arr = arr_2.arr; // Copy the array of arr_2
        size_t tmp_size = arr_2.arraySize;
        size_t tmp_capacity = arr_2.arrayCapacity;
        arr_2.arr = arr;
        arr_2.arraySize = arraySize;
        arr_2.arrayCapacity = arrayCapacity;

        arr = tmp_arr;
        arraySize = tmp_size;
        arrayCapacity = tmp_capacity;
    }

    // bool empty() - returns 1 ("true") if the array has size 0 and 0 ("false") otherwise
    bool empty() const noexcept {
        return arraySize == 0;
    }

    // void clear() - removes all elements from the array (which are destroyed), leaving the container with a size of 0.
    void clear() {
        if (arraySize > 0) {
            // If there are some elements in the array
            for (size_t i = 0; i < arraySize; i++) {
                arr[i].~T(); // Call the destructor of an element
            }
            arraySize = 0;
        }
    }

private:

    // T* allocate(size_t size) - allocates a new array of a given size
    void allocate(size_t size) {
        arrayCapacity = size; // Set array capacity
        arr = (T*)malloc(sizeof(T) * size); // Allocate memory
        setToZero(); // Set everything to zero
    }

    // void deallocate() - deallocates the array memory
    void deallocate() {
        if(arr) {
            for (size_t i = 0; i < arrayCapacity; i++) {
                arr[i].~T(); // Call the destructor of an element
            }

            free((void*)arr); // Free the memory
            arr = nullptr; // Delete array pointer
            arraySize = 0; // Set array size to 0
            arrayCapacity = 0; // Set capacity to 0
        }
    }

    // void setToZero() - initialize all objects in the array with 0's
    void setToZero() {
        for (size_t i = 0; i < arrayCapacity * sizeof(T); i++) {
            ((char*)arr)[i] = 0;
        }
    }
};
