#include <iostream>

// Picture elements
#define EMPTY "   "
#define HORIZONTAL " │ "
#define VERTICAL " ─┤ "
#define LEFT_CORNER " ┌ "
#define RIGHT_CORNER " ┐ "

// Print picture row
void print_row(std::string lc, std::string h, std::string v, std::string rc) {
    std::cout << lc << h << h << h << rc << std::endl;
    std::cout << v << EMPTY << EMPTY << EMPTY << v << std::endl;
}

int main() {
    // Print picture
    print_row(LEFT_CORNER, HORIZONTAL, VERTICAL, RIGHT_CORNER);
    print_row(VERTICAL, EMPTY, EMPTY, VERTICAL);
    print_row(VERTICAL, EMPTY, EMPTY, VERTICAL);
    print_row(LEFT_CORNER, HORIZONTAL, HORIZONTAL, RIGHT_CORNER);
    std::cout << EMPTY << EMPTY << EMPTY << EMPTY << EMPTY << std::endl;
    print_row(VERTICAL, EMPTY, EMPTY, VERTICAL);
    print_row(LEFT_CORNER, HORIZONTAL, HORIZONTAL, RIGHT_CORNER);

    return 0;
}