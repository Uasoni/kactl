#pragma once

// taken from https://stackoverflow.com/a/40873657/2014771
template <class F> struct YCombinator {
	F f; // the lambda will be stored here

	// a forwarding operator():
	template <class... argument_types>
	decltype(auto) operator()(argument_types&&... arguments) const {
		// we pass ourselves to f, then the arguments.
		return f(ref(*this), forward<argument_types>(arguments)...);
	}
};

// helper function that deduces the type of the lambda:
template <class F> YCombinator<std::decay_t<F>> make_y_combinator(F &&f) { return {std::forward<F>(f)}; }
