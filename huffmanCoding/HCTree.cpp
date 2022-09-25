#include "HCTree.hpp"
#include <stack>
using namespace std;

    void HCTree::delete_node(HCNode* node) {
        // traverse through nodes
        if (node != nullptr) {
            if (node->c0 != nullptr) {
                delete_node(node->c0);
            }
            if (node->c1 != nullptr) {
                delete_node(node->c1);
            }
            delete node;
        }
    }

    HCTree::~HCTree() {
        // begin at root
        if (root != nullptr) {
            delete_node(root);
        }
    }

    void HCTree::build(const vector<int>& freqs) {
        // start with forest of single-node tree (symbol w/ frequencies)
        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> forest;
        // int count = 0;

        // make a leaf node for each byte and add to priority queue
        for (unsigned long int i = 0; i < freqs.size(); i++) {
            if (freqs[i] > 0) {
                HCNode *leaf = new HCNode(freqs[i], i);
                // add new leaf node to priority queue
                leaves[i] = leaf;
                // add leaf node containing byte i
                forest.push(leaves[i]);
            }

            // // increment count if frequency at index is 0
            // if (freqs[i] == 0) {
            //     count++;
            // }
        }

        // while there is still more than 1 tree in the forest
        while (forest.size() > 1) {
            // empty parent node
            HCNode *parent = new HCNode(0, 0);
            // sum of least frequent nodes
            int sum = -1;

            // 2 least frequent nodes
            parent->c0 = forest.top();
            forest.pop();

            parent->c1 = forest.top();
            forest.pop();

            // parent value is sum of children's values
            sum = parent->c0->count + parent->c1->count;
            parent->count = sum;

            // set parent pointers of leaves
            parent->c0->p = parent;
            parent->c1->p = parent;

            // insert new tree(node(s)) into priority queue
            forest.push(parent);
        }

        // set root if not reading from an empty file
        // if (count != 256) {
            root = forest.top();
            forest.pop();
        // }

    }

    void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
        // access node at symbol index
        HCNode *sym = leaves[symbol];

        // stack to store path fro mleaf to root
        stack<int> stack;

        // iterate up tree from leaf node
        while (/*sym != nullptr*/sym->p != nullptr) {
            if (sym->p == nullptr) {
                break;
            }
            if (sym->p->c0 == sym) {
                stack.push(0);
            }
            else if (sym->p->c1 == sym) {
                stack.push(1);
            }
            sym = sym->p;
        }

        // output stack to file
        while (!stack.empty()) {
            out.write_bit(stack.top());
            // cout << stack.top() << endl;
            stack.pop();
        }
    }

    unsigned char HCTree::decode(FancyInputStream & in) const {
        // start at root node
        HCNode *curr = root;
        // single bit
        int b = 0;

        // iterate root to find leaf node
        while (curr->c0 != nullptr || curr->c1 != nullptr) {
            b = in.read_bit();
            // if current bit is '0'
            if (b == 0) {
                // iterate down '0' edge
                curr = curr->c0;
            }
            // if current bit is '1'
            if (b == 1) {
                // iterate down '1' edge
                curr = curr->c1;
            }
        }
        // return leaf node
        return curr->symbol;
    }
