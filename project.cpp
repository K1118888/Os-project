#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

// IPC Buffers
queue<string> pipeBuffer;
queue<string> messageQueue;
string sharedMemory;

// Debug Log
stack<string> debugLog;

// ---------------- PIPE ----------------

void pipeSend(const string& msg) {
    pipeBuffer.push(msg);
    debugLog.push("PIPE SEND: " + msg);
}

void pipeReceive() {
    if (pipeBuffer.empty()) {
        debugLog.push("PIPE DEADLOCK: Read on empty pipe");
        cout << "[PIPE] Deadlock detected\n";
        return;
    }

    string msg = pipeBuffer.front();
    pipeBuffer.pop();
    debugLog.push("PIPE RECEIVE: " + msg);
    cout << "[PIPE] Received: " << msg << endl;
}

// ---------------- MESSAGE QUEUE ---------------- 

void mqSend(const string& msg) {
    messageQueue.push(msg);
    debugLog.push("MQ SEND: " + msg);
}

void mqReceive() {
    if (messageQueue.empty()) {
        debugLog.push("MQ DEADLOCK: Receive on empty queue");
        cout << "[MQ] Deadlock detected\n";
        return;
    }

    string msg = messageQueue.front();
    messageQueue.pop();
    debugLog.push("MQ RECEIVE: " + msg);
    cout << "[MQ] Received: " << msg << endl;
}

// ---------------- SHARED MEMORY ---------------- 

void shmWrite(const string& msg) {
    sharedMemory = msg;
    debugLog.push("SHM WRITE: " + msg);
}

void shmRead() {
    if (sharedMemory.empty()) {
        debugLog.push("SHM DEADLOCK: Read before write");
        cout << "[SHM] Deadlock detected\n";
        return;
    }

    cout << "[SHM] Read: " << sharedMemory << endl;
    debugLog.push("SHM READ: " + sharedMemory);
    sharedMemory.clear();   
}


// --------------------- SHOW STATUS --------------------
void showStatus() {
    cout << "\n--- IPC STATUS ---\n";
    cout << "Pipe buffer size: " << pipeBuffer.size() << endl;
    cout << "Message queue size: " << messageQueue.size() << endl;
    cout << "Shared memory: "
         << (sharedMemory.empty() ? "EMPTY" : sharedMemory) << endl;
}

// --------------------- SHOW DEBUG --------------------
void showDebug() {
    if (debugLog.empty()) {
        cout << "No debug history\n";
        return;
    }

    cout << "\n--- DEBUG LOG ---\n";
    stack<string> temp = debugLog;
    while (!temp.empty()) {
        cout << temp.top() << endl;
        temp.pop();
    }
}



int main() {
    int choice;
    string msg;

    while (true) {
        cout << "\n1. Pipe Send\n2.Pipe Receive\n";
        cout << "3. MQ Send\n4.MQ Receive\n";
        cout << "5. SHM Write\n6.SHM Read\n";
        cout << "7. Show Status\n8.Show Debug Log\n9.Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter message: ";
                getline(cin, msg);
                pipeSend(msg);
                break;

            case 2:
                pipeReceive();
                break;

            case 3:
                cout << "Enter message: ";
                getline(cin, msg);
                mqSend(msg);
                break;

            case 4:
                mqReceive();
                break;

            case 5:
                cout << "Enter data: ";
                getline(cin, msg);
                shmWrite(msg);
                break;

            case 6:
                shmRead();
                break;

            case 7:
                showStatus();
                break;

            case 8:
                showDebug();
                break;

            case 9:
                cout << "Exiting...\n";
                return 0;

            default:
                cout << "Invalid choice\n";
        }
    }
}
