#ifndef _TREE_
#define _TREE_

#include "lang.h"
#include "list.h"

struct _tree;
typedef struct _tree _Tree;
typedef _Tree* Tree;

typedef struct _tree_node _TreeNode;
typedef _TreeNode* TreeNode;

struct _tree_node {
    
    Any content;
    Tree container;
    TreeNode parent;
    LIST_OF(TreeNode) children;

    FUNC(void, _free, TreeNode, Boolean, AnyConsumer);

};

#define TREE_NODE_CONTENT(self) FIELD(self, content)
#define TREE_NODE_CONTENT_AS(self, type) CAST(type, TREE_NODE_CONTENT(self))
#define TREE_NODE_CONTAINER(self) FIELD(self, container)
#define TREE_NODE_PARENT(self) FIELD(self, parent)
#define TREE_NODE_IS_ROOT(self) IS_NULL(TREE_NODE_PARENT(self))
#define TREE_NODE_CHILDREN(self) FIELD(self, children)
#define TREE_NODE_IS_LEAF(self) LIST_IS_EMPTY(TREE_NODE_CHILDREN(self))
#define TREE_NODE_FREE(self, free_content, custom_free) METHOD(self, _free, free_content, custom_free)

struct _tree {
    
    TreeNode root;

    FUNC(void, _free, Tree, Boolean, AnyConsumer);

};

#define TREE_ROOT(self) FIELD(self, root)
#define TREE_IS_EMPTY(self) IS_NULL(TREE_ROOT(self))
#define TREE_FREE(self, free_content, custom_free) METHOD(self, _free, free_content, custom_free)

typedef struct _tree_factory TreeFactory;

struct _tree_factory {
    
    FUNC(Tree, _new, void);

};

extern const TreeFactory Trees;

#define TREE_NEW() Trees._new()

#endif
