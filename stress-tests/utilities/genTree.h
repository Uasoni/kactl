#pragma once

/*
  prufer code to tree
  complexity: O(vlog_v)
 */

vector<pair<int,int>> prufer_code_to_tree(vector<int> &prufer_code) {
	// stores number count of nodes in the prufer code
	unordered_map<int,int> node_count;

	// set of integers absent in prufer code. they are the leaves
	set<int> leaves;

	int len = (int) prufer_code.size();
	int node = len + 2;

	// count frequency of nodes
	for ( int i = 0; i < len; i++ ) {
		int t = prufer_code[i];
		node_count[t]++;
	}

	// find the absent nodes
	for ( int i = 1; i <= node; i++ ) {
		if ( node_count.find ( i ) == node_count.end() ) leaves.insert ( i );
	}

	vector<pair<int,int>> edges;
	/*connect edges*/
	for ( int i = 0; i < len; i++ ){
		int a = prufer_code[i]; // first node

		//find the smallest number which is not present in prufer code now
		int b = *leaves.begin(); // the leaf

		edges.push_back({a,b}); // edge of the tree

		leaves.erase ( b ); // remove from absent list
		node_count[a]--; // remove from prufer code
		if ( node_count[a] == 0 ) leaves.insert ( a ); // if a becomes absent
	}

	// the final edge
	edges.push_back({*leaves.begin(), *leaves.rbegin()});
	return edges;
}

vector<pair<int, int>> gen_random_tree(int n) {
	vector<int> prufer_code;
	for (int i=0; i<n-2; i++) {
		prufer_code.push_back(rand()%(n-1) + 1);
	}
	auto edges = prufer_code_to_tree(prufer_code);
	for (auto &p: edges)
		p.first--, p.second--;
	return edges;
}
