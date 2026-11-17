#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;
class Operation {
public:
    string name; 
    char type;   // Operator type: '+', '-', '*', or '/'
    int operand; // The number value used in the operation

    // Constructor 
    Operation(string n, char t, int o) {
        name = n;
        type = t;
        operand = o;
    }
};

// ==========================================
// 2. BACKTRACKING SOLVER FUNCTION WITH EXPLICIT STACK
// ==========================================
void findMacros(int current_val, int target_val, int ops_left, 
                const vector<Operation>& allowed_ops, 
                stack<string>& opStack, 
                int overflow_bound) {
    
    // Base Case 1
    if (current_val == target_val) {
        cout << "Valid Macro Sequence Found: ";
        
        // Print the contents of the stack from bottom to top
        vector<string> temp_print;
        stack<string> temp_stack = opStack;
        while (!temp_stack.empty()) {
            temp_print.insert(temp_print.begin(), temp_stack.top());
            temp_stack.pop();
        }
        for (const string& op_str : temp_print) {
            cout << op_str << " ";
        }
        cout << endl;
        return;
    }

    // Base Case 2: If we ran out of operation budget, stop exploring this path
    if (ops_left == 0) {
        return;
    }

    // Try each allowed operation from our vector
    for (const Operation& op : allowed_ops) {
        int next_val = current_val;

        // Perform the math operation
        if (op.type == '+') {
            next_val += op.operand;
        } else if (op.type == '-') {
            next_val -= op.operand;
        } else if (op.type == '*') {
            next_val *= op.operand;
        } else if (op.type == '/') {
            // Avoid division by zero and check if it divides cleanly
            if (op.operand == 0 || next_val % op.operand != 0) {
                continue; 
            }
            next_val /= op.operand;
        }

        // Constraint Check: Value must not become negative and must not exceed overflow bound
        if (next_val >= 0 && next_val <= overflow_bound) {
            
            // 1. PUSH the operation onto the explicit stack before recursing
            opStack.push(op.name);

            // 2. RECURSE: Try the next operation with 1 less operation budget
            findMacros(next_val, target_val, ops_left - 1, allowed_ops, opStack, overflow_bound);

            // 3. POP the operation off the stack when backtracking ("undoing" the choice)
            opStack.pop();
        }
    }
}
//Main Function
int main() {
    int initial_state = 2;
    int target_state = 12;
    int max_operations = 4;     // Maximum operation budget (K)
    int overflow_bound = 100;   // System overflow bound

    // Define allowed atomic operations using our new Operation class
    vector<Operation> allowed_ops = {
        Operation("+3", '+', 3),
        Operation("*2", '*', 2),
        Operation("-1", '-', 1)
    };

    // Explicit Operation Stack to manage state mutations and rollbacks
    stack<string> opStack;

    cout << "Synthesizing macros to transform " << initial_state << " into " << target_state << "...\n\n";
    
    // Run the recursive backtracking function
    findMacros(initial_state, target_state, max_operations, allowed_ops, opStack, overflow_bound);

    return 0;
}