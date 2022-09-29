//
// Created by Mohamed amine Bounya on 9/28/22.
//

#include "address_port.hpp"

address_port::address_port(const std::string &ip, in_port_t port) : ip(ip), port(port) {
}

std::string address_port::get_full_address() const {
    std::stringstream host_port;
    host_port << this->ip << ":" << this->port;
    return host_port.str();
}

const std::string &address_port::get_ip() const {
    return this->ip;
}

in_port_t address_port::get_port() const {
    return this->port;
}

bool address_port::operator==(const address_port &address) const {
    return (address.ip == this->ip && address.port == this->port);
}

bool address_port::operator<(const address_port &address) const {
    return (this->get_full_address() < address.get_full_address());
}
