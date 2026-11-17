#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
public:
    string container_id;
    int weight;
    Node* next;
    Node(string cid, int w) {
        container_id = cid;
        weight = w;
        next = nullptr;
    }
};
bool solveLoading(Node* container_node, 
    const vector<int>& truck_capacities, 
    int truck_index, 
    int current_truck_load, 
    vector<vector<string>>& assignments) {

    if (container_node == nullptr) {
        return true;
    }

   //Rule 2
    int remaining_weight = 0;
    Node* temp = container_node;
    while (temp != nullptr) {
        remaining_weight += temp->weight;
        temp = temp->next;
    }

    // Calculate total capacity left 
    int total_remaining_capacity = 0;
    for (size_t i = truck_index; i < truck_capacities.size(); i++) {
        total_remaining_capacity += truck_capacities[i];
    }

    // If remaining weight is greater
    if (remaining_weight > total_remaining_capacity) {
        return false; 
    }

    int max_cap = truck_capacities[truck_index];

    //RUle 1
    if (current_truck_load + container_node->weight <= max_cap) {
        assignments[truck_index].push_back(container_node->container_id);

        // Recurse for the NEXT container
        if (solveLoading(container_node->next, truck_capacities, truck_index, 
                         current_truck_load + container_node->weight, assignments)) {
            return true;
        }

        // Backtrack (undo if it didn't work)
        assignments[truck_index].pop_back();
    }
    // Option B
    if (truck_index + 1 < truck_capacities.size()) {
        if (container_node->weight <= truck_capacities[truck_index + 1]) {
            assignments[truck_index + 1].push_back(container_node->container_id);

            if (solveLoading(container_node->next, truck_capacities, truck_index + 1, 
                             container_node->weight, assignments)) {
                return true;
            }

            // Backtrack
            assignments[truck_index + 1].pop_back();
        }
    }
    return false;
}

//Main
int main() {
    // Create Linked List of Containers: C1(2 tons) -> C2(3 tons) -> C3(4 tons) -> C4(1 ton)
    Node* head = new Node("C1", 2);
    head->next = new Node("C2", 3);
    head->next->next = new Node("C3", 4);
    head->next->next->next = new Node("C4", 1);

    // Queue of trucks: Truck 1 capacity = 5, Truck 2 capacity = 5
    vector<int> truck_capacities = {5, 5};

    // Store which containers go into which truck
    vector<vector<string>> assignments(truck_capacities.size());

    // Run the solver
    bool success = solveLoading(head, truck_capacities, 0, 0, assignments);

    // Results
    if (success) {
        cout << "Loading Successful!\n";
        for (size_t i = 0; i < truck_capacities.size(); i++) {
            cout << "Truck " << (i + 1) << " (Capacity " << truck_capacities[i] << "): ";
            for (const string& cid : assignments[i]) {
                cout << cid << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Insufficient truck capacity.\n";
    }

    // Clean Up
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    return 0;
}