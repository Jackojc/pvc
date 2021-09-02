#ifndef BR_TRAIT_H
#define BR_TRAIT_H

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
	inline constexpr bool is_pointer_v = is_pointer<T>::value;


	// is_same
	template <typename T, typename U>
	struct is_same:
		false_type {};

	template <typename T>
	struct is_same<T, T>:
		true_type {};

	template <typename T, typename U>
	inline constexpr bool is_same_v = is_same<T, U>::value;



	// is_null_pointer
	template <typename T>
	struct is_null_pointer:
		is_same<nullptr_t, remove_cv_t<T>> {};

	template <typename T>
	inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

}

#endif
