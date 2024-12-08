#ifndef BUTTON_H
#define BUTTON_H

#include <zstring.h>
#include <ztypes.h>

FORWARD(String)
FORWARD(Result_bool)

typedef struct _Button {
    u32 id;
    String* string;
} Button;

Result_bool handlePress(Button* button);

#endif // BUTTON_H
