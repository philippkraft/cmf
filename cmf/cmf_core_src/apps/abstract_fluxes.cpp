//
// Created by kraft-p on 13.06.2019.
//

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

struct Node;
struct Arc {
    Node *left,*right;
private:
    // shared pointer to self, manages the lifetime.
    std::shared_ptr<Arc> skyhook;
public:
    // c'tor of Arc, registers Arc with its nodes (as weak pointers of skyhook)
    explicit Arc(Node* a_, Node* b_, bool throw_exc);
    // resets skyhook to kill it self
    void free() {
        std::cout << "  Arc::free();\n" << std::flush;
        skyhook.reset();
    }
    virtual ~Arc() {
        std::cout << "  Arc::~Arc();\n" << std::flush;
    }
    typedef std::shared_ptr<Arc> ptr;
    static Arc::ptr make(Node *a_, Node *b_, bool throw_exc);
};

struct Node {
    explicit Node() {
        std::cout << "  Node::Node()\n" << std::flush;
    }
    std::vector<std::weak_ptr<Arc> > arcs;
    ~Node() {
        std::cout << "  Node::~Node();\n" << std::flush;
        for(const auto &w : arcs) {
            if(const auto a=w.lock()) {
                a->free();
            }
        }
    }
};

Arc::Arc(Node *a_, Node *b_, bool throw_exc) : left(a_), right(b_) {
    std::cout << "  Arc::Arc()\n" << std::flush;
    if (throw_exc) {
        throw std::runtime_error("throw in Arc::Arc(...)");
    }

}

Arc::ptr Arc::make(Node *a_, Node *b_, bool throw_exc)  {
    std::cout << "  Arc::make(...)\n" << std::flush;
    auto self = std::make_shared<Arc>(a_, b_, throw_exc);
    a_->arcs.push_back(self);
    b_->arcs.push_back(self);
    self->skyhook = self;
    return self;
}


int main() {
    std::cout << "n1=new Node()\n" << std::flush;
    Node *n1 = new Node();
    std::cout << "n2=new Node()\n" << std::flush;
    Node *n2 = new Node();
    std::cout << "try a=new Arc()\n" << std::flush;
    try {
        Arc::make(n1, n2, true);
    } catch (const std::runtime_error &e) {
        std::cout << "Failed to build Arc: " << e.what() << "\n" << std::flush;
    }
    std::cout << "delete n1\n" << std::flush;
    delete n1;
    std::cout << "delete n2\n" << std::flush;
    delete n2;

}
/*
#include "project.h"
#include "water/simple_connections.h"

struct P {
    cmf::project p;
    cmf::water::flux_node::ptr l;
    cmf::water::DirichletBoundary::ptr r;

    P() {
        l = p.NewOutlet("w", 0, 0, 0);
        r = p.NewOutlet("o", 0, 0, 1);
        r->is_source = true;
    }
};

void  SWIG_exception(std::string etype, std::string ewhat) {
    std::cerr << etype << "\n" << ewhat << "\n";
}


int main(int argc, char* argv[]) {
    P p;
    cmf::water::LinearGradientFlux* result = nullptr;
    std::cout << "burp\n" << std::flush;
    try {
        std::cerr << "Try result = new cmf::water::LinearGradientFlux(p.l, p.r ,1.0, 1.0);\n" << std::flush;
        result = new cmf::water::LinearGradientFlux(p.l, p.r, 1.0, 1.0);
    } catch (const cmf::water::flux_connection_construction_error& e) {
        std::cerr << "except flux_connection_construction_error(" << e.what() << "\n" << std::flush;
        e.connection->kill_me();
        std::cerr << "Connection killed\n" << std::flush;
    } catch (const std::out_of_range& e) {
        SWIG_exception("SWIG_IndexError", e.what());
    } catch (const std::exception& e) {
        std::cerr << "except exception\n" << std::flush;
        SWIG_exception("SWIG_RuntimeError", e.what());
    } catch (...) {
        std::cerr << "except unknown exception\n" << std::flush;
        SWIG_exception("SWIG_RuntimeError", "unknown error");
    }
    std::cout << p.l->to_string() << "->" << p.r->to_string() << ": " << p.l->flux_to(*p.r, cmf::math::h) << "\n" << std::flush
 }

*/