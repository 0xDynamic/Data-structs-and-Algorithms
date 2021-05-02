template<typename T>
class SegmentTree {
private:
    uint32_t n; vector<T> BST;

    T Fn(T a, T b) {                                    	// Fn() = min(), max(), etc
        return a + b;
    }

    vector<int> resolve(uint32_t i, uint32_t j) {       // Returns a set of inner
        vector<int> segments;                           			// nodes that span the
																						// range [i, j].
        i += n, j += n;

        while (i <= j) {
            if (i == j) {                               		// Base case #1.
                segments.push_back(i);
                break;
            }

            else if (abs(i - j) == 1) {                 // Base case #2.
                segments.push_back(i);
                segments.push_back(j);
                break;
            }

            else {
                if ( i & 1) segments.push_back(i);      // LSB(i) = 1 -> Left turn.
                if (~j & 1) segments.push_back(j);      // LSB(j) = 0 -> Right turn.
            }

            i = (i / 2) + ( i & 1);                     			// Integrated subtree hops.
            j = (j / 2) - (~j & 1);
        }

        return segments;
    }

public:
    SegmentTree(const vector<T> &V) {
        n = 1;
        while (n < V.size()) n <<= 1;                   // Next power of 2.

        BST.assign(2 * n, 0);
        copy(V.begin(), V.end(), BST.begin() + n);      // Leaf nodes.

        for (int i = n - 1; i >= 1; i -= 1)
            BST[i] = Fn(BST[2 * i], BST[2 * i + 1]);    // Inner nodes.
    }

    T get_range(int i, int j) {
        vector<int> segments = resolve(i, j);           // i and j are 0-indexed.

        T query; bool unset = true;

        for (int seg: segments)
            if (unset == true)
                query = BST[seg], unset = false;

            else
                query = Fn(query, BST[seg]);            // Fn() describes the segment
																				// signature, returning range
        return query;                                   			// -min, -max, -sum, etc.
    }
};