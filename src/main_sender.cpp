#include "message_module.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <mqueue.h>
#include <cstring>

void send_message(mqd_t mq, const char* message, unsigned int priority) {
    if (mq_send(mq, message, strlen(message), priority) == -1) {
        perror("Sender: mq_send");
        throw std::runtime_error("Sender: mq_send failed for message: " + std::string(message));
    } else {
        std::cout << "Sent: " << message << " with priority: " << priority << std::endl;
    }
}

void receive_response(mqd_t mq) {
    char buffer[1024];
    unsigned int priority;

    ssize_t bytes_read = mq_receive(mq, buffer, 1024, &priority);
    if (bytes_read >= 0) {
        buffer[bytes_read] = '\0';
        std::cout << "Received response: " << buffer << std::endl;
    } else {
        perror("Sender: mq_receive");
        throw std::runtime_error("Sender: mq_receive failed for response");
    }
}

int main() {
    try {
        mqd_t mq_send = mq_open("/test_queue", O_WRONLY);
        if (mq_send == (mqd_t)-1) {
            perror("Sender: mq_open");
            throw std::runtime_error("Sender: mq_open failed for /test_queue");
        }

        mqd_t mq_receive = mq_open("/response_queue", O_RDONLY);
        if (mq_receive == (mqd_t)-1) {
            perror("Sender: mq_open response_queue");
            throw std::runtime_error("Sender: mq_open failed for /response_queue");
        }

        std::string message;
        unsigned int priority;

        while (true) {
            std::cout << "Enter message (or 'exit' to quit): ";
            std::getline(std::cin, message);
            if (message == "exit") {
                send_message(mq_send, "exit", 0);
                break;
            }
            std::cout << "Enter priority: ";
            std::cin >> priority;
            std::cin.ignore(); // Ignore remaining newline

            send_message(mq_send, message.c_str(), priority);
            receive_response(mq_receive);
        }

        if (mq_close(mq_send) == -1) {
            perror("Sender: mq_close");
            throw std::runtime_error("Sender: mq_close failed for /test_queue");
        }

        if (mq_close(mq_receive) == -1) {
            perror("Sender: mq_close response_queue");
            throw std::runtime_error("Sender: mq_close failed for /response_queue");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
