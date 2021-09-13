#ifndef BR_TRAIT_H
#define BR_TRAIT_H

#include <lib/def.hpp>

namespace br {

	// https://en.cppreference.com/w/cpp/types/integral_constant
	// Integral constants and true/false types.
	template <typename T, T v>
	struct integral_constant {
		static constexpr T value = v;
		using value_type = T;
	};

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;


	// https://en.cppreference.com/w/cpp/types/remove_cv
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


	// https://en.cppreference.com/w/cpp/types/is_pointer
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


	// https://en.cppreference.com/w/cpp/types/is_same
	// is_same
	template <typename T, typename U>
	struct is_same:
		false_type {};

	template <typename T>
	struct is_same<T, T>:
		true_type {};

	template <typename T, typename U>
	constexpr bool is_same_v = is_same<T, U>::value;


	// https://en.cppreference.com/w/cpp/types/is_null_pointer
	// is_null_pointer
	template <typename T>
	struct is_null_pointer:
		is_same<nullptr_t, remove_cv_t<T>> {};

	template <typename T>
	constexpr bool is_null_pointer_v = is_null_pointer<T>::value;


	// https://en.cppreference.com/w/cpp/types/is_integral
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


	// is_array
	template <typename T>
	struct is_array: false_type {};

	template <typename T>
	struct is_array<T[]>: true_type {};

	template <typename T, size_t N>
	struct is_array<T[N]>: true_type {};


	template <typename T>
	constexpr bool is_array_v = is_array<T>::value;


	// add_rvalue_reference
	template <typename T>
	struct type_identity { using type = T; };

	template <typename T>
	auto try_add_rvalue_reference(int) -> type_identity<T&&>;

	template <typename T>
	auto try_add_rvalue_reference(...) -> type_identity<T>;

	template <typename T>
	struct add_rvalue_reference: decltype(try_add_rvalue_reference<T>(0)) {};


	// declval
	template <typename T>
	typename add_rvalue_reference<T>::type declval();


	// conditional
	template <bool B, typename T, typename F>
	struct conditional { using type = T; };

	template <typename T, typename F>
	struct conditional<false, T, F> { using type = F; };

	template <bool B, typename T, typename F>
	using conditional_t = typename conditional<B, T, F>::type;


	// conjunction (logical and)
	template <typename...> struct conjunction: true_type { };

	template <typename B1> struct conjunction<B1>: B1 { };
	template <typename B1, typename... Bn>

	struct conjunction<B1, Bn...>: conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

	template <typename... B>
	constexpr bool conjunction_v = conjunction<B...>::value;


	// disjunction (logical or)
	template <typename...> struct disjunction: false_type { };

	template <typename B1> struct disjunction<B1>: B1 { };
	template <typename B1, typename... Bn>

	struct disjunction<B1, Bn...>: conditional_t<bool(B1::value), B1, disjunction<Bn...>>  { };

	template <typename... B>
	constexpr bool disjunction_v = disjunction<B...>::value;


	// is_void
	template <typename T>
	struct is_void: is_same<void, typename remove_cv<T>::type> {};

	template <typename T>
	constexpr bool is_void_v = is_void<T>::value;


	// is_convertible
	namespace detail {
		template <typename T>
		constexpr auto test_returnable(int) -> decltype(
			void(static_cast<T(*)()>(nullptr)), true_type{}
		);

		template <typename>
		constexpr auto test_returnable(...) -> false_type;

		template <typename From, typename To>
		constexpr auto test_implicitly_convertible(int) -> decltype(
			void(declval<void(&)(To)>()(declval<From>())), true_type{}
		);

		template <typename, typename>
		constexpr auto test_implicitly_convertible(...) -> false_type;
	}

	template <typename From, typename To>
	struct is_convertible: integral_constant<bool,
		(decltype(detail::test_returnable<To>(0))::value &&
		decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
		(is_void<From>::value && is_void<To>::value)
	> {};

	template <typename From, typename To>
	constexpr bool is_convertible_v = is_convertible<From, To>::value;


	// equivalence (Ts are convertible to T)
	template <typename T, typename... Ts>
	struct equivalence: conjunction<is_convertible<Ts, T>...> {};

	template <typename T, typename... Ts>
	constexpr bool equivalence_v = equivalence<T, Ts...>::value;


	// parity (Ts are the same as T)
	template <typename T, typename... Ts>
	struct parity: conjunction<is_same<T, Ts>...> {};

	template <typename T, typename... Ts>
	constexpr bool parity_v = parity<T, Ts...>::value;


	// first/last (first and last types in variadic pack)
	template <typename T, typename... Ts>
	struct first { using type = T; };

	template <typename T, typename... Ts>
	struct last { using type = typename last<Ts...>::type; };


	template <typename... Ts>
	using first_t = typename first<Ts...>::type;

	template <typename... Ts>
	using last_t = typename last<Ts...>::type;


	// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf.
	template <typename...>
	using void_t = void;


	template <typename Default, typename AlwaysVoid, template<typename...> typename Op, typename... Args>
	struct detector {
		using value_t = false_type;
		using type = Default;
	};

	template <typename Default, template<typename...> typename Op, typename... Args>
	struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
		using value_t = true_type;
		using type = Op<Args...>;
	};

	struct nonesuch {
		nonesuch() = delete;
		~nonesuch() = delete;
		nonesuch(nonesuch const&) = delete;
		void operator=(nonesuch const&) = delete;
	};

	template <template<typename...> typename Op, typename... Args>
	using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

	template <template<typename...> typename Op, typename... Args>
	constexpr bool is_detected_v = is_detected<Op, Args...>::value;

	template <template<typename...> typename Op, typename... Args>
	using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

	template <typename Default, template<typename...> typename Op, typename... Args>
	using detected_or = detector<Default, void, Op, Args...>;


	// is_specialisation
	template <typename Test, template<typename...> class Ref>
	struct is_specialisation: false_type {};

	template <template<typename...> class Ref, typename... Args>
	struct is_specialisation<Ref<Args...>, Ref>: true_type {};

	template <typename Test, template<typename...> class Ref>
	constexpr bool is_specialisation_v = is_specialisation<Test, Ref>::value;

}

#endif
