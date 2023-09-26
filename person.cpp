/**
 *
 */

#include "person.hpp"

gtestbox::person::Person::Person() :
    name("unknown"),
    age(0) {}

void gtestbox::person::Person::sayHello(std::string msg) {
    std::cout << this->name << ", age " << this->age << ", says '" << msg << "'" << std::endl;
}
