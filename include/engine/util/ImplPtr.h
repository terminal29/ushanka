#pragma once

#include <utility>
#include <memory>

namespace U::Engine::Util {

	/// <summary>
	/// Wrapper for a pointer to an implementation
	/// 
	/// Adapted from https://github.com/sth/impl_ptr/blob/master/include/impl_ptr.hpp
	/// </summary>
	template<typename T>
	class ImplPtr {
	protected:
		std::default_delete<T> del;
		T* raw;

	public:
		template<typename ...Args>
		ImplPtr(Args... args) : raw(new T(std::forward<Args>(args)...)) {
		}

		ImplPtr(const ImplPtr<T>& other) : raw(new T(*other.raw)) {
		}

		ImplPtr(ImplPtr<T>&& other) : raw(new T(std::move(*other.raw))) {
		}

		~ImplPtr() noexcept (std::is_nothrow_destructible<T>::value) {
			del(raw);
		}

		ImplPtr& operator=(const ImplPtr<T>& other) noexcept(std::is_nothrow_copy_assignable<T>::value) {
			*raw = *other.raw;
			return *this;
		}

		ImplPtr& operator=(ImplPtr<T>&& other) noexcept(std::is_nothrow_move_assignable<T>::value) {
			*raw = std::move(*other.raw);
			return *this;
		}

		T* operator->() noexcept {
			return raw;
		}

		const T* operator->() const noexcept {
			return raw;
		}

		T& operator*() & noexcept {
			return *raw;
		}

		const T& operator*() const& noexcept {
			return *raw;
		}

		T&& operator*() && noexcept {
			return std::move(*raw);
		}
	};

}