#ifndef ZLAYOUT_H
#define ZLAYOUT_H

#include <zsl.h>

typedef enum {
  FLEX_DIRECTION_COLUMN = 0,
  FLEX_DIRECTION_COLUMN_REVERSE,
  FLEX_DIRECTION_ROW,
  FLEX_DIRECTION_ROW_REVERSE
} FlexDirection;

typedef enum {
  JUSTIFY_FLEX_START = 0,
  JUSTIFY_CENTER,
  JUSTIFY_FLEX_END,
  JUSTIFY_SPACE_BETWEEN,
  JUSTIFY_SPACE_AROUND,
  JUSTIFY_SPACE_EVENLY
} JustifyContent;

typedef struct {
  f32 width;
  f32 height;

  f32 flexGrow;
  f32 flexShrink;
  FlexDirection flexDirection;
  JustifyContent justifyContent;

  f32 margin;
  f32 marginTop;
  f32 marginBottom;
  f32 marginLeft;
  f32 marginRight;

  f32 padding;

  f32 gap;
} LayoutStyle;

struct sLayoutNode;
typedef struct sLayoutNode LayoutNode;

typedef struct tLayoutResult {
  u32 id;
  rec bounds;
} LayoutResult;

LayoutNode* newLayoutNode(Arena* arena, i32 id, LayoutStyle style);
LayoutNode* addChild(Arena* arena, LayoutNode* parent, i32 id, LayoutStyle style);
Array_pLayoutResult* calculate(Arena* arena, LayoutNode* root);

#ifdef ZLAYOUT_IMPLEMENTATION
#undef ZLAYOUT_IMPLEMENTATION

static LayoutResult* newLayoutResult(Arena* arena);
static void calculate_(Arena* arena, LayoutNode* root);

Array_pLayoutResult* calculatedValues_;

typedef struct sLayoutNode {
  Array_pLayoutNode* children;
  LayoutStyle style;
  i32 id;

  rec resultBounds_;
} LayoutNode;

LayoutNode* newLayoutNode(Arena* arena, i32 id, LayoutStyle style)
{
  LayoutNode* ln = allocate(arena, sizeof(LayoutNode));
  ln->children = newArray_pLayoutNode(arena);
  ln->id = id;
  ln->style = style;
  return ln;
}

LayoutNode* addChild(Arena* arena, LayoutNode* parent, i32 id, LayoutStyle style)
{
  LayoutNode* newNode = newLayoutNode(arena, id, style);
  push_pLayoutNode(arena, parent->children, newNode);
  return newNode;
}

LayoutResult* newLayoutResult(Arena* arena)
{
  return allocate(arena, sizeof(LayoutResult));
}

void calculate_(Arena* arena, LayoutNode* root)
{
  LayoutResult* layoutResult = newLayoutResult(arena);
  layoutResult->id = root->id;
  layoutResult->bounds = root->resultBounds_;
  push_pLayoutResult(arena, calculatedValues_, layoutResult);

  // calculate children bounds
  f32 childHeight = layoutResult->bounds.height / root->children->length;
  for (u32 i = 0; i < root->children->length; i++) {
    LayoutNode* childNode = get_pLayoutNode(root->children, i).result;

    childNode->resultBounds_.width = layoutResult->bounds.width;
    childNode->resultBounds_.height = childHeight;
    childNode->resultBounds_.x = layoutResult->bounds.x;
    childNode->resultBounds_.y = layoutResult->bounds.y + childHeight * i;
  }

  // recurse into children
  for (u32 i = 0; i < root->children->length; i++) {
    Result_pLayoutNode node = get_pLayoutNode(root->children, i);
    assert(node.error == SUCCESS);
    calculate_(arena, node.result);
  }
}

Array_pLayoutResult* calculate(Arena* arena, LayoutNode* root)
{
  root->resultBounds_.x = 0.0f;
  root->resultBounds_.y = 0.0f;
  root->resultBounds_.width = root->style.width;
  root->resultBounds_.height = root->style.height;

  calculatedValues_ = newArray_pLayoutResult(arena);
  calculate_(arena, root);
  return calculatedValues_;
}

#endif // ZLAYOUT_IMPLEMENTATION

#endif // ZLAYOUT_H
