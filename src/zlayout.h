#ifndef ZLAYOUT_H
#define ZLAYOUT_H

#include <zsl.h>

typedef enum {
    FLEX_DIRECTION_COLUMN,
    FLEX_DIRECTION_ROW
} FlexDirection;

typedef struct {
    FlexDirection flexDirection;

    f32 width;
    f32 height;

    f32 pad;
    f32 padTop;
    f32 padBottom;
    f32 padLeft;
    f32 padRight;
} LayoutStyle;

struct sLayoutNode;
typedef struct sLayoutNode LayoutNode;

typedef struct tLayoutResult {
    u32 id;
    rec bounds;
} LayoutResult;

LayoutNode* newLayoutNode(Arena* arena, i32 id, LayoutStyle style);
LayoutNode* addChild(Arena* arena, LayoutNode* parent, i32 id, LayoutStyle style);
Array_pLayoutResult* calculate(Arena* arena, LayoutNode* root, v2f pos);

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
    // calculate root pad
    rec newBounds = root->resultBounds_;

    if (root->style.pad > 0.0f) {
        newBounds.x += root->style.pad;
        newBounds.y += root->style.pad;
        newBounds.width -= root->style.pad * 2;
        newBounds.height -= root->style.pad * 2;
    } else {
        if (root->style.padLeft > 0.0f) {
            newBounds.x += root->style.padLeft;
            newBounds.width -= root->style.padLeft;
        }

        if (root->style.padRight > 0.0f)
            newBounds.width -= root->style.padRight;

        if (root->style.padTop > 0.0f) {
            newBounds.y += root->style.padTop;
            newBounds.height -= root->style.padTop;
        }

        if (root->style.padBottom > 0.0f)
            newBounds.height -= root->style.padBottom;
    }

    // add root to calculated values
    LayoutResult* layoutResult = newLayoutResult(arena);
    layoutResult->id = root->id;
    layoutResult->bounds = newBounds;
    push_pLayoutResult(arena, calculatedValues_, layoutResult);

    // calculate children bounds
    if (root->style.flexDirection == FLEX_DIRECTION_ROW) {
        f32 styleWidth = 0.0f;
        u32 styleWidthCount = 0;
        for_pLayoutNode (childNode, root->children) {
            if (childNode->style.width <= 0.0f)
                continue;
            styleWidth += childNode->style.width;
            styleWidthCount++;
        }

        f32 curx = 0.0f;
        f32 childWidth = (newBounds.width - styleWidth) / (root->children->length - styleWidthCount);
        for_pLayoutNode (childNode, root->children) {
            childNode->resultBounds_.x = newBounds.x + curx;
            childNode->resultBounds_.y = newBounds.y;

            f32 resolvedChildWidth = childNode->style.width > 0.0f ? childNode->style.width : childWidth;
            childNode->resultBounds_.width = resolvedChildWidth;
            curx += resolvedChildWidth;

            f32 resolvedChildHeight = childNode->style.height > 0.0f ? childNode->style.height : newBounds.height;
            childNode->resultBounds_.height = resolvedChildHeight;
        }
    }

    if (root->style.flexDirection == FLEX_DIRECTION_COLUMN) {
        f32 styleHeight = 0.0f;
        u32 styleHeightCount = 0;
        for_pLayoutNode (childNode, root->children) {
            if (childNode->style.height <= 0.0f)
                continue;
            styleHeight += childNode->style.height;
            styleHeightCount++;
        }

        f32 cury = 0.0f;
        f32 childHeight = (newBounds.height - styleHeight) / (root->children->length - styleHeightCount);
        for_pLayoutNode (childNode, root->children) {
            childNode->resultBounds_.x = newBounds.x;
            childNode->resultBounds_.y = newBounds.y + cury;

            f32 resolvedChildWidth = childNode->style.width > 0.0f ? childNode->style.width : newBounds.width;
            childNode->resultBounds_.width = resolvedChildWidth;

            f32 resolvedChildHeight = childNode->style.height > 0.0f ? childNode->style.height : childHeight;
            childNode->resultBounds_.height = resolvedChildHeight;
            cury += resolvedChildHeight;
        }
    }

    // recurse into children
    for_pLayoutNode (node, root->children)
        calculate_(arena, node);
}

Array_pLayoutResult* calculate(Arena* arena, LayoutNode* root, v2f pos)
{
    root->resultBounds_.x = pos.x;
    root->resultBounds_.y = pos.y;
    root->resultBounds_.width = root->style.width;
    root->resultBounds_.height = root->style.height;

    calculatedValues_ = newArray_pLayoutResult(arena);
    calculate_(arena, root);

    return calculatedValues_;
}

#endif // ZLAYOUT_IMPLEMENTATION

#endif // ZLAYOUT_H
