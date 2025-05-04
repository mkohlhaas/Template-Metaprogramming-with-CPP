#include <iostream>
#include <print>

template <typename T>
T
max(T const a, T const b)
{
    return a > b ? a : b;
}

template <typename T>
void
swap(T *a, T *b)
{
    T t = *a;
    *a  = *b;
    *b  = t;
}

template <typename T>
int
partition(T arr[], int const low, int const high)
{
    T   pivot = arr[high];
    int i     = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

template <typename T>
void
quicksort(T arr[], int const low, int const high)
{
    if (low < high)
    {
        int const pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

template <typename T>
struct vector
{
    T      at(size_t const index) const;
    T      operator[](size_t const index) const;
    bool   empty() const;
    size_t capacity() const;
    size_t size() const;
    void   clear();
    void   pop_back();
    void   push_back(T value [[maybe_unused]]) {};
    void   resize(size_t const size);

  private:
    T     *data_;
    size_t size_;
    size_t capacity_;
};

template <typename T>
constexpr T NewLine = T('\n');

int
main()
{
    max<int>(1, 2);        // Ok
    max(1, 2);             // OK, compares ints
    max<double>(1.0, 2.0); // Ok
    max(1.0, 2.0);         // OK, compares doubles
    // max(1.0, 2);        // Error

    struct foo
    {
    };

    foo f1 [[maybe_unused]];
    foo f2 [[maybe_unused]];
    // Error, operator> not overloaded for foo
    // max<foo>(f1, f2);
    // max(f1, f2);

    vector<int> v [[maybe_unused]];
    v.push_back(5);
    v.push_back(10);

    int arr[] = {13, 1, 8, 3, 5, 2, 1};
    int n     = sizeof(arr) / sizeof(arr[0]);
    quicksort(arr, 0, n - 1);
    std::println("first: {}, last: {}", arr[0], arr[n - 1]);

    std::wstring test = L"demo";
    test += NewLine<wchar_t>;
    std::wcout << test;
}
