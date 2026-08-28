/**
 * Author: simon lindholm
 * Date: 2016-03-22
 * License: CC0
 * Source: hac_kit, NWERC 2015
 * Description: A set (not multiset!) with support for finding the n'th
 *  element and the number of elements smaller than a value. find\_by\_order
 *  is 0-indexed. Uses GNU policy-based data structures.
 * Time: O(\log N)
 */
#pragma once

#include <ext/pb_ds/assoc_container.hpp> /** keep-include */
#include <ext/pb_ds/tree_policy.hpp> /** keep-include */

template<class T, class Compare = less<T>>
using OrderStatisticTree = __gnu_pbds::tree<T, __gnu_pbds::null_type,
	Compare, __gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update>;

void example() {
	OrderStatisticTree<int> t, t2; t.insert(8);
	auto it = t.insert(10).first;
	assert(it == t.lower_bound(9));
	assert(t.order_of_key(10) == 1);
	assert(t.order_of_key(11) == 2);
	assert(*t.find_by_order(0) == 8);
	t.join(t2); // assuming T < T2 or T > T2, merge t2 into t
}
