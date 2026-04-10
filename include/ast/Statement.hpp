#include <string>
#include <vector>
#include <memory>
#pragma once

class Statement {
public:
    virtual ~Statement () = default;

    virtual void execute() = 0;
};