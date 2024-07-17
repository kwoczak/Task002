#include "message_module.h"
#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <stdexcept>
#include <chrono>
#include <thread>

#define QUEUE_NAME "/task_queue"
#define RESPONSE_QUEUE_NAME "/response_queue"
#define MAX_SIZE 1024
#define MSG_STOP "exit"

// Funkcja inicjująca odbiornik
void init_receiver() {
    mqd_t mq;
    mqd_t response_mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    unsigned int priority;

    // Atrybuty kolejki wiadomości
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Tworzenie kolejki wiadomości
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Receiver: mq_open");
        throw std::runtime_error("Receiver: mq_open failed for " + std::string(QUEUE_NAME));
    }

    // Tworzenie kolejki odpowiedzi
    response_mq = mq_open(RESPONSE_QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (response_mq == (mqd_t)-1) {
        perror("Receiver: mq_open response_queue");
        throw std::runtime_error("Receiver: mq_open failed for " + std::string(RESPONSE_QUEUE_NAME));
    }

    std::cout << "Receiver started and waiting for messages." << std::endl;

    // Odbieranie wiadomości
    while (true) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received: " << buffer << " with priority: " << priority << std::endl;

            if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
                break;
            }

            // Wysyłanie odpowiedzi
            std::string response = "Length of received message: " + std::to_string(strlen(buffer));
            if (mq_send(response_mq, response.c_str(), response.size(), 0) == -1) {
                perror("Receiver: mq_send response");
                throw std::runtime_error("Receiver: mq_send failed for response");
            }
        } else {
            perror("Receiver: mq_receive");
            throw std::runtime_error("Receiver: mq_receive failed for message");
        }
    }

    // Zamknięcie kolejek wiadomości
    if (mq_close(mq) == -1) {
        perror("Receiver: mq_close");
        throw std::runtime_error("Receiver: mq_close failed for " + std::string(QUEUE_NAME));
    }

    if (mq_close(response_mq) == -1) {
        perror("Receiver: mq_close response_queue");
        throw std::runtime_error("Receiver: mq_close failed for " + std::string(RESPONSE_QUEUE_NAME));
    }

    // Usunięcie kolejek wiadomości
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("Receiver: mq_unlink");
        throw std::runtime_error("Receiver: mq_unlink failed for " + std::string(QUEUE_NAME));
    }

    if (mq_unlink(RESPONSE_QUEUE_NAME) == -1) {
        perror("Receiver: mq_unlink response_queue");
        throw std::runtime_error("Receiver: mq_unlink failed for " + std::string(RESPONSE_QUEUE_NAME));
    }
}
