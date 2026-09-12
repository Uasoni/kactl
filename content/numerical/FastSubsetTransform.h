/**
 * Author: lucian bicsi
 * Date: 2015-06-25
 * License: GNU free documentation license 1.2
 * Source: csacademy
 * Description: transform to a basis with fast convolutions of the form
 * $\displaystyle c[z] = \sum\nolimits_{z = x \oplus y} a[x] \cdot b[y]$,
 * where $\oplus$ is one of AND, OR, XOR. the size of $a$ must be a power of two.
 * Default is AND. Select exactly one butterfly below; for XOR also enable
 * the final inverse division. conv requires equal, nonempty power-of-two
 * lengths; pad both with zeros to cover all masks (no doubling needed).
 * Indices are bitmasks starting at 0. Intermediates must fit in int; widen
 * vector elements, references and pairs together if needed.
 * Modulo an odd prime, reduce every operation, normalize subtractions, and
 * for XOR replace division by N with multiplication by its modular inverse.
 * Usage: auto c = conv(a, b); // c[z] = sum of a[x]*b[y] with (x AND y) == z
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

void fast_subset_transform(vector<int>& a, bool inv) {
	for (int n = (int)(a).size(), step = 1; step < n; step *= 2) {
		for (int i = 0; i < n; i += 2 * step) for (int j = i; j < (i+step); ++j) {
			int &u = a[j], &v = a[j + step]; tie(u, v) =
				inv ? pii(v - u, u) : pii(v, u + v); // AND
				// inv ? pii(v, u - v) : pii(u + v, u); // OR /// include-line
				// pii(u + v, u - v);                   // XOR /// include-line
		}
	}
	// if (inv) for (int& x : a) x /= (int)(a).size(); // XOR only /// include-line
}
vector<int> conv(vector<int> a, vector<int> b) {
	fast_subset_transform(a, 0); fast_subset_transform(b, 0);
	for (int i = 0; i < ((int)(a).size()); ++i) a[i] *= b[i];
	fast_subset_transform(a, 1); return a;
}
