#ifndef __NODE_TREE_H__
#define __NODE_TREE_H__

#include <unistd.h>

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <typeinfo>

#include "mempool.h"
#include "string/string_util.h"

#include "container_return.h"
#include "container_log.h"

namespace infra::container {

class NodeTree {
public:
    class ElementAny {
    public:
        friend class mempool::Mempool;
    public:
        virtual ~ElementAny() { };
        
        virtual const std::type_info& GetTypeinfo() {
            return typeid(ElementAny);
        }
        virtual ElementAny* Clone(int depth = 0);

        std::string GetName();
        ElementAny*& GetParent();
        ElementAny*& GetChild();
        ElementAny*& GetNext();
        ElementAny*& GetPrev();

        virtual ElementAny* Search(std::string path);
        ContainerRet Search(std::string path, std::vector<ElementAny*>& vec);

        virtual void Erase(int depth = 0);

        ElementAny* Insert(std::string path);
        ElementAny* Insert(ElementAny& element);

        void PrintBranch(int depth = 0, std::string line = "");

    protected:
        ElementAny();
        ElementAny(std::string ename);
        ElementAny(const ElementAny& other);

        std::string name;
        ElementAny* next = nullptr;
        ElementAny* prev = nullptr;
        ElementAny* parent = nullptr;
        ElementAny* child = nullptr;

        ElementAny* vecnext = nullptr;
        ElementAny* vecprev = nullptr;

        ElementAny* SearchChild(std::string name);
        ElementAny* SearchBrother(std::string ename);
        ContainerRet Search(std::vector<std::string> path_vec, std::vector<ElementAny*>& vec);
        ContainerRet SearchChild(std::string name, std::vector<ElementAny*>& vec);
        ContainerRet SearchBrother(std::string ename, std::vector<ElementAny*>& vec);

        ElementAny* Insert(ElementAny* element);
        ElementAny* InsertChild(ElementAny* element);
        ElementAny* InsertNext(ElementAny* element);
        ElementAny* InsertPrev(ElementAny* element);
    };

public:
    template < typename T > class ElementData;
    class ElementPath : public ElementAny {
    public:
        friend class mempool::Mempool;
        friend class NodeTree;
    public:
        ~ElementPath() { }

        ElementPath* Search(std::string path) {
            ElementAny* ptr = ElementAny::Search(path);
            if (!ptr) {
                return NULL;
            }
            ElementPath* eptr = dynamic_cast<ElementPath*>(ptr);
            if (!eptr) {
                CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(ElementPath).name(), ptr->GetTypeinfo().name());
                return NULL;
            }
            return eptr;
        }
        ContainerRet Search(std::string path, std::vector<ElementPath*>& vec) {
            std::vector<NodeTree::ElementAny*> evec;
            ElementAny::Search(path, evec);
            if (evec.size() == 0) {
                return ContainerRet::NT_ENOTFOUND;
            } else {
                for (auto it : evec) {
                    ElementPath* eptr = dynamic_cast<ElementPath*>(it);
                    if (!eptr) {
                        CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(ElementPath).name(), it->GetTypeinfo().name());
                        return ContainerRet::NT_ENOTFOUND;
                    }
                    vec.push_back(eptr);
                }
            }
            return ContainerRet::SUCCESS;
        }

        ElementPath* Insert(std::string name) {
            ElementPath element(name);
            return Insert(element);
        }

