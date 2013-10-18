#pragma once

class BaseGrid
{
public:
    BaseGrid(const BaseGrid* parent = 0);
    ~BaseGrid();

protected:
    BaseGrid* _parent;

private:

};
