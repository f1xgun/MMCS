#include "accountrecord.h"

AccountRecord::AccountRecord() {}

AccountRecord::AccountRecord(std::string site_name, std::string login, std::string password)
    : site_name(site_name), login(login), password(password) {}

std::string AccountRecord::get_site_name() {
    return this->site_name;
}

std::string AccountRecord::get_login() {
    return this->login;
}

std::string AccountRecord::get_password() {
    return this->password;
}
