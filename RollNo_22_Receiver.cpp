// 22_receiver.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_set>

#define PORT 8080
#define MAX_PACKET_SIZE 1024
#define WINDOW_SIZE 4

using namespace std;

string mode;
int expectedSeqNum = 0;
bool received[100] = {false};

int main() {
    cout << "Enter mode (GBN/SR): ";
    cin >> mode;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in receiverAddr{}, senderAddr{};
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.22", &receiverAddr.sin_addr);
    bind(sockfd, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

    socklen_t len = sizeof(senderAddr);
    char buffer[1024];

    while (true) {
        int bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&senderAddr, &len);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            int seqNum = atoi(buffer);
            cout << "[Receiver] Received packet " << seqNum << endl;

            if (mode == "GBN") {
                if (seqNum == expectedSeqNum) {
                    cout << "[Receiver] Accepted packet " << seqNum << endl;
                    sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&senderAddr, len);
                    expectedSeqNum++;
                } else {
                    cout << "[Receiver] Discarded packet " << seqNum << ", expected " << expectedSeqNum << endl;
                    string ack = to_string(expectedSeqNum - 1);
                    sendto(sockfd, ack.c_str(), ack.size(), 0, (sockaddr*)&senderAddr, len);
                }
            } else if (mode == "SR") {
                if (!received[seqNum]) {
                    received[seqNum] = true;
                    cout << "[Receiver] Buffered packet " << seqNum << endl;
                    string ack = to_string(seqNum);
                    sendto(sockfd, ack.c_str(), ack.size(), 0, (sockaddr*)&senderAddr, len);
                }
            }
        }

        if (expectedSeqNum >= 10) break;
    }

    close(sockfd);
    return 0;
}