        template < typename D > ElementData<D>* Search(std::string path) {
            ElementAny* ptr = ElementAny::Search(path);
            if (!ptr) {
                return NULL;
            }
            ElementData<D>* eptr = dynamic_cast<ElementData<D>*>(ptr);
            if (!eptr) {
                CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(D).name(), ptr->GetTypeinfo().name());
                return NULL;
            }
            return eptr;
        }
        template < typename D > ContainerRet Search(std::string path, std::vector<ElementData<D>*>& vec) {
            std::vector<NodeTree::ElementAny*> evec;
            ElementAny::Search(path, evec);
            if (evec.size() == 0) {
                return ContainerRet::NT_ENOTFOUND;
            } else {
                for (auto it : evec) {
                    ElementData<D>* eptr = dynamic_cast<ElementData<D>*>(it);
                    if (!eptr) {
                        CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(D).name(), it->GetTypeinfo().name());
                        return ContainerRet::NT_ENOTFOUND;
                    }
                    vec.push_back(eptr);
                }
            }
            return ContainerRet::SUCCESS;
        }

        template < typename D > ElementData<D>* Insert(std::string name, const D& data) {
            ElementData<D> element(name);
            element.SetData(data);
            return Insert(element);
        }

        void Erase(int depth = 0) {
            if (depth == 0) {
                if (child) {
                    child->Erase(depth + 1);
                }
                if (parent->GetChild() == this) {
                    parent->GetChild() = next;
                }
                if (next) {
                    next->GetPrev() = prev;
                }
                if (prev) {
                    prev->GetNext() = next;
                }
            } else {
                if (child) {
                    child->Erase(depth + 1);
                }
                if (next) {
                    next->Erase(depth);
                }
            }
            mempool::Mempool::getInstance()->Free<ElementPath>(this);
        }
    private:
        ElementPath() : ElementAny() {
            mempool_ = mempool::Mempool::getInstance();
        }

        ElementPath(std::string name) : ElementAny(name) {
            mempool_ = mempool::Mempool::getInstance();
        }

        ElementPath(const ElementPath& other) : ElementAny(other){
        }

        const std::type_info& GetTypeinfo() {
            return typeid(ElementPath);
        }
        
        ElementPath* Clone(int depth = 0) {
            ElementPath* self = mempool::Mempool::getInstance()->Malloc<ElementPath>(*this);
            if (depth == 0) {
                self->parent = nullptr;
                self->next = nullptr;
                self->prev = nullptr;
                if (self->child) {
                    self->child = self->child->Clone(depth + 1); 
                    self->child->GetParent() = this;
                }   
            } else {
                if (self->next) {
                    self->next = self->next->Clone(depth);
                    self->next->GetPrev() = this;
                }   
                if (self->child) {
                    self->child = self->child->Clone(depth + 1); 
                    self->child->GetParent() = this;
                }   
            }   
            return self;
        }
        
        ElementPath* Insert(ElementPath* element) {
            if (element == NULL) {
                return NULL;
            }
            return dynamic_cast<ElementPath*>(ElementAny::Insert(element));
        }
        ElementPath* Insert(ElementPath& element) {
            ElementPath* p_element = element.Clone();
            if (p_element == NULL) {
                return NULL;
            }
            return Insert(p_element);
        }

        template < typename D > ElementData<D>* Insert(ElementData<D>* element) {
            if (element == NULL) {
                return NULL;
            }
            return dynamic_cast<ElementData<D>*>(ElementAny::Insert(element));
        }
        template < typename D > ElementData<D>* Insert(ElementData<D>& element) {
            ElementData<D>* p_element = element.Clone();
            if (p_element == NULL) {
                return NULL;
            }
            return Insert(p_element);
        }

    private:
        mempool::Mempool* mempool_ = nullptr;
    };
