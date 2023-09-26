/**
 * 
 */

#pragma once

#include <string>
#include <iostream>

namespace gtestbox {
    namespace person {
        class Person {
        private:
        protected:
        public:
            std::string name;
            int age;
            Person();
            void sayHello(std::string msg);
        };
    }
}
