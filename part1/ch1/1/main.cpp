#include <cstddef>

int
max(int const a, int const b)
{
    return a > b ? a : b;
}

double
max(double const a, double const b)
{
    return a > b ? a : b;
}

using swap_fn    = void (*)(void *, int const, int const);
using compare_fn = bool (*)(void *, int const, int const);

int
partition(void *arr, int const low, int const high, compare_fn cmp, swap_fn swap)
{
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (cmp(arr, j, high))
        {
            i++;
            swap(arr, i, j);
        }
    }

    swap(arr, i + 1, high);

    return i + 1;
}

void
quicksort(void *arr, int const low, int const high, compare_fn cmp, swap_fn swap)
{
    if (low < high)
    {
        int const pi = partition(arr, low, high, cmp, swap);
        quicksort(arr, low, pi - 1, cmp, swap);
        quicksort(arr, pi + 1, high, cmp, swap);
    }
}

void
swap_int(void *arr, int const i, int const j)
{
    int *iarr = (int *)arr;
    int  t    = iarr[i];
    iarr[i]   = iarr[j];
    iarr[j]   = t;
}

bool
less_int(void *arr, int const i, int const j)
{
    int *iarr = (int *)arr;
    return iarr[i] <= iarr[j];
}

struct int_vector
{
    int_vector();

    bool   empty() const;
    int    at(size_t const index) const;
    int    operator[](size_t const index) const;
    size_t capacity() const;
    size_t size() const;
    void   clear();
    void   pop_back();
    void   push_back(int value);
    void   resize(size_t const size);

  private:
    int   *data_;
    size_t size_;
    size_t capacity_;
};

constexpr char     NewLine [[maybe_unused]]    = '\n';
constexpr wchar_t  NewLineW [[maybe_unused]]   = L'\n';
constexpr char8_t  NewLineU8 [[maybe_unused]]  = u8'\n';
constexpr char16_t NewLineU16 [[maybe_unused]] = u'\n';
constexpr char32_t NewLineU32 [[maybe_unused]] = U'\n';

int
main()
{

    int arr[] = {13, 1, 8, 3, 5, 2, 1};
    int n     = sizeof(arr) / sizeof(arr[0]);
    quicksort(arr, 0, n - 1, less_int, swap_int);
}
