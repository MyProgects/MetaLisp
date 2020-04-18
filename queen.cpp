#include "MetaLisp.hpp"
#include <iostream>

template <typename low, typename high>
struct enumerate_interval {
    using type =
        typename if_else<is_greater<low, high>, null, cons<low, enumerate_interval<add<low, number<1>>, high>>>::type;
    using tag = typename type::tag;
};

template <typename lst0, typename lst1>
struct zip {
    using type = typename if_else<or_<is_null<lst0>, is_null<lst1>>,
                                  null,
                                  cons<cons<car<lst0>, car<lst1>>, zip<cdr<lst0>, cdr<lst1>>>>::type;
    using tag = typename type::tag;
};

using empty_board = null;

template <typename k, typename positions>
struct is_safe {
    template <typename v0, typename v1>
    using is_same_row = is_equal<car<v0>, car<v1>>;

    template <typename v0, typename v1>
    struct is_same_diag {
        using tmp0 = typename abs_<sub<car<v0>, car<v1>>>::type;
        using tmp1 = typename abs_<sub<cdr<v0>, cdr<v1>>>::type;
        using type = typename is_equal<tmp0, tmp1>::type;
    };

    using pair_list = typename zip<positions, enumerate_interval<number<1>, k>>::type;
    using val = typename car<pair_list>::type;

    template <typename x>
    struct filter_op : public or_<is_same_row<x, val>, is_same_diag<x, val>> {};

    using type = typename is_equal<length<filter<filter_op, cdr<pair_list>>>, number<0>>::type;
    using tag = typename type::tag;
};

template <typename new_row, typename k, typename rest_of_queens>
using adjoin_position = cons<new_row, rest_of_queens>;

template <typename board_size>
struct queen {
    template <typename k>
    struct queen_cols {
        template <typename positions>
        using filter_lambda = is_safe<k, positions>;

        template <typename rest_of_queens>
        struct flatmap_lambda {
            template <typename new_row>
            using map_lambda = adjoin_position<new_row, k, rest_of_queens>;
            using type = typename map<map_lambda, enumerate_interval<number<1>, board_size>>::type;
        };

        using type =
            typename if_else<is_equal<k, number<0>>,
                             list<empty_board>,
                             filter<filter_lambda, flatmap<flatmap_lambda, queen_cols<sub<k, number<1>>>>>>::type;
        using tag = typename type::tag;
    };
    using type = typename queen_cols<board_size>::type;
    using tag = typename type::tag;
};

int main(int argc, char **argv) {
    using answer = queen<number<5>>;
    display<answer>();
    return 0;
}
