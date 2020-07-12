#include <sstream>
#include "node_tree.h"

namespace container {

NodeTree::ElementAny::ElementAny()
{
    name = "";
    next = NULL;
    prev = NULL;
    parent = NULL;
    child = NULL;
}

NodeTree::ElementAny::ElementAny(std::string ename) : name(ename)
{
    next = NULL;
    prev = NULL;
    parent = NULL;
    child = NULL;
}

NodeTree::ElementAny::ElementAny(const NodeTree::ElementAny& other)
{
    name = other.name;
    next = other.next;
    prev = other.prev;
    parent = other.parent;
    child = other.child;
}

std::string NodeTree::ElementAny::GetName()
{
    return name;
}
NodeTree::ElementAny*& NodeTree::ElementAny::GetParent()
{
    return parent;
}
NodeTree::ElementAny*& NodeTree::ElementAny::GetChild()
{
    return child;
}
NodeTree::ElementAny*& NodeTree::ElementAny::GetNext()
{
    return next;
}
NodeTree::ElementAny*& NodeTree::ElementAny::GetPrev()
{
    return prev;
}

void NodeTree::ElementAny::Erase(int depth)
{
    if (depth == 0) {
        if (child) {
            child->Erase(depth + 1);
        }
        if (parent->child == this) {
            parent->child = next;
        }
        if (next) {
            next->prev = prev;
        }
        if (prev) {
            prev->next = next;
        }
    } else {
        if (child) {
            child->Erase(depth + 1);
        }
        if (next) {
            next->Erase(depth);
        }
    }
    mempool::Mempool::getInstance()->Free<ElementAny>(this);
}

NodeTree::ElementAny* NodeTree::ElementAny::Clone(int depth)
{
    NodeTree::ElementAny* self = mempool::Mempool::getInstance()->Malloc<NodeTree::ElementAny>(*this);
    if (depth == 0) {
        self->parent = nullptr;
        self->next = nullptr;
        self->prev = nullptr;
        if (self->child) {
            self->child = self->child->Clone(depth + 1);
            self->child->parent = this;
        }
    } else {
        if (self->next) {
            self->next = self->next->Clone(depth);
            self->next->prev = this;
        }
        if (self->child) {
            self->child = self->child->Clone(depth + 1);
            self->child->parent = this;
        }
    }
    return self;
}

NodeTree::ElementAny* NodeTree::ElementAny::Search(std::string path)
{
    std::vector<std::string> path_vec;
    util::String::Split(path, "/", path_vec);

    NodeTree::ElementAny* ptr = this;
    for (auto it : path_vec) {
        ptr = ptr->SearchChild(it);
        if (!ptr) return NULL;
    }
    return ptr;
}

NodeTree::ElementAny* NodeTree::ElementAny::SearchChild(std::string name)
{
    if (!child) {
        return NULL;
    }
    return child->SearchBrother(name);
}

NodeTree::ElementAny* NodeTree::ElementAny::SearchBrother(std::string ename)
{
    if (name == ename) {
        return this;
    }
    NodeTree::ElementAny* ptr = prev;
    while(ptr) {
        if (ptr->name == ename) {
            return ptr;
        }
        ptr = ptr->prev;
    }
    ptr = next;
    while(ptr) {
        if (ptr->name == ename) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

ContainerRet NodeTree::ElementAny::Search(std::string path, std::vector<NodeTree::ElementAny*>& vec)
{
    std::vector<std::string> path_vec;
    util::String::Split(path, "/", path_vec);
    std::reverse(std::begin(path_vec), std::end(path_vec));

    return Search(path_vec, vec);
}

ContainerRet NodeTree::ElementAny::Search(std::vector<std::string> path_vec, std::vector<NodeTree::ElementAny*>& vec)
{
    if (path_vec.size() == 0) {
        return ContainerRet::SUCCESS;
    } else if (path_vec.size() == 1) {
        std::string path = path_vec.back();
        path_vec.pop_back();
        return SearchChild(path, vec);
    } else {
        std::string path = path_vec.back();
        path_vec.pop_back();
        std::vector<NodeTree::ElementAny*> path_vec_tmp;
        SearchChild(path, path_vec_tmp);
        for (auto it : path_vec_tmp) {
            it->Search(path_vec, vec);
        }
        return ContainerRet::SUCCESS;
    }
}

ContainerRet NodeTree::ElementAny::SearchChild(std::string name, std::vector<NodeTree::ElementAny*>& vec)
{
    if (!child) {
        return ContainerRet::NT_ENOTFOUND;
    }
    return child->SearchBrother(name, vec);
}

ContainerRet NodeTree::ElementAny::SearchBrother(std::string ename, std::vector<NodeTree::ElementAny*>& vec)
{
    if (name == ename) {
        vec.push_back(this);
    }
    NodeTree::ElementAny* ptr = prev;
    while(ptr) {
        if (ptr->name == ename) {
            vec.push_back(ptr);
        }
        ptr = ptr->prev;
    }
    ptr = next;
    while(ptr) {
        if (ptr->name == ename) {
            vec.push_back(ptr);
        }
        ptr = ptr->next;
    }
    return vec.size() ? ContainerRet::SUCCESS : ContainerRet::NT_ENOTFOUND;
}

NodeTree::ElementAny* NodeTree::ElementAny::Insert(std::string path) {
    std::vector<std::string> path_vec;
    util::String::Split(path, "/", path_vec);

    ElementAny* ptr = this;
    bool new_path = false;
    for (auto it : path_vec) {
        if (!new_path) {
            ElementAny* search = ptr->Search(it);
            if (!search) {
                ElementAny element(it);
                ptr = ptr->Insert(element);
                new_path = true;
            } else {
                ptr = search;
            }
        } else {
            ElementAny element(it);
            ptr = ptr->Insert(element);
        }
    }
    return ptr;
}

NodeTree::ElementAny* NodeTree::ElementAny::Insert(NodeTree::ElementAny& element) {
    ElementAny* p_element = element.Clone();
    if (p_element == NULL) {
        return NULL;
    }
    return Insert(p_element);
}

NodeTree::ElementAny* NodeTree::ElementAny::Insert(NodeTree::ElementAny* element) {
    if (element == NULL) {
        return NULL;
    }
    return InsertChild(element);
}

NodeTree::ElementAny* NodeTree::ElementAny::InsertChild(NodeTree::ElementAny* element)
{
    if (!child) {
        child = element;
        child->parent = this;
        return child;
    }
    return child->InsertNext(element);
}

NodeTree::ElementAny* NodeTree::ElementAny::InsertNext(NodeTree::ElementAny* element)
{
    if (!next) {
        next = element;
        next->prev = this;
        next->parent = parent;
        return next;
    }
    NodeTree::ElementAny* ptr = next;
    while(ptr->next) {
        ptr = ptr->next;
    }
    ptr->next = element;
    ptr->next->prev = ptr;
    ptr->next->parent = parent;

    return ptr->next;
}

NodeTree::ElementAny* NodeTree::ElementAny::InsertPrev(NodeTree::ElementAny* element)
{
    if (!prev) {
        prev = element;
        prev->next = this;
        prev->parent = parent;
        return prev;
    }
    NodeTree::ElementAny* ptr = prev;
    while(!ptr->prev) {
        ptr = ptr->prev;
    }
    ptr->prev = element;
    ptr->prev->next = ptr;
    ptr->prev->parent = parent;

    return ptr->prev;
}


void NodeTree::ElementAny::PrintBranch(int depth, std::string line)
{
#define SINGLE_CHILD_LINE "---"
#define FIRST_CHILD_LINE "-+-"
#define NEXT_CHILD_LINE " |-"
#define LAST_CHILD_LINE " `-"
#define INDENT_CHAR '|'
#define EXTEN_CHAR '+'
    std::string raw_line = line;
    for (auto& c : raw_line) {
        if (c != EXTEN_CHAR && c != INDENT_CHAR) {
            c = ' ';
        } else {
            c = INDENT_CHAR;
        }
    }

    std::stringstream test;
    test << name << "(" << this << ")";
    std::string tt;
    test >> tt;
    if (depth == 0) {
        line += tt;
    } else {
        if (!next && !prev) {
            line += SINGLE_CHILD_LINE;
        } else if (!prev) {
            line += FIRST_CHILD_LINE;
        } else if (!next) {
            line += LAST_CHILD_LINE;
        } else {
            line += NEXT_CHILD_LINE;
        }
        line += tt;
    }

    if (!child) {
        std::cout << line << std::endl;
        if (next && depth) {
            next->PrintBranch(depth, raw_line);
        }
    } else {
        child->PrintBranch(depth+1, line);
        if (next && depth) {
            next->PrintBranch(depth, raw_line);
        }
    }
}

}
