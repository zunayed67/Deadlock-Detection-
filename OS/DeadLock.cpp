#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Display matrices
void displayState(const vector<vector<int>>& allocation,
                  const vector<vector<int>>& request,
                  const vector<int>& available,
                  int n, int m) {

    cout << "\n--- System State ---\n";

    cout << "\nAllocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << " : ";
        for (int j = 0; j < m; j++)
            cout << setw(3) << allocation[i][j];
        cout << endl;
    }

    cout << "\nRequest Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << " : ";
        for (int j = 0; j < m; j++)
            cout << setw(3) << request[i][j];
        cout << endl;
    }

    cout << "\nAvailable:\n";
    for (int j = 0; j < m; j++)
        cout << setw(3) << available[j];
    cout << endl;
}

// Deadlock Detection with tracing
bool detectDeadlock(const vector<vector<int>>& allocation,
                    const vector<vector<int>>& request,
                    const vector<int>& available,
                    int n, int m,
                    vector<int>& safeSeq) {

    vector<bool> finish(n, false);
    vector<int> work = available;
    safeSeq.clear();

    cout << "\n--- Detection Steps ---\n";

    bool progress = true;
    while (progress) {
        progress = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canRun = true;

                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }

                if (canRun) {
                    cout << "Process P" << i << " can finish. Releasing resources.\n";

                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    safeSeq.push_back(i);
                    progress = true;
                }
            }
        }
    }

    cout << "\nFinal Work: ";
    for (int j = 0; j < m; j++)
        cout << work[j] << " ";
    cout << endl;

    bool deadlock = false;
    cout << "\nDeadlocked Processes: ";
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            cout << "P" << i << " ";
            deadlock = true;
        }
    }

    if (!deadlock) cout << "None";

    cout << endl;
    return deadlock;
}

// Suggest recovery
void suggestRecovery(const vector<vector<int>>& allocation,
                     const vector<bool>& finish) {

    cout << "\n--- Recovery Suggestion ---\n";
    cout << "Terminate one of these processes to break deadlock:\n";

    for (int i = 0; i < finish.size(); i++) {
        if (!finish[i]) {
            cout << "P" << i << endl;
        }
    }
}

int main() {
    int n, m;

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> request(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> allocation[i][j];

    cout << "\nEnter Request Matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> request[i][j];

    cout << "\nEnter Available Resources:\n";
    for (int j = 0; j < m; j++)
        cin >> available[j];

    int choice;
    vector<int> safeSeq;

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Display State\n";
        cout << "2. Detect Deadlock\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayState(allocation, request, available, n, m);
                break;

            case 2: {
                bool deadlock = detectDeadlock(allocation, request, available, n, m, safeSeq);

                if (!deadlock) {
                    cout << "\nSystem is SAFE.\nSafe sequence: ";
                    for (int p : safeSeq)
                        cout << "P" << p << " ";
                    cout << endl;
                } else {
                    cout << "\nSystem is in DEADLOCK.\n";
                }
                break;
            }

            case 3:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 3);

    return 0;
}