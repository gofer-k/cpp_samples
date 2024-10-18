/*
Our algorithm for finding prime numbers is a simple sieve, represented in Haskell as:

  primesTo n = sieve [2..n]
               where 
               sieve (x:xs) =
                 x:(sieve $
                    filter (\a -> a 'mod' x /= 0)
                           xs)
               sieve [] = []
*/
namespace Primers {

template <typename T, T Val>
struct integral_constant {
    static const T value = Val;
    typedef T value_type;
    typedef integral_constant type;
};

template <bool Condition, typename Then, typename Else>
struct conditional;

template <typename Then, typename Else>
struct conditional<true, Then, Else> {
    typedef Then type;
};

template <typename Then, typename Else>
struct conditional<false, Then, Else> {
    typedef Else type;
};

struct nil {};

template <typename Head, typename Tail = nil>
struct list {};

template <template <typename> class Pred, typename List>
struct filter;

template <template <typename> class Pred, typename Head, typename Tail>
struct filter<Pred, list<Head, Tail> > {
    typedef
        typename conditional<Pred<Head>::value,
                             list<Head, typename filter<Pred, Tail>::type>,
                             typename filter<Pred, Tail>::type>::type type;
};

template <template <typename> class Pred>
struct filter<Pred, nil> {
    typedef nil type;
};

template <int first, int last>
struct range {
    typedef list<integral_constant<int, first>,
                 typename range<first + 1, last>::type>
        type;
};

template <int last>
struct range<last, last> {
    typedef nil type;
};

template <typename T>
struct sieve_prime;

template <typename Head, typename Tail>
struct sieve_prime<list<Head, Tail> > {
    template <typename T>
    struct is_not_divisible
        : integral_constant<bool, (T::value % Head::value) != 0> {};

    typedef list<Head, typename sieve_prime<typename filter<
                           is_not_divisible, Tail>::type>::type>
        type;
};

template <>
struct sieve_prime<nil> {
    typedef nil type;
};

template <int N>
struct primes_to : sieve_prime<typename range<2, N + 1>::type>::type {};


template <typename Head, typename Tail>
void output(list<Head, Tail>)
{
  std::cout << Head::value << "\t"; 
  output(Tail());
}

inline void output(nil)
{}


} // Primes