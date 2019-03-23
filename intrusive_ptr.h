//
// Created by Семен Трапезников on 23/03/2019.
//

#ifndef UNTITLED_INTRUSIVE_PTR_H
#define UNTITLED_INTRUSIVE_PTR_H

template <class T>
class intrusive_ptr {
private:
    T* ptr;

    int add_ref() {
        if (ptr == nullptr) {
            return 0;
        }
        ptr->add_ref();
        return ptr->get_refcnt();
    }

    int release() {
        if (ptr == nullptr) {
            return 0;
        }
        unsigned int res = ptr->get_refcnt();
        ptr->release();
        return res;
    }

public:
    intrusive_ptr() {
        ptr = nullptr;
    }

    intrusive_ptr(T* arg) {
        ptr = arg;
        add_ref();
    }

    template <class U>
    explicit intrusive_ptr(U* arg) {
        ptr = static_cast<T*>(arg);
        add_ref();
    }

    intrusive_ptr(const intrusive_ptr<T>& arg) {
        ptr = arg.ptr;
        add_ref();
    }

    intrusive_ptr(const intrusive_ptr<T>&& arg) {
        ptr = arg.ptr;
        add_ref();
    }

    template <class U>
    intrusive_ptr(const intrusive_ptr<U>& arg) {
        ptr = static_cast<T*>(arg.get());
        add_ref();
    }

    ~intrusive_ptr() {
        release();
    }

    int get_refcnt() {
        if (ptr == nullptr) {
            return 0;
        }
        return ptr->get_refcnt();
    }

    intrusive_ptr& operator=(const intrusive_ptr& arg) {
        release();
        ptr = arg.ptr;
        add_ref();
        return *this;
    }

    intrusive_ptr& operator=(const intrusive_ptr&& arg) {
        release();
        ptr = arg.ptr;
        add_ref();
        return *this;
    }

    template <class U>
    intrusive_ptr& operator=(const intrusive_ptr<U>& arg) {
        release();
        ptr = static_cast<T*>(arg.ptr);
        add_ref();
        return *this;
    }

    intrusive_ptr& operator=(T* arg) {
        release();
        ptr = arg;
        add_ref();
        return *this;
    }

    void reset() {
        release();
        ptr = nullptr;
    }

    void reset(T* arg) {
        release();
        ptr = arg;
        add_ref();
    }

    template <class U>
    friend void swap(intrusive_ptr<U>& a, intrusive_ptr<U>& b) noexcept;

    template <class U>
    void swap(intrusive_ptr<U>& arg) {
        U* buf = arg.ptr;
        arg.ptr = ptr;
        ptr = buf;
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    explicit operator bool() const {
        return (ptr != nullptr);
    }

    bool operator!() {
        return (ptr == nullptr);
    }
};

template <class U>
void swap(intrusive_ptr<U>& a, intrusive_ptr<U>& b) noexcept {
    a.swap(b);
}

template<class T, class U>
bool operator==(const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) {
    return (a.get() == b.get());
}

template<class T, class U>
bool operator!=(const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) {
    return (a.get() != b.get());
}

template<class T, class U>
bool operator==(const intrusive_ptr<T>& a, U* b) {
    return (a.get() == b);
}

template<class T, class U>
bool operator!=(const intrusive_ptr<T>& a, U* b) {
    return (a.get() != b);
}

template<class T, class U>
bool operator==(T* a, const intrusive_ptr<U>& b) {
    return (a == b.get());
}

template<class T, class U>
bool operator!=(T* a, const intrusive_ptr<U>& b) {
    return (a != b.get());
}

template <class T, class U>
intrusive_ptr<T> dynamic_pointer_cast(const intrusive_ptr<U>& arg) {
    return intrusive_ptr<T>(dynamic_cast<T*>(arg.get()));
}


#endif //UNTITLED_INTRUSIVE_PTR_H
