#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

queue<string> pipeBuffer;
queue<string> messageQueue;
string sharedMemory;

stack<string> debugLog;

void pipeSend(string msg) {
    pipeBuffer.push(msg);
    debugLog.push("PIPE SEND: " + msg);
}

void pipeReceive() {
    if (pipeBuffer.empty()) {
        debugLog.push("PIPE DEADLOCK: Read on empty pipe");
        cout << "[PIPE] Deadlock detected\n";
        return;
    }
    cout << "[PIPE] Received: " << pipeBuffer.front() << endl;
    pipeBuffer.pop();
}

void mqSend(string msg) {
    messageQueue.push(msg);
    debugLog.push("MQ SEND: " + msg);
}

void mqReceive() {
    if (messageQueue.empty()) {
        debugLog.push("MQ DEADLOCK: Receive on empty queue");
        cout << "[MQ] Deadlock detected\n";
        return;
    }
    cout << "[MQ] Received: " << messageQueue.front() << endl;
    messageQueue.pop();
}

void shmWrite(string msg) {
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
}

void showStatus() {
    cout << "\n--- IPC STATUS ---\n";
    cout << "Pipe buffer size: " << pipeBuffer.size() << endl;
    cout << "Message queue size: " << messageQueue.size() << endl;
    cout << "Shared memory: " 
         << (sharedMemory.empty() ? "EMPTY" : sharedMemory) << endl;
}

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
        cout << "\n1.Pipe Send\n2.Pipe Receive\n";
        cout << "3.MQ Send\n4.MQ Receive\n";
        cout << "5.SHM Write\n6.SHM Read\n";
        cout << "7.Show Status\n8.Show Debug Log\n9.Exit\n";
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
                return 0;
            default:
                cout << "Invalid choice\n";
        }
    }
}
