// 22_sender.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

#define PORT 8080
#define MAX_PACKET_SIZE 1024
#define WINDOW_SIZE 4
#define TIMEOUT 3 // seconds

using namespace std;

string mode;
int base = 0;
int nextSeqNum = 0;
int totalPackets = 10;
bool acked[100] = {false};

void receiveAcks(int sockfd) {
    sockaddr_in recvAddr;
    socklen_t len = sizeof(recvAddr);
    char buffer[1024];

    while (true) {
        int bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&recvAddr, &len);
        if (bytes > 0) {
            int ackNum = atoi(buffer);
            cout << "[Sender] Received ACK for packet " << ackNum << endl;

            if (mode == "GBN") {
                base = ackNum + 1;
            } else if (mode == "SR") {
                acked[ackNum] = true;
                while (acked[base]) base++;
            }

            if (base >= totalPackets) break;
        }
    }
}

int main() {
    cout << "Enter mode (GBN/SR): ";
    cin >> mode;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in senderAddr{}, receiverAddr{};
    senderAddr.sin_family = AF_INET;
    senderAddr.sin_port = htons(PORT + 1); // sender's own port
    inet_pton(AF_INET, "127.0.0.22", &senderAddr.sin_addr);
    bind(sockfd, (sockaddr*)&senderAddr, sizeof(senderAddr));

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.22", &receiverAddr.sin_addr);

    thread ackThread(receiveAcks, sockfd);

    while (base < totalPackets) {
        while (nextSeqNum < base + WINDOW_SIZE && nextSeqNum < totalPackets) {
            string msg = to_string(nextSeqNum);
            sendto(sockfd, msg.c_str(), msg.size(), 0, (sockaddr*)&receiverAddr, sizeof(receiverAddr));
            cout << "[Sender] Sent packet " << nextSeqNum << endl;
            nextSeqNum++;
        }

        if (mode == "GBN") {
            sleep(TIMEOUT);
            if (base < nextSeqNum) {
                cout << "[Sender] Timeout! Resending from packet " << base << endl;
                for (int i = base; i < nextSeqNum; ++i) {
                    string msg = to_string(i);
                    sendto(sockfd, msg.c_str(), msg.size(), 0, (sockaddr*)&receiverAddr, sizeof(receiverAddr));
                    cout << "[Sender] Resent packet " << i << endl;
                }
            }
        } else {
            sleep(1); // SR has individual timeouts (simplified here)
            for (int i = base; i < nextSeqNum; ++i) {
                if (!acked[i]) {
                    string msg = to_string(i);
                    sendto(sockfd, msg.c_str(), msg.size(), 0, (sockaddr*)&receiverAddr, sizeof(receiverAddr));
                    cout << "[Sender] SR Resent packet " << i << endl;
                }
            }
        }
    }

    ackThread.join();
    close(sockfd);
    return 0;
}
