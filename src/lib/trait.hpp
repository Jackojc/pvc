#ifndef BR_TRAIT_H
#define BR_TRAIT_H

#include <lib/def.hpp>

namespace br {

	// https://en.cppreference.com/w/cpp/types/integral_constant
	// Integral constants and true/false types.
	template <class T, T v>
	struct integral_constant {
		static constexpr T value = v;
		using value_type = T;
	};

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;


	// remove_cv
	template <typename T> struct remove_cv                   { using type = T; };
	template <typename T> struct remove_cv<const T>          { using type = T; };
	template <typename T> struct remove_cv<volatile T>       { using type = T; };
	template <typename T> struct remove_cv<const volatile T> { using type = T; };

	template <typename T> struct remove_const                { using type = T; };
	template <typename T> struct remove_const<const T>       { using type = T; };

	template <typename T> struct remove_volatile             { using type = T; };
	template <typename T> struct remove_volatile<volatile T> { using type = T; };

	template <typename T> using remove_cv_t       = typename remove_cv<T>::type;
	template <typename T> using remove_const_t    = typename remove_const<T>::type;
	template <typename T> using remove_volatile_t = typename remove_volatile<T>::type;


	// is_pointer
	template <typename T>
	struct is_pointer_helper:
		false_type {};

	template <typename T>
	struct is_pointer_helper<T*>:
		true_type {};

	template <typename T>
	struct is_pointer:
		is_pointer_helper<typename remove_cv<T>::type> {};

	template <typename T>
	constexpr bool is_pointer_v = is_pointer<T>::value;


	// is_same
	template <typename T, typename U>
	struct is_same:
		false_type {};

	template <typename T>
	struct is_same<T, T>:
		true_type {};

	template <typename T, typename U>
	constexpr bool is_same_v = is_same<T, U>::value;



	// is_null_pointer
	template <typename T>
	struct is_null_pointer:
		is_same<nullptr_t, remove_cv_t<T>> {};

	template <typename T>
	constexpr bool is_null_pointer_v = is_null_pointer<T>::value;


	// is_integral
	template <typename> struct is_integral_base: false_type {};


	template <> struct is_integral_base<bool>:  true_type {};

	template <> struct is_integral_base<u8_t>:  true_type {};
	template <> struct is_integral_base<i8_t>:  true_type {};

	template <> struct is_integral_base<u16_t>: true_type {};
	template <> struct is_integral_base<i16_t>: true_type {};

	template <> struct is_integral_base<u32_t>: true_type {};
	template <> struct is_integral_base<i32_t>: true_type {};

	template <> struct is_integral_base<u64_t>: true_type {};
	template <> struct is_integral_base<i64_t>: true_type {};


	template <typename T> struct is_integral: is_integral_base<remove_cv_t<T>> {};

	template <typename T>
	constexpr bool is_integral_v = is_integral<T>::value;


	// is_signed/is_unsigned
	template <typename> struct is_signed_base: false_type {};
	template <typename> struct is_unsigned_base: false_type {};


	template <> struct is_signed_base<i8_t>:  true_type {};
	template <> struct is_signed_base<i16_t>: true_type {};
	template <> struct is_signed_base<i32_t>: true_type {};
	template <> struct is_signed_base<i64_t>: true_type {};

	template <> struct is_unsigned_base<u8_t>:  true_type {};
	template <> struct is_unsigned_base<u16_t>: true_type {};
	template <> struct is_unsigned_base<u32_t>: true_type {};
	template <> struct is_unsigned_base<u64_t>: true_type {};


	template <typename T> struct is_signed: is_signed_base<remove_cv_t<T>> {};
	template <typename T> struct is_unsigned: is_unsigned_base<remove_cv_t<T>> {};

	template <typename T> constexpr bool is_signed_v = is_signed<T>::value;
	template <typename T> constexpr bool is_unsigned_v = is_unsigned<T>::value;

}

#endif
