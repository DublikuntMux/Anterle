#pragma once

namespace dmsl {
template <class T>
inline void construct_range(T* begin, T* end);

template <class T>
inline void copy_range(T* begin, T* end, T* dest);

template <class T>
inline void destruct_range(T* begin, T* end);

template <class T>
class vector
{
public:
    using size_type = size_t;

    vector() = default;
    vector(const vector& other);
    vector(vector&& other) noexcept;
    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;
    ~vector();

    T& operator[](size_type pos);
    const T& operator[](size_type pos) const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    T* data() noexcept;
    const T* data() const noexcept;

    T* begin() noexcept;
    const T* begin() const noexcept;

    T* end() noexcept;
    const T* end() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    void reserve(size_type new_cap);
    [[nodiscard]] size_type capacity() const noexcept;
    void shrink_to_fit();


    void clear() noexcept;

    void push_back(const T& value);
    void push_back(T&& value);

    template< class... Args >
    void emplace_back(Args&&... args);
    
    void pop_back();
    void resize(size_type count);

    static constexpr size_type grow_factor = 2;

private:
    T* m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity = 0;
};
}
