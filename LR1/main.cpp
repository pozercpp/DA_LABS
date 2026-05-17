#include <iostream>

template <class T>
class MyVector
{
private:
    size_t sz;
    size_t capacity;
    T* data = nullptr;

public:
    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    private:
        pointer m_ptr;
    public:
        iterator(pointer ptr = nullptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        iterator& operator++() { ++m_ptr; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator& operator--() { --m_ptr; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
        iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        iterator operator+(difference_type n) const { iterator tmp = *this; tmp += n; return tmp; }
        iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
        iterator operator-(difference_type n) const { iterator tmp = *this; tmp -= n; return tmp; }
        difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }
        reference operator[](difference_type n) const { return *(m_ptr + n); }
        bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        bool operator<(const iterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const iterator& other) const { return other < *this; }
        bool operator<=(const iterator& other) const { return !(*this > other); }
        bool operator>=(const iterator& other) const { return !(*this < other); }
    };
    class const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    private:
        pointer m_ptr;
    public:
        const_iterator(pointer ptr = nullptr) : m_ptr(ptr) {}
        const_iterator(const iterator& it) : m_ptr(it.m_ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        const_iterator& operator++() { ++m_ptr; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        const_iterator& operator--() { --m_ptr; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }
        const_iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        const_iterator operator+(difference_type n) const { const_iterator tmp = *this; tmp += n; return tmp; }
        const_iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
        const_iterator operator-(difference_type n) const { const_iterator tmp = *this; tmp -= n; return tmp; }
        difference_type operator-(const const_iterator& other) const { return m_ptr - other.m_ptr; }
        reference operator[](difference_type n) const { return *(m_ptr + n); }
        bool operator==(const const_iterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
        bool operator<(const const_iterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const const_iterator& other) const { return other < *this; }
        bool operator<=(const const_iterator& other) const { return !(*this > other); }
        bool operator>=(const const_iterator& other) const { return !(*this < other); }
    };
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
    ~MyVector() noexcept {
        delete[] data;
    	data = nullptr;
    	sz = 0;
    	capacity = 0;
    }
    
    void pop()
    {
        if (sz > 0) sz--;
    }
    
    void clear() {
        sz = 0;
    }

    void push_back(const T& n)
    {
        if (sz >= capacity) {
            reallocate(capacity ? capacity * 2 : 1);
        }
        data[sz] = n;
        sz++;
    }
    
    void push_back(T&& n)
    {
        if (sz >= capacity) {
            reallocate(capacity ? capacity * 2 : 1);
        }
        data[sz] = std::move(n);
        sz++;
    }
    
    MyVector() 
        : sz(0), capacity(0), data(nullptr){};
    
    MyVector(const size_t& n) 
        : sz(n), capacity(n), data(new T[n]) {}

    MyVector(const size_t& n, const T& elem) 
        : sz(n), capacity(n), data(new T[n]) {
            for (size_t i = 0; i < n; ++i) {
                data[i] = elem;
            }
        }
    
    MyVector(const std::initializer_list<T>& t)
        : sz(0), capacity(t.size()), data(new T[t.size()])
    {
        for (const T& i : t) {
            push_back(i);
        }
    }
    
    MyVector(const MyVector<T>& other)
        : sz(other.sz), capacity(other.capacity), data(new T[other.capacity])
    {
        std::copy(other.data, other.data + other.sz, data);
    }
    
    MyVector(MyVector<T>&& other) noexcept
        : sz(other.sz), capacity(other.capacity), data(other.data)
    {
        other.data = nullptr;
        other.sz = 0;
        other.capacity = 0;
    }
    
    void erase(size_t ind) {
        for (size_t i = ind; i < sz - 1; ++i) {
            data[i] = data[i + 1];
        }
        sz--;
    }
    
    void reallocate(size_t newsize) {
        if (newsize == 0) newsize = 1;
        T* newdata = new T[newsize];
        if (data) {
            for (size_t i = 0; i < sz; ++i) {
                newdata[i] = std::move(data[i]);
            }
            delete[] data;
        }
        data = std::move(newdata);
        capacity = newsize;
    }
    
    void resize(size_t newsize) {
        if (newsize > sz) {
            reallocate(newsize);
        }
        sz = newsize;
    }
    
    T& operator[](size_t ind) {
        return data[ind];
    }
    
    const T& operator[](size_t ind) const {
        return data[ind];
    }
    
    const size_t size() const {
        return sz;
    }
    
    MyVector<T>& operator=(const MyVector<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
    
        capacity = other.capacity;
        sz = other.sz;
        data = new T[capacity];
    
        std::copy(other.data, other.data + other.sz, data);
    
        return *this;
    }

    MyVector<T>& operator=(MyVector<T>&& other) {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            capacity = other.capacity;
            other.data = nullptr;
            other.sz = 0;
            other.capacity = 0;
        }
        return *this;
    }

    iterator begin() noexcept { return iterator(data); }
    iterator end() noexcept { return iterator(data + sz); }
    const_iterator begin() const noexcept { return const_iterator(data); }
    const_iterator end() const noexcept { return const_iterator(data + sz); }
    const_iterator cbegin() const noexcept { return const_iterator(data); }
    const_iterator cend() const noexcept { return const_iterator(data + sz); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    
};

using String = MyVector<char>;

struct node {
    String key;
    String val;
    node() = default;
    node(const node& other) : key(other.key), val(other.val) {}
    node(node&& other) : key(std::move(other.key)), val(std::move(other.val)) {}
    ~node() noexcept = default;
    node& operator =(const node& other) {
        key = other.key;
        val = other.val; 
        return *this;
    }
    node& operator =(node&& other) {
        key = std::move(other.key);
        val = std::move(other.val); 
        return *this;
    }
};

struct date {
    size_t day, month, year;
};

std::size_t find(const String& line, char c, std::size_t start = 0) {
    for (std::size_t i = start; i < line.size(); ++i) {
        if (line[i] == c) {
            return i;
        }
    }
    return line.size();
}

String substr(const String& line, std::size_t start, std::size_t end) {
    String result;
    for (std::size_t i = start; i < end && i < line.size(); ++i) {
        result.push_back(line[i]);
    }
    return result;
}

int stoi(const String& line) {
    int result = 0;
    for (std::size_t i = 0; i < line.size(); ++i) {
        result = result * 10 + (line[i] - '0');
    }
    return result;
}

date date_to_numb(const String& s) {
    size_t day = 0, month = 0, year = 0;
    date d;
    int part = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '.') {
            part++;
            continue;
        }
        if (part == 0)
            d.day = day * 10 + (s[i] - '0');
        else if (part == 1)
            d.month = month * 10 + (s[i] - '0');
        else
            d.year = year * 10 + (s[i] - '0');
    }
    return d;
}

enum dates {
    day = 0,
    month = 1,
    year = 2
};

template<typename iterator>
size_t get_key(iterator it, dates fl) {
    if (fl == day) {
        return it->day;
    }
    if (fl == month) {
        return it->month;
    }
    return it->year;
}

template<typename iterator>
void count_sort(iterator start, iterator end, dates fl) {
    if (start == end) return;
    DynamicArray<size_t> cnt(65536);
    for (auto it = start; it != end; ++it) {
        ++cnt[get_key(it, fl)];
    }
    for (size_t i = 0; i + 1 < cnt.size(); ++i) {
        cnt[i + 1] += cnt[i];
    }
    DynamicArray<date> res(std::distance(start, end));
    for (int i = res.size() - 1; i > -1; --i) {
        res[--cnt[get_key(start + i, fl)]] = *(start + i);
    }
    for (size_t i = 0; i < res.size(); ++i) {
        *(start + i) = res[i];
    }
}

template<typename iterator>
void radix_sort(iterator begin, iterator end) {
    count_sort(start, end, day);
    count_sort(start, end, month);
    count_sort(start, end, year);
}

std::istream& operator>>(std::istream& is, String& s) {    
    char c;
    while (is.get(c) && std::isspace(c)); 
    if (!is) return is;
    s.clear();
    s.push_back(c);
    while(is.get(c) && !std::isspace(c)) {
        s.push_back(c);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    for (char c : s) {
        os << c;
    }
    return os;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    MyVector<node> v;
    node elem;
    MyVector<date> vals;
    while (std::cin >> elem.key >> elem.val) {
        vals.push_back(date_to_numb(elem.key));
        v.push_back(std::move(elem));
    }
    
    MyVector<size_t> order(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        order[i] = i;
    }
    return 0;
}