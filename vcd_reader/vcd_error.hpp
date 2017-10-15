#pragma once
#include <stdexcept>
#include <string>

#include "vcd_parser/location.hh"

namespace vcd {

class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& msg, location new_loc)
            : std::runtime_error(msg)
            , loc_(new_loc)
            {}

        location loc() { return loc_; }

    private:
        location loc_;
};

} //namespace