public:
    template < typename T > class ElementData : public ElementAny {
    public:
        friend class mempool::Mempool;
        friend class NodeTree;
    public:
        ~ElementData() { }

        template < typename D > ElementData<D>* Search(std::string path) {
            ElementAny* ptr = ElementAny::Search(path);
            if (!ptr) {
                return NULL;
            }
            ElementData<D>* eptr = dynamic_cast<ElementData<D>*>(ptr);
            if (!eptr) {
                CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(D).name(), ptr->GetTypeinfo().name());
                return NULL;
            }
            return eptr;
        }
        template < typename D > ContainerRet Search(std::string path, std::vector<ElementData<D>*>& vec) {
            std::vector<NodeTree::ElementAny*> evec;
            ElementAny::Search(path, evec);
            if (evec.size() == 0) {
                return ContainerRet::NT_ENOTFOUND;
            } else {
                for (auto it : evec) {
                    ElementData<D>* eptr = dynamic_cast<ElementData<D>*>(it);
                    if (!eptr) {
                        CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(D).name(), it->GetTypeinfo().name());
                        return ContainerRet::NT_ENOTFOUND;
                    }
                    vec.push_back(eptr);
                }
            }
            return ContainerRet::SUCCESS;
        }

        template < typename D > ElementData<D>* Insert(std::string name, const D& data) {
            ElementData<D> element(name);
            element.SetData(data);
            return Insert(element);
        }

        ElementPath* Search(std::string path) {
            ElementAny* ptr = ElementAny::Search(path);
            if (!ptr) {
                return NULL;
            }
            ElementPath* eptr = dynamic_cast<ElementPath*>(ptr);
            if (!eptr) {
                CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(ElementPath).name(), ptr->GetTypeinfo().name());
                return NULL;
            }
            return eptr;
        }
        ContainerRet Search(std::string path, std::vector<ElementPath*>& vec) {
            std::vector<NodeTree::ElementAny*> evec;
            ElementAny::Search(path, evec);
            if (evec.size() == 0) {
                return ContainerRet::NT_ENOTFOUND;
            } else {
                for (auto it : evec) {
                    ElementPath* eptr = dynamic_cast<ElementPath*>(it);
                    if (!eptr) {
                        CONTAINER_ERROR("Mismatched type \"%s\" need \"%s\"", typeid(ElementPath).name(), it->GetTypeinfo().name());
                        return ContainerRet::NT_ENOTFOUND;
                    }
                    vec.push_back(eptr);
                }
            }
            return ContainerRet::SUCCESS;
        }

        ElementPath* Insert(std::string name) {
            ElementPath element(name);
            return Insert(element);
        }

        void Erase(int depth = 0) {
            if (depth == 0) {
                if (child) {
                    child->Erase(depth + 1);
                }
                if (parent->GetChild() == this) {
                    parent->GetChild() = next;
                }
                if (next) {
                    next->GetPrev() = prev;
                }
                if (prev) {
                    prev->GetNext() = next;
                }
            } else {
                if (child) {
                    child->Erase(depth + 1);
                }
                if (next) {
                    next->Erase(depth);
                }
            }
            mempool::Mempool::getInstance()->Free<ElementData<T>>(this);
        }

        ElementData<T>& SetData(const T& data) {
            element_data_ = data;
            return *this;
        }

        T& GetData() {
            return element_data_;
        }

    private:
        ElementData() : ElementAny() {
            mempool_ = mempool::Mempool::getInstance();
        }

        ElementData(std::string name) : ElementAny(name) {
            mempool_ = mempool::Mempool::getInstance();
        }

        ElementData(const ElementData& other) : ElementAny(other){
            element_data_ = other.element_data_;
        }

        const std::type_info& GetTypeinfo() {
            return typeid(T);
        }
        
        ElementData<T>* Clone(int depth = 0) {
            ElementData<T>* self = mempool::Mempool::getInstance()->Malloc<ElementData<T>>(*this);
            if (depth == 0) {
                self->parent = nullptr;
                self->next = nullptr;
                self->prev = nullptr;
                if (self->child) {
                    self->child = self->child->Clone(depth + 1); 
                    self->child->GetParent() = this;
                }   
            } else {
                if (self->next) {
                    self->next = self->next->Clone(depth);
                    self->next->GetPrev() = this;
                }   
                if (self->child) {
                    self->child = self->child->Clone(depth + 1); 
                    self->child->GetParent() = this;
                }   
            }   
            return self;
        }

        template < typename D > ElementData<D>* Insert(ElementData<D>* element) {
            if (element == NULL) {
                return NULL;
            }
            return dynamic_cast<ElementData<D>*>(ElementAny::Insert(element));
        }
        template < typename D > ElementData<D>* Insert(ElementData<D>& element) {
            ElementData<D>* p_element = element.Clone();
            if (p_element == NULL) {
                return NULL;
            }
            return Insert(p_element);
        }

        ElementPath* Insert(ElementPath* element) {
            if (element == NULL) {
                return NULL;
            }
            return dynamic_cast<ElementPath*>(ElementAny::Insert(element));
        }
        ElementPath* Insert(ElementPath& element) {
            ElementPath* p_element = element.Clone();
            if (p_element == NULL) {
                return NULL;
            }
            return Insert(p_element);
        }

    private:
        mempool::Mempool* mempool_ = nullptr;
        T element_data_;
    };

public:
    NodeTree(std::string name) : root_(name) {
        root_.Insert("root");
    }

    NodeTree(NodeTree& other) {
        ElementPath* root = GetRoot();
        if (root) {
            printf("---%d---\n",__LINE__);
            root->Erase();
        }
        root = other.GetRoot();
        if (root) {
            printf("---%d---\n",__LINE__);
            root_.Insert(root->Clone());
        }
    }
    ~NodeTree(){
        ElementPath* root = GetRoot();
        if (root) {
            root->Erase();
        }
    }

    NodeTree& operator=(NodeTree& other) {
        printf("---%d---\n",__LINE__);
        ElementPath* root = GetRoot();
        if (root) {
            printf("---%d---\n",__LINE__);
            root->Erase();
        }
        root = other.GetRoot();
        if (root) {
            printf("---%d---\n",__LINE__);
            root_.Insert(root->Clone());
        }
        return *this;
    }

    ElementPath* GetRoot() {
        return root_.Search("root");
    }

    NodeTree& Print() {
        root_.PrintBranch();
        return *this;
    }

private:

    ElementPath root_;
};

} //namespace end

#endif
