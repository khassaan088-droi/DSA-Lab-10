#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 1. MICROSERVICE NODE CLASS (BST)
class Node {
public:
    string serviceID;   // Service ID like "S1", "S2"
    int monthlyCost;    // Cost in dollars/thousands
    Node* left;         // Left child (smaller costs)
    Node* right;        // Right child (larger costs)

    // Constructor to initialize a node
    Node(string id, int cost) {
        serviceID = id;
        monthlyCost = cost;
        left = nullptr;
        right = nullptr;
    }
};
// PART A: Hierarchical Path Backtracking

// ==========================================
void downwardPathHelper(Node* node, int target_budget, vector<string>& current_path, int current_sum) {
    // Base Case: if node is null, stop
    if (node == nullptr) {
        return;
    }

    // 1. PUSH the current node onto our path vector
    current_path.push_back(node->serviceID);
    current_sum += node->monthlyCost;

    // Check if our current downward path sums to the target budget
    if (current_sum == target_budget) {
        cout << "Downward Path Found: ";
        for (const string& id : current_path) {
            cout << id << " ";
        }
        cout << endl;
    }

    // Recurse down to left and right descendants
    downwardPathHelper(node->left, target_budget, current_path, current_sum);
    downwardPathHelper(node->right, target_budget, current_path, current_sum);

    // 3. POP the node off the vector when backtracking ("undoing" the choice)
    current_path.pop_back();
}

// Function to start downward path search from EVERY node in the tree
void findAllDownwardPaths(Node* root, int target_budget) {
    if (root == nullptr) return;

    // Check paths starting from the current root
    vector<string> path;
    downwardPathHelper(root, target_budget, path, 0);

    // Recursively check left and right subtrees as new starting points
    findAllDownwardPaths(root->left, target_budget);
    findAllDownwardPaths(root->right, target_budget);
}


// PART B: BST Bounded Subset Pruning

void findBSTSubsets(Node* node, int target_budget, int current_sum, vector<string>& current_subset) {
    // Base Case
    if (node == nullptr) {
        return;
    }

    // Try including the current node in our subset
    int new_sum = current_sum + node->monthlyCost;

    if (new_sum == target_budget) {
        current_subset.push_back(node->serviceID);
        cout << "BST Subset Found: ";
        for (const string& id : current_subset) {
            cout << id << " ";
        }
        cout << endl;
        current_subset.pop_back(); // Backtrack
    }
    if (new_sum <= target_budget) {
        // Option 1: Include node and explore left subtree
        current_subset.push_back(node->serviceID);
        findBSTSubsets(node->left, target_budget, new_sum, current_subset);
        
        // Explore right subtree only if it doesn't violate pruning rule
        findBSTSubsets(node->right, target_budget, new_sum, current_subset);
        
        // Backtrack
        current_subset.pop_back();
    } else {
        // If adding this node already exceeds budget, right subtree is guaranteed to exceed too.
        // We can prune the right subtree and only check the left subtree (smaller values).
        findBSTSubsets(node->left, target_budget, current_sum, current_subset);
        return; // Prune right subtree immediately!
    }

    // Option 2: Exclude current node and explore subtrees normally
    findBSTSubsets(node->left, target_budget, current_sum, current_subset);
    findBSTSubsets(node->right, target_budget, current_sum, current_subset);
}


int main() {
    Node* root = new Node("S2", 10);
    root->left = new Node("S1", 5);
    root->right = new Node("S3", 15);

    int target_budget = 15;

    cout << "=== PART A: Downward Paths Summing to " << target_budget << " ===\n";
    findAllDownwardPaths(root, target_budget);

    cout << "\n=== PART B: BST Bounded Subsets Summing to " << target_budget << " ===\n";
    vector<string> subset;
    findBSTSubsets(root, target_budget, 0, subset);

    // Clean up dynamic memory
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}