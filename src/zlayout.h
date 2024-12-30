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

#include <yoga/Yoga.h>

static LayoutResult* newLayoutResult(Arena* arena);
static rec toRec(YGNodeRef node);
static void calculate_(Arena* arena, LayoutNode* root, f32 leftOffset, f32 topOffset);

Array_pLayoutResult* calculatedValues_;

typedef struct sLayoutNode {
  Array_pLayoutNode* children;
  YGNodeRef node;
  LayoutStyle style;
  i32 id;
} LayoutNode;

rec toRec(YGNodeRef node)
{
  rec res = (rec){
    YGNodeLayoutGetLeft(node),
    YGNodeLayoutGetTop(node),
    YGNodeLayoutGetWidth(node),
    YGNodeLayoutGetHeight(node),
  };
  return res;
}

LayoutNode* newLayoutNode(Arena* arena, i32 id, LayoutStyle style)
{
  LayoutNode* ln = allocate(arena, sizeof(LayoutNode));
  ln->children = newArray_pLayoutNode(arena);
  ln->id = id;
  ln->node = YGNodeNew();

  if (style.width)
    YGNodeStyleSetWidth(ln->node, style.width);
  if (style.height)
    YGNodeStyleSetHeight(ln->node, style.height);

  if (style.flexGrow)
    YGNodeStyleSetFlexGrow(ln->node, style.flexGrow);
  if (style.flexShrink)
    YGNodeStyleSetFlexShrink(ln->node, style.flexShrink);
  if (style.flexDirection)
    YGNodeStyleSetFlexDirection(ln->node, (YGFlexDirection)style.flexDirection);

  if (style.margin)
    YGNodeStyleSetMargin(ln->node, YGEdgeAll, style.margin);
  if (style.marginTop)
    YGNodeStyleSetMargin(ln->node, YGEdgeTop, style.marginTop);
  if (style.marginBottom)
    YGNodeStyleSetMargin(ln->node, YGEdgeBottom, style.marginBottom);
  if (style.marginLeft)
    YGNodeStyleSetMargin(ln->node, YGEdgeLeft, style.marginLeft);
  if (style.marginRight)
    YGNodeStyleSetMargin(ln->node, YGEdgeRight, style.marginRight);

  if (style.gap)
    YGNodeStyleSetGap(ln->node, YGGutterAll, style.gap);

  return ln;
}

LayoutNode* addChild(Arena* arena, LayoutNode* parent, i32 id, LayoutStyle style)
{
  LayoutNode* newNode = newLayoutNode(arena, id, style);
  YGNodeInsertChild(parent->node, newNode->node, parent->children->length);
  push_pLayoutNode(arena, parent->children, newNode);
  return newNode;
}

LayoutResult* newLayoutResult(Arena* arena)
{
  return allocate(arena, sizeof(LayoutResult));
}

void calculate_(Arena* arena, LayoutNode* root, f32 leftOffset, f32 topOffset)
{
  rec offsetRoot = toRec(root->node);
  if (offsetRoot.x <= leftOffset)
    offsetRoot.x += leftOffset;
  if (offsetRoot.y <= topOffset)
    offsetRoot.y += topOffset;

  LayoutResult* layoutResult = newLayoutResult(arena);
  layoutResult->id = root->id;
  layoutResult->bounds = offsetRoot;
  push_pLayoutResult(arena, calculatedValues_, layoutResult);

  for (u32 i = 0; i < root->children->length; i++) {
    Result_pLayoutNode node = get_pLayoutNode(root->children, i);
    assert(node.error == SUCCESS);
    calculate_(arena, node.result, offsetRoot.x, offsetRoot.y);
  }
}

Array_pLayoutResult* calculate(Arena* arena, LayoutNode* root)
{
  Arena* scratch = newArena(KILOBYTES(16));

  calculatedValues_ = newArray_pLayoutResult(scratch);
  rec rootBounds = toRec(root->node);
  YGNodeCalculateLayout(root->node, rootBounds.width, rootBounds.height, YGDirectionLTR);
  calculate_(scratch, root, 0.0f, 0.0f);

  Array_pLayoutResult* res = newArrayl_pLayoutResult(arena, calculatedValues_->length);
  for (u32 i = 0; i < calculatedValues_->length; i++) {
    res->items[i] = allocate(arena, sizeof(LayoutResult));
    memcpy(res->items[i], calculatedValues_->items[i], sizeof(LayoutResult));
  }

  YGNodeFreeRecursive(root->node);
  freeArena(scratch);
  return res;
}

#endif // ZLAYOUT_IMPLEMENTATION

#endif // ZLAYOUT_H
