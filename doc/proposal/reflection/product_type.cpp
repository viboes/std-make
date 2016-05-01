// <product_type>
#include <type_traits>
#include <utility>

namespace std {
    template <size_t N>
    using size_t_constant = integral_constant<size_t, N>;

    namespace detail {
        template <class PT>
        PT* type() {return nullptr;}
    }
    template <class PT>
    using type = PT* (*)();

namespace product_type {

namespace detail {
    template<typename T>
    struct has_product_type_size
    {
        struct yes{ char a[1]; };
        struct no { char a[2]; };

        template<typename U>
            static constexpr yes test(decltype(product_type_size(&std::detail::type<U>)) *);
        template<typename U>
            static constexpr no test(...);
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
    };

    template<typename T>
    struct has_product_type_size_static_member
    {
        struct yes{ char a[1]; };
        struct no { char a[2]; };

        template<typename U> static yes test(decltype(U::product_type_size())*);
        template<typename U> static no test(...);
        static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
    };
}
    template <class PT, class Enabler = void>
    struct size;

    template <class T> struct size<const T> : size<T> {};
    template <class T> struct size<volatile T> : size<T> {};
    template <class T> struct size<const volatile T> : size<T> {};

    // customization for types defining product_type_size(T*(*)())
    template <class PT>
    struct size<PT, typename std::enable_if<detail::has_product_type_size<PT>::value>::type>
        : size_t_constant< product_type_size(&std::detail::type<PT>)>
    {};
    // customization for types defining PT::product_type_size()
    template <class PT>
    struct size<PT, typename std::enable_if<detail::has_product_type_size_static_member<PT>::value>::type>
        : size_t_constant<PT::product_type_size()>
    {};

    template <size_t I, class PT>
    struct element;
    template <size_t I, class T> struct element<I, const T> : element<I, T> {};
    template <size_t I, class T> struct element<I, volatile T> : element<I, T> {};
    template <size_t I, class T> struct element<I, const volatile T> : element<I, T> {};


    // C-arrays
    template <class T, size_t N>
      struct size<T [N]> : public integral_constant<std::size_t, N> {};

    template <size_t I, class T, size_t N>
      struct element<I, T [N] > { public: using type = T; };

    template <size_t I, class T, size_t N>
      constexpr const T& get(const T (&arr)[N]) noexcept { return arr[I]; }
    template <size_t I, class T, size_t N>
      constexpr T& get(T (&arr)[N]) noexcept { return arr[I]; }

    // case based on customization get<N>(pt)
    template <size_t N, class PT>
    constexpr auto get(PT const& pt) -> decltype(product_type_get(size_t_constant<N>{},pt))
    {
        return product_type_get(size_t_constant<N>{},pt);
    }
    template <size_t N, class PT>
    constexpr auto get(PT & pt) -> decltype(product_type_get(size_t_constant<N>{},pt))
    {
        return product_type_get(size_t_constant<N>{}, pt);
    }
    template <size_t N, class PT>
    constexpr auto get(PT&& pt) -> decltype(product_type_get(size_t_constant<N>{},forward<PT>(pt)))
    {
        return product_type_get(size_t_constant<N>{}, forward<PT>(pt));
    }

    // case based on customization pt.get<N>()
    template <size_t N, class PT>
    constexpr auto get(PT const& pt)
        -> decltype(pt.get(size_t_constant<N>{}))
    {
        return pt.get(size_t_constant<N>{});
    }
    template <size_t N, class PT>
    constexpr auto get(PT & pt)
        -> decltype(pt.get(size_t_constant<N>{}))
    {
        //return pt.get<N>();
        return pt.get(size_t_constant<N>{});
    }
    template <size_t N, class PT>
    constexpr auto get(PT&& pt)
        -> decltype(pt.get(size_t_constant<N>{}))
    {
        return pt.get(size_t_constant<N>{});
    }



}

        // customization for pair
    //template <class T1, class T2>
    //constexpr size_t product_type_size(type< pair<T1,T2> >) { return 2; };
    template <class T1, class T2>
    constexpr size_t product_type_size(type<pair<T1,T2>>) { return 2; };

    template <class T1, class T2>
    constexpr T1& product_type_get(size_t_constant<0>, pair<T1,T2> & x) { return x.first; };
    template <class T1, class T2>
    constexpr T2& product_type_get(size_t_constant<1>, pair<T1,T2> & x) { return x.second; };

    template <class T1, class T2>
    constexpr T1 const& product_type_get(size_t_constant<0>, pair<T1,T2> const& x) { return x.first; };
    template <class T1, class T2>
    constexpr T2 const& product_type_get(size_t_constant<1>, pair<T1,T2> const& x) { return x.second; };

}


// main
#include <iostream>


int main() {
    {
        int arr[] = {0,1,2};
        std::cout << std::product_type::size<decltype(arr)>::value << "\n";
        std::cout << std::product_type::get<0>(arr) << "\n";
        std::cout << std::product_type::get<1>(arr) << "\n";
        std::cout << std::product_type::get<2>(arr) << "\n";
        std::cout <<"\n";
    }
    {
        using namespace std::product_type;
        int arr[] = {0,1,2};
        std::cout << get<0>(arr) << "\n";
        std::cout << get<1>(arr) << "\n";
        std::cout << get<2>(arr) << "\n";
        std::cout <<"\n";
    }
    {
        using namespace std::product_type;
        int arr[] = {0,1,2};
        get<0>(arr)++;
        std::cout << get<0>(arr) << "\n";
        std::cout << get<1>(arr) << "\n";
        std::cout << get<2>(arr) << "\n";
        std::cout <<"\n";
    }
    {
        using T = std::pair<int,int>;
        T p = {0,1};
        static_assert(std::is_same<std::type<T>, decltype(&std::detail::type<T>)>::value, "Hrr");
        static_assert(std::product_type::detail::has_product_type_size<T>::value, "Hrr");
        std::cout << std::product_type::size<T>::value << "\n";
        std::cout << std::product_type::get<0>(p) << "\n";
        std::cout << std::product_type::get<1>(p) << "\n";
        std::cout <<"\n";

    }
    {
        using T = std::pair<int,int>;
        const T p = {0,1};
        static_assert(std::is_same<std::type<T>, decltype(&std::detail::type<T>)>::value, "Hrr");
        std::cout << product_type_size(std::type<T>{}) << "\n";
        std::cout << product_type_size(&std::detail::type<T>) << "\n";
        std::cout << product_type_size((T*(*)())([]() -> T* {return nullptr;})) << "\n";
        std::cout << std::product_type::size<T>::value << "\n";
        std::cout << std::product_type::size<decltype(p)>::value << "\n";
        std::cout << std::product_type::get<0>(p) << "\n";
        std::cout << std::product_type::get<1>(p) << "\n";
        std::cout <<"\n";

    }

}
