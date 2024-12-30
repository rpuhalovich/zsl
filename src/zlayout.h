#ifndef ZLAYOUT_H
#define ZLAYOUT_H

#include <zsl.h>

typedef enum {
  FLEX_DIRECTION_COLUMN = 0,
  FLEX_DIRECTION_ROW
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
  // calculate root margin
  rec newBounds = root->resultBounds_;

  if (root->style.margin > 0.0f) {
    newBounds.x = root->resultBounds_.x + root->style.margin;
    newBounds.y = root->resultBounds_.y + root->style.margin;
    newBounds.width = root->resultBounds_.width - root->style.margin * 2;
    newBounds.height = root->resultBounds_.height - root->style.margin * 2;
  } else {
    if (root->style.marginLeft > 0.0f) {
      newBounds.x = root->resultBounds_.x + root->style.marginLeft;
      newBounds.width = root->resultBounds_.width - root->style.marginLeft;
    }

    if (root->style.marginRight > 0.0f) {
      newBounds.width = root->resultBounds_.width - root->style.marginRight;
    }

    if (root->style.marginTop > 0.0f) {
      newBounds.y = root->resultBounds_.y + root->style.marginTop;
      newBounds.height = root->resultBounds_.height - root->style.marginTop;
    }

    if (root->style.marginBottom > 0.0f) {
      newBounds.height = root->resultBounds_.height - root->style.marginBottom;
    }
  }

  // add root to calculated values
  LayoutResult* layoutResult = newLayoutResult(arena);
  layoutResult->id = root->id;
  layoutResult->bounds = newBounds;
  push_pLayoutResult(arena, calculatedValues_, layoutResult);

  // calculate children bounds
  if (root->style.flexDirection == FLEX_DIRECTION_COLUMN) {
    f32 styleHeight = 0.0f;
    u32 styleHeightCount = 0;
    for (u32 i = 0; i < root->children->length; i++) {
      LayoutNode* childNode = get_pLayoutNode(root->children, i).result;
      if (childNode->style.height > 0.0f) {
        styleHeight += childNode->style.height;
        styleHeightCount++;
      }
    }

    f32 cury = 0.0f;
    f32 childHeight = (layoutResult->bounds.height - styleHeight) / (root->children->length - styleHeightCount);
    for (u32 i = 0; i < root->children->length; i++) {
      LayoutNode* childNode = get_pLayoutNode(root->children, i).result;
      childNode->resultBounds_.x = layoutResult->bounds.x;
      childNode->resultBounds_.y = layoutResult->bounds.y + cury;

      f32 resolvedChildWidth = childNode->style.width > 0.0f ? childNode->style.width : layoutResult->bounds.width;
      childNode->resultBounds_.width = resolvedChildWidth;

      f32 resolvedChildHeight = childNode->style.height > 0.0f ? childNode->style.height : childHeight;
      childNode->resultBounds_.height = resolvedChildHeight;
      cury += resolvedChildHeight;
    }
  }

  if (root->style.flexDirection == FLEX_DIRECTION_ROW) {
    f32 styleWidth = 0.0f;
    u32 styleWidthCount = 0;
    for (u32 i = 0; i < root->children->length; i++) {
      LayoutNode* childNode = get_pLayoutNode(root->children, i).result;
      if (childNode->style.width > 0.0f) {
        styleWidth += childNode->style.width;
        styleWidthCount++;
      }
    }

    f32 curx = 0.0f;
    f32 childWidth = (layoutResult->bounds.width - styleWidth) / (root->children->length - styleWidthCount);
    for (u32 i = 0; i < root->children->length; i++) {
      LayoutNode* childNode = get_pLayoutNode(root->children, i).result;
      childNode->resultBounds_.x = layoutResult->bounds.x + curx;
      childNode->resultBounds_.y = layoutResult->bounds.y;

      f32 resolvedChildWidth = childNode->style.width > 0.0f ? childNode->style.width : childWidth;
      childNode->resultBounds_.width = resolvedChildWidth;
      curx += resolvedChildWidth;

      f32 resolvedChildHeight = childNode->style.height > 0.0f ? childNode->style.height : layoutResult->bounds.height;
      childNode->resultBounds_.height = resolvedChildHeight;
    }
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
