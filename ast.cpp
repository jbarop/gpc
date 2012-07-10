#include <cstdlib>
#include <limits> 
#include "ast.hpp"

using namespace gpc;

static double max_value = 9999999;
static double min_value = -9999999;

node::node() {
}

node::~node() {
}

number_node::number_node(int value)
    : m_value(value) {}

double number_node::eval() {
    if (m_value < min_value) {
        throw "Number to small.";
    } else if (m_value > max_value) {
        throw "Number to big.";
    }

    return m_value;
}

unary_minus_node::unary_minus_node(node* child)
    : m_child(child) {}

unary_minus_node::~unary_minus_node() {
    delete m_child;
}

double unary_minus_node::eval() {
    return m_child->eval() * -1;
}

op_node_base::op_node_base(node* left, node* right)
    : m_left(left), m_right(right) {}

op_node_base::~op_node_base() {
    delete m_right;
    delete m_left;
}

add_op_node::add_op_node(node* left, node* right)
    : op_node_base(left, right) {}

double add_op_node::eval() {
    double left = m_left->eval();
    double right = m_right->eval();

    if ((max_value - right) < left) {
        throw "Overflow while adding";
    } else if ((min_value - right) > left) {
        throw "Underflow while adding";
    }

    return left + right;
}

sub_op_node::sub_op_node(node* left, node* right)
    : op_node_base(left, right) {}

double sub_op_node::eval() {
    double left = m_left->eval();
    double right = m_right->eval();

    if ((max_value + right) < left) {
        throw "Overflow while subtracting";
    } else if ((min_value + right) > left) {
        throw "Underflow while subtracting";
    }

    return left - right;
}

mul_op_node::mul_op_node(node* left, node* right)
    : op_node_base(left, right) {}

double mul_op_node::eval() {
    double left = m_left->eval();
    double right = m_right->eval();
    double left_abs = (left < 0) ? -1.0 * left : left;
    double right_abs = (right < 0) ? -1.0 * right : right;

    if ((max_value / right_abs) < left_abs) {
        throw "Overflow while multiplying";
    } else if ((min_value / right_abs) > left_abs) {
        throw "Underflow while multiplying";
    }

    return left * right;
}

div_op_node::div_op_node(node* left, node* right)
    : op_node_base(left, right) {}

double div_op_node::eval() {
    double right = m_right->eval();
    double left = m_left->eval();
    double right_abs = (right < 0) ? -1.0 * right : right;

    if (right_abs <= std::numeric_limits<double>::epsilon()) {
        throw "Can not divide by zero";
    }

    return left / right;
}
