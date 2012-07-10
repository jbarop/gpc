#ifndef __GPC_AST_HPP_INCLUDED__
#define __GPC_AST_HPP_INCLUDED__

namespace gpc {

    /**
     * Abstract base class for all nodes in the syntax tree.
     */
    class node {
    protected:

        /**
         * Reduced visibility.
         */
        node();

    public:
        /**
         * Made deconstructor virtual.
         */
        virtual ~node();

        /**
         * Evaluate the node and/or its children.
         */
        virtual double eval() = 0;
    };

    /**
     * Leaf which represents a numeric in the syntax tree.
     */
    class number_node : public node {
    public:

        /**
         * Construct a new number_node with the given value.
         */
        number_node(int value);
        
        /**
         * Return the number.
         */
        double eval();

    private:
        int m_value;
    };

    /**
     * Leaf which negates its child.
     */
    class unary_minus_node : public node {
    public:

        /**
         * Construct a new number_node with the given node to negate.
         */
        unary_minus_node(node* child);

        /**
         * Cleanup.
         */
        ~unary_minus_node();

        /**
         * Return the number.
         */
        double eval();

    private:
        node* m_child;
    };

    /**
     * Abstract base class for operations in the tree.
     */
    class op_node_base : public node {
    protected:

        /**
         * Construct a new op node with the given children nodes.
         */
        op_node_base(node* left, node* right);
        ~op_node_base();

        node* m_left;
        node* m_right;
    };

    /**
     * Addition operation node.
     */
    class add_op_node : public op_node_base {
    public:
        add_op_node(node* left, node* right);
        double eval();
    };

    /**
     * Subtraction operation node.
     */
    class sub_op_node : public op_node_base {
    public:
        sub_op_node(node* left, node* right);
        double eval();
    };

    /**
     * Multiply operation node.
     */
    class mul_op_node : public op_node_base {
    public:
        mul_op_node(node* left, node* right);
        double eval();
    };

    /**
     * Divide operation node.
     */
    class div_op_node : public op_node_base {
    public:
        div_op_node(node* left, node* right);
        double eval();
    };

}

#endif // __GPC_AST_HPP_INCLUDED__
