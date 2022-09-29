//
// Created by Mohamed amine Bounya on 9/28/22.
//

#ifndef WEBSERV_ADDRESS_PORT_HPP
#define WEBSERV_ADDRESS_PORT_HPP

#include <sys/types.h>

#include <iostream>
#include <sstream>
#include <string>

class address_port {
public:
    address_port(const std::string &ip, in_port_t port);

    const std::string &get_ip() const;
    in_port_t get_port() const;
    std::string get_full_address() const;

    bool operator==(const address_port &address) const;
    bool operator<(const address_port &address) const;

private:
    std::string ip;
    in_port_t port;
};


#endif //WEBSERV_ADDRESS_PORT_HPP
