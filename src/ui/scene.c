#include "../../includes/util/contract.h"
#include "../../includes/ui/scene.h"
#include "../../includes/ui/drawables/drawable.h"

SceneNode _SceneNode_new(String name, TreeNode container);
void _SceneNode_init_methods(SceneNode self);
Transform _SceneNode_global_transform(SceneNode self);
void _SceneNode_free(SceneNode self);

const SceneNodeFactory SceneNodes = {
    _SceneNode_new
};

Scene _Scene_new(String name);
void _Scene_init_methods(Scene self);
void _Scene_draw(Scene self);
void _Scene_draw_recursive(TreeNode root, Transform transform);
void _Scene_free(Scene self);

const SceneFactory Scenes = {
    _Scene_new
};

SceneNode _SceneNode_new(String name, TreeNode container)
{

    REQUIRE_NON_NULL(name);
    REQUIRE_NON_NULL(container);

    SceneNode self = NEW(_SceneNode);
    SCENE_NODE_NAME(self) = name;
    SCENE_NODE_LOCAL_TRANSFORM(self) = TRANSFORM_IDENTITY();
    SCENE_NODE_DRAWABLES(self) = LIST_NEW();
    SCENE_NODE_CONTAINER(self) = container;
    SCENE_NODE_IS_VISIBLE(self) = TRUE;
    _SceneNode_init_methods(self);

    return self;

}

void _SceneNode_init_methods(SceneNode self)
{

    FIELD(self, _global_transform) = _SceneNode_global_transform;
    FIELD(self, _free) = _SceneNode_free;

}

Transform _SceneNode_global_transform(SceneNode self)
{
    

    Transform local_transform = SCENE_NODE_LOCAL_TRANSFORM(self);

    LIST_OF(Transform) genealogy = LIST_NEW();
    TreeNode index = TREE_NODE_PARENT(SCENE_NODE_CONTAINER(self));
    while (NON_NULL(index))
    {
        SceneNode current_node = TREE_NODE_CONTENT_AS(index, SceneNode);
        LIST_PUSH(genealogy, SCENE_NODE_LOCAL_TRANSFORM(current_node));
        index = index -> parent;
    }

    if (LIST_IS_EMPTY(genealogy))
        return TRANSFORM_COMBINE(TRANSFORM_IDENTITY(), local_transform);
    
    Iterator it = LIST_ITERATOR(genealogy);
    Transform a = ITER_NEXT_AS(it, Transform);
    if (!ITER_HAS_NEXT(it))
        return TRANSFORM_COMBINE(a, local_transform);

    Transform b = ITER_NEXT_AS(it, Transform);
    a = TRANSFORM_COMBINE(a, b);
    Transform prev_a = a;
    
    while (ITER_HAS_NEXT(it))
    {

        b = ITER_NEXT_AS(it, Transform);
        a = TRANSFORM_COMBINE(a, b);
        TRANSFORM_FREE(prev_a);
        prev_a = a;
    
    }

    return TRANSFORM_COMBINE(a, local_transform);

}

void _SceneNode_free(SceneNode self)
{

    ENSURE_FREEABLE(self);

    String_free(SCENE_NODE_NAME(self));
    TRANSFORM_FREE(SCENE_NODE_LOCAL_TRANSFORM(self));
    LIST_FREE(SCENE_NODE_DRAWABLES(self), FALSE, NULL);
    FREE(self);

}

Scene _Scene_new(String name)
{

    REQUIRE_NON_NULL(name);

    Scene self = NEW(_Scene);
    SCENE_NAME(self) = name;
    SCENE_TREE(self) = TREE_NEW();
    TreeNode root = NEW(_TreeNode);
    root -> children = LIST_NEW();
    root -> container = SCENE_TREE(self);
    root -> content = NULL;
    root -> parent = NULL;
    TREE_ROOT(SCENE_TREE(self)) = root;

    _Scene_init_methods(self);

    return self;

}

void _Scene_init_methods(Scene self)
{

    FIELD(self, _draw) = _Scene_draw;
    FIELD(self, _free) = _Scene_free;

}

void _Scene_draw(Scene self)
{

    REQUIRE_NON_NULL(self);

    Transform identity = TRANSFORM_IDENTITY();
    _Scene_draw_recursive(TREE_ROOT(SCENE_TREE(self)), identity);
    TRANSFORM_FREE(identity);

}

void _Scene_draw_recursive(TreeNode node, Transform global_transform)
{

    if (IS_NULL(node))
        return;

    SceneNode scene_node = TREE_NODE_CONTENT_AS(node, SceneNode);
    if (!SCENE_NODE_IS_VISIBLE(scene_node))
        return;

    Transform local_transform = SCENE_NODE_LOCAL_TRANSFORM(scene_node);
    Transform globalized_transform = TRANSFORM_COMBINE(global_transform, local_transform);

    LIST_OF(Drawable) drawables = SCENE_NODE_DRAWABLES(scene_node);
    Iterator it = LIST_ITERATOR(drawables);
    while (ITER_HAS_NEXT(it))
        DRAWABLE_DRAW(ITER_NEXT_AS(it, Drawable), globalized_transform);

    LIST_OF(TreeNode) children = TREE_NODE_CHILDREN(node);
    it = LIST_ITERATOR(children);
    while (ITER_HAS_NEXT(it))
        _Scene_draw_recursive(ITER_NEXT_AS(it, TreeNode), globalized_transform);

    TRANSFORM_FREE(globalized_transform);

}

void _Scene_free(Scene self)
{

    ENSURE_FREEABLE(self);

    String_free(SCENE_NAME(self));
    TREE_FREE(SCENE_TREE(self), TRUE, CAST(AnyConsumer, _SceneNode_free));
    FREE(self);

}
