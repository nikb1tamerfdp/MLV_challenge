#include "../../includes/util/tree.h"
#include "../../includes/util/contract.h"

/* --- declaration of methods for TreeNode */
TreeNode _TreeNode_new(Tree root, TreeNode parent, Any content);
void _TreeNode_free(TreeNode self, Boolean free_content, AnyConsumer custom_free);

/* --- declaration of methods for Tree */
Tree _Tree_new();
void _Tree_init_methods(Tree self);
void _Tree_free(Tree self, Boolean free_content, AnyConsumer custom_free);
void _Tree_free_recursive(TreeNode node, Boolean free_content, AnyConsumer custom_free);

const TreeFactory Trees = {
    _Tree_new
};

TreeNode _TreeNode_new(Tree container, TreeNode parent, Any content)
{

    REQUIRE_NON_NULL(container);

    TreeNode self = NEW(_TreeNode);
    TREE_NODE_CONTENT(self) = content;
    TREE_NODE_CONTAINER(self) = container;
    TREE_NODE_PARENT(self) = parent;
    TREE_NODE_CHILDREN(self) = LIST_NEW();

    return self;

}

void _TreeNode_free(TreeNode self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    if (free_content && NON_NULL(TREE_NODE_CONTENT(self)))
        IF_NULL_DEFAULT(custom_free, FREE)(TREE_NODE_CONTENT(self));
    
    LIST_FREE(TREE_NODE_CHILDREN(self), FALSE, NULL);
    FREE(self);

}

Tree _Tree_new()
{

    Tree self = NEW(_Tree);
    TREE_ROOT(self) = NULL;

    return self;

}

void _Tree_init_methods(Tree self)
{
    FIELD(self, _free) = _Tree_free;
}

void _Tree_free(Tree self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    TREE_NODE_FREE(TREE_ROOT(self), free_content, custom_free);
    FREE(self);

}

void _Tree_free_recursive(TreeNode node, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(node);

    /* ask each child to free itself */
    Iterator it = LIST_ITERATOR(TREE_NODE_CHILDREN(node));
    while (ITER_HAS_NEXT(it))
        _Tree_free_recursive(ITER_NEXT(it), free_content, custom_free);
    
    _TreeNode_free(node, free_content, custom_free);

}
