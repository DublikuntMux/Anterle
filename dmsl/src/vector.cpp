#include <cassert>
#include <cstring>
#include <new>
#include <type_traits>

#include "vector.hpp"

template <class T>
inline void construct_range(T* begin, T* end)
{
    while (begin != end)
    {
        new (begin) T;
        begin++;
    }
}

template <class T>
inline void copy_range(T* begin, T* end, T* dest)
{
    while (begin != end)
    {
        new (dest) T(*begin);
        begin++;
        dest++;
    }
}

template <class T>
inline void destruct_range(T* begin, T* end)
{
    while (begin != end)
    {
        begin->~T();
        begin++;
    }
}

template <class T>
dmsl::vector<T>::vector(const vector& other)
    : m_data(reinterpret_cast<T*>(std::malloc(sizeof(T) * other.m_capacity))), m_size(other.m_size)
    , m_capacity(other.m_capacity)
{
    

    if (std::is_trivial_v<T>)
    {
        std::memcpy(m_data, other.m_data, other.m_size);
    }
    else
    {
        copy_range(other.begin(), other.end(), m_data);
    }
}

template <class T>
dmsl::vector<T>::vector(vector&& other) noexcept
    : m_data(other.m_data)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
{
    other.m_data = nullptr;
}

template <class T>
dmsl::vector<T>& dmsl::vector<T>::operator=(const vector& other)
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    m_data = reinterpret_cast<T*>(std::malloc(sizeof(T) * other.m_capacity));

    if (std::is_trivial_v<T>)
    {
        std::memcpy(m_data, other.m_data, other.m_size);
    }
    else
    {
        copy_range(other.begin(), other.end(), m_data);
    }

    return *this;
}

template <class T>
dmsl::vector<T>& dmsl::vector<T>::operator=(vector&& other) noexcept
{
    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_data = nullptr;

    return *this;
}

template <class T>
dmsl::vector<T>::vector::~vector()
{
    if (!std::is_trivial_v<T>)
    {
        destruct_range(begin(), end());
    }

    std::free(m_data);
}

template <class T>
T& dmsl::vector<T>::operator[](size_type pos)
{
    assert(pos < m_size && "Position is out of range");
    return m_data[pos];
}

template <class T>
const T& dmsl::vector<T>::operator[](size_type pos) const
{
    assert(pos < m_size && "Position is out of range");
    return m_data[pos];
}

template <class T>
T& dmsl::vector<T>::front()
{
    assert(m_size > 0 && "Container is empty");
    return m_data[0];
}

template <class T>
const T& dmsl::vector<T>::front() const
{
    assert(m_size > 0 && "Container is empty");
    return m_data[0];
}

template <class T>
T& dmsl::vector<T>::back()
{
    assert(m_size > 0 && "Container is empty");
    return m_data[m_size - 1];
}

template <class T>
const T& dmsl::vector<T>::back() const
{
    assert(m_size > 0 && "Container is empty");
    return m_data[m_size - 1];
}

template <class T>
T* dmsl::vector<T>::data() noexcept
{
    return m_data;
}

template <class T>
const T* dmsl::vector<T>::data() const noexcept
{
    return m_data;
}

template <class T>
T* dmsl::vector<T>::begin() noexcept
{
    return m_data;
}

template <class T>
const T* dmsl::vector<T>::begin() const noexcept
{
    return m_data;
}

template <class T>
T* dmsl::vector<T>::end() noexcept
{
    return m_data + m_size;
}

template <class T>
const T* dmsl::vector<T>::end() const noexcept
{
    return m_data + m_size;
}

template <class T>
bool dmsl::vector<T>::empty() const noexcept
{
    return m_size == 0;
}

template <class T>
typename dmsl::vector<T>::size_type dmsl::vector<T>::size() const noexcept
{
    return m_size;
}

template <class T>
void dmsl::vector<T>::reserve(size_type new_cap)
{
    assert(new_cap > m_capacity && "Capacity is already equal to or greater than the passed value");

    if constexpr (std::is_trivial_v<T>)
    {
        m_data = reinterpret_cast<T*>(std::realloc(m_data, sizeof(T) * new_cap));
        assert(m_data != nullptr && "Reallocation failed");
    }
    else
    {
        T* new_data_location = reinterpret_cast<T*>(std::malloc(sizeof(T) * new_cap));
        assert(new_data_location != nullptr && "Allocation failed");

        copy_range(begin(), end(), new_data_location);
        destruct_range(begin(), end());

        std::free(m_data);

        m_data = new_data_location;
    }

    m_capacity = new_cap;
}

template <class T>
typename dmsl::vector<T>::size_type dmsl::vector<T>::capacity() const noexcept
{
    return m_capacity;
}

template <class T>
void dmsl::vector<T>::shrink_to_fit()
{
    if (m_size < m_capacity)
    {
        if constexpr (std::is_trivial_v<T>)
        {
            m_data = reinterpret_cast<T*>(std::realloc(m_data, sizeof(T) * m_size));
            assert(m_data != nullptr && "Reallocation failed");
        }
        else
        {
            T* new_data_location = reinterpret_cast<T*>(std::malloc(sizeof(T) * m_size));
            assert(new_data_location != nullptr && "Allocation failed");

            copy_range(begin(), end(), new_data_location);
            destruct_range(begin(), end());

            std::free(m_data);

            m_data = new_data_location;
        }
    }
}

template <class T>
void dmsl::vector<T>::clear() noexcept
{
    if constexpr (!std::is_trivial_v<T>)
    {
        destruct_range(begin(), end());
    }

    m_size = 0;
}

template <class T>
void dmsl::vector<T>::push_back(const T& value)
{
    if (m_size == m_capacity)
    {
        reserve(m_capacity * vector::grow_factor + 1);
    }

    if constexpr (std::is_trivial_v<T>)
    {
        m_data[m_size] = value;
    }
    else
    {
        new (m_data + m_size) T(value);
    }

    m_size++;
}

template <class T>
void dmsl::vector<T>::push_back(T&& value)
{
    if (m_size == m_capacity)
    {
        reserve(m_capacity * vector::grow_factor + 1);
    }

    if constexpr (std::is_trivial_v<T>)
    {
        m_data[m_size] = value;
    }
    else
    {
        new (m_data + m_size) T(std::move(value));
    }

    m_size++;
}

template <class T>
template< class... Args >
void dmsl::vector<T>::emplace_back(Args&&... args)
{
    static_assert(!std::is_trivial_v<T>, "Use push_back() instead of emplace_back() with trivial types");

    if (m_size == m_capacity)
    {
        reserve(m_capacity * vector::grow_factor + 1);
    }

    new (m_data + m_size) T(std::forward<Args>(args)...);

    m_size++;
}

template <class T>
void dmsl::vector<T>::pop_back()
{
    assert(m_size > 0 && "Container is empty");

    if constexpr (!std::is_trivial_v<T>)
    {
        m_data[m_size - 1].~T();
    }

    m_size--;
}

template <class T>
void dmsl::vector<T>::resize(size_type count)
{
    assert(count != m_size && "Size is already equal to the passed value");

    if (count > m_capacity)
    {
        reserve(count);
    }

    if constexpr (!std::is_trivial_v<T>)
    {
        if (count > m_size)
        {
            construct_range(m_data + m_size, m_data + count);
        }
        else if (count < m_size)
        {
            destruct_range(m_data + count, m_data + m_size);
        }
    }

    m_size = count;
}
