#ifndef _SCENE_
#define _SCENE_

#include "../util/str.h"
#include "../util/list.h"
#include "../util/tree.h"
#include "point.h"
#include "transform.h"

typedef struct _scene_node _SceneNode;
typedef _SceneNode* SceneNode;

typedef struct _drawable* Drawable;

struct _scene_node {

    String name;
    Transform transform;
    Boolean visible;
    LIST_OF(Drawable) drawables;
    TreeNode container;

    FUNC(Transform, _global_transform, SceneNode);
    FUNC(void, _free, SceneNode);

};

#define SCENE_NODE_NAME(self) FIELD(self, name)
#define SCENE_NODE_LOCAL_TRANSFORM(self) FIELD(self, transform)
#define SCENE_NODE_IS_VISIBLE(self) FIELD(self, visible)
#define SCENE_NODE_DRAWABLES(self) FIELD(self, drawables)
#define SCENE_NODE_IS_EMPTY(self) LIST_IS_EMPTY(SCENE_NODE_DRAWABLES(self))
#define SCENE_NODE_CONTAINER(self) FIELD(self, container)
#define SCENE_NODE_GLOBAL_TRANSFORM(self) METHOD(self, _global_transform)
#define SCENE_NODE_FREE(self) METHOD(self, _free)

typedef struct _scene_node_factory SceneNodeFactory;

struct _scene_node_factory {

    FUNC(SceneNode, _new, String, TreeNode);

};

extern const SceneNodeFactory SceneNodes;

#define SCENE_NODE_NEW(name, container) SceneNodes._new(name, container)

typedef struct _scene _Scene;
typedef _Scene* Scene;

struct _scene {

    String name;
    Tree tree;

    FUNC(void, _draw, Scene);
    FUNC(void, _free, Scene);

};

#define SCENE_NAME(self) FIELD(self, name)
#define SCENE_TREE(self) FIELD(self, tree)
#define SCENE_DRAW(self) METHOD(self, _draw)
#define SCENE_FREE(self) METHOD(self, _free)

typedef struct _scene_factory SceneFactory;

struct _scene_factory {

    FUNC(Scene, _new, String);

};

extern const SceneFactory Scenes;

#define SCENE_NEW(name) Scenes._new(name)

#endif
